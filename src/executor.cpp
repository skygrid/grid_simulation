//
// Created by ken on 11.08.16.
//
#include <simgrid/msg.h>
#include <string>
#include <vector>
#include "my_structures.h"
#include "myfunc_list.h"


std::vector<InputInfo*>* get_input_file_path(Job* job);
int copy_from_tape_to_disk(std::vector<InputInfo*>* data_info);
void download_or_read_file(Job* jobInfo, std::vector<InputInfo*>* dataInfo);
int task_executor(Job* jobInfo);


void plusOneActiveCore();
void minusOneActiveCore();

int my_on_exit(void* ignored1, void *ignored2);
int copy_tape_disk_process(int argc, char* argv[]);
static int download_read_file_process(int argc, char* argv[]);

extern map<std::string, double> cumulative_input_site;
extern map<std::string, double> cumulative_output_site;

static msg_bar_t barrier;

XBT_LOG_NEW_DEFAULT_CATEGORY(executor, "messages specific for executor");

int executor(int argc, char* argv[]){
    //MSG_process_on_exit(my_on_exit, NULL);

    plusOneActiveCore();

    Job* job = (Job*) MSG_process_get_data(MSG_process_self());
    switch (job->type){
        case MCSIMULATION:
            break;
        default:
            std::vector<InputInfo*>* fullPathVector = get_input_file_path(job);
            barrier = MSG_barrier_init((unsigned int) fullPathVector->size());
            copy_from_tape_to_disk(fullPathVector);
            download_or_read_file(job, fullPathVector);
            break;
    }
    task_executor(job);
    MSG_process_sleep(5);
    MSG_process_create("dataRep", data_replicator, job, MSG_host_self());
    //MSG_process_kill(MSG_process_self());
    return 0;
}



std::vector<InputInfo*>* get_input_file_path(Job* jobInfo){

    std::vector<InputInfo*>* fullPathVector = new std::vector<InputInfo*>; // return type
    std::string hostName = string(MSG_host_get_name(MSG_host_self()));

    std::vector<std::string> const& inputFiles = jobInfo->InputFiles;

    for (size_t i = 0; i < inputFiles.size(); ++i) {
        std::vector<std::string> const& fileStorages = (FILES_DATABASE->at(inputFiles.at(i)))->Storages;

        std::string storage;
        bool disk = false;
        bool tape = false;

        for (size_t j = 0; j < fileStorages.size(); ++j) {
            size_t len_stor = fileStorages.at(j).size();
            const char *last_four = &fileStorages.at(j).c_str()[len_stor-4];

            if (!strncmp(hostName.c_str(), fileStorages.at(j).c_str(), hostName.size()) && !strcmp(last_four, "DISK")){
                storage = fileStorages.at(j);
                disk = true;
                break;
            }
            else if (!strcmp(last_four, "DISK")){
                storage = fileStorages.at(j);
                disk = true;
                continue;
            }
            else if (!strncmp(hostName.c_str(), fileStorages.at(j).c_str(), hostName.size()) && !strcmp(last_four, "TAPE") && !disk) {
                storage = fileStorages.at(j);
                tape = true;
            }
            else if (!disk && !tape){
                storage = fileStorages.at(j);
                tape = true;
            }
        }

        if (!storage.empty()){
            InputInfo* inputInfo = new InputInfo;
            inputInfo->localInputFilePath = inputFiles.at(i);
            inputInfo->storage = storage;
            inputInfo->storageType = disk;
            inputInfo->hostName = storage.erase(storage.length()-5);
            fullPathVector->push_back(inputInfo);
        }
    }
    return fullPathVector;
}

int copy_tape_disk_process(int argc, char* argv[]){

    InputInfo* inputInfo = (InputInfo*) MSG_process_get_data(MSG_process_self());
    std::string remoteHostName = inputInfo->storage;
    remoteHostName.erase(remoteHostName.length()-4);

    std::string diskStorageName = remoteHostName + "-" + "DISK";
    std::string tapeFileName = "/" + inputInfo->storage + inputInfo->localInputFilePath;
    std::string diskFileName = "/" + diskStorageName + inputInfo->localInputFilePath;

    msg_file_t file = MSG_file_open(tapeFileName.c_str(), NULL);;
    file_usage_counter(tapeFileName);

    MSG_file_rcopy(file, MSG_host_by_name(remoteHostName.c_str()), diskFileName.c_str());
    inputInfo->storage = diskStorageName;
    MSG_file_close(file);

    create_file_label(diskFileName);

    //trace
    dataset_number_change(diskStorageName, 1);

    return 0;
}


static int download_read_file_process(int argc, char* argv[]){

    InputAndJobInfo* data = (InputAndJobInfo*) MSG_process_get_data(MSG_process_self());
    InputInfo* inputInfo = data->inputInfo;
    Job* job = data->job;

    std::string host_name = MSG_host_get_name(MSG_host_self());
    std::string storage_name = host_name + "-DISK";
    std::string fullInputPath = "/" + inputInfo->storage + inputInfo->localInputFilePath;
    std::string fullNewPath = "/" + storage_name + inputInfo->localInputFilePath;
    msg_file_t file;

    if (inputInfo->storage.compare(storage_name) != 0){

        // DOWNLOADING FILE FROM ANOTHER TIER
        file = MSG_file_open(fullInputPath.c_str(), NULL);

        file_usage_counter(fullInputPath);

        plusLinkCounter(inputInfo->hostName, host_name);
        msg_error_t error = MSG_file_rcopy(file, MSG_host_self(), fullNewPath.c_str());
        create_file_label(fullNewPath);

        //tracing number of dataset
        dataset_number_change(storage_name, 1);
        cumulative_input_per_site(host_name, (double) MSG_file_get_size(file));

        if (error == MSG_OK){
            tracer_traffic(inputInfo->hostName, host_name, (double) MSG_file_get_size(file));
            //tracer_storage(host_name, dataInfo->storage_type.c_str());
        } else{
            minusLinkCounter(inputInfo->hostName, host_name);
            minusOneActiveCore();
            job->StartExecTime = 0;
            job->EndExecTime = 0;
            writeToFile(job);
            MSG_file_close(file);
            MSG_process_kill(MSG_process_self());
        }
        minusLinkCounter(inputInfo->hostName, host_name);
        MSG_file_close(file);
        inputInfo->storage = storage_name;
    }

    //Now I have data, I open and read it
    msg_file_t i_data = MSG_file_open(fullInputPath.c_str(), NULL);
    file_usage_counter(fullInputPath);
    MSG_file_read(i_data, (sg_size_t) MSG_file_get_size(i_data));
    MSG_file_close(i_data);
    MSG_barrier_wait(barrier);
    delete data;
    delete inputInfo;
    return 0;
}

int copy_from_tape_to_disk(std::vector<InputInfo*>* inputInfoVector){


    size_t fileAmount = inputInfoVector->size();
    for (size_t i = 0; i < fileAmount; ++i) {
        InputInfo* inputInfo = inputInfoVector->at(i);

        if (!inputInfo->storageType && (!inputInfo->storage.empty())){
            MSG_process_create("copytodisk", copy_tape_disk_process, inputInfo, MSG_host_self());
        }
    }

    return 0;
}


void download_or_read_file(Job* jobInfo, std::vector<InputInfo*>* inputInfoVector){
    size_t size = inputInfoVector->size();
    for (size_t i = 0; i < size; ++i) {
        InputAndJobInfo* data = new InputAndJobInfo;
        data->job = jobInfo;
        data->inputInfo = inputInfoVector->at(i);

        if (!inputInfoVector->at(i)->storage.empty()){
            MSG_process_create("reader", download_read_file_process, data, MSG_host_self());
        }
    }
    return;
}


int task_executor(Job* jobInfo){
    string host_name = string(MSG_host_get_name(MSG_host_self()));
    string storage_name = host_name + "-DISK";
    msg_task_t task;
    msg_file_t outFile;

    // CREATING AND EXECUTION OF TASK

    //task = MSG_task_create(std::to_string(jobInfo->JobId).c_str(), jobInfo->TotalCPUTime, 1, NULL);
    task = MSG_task_create("kotok", 1e11, 1000, NULL);

    jobInfo->StartExecTime = MSG_get_clock();
    addActiveCoreT();
    msg_error_t b = MSG_task_execute(task);
    subActiveCoreT();
    jobInfo->EndExecTime = MSG_get_clock();
    minusOneActiveCore();
    jobInfo->successExecuted = 1;
    jobInfo->tier = host_name;

    //Anomalies of tier host
    if (b == MSG_OK){
        XBT_INFO("%zd has successfully executed", jobInfo->JobId);
        MSG_task_destroy(task);
        task = NULL;
    }else{
        XBT_INFO("Error has occurred while executing %zd", jobInfo->JobId);
        MSG_task_destroy(task);
        task = NULL;
    }

    //Create and write outputs file
    size_t outputAmount = jobInfo->OutputFiles.size();
    for (size_t i = 0; i < outputAmount; ++i) {
        std::string outputFilePath = "/" + storage_name + jobInfo->OutputFiles.at(i);
        outFile = MSG_file_open(outputFilePath.c_str(), NULL);
        MSG_file_write(outFile, (sg_size_t) FILES_DATABASE->at(jobInfo->OutputFiles.at(i))->Size);
        create_file_label(outputFilePath);
        MSG_file_close(outFile);

        // tracing: one new file
        dataset_number_change(storage_name, 1);
    }
    writeToFile(jobInfo);
    return 0;
}


void plusOneActiveCore(){
    MSG_sem_acquire(sem_link);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), xbt_free_f);
    MSG_sem_release(sem_link);
}

void minusOneActiveCore(){
    MSG_sem_acquire(sem_link);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), xbt_free_f);
    MSG_sem_release(sem_link);
}


int my_on_exit(void* ignored1, void *ignored2){
    Job* jobInfo = (Job*) MSG_process_get_data(MSG_process_self());
    writeToFile(jobInfo);
    return 0;
}


