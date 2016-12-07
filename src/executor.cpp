//
// Created by ken on 11.08.16.
//
#include <simgrid/msg.h>
#include <string>
#include <vector>
#include "my_structures.h"
#include "myfunc_list.h"


std::vector<InputInfo*>* get_input_file_path(Job* job);
int copy_from_tape_to_disk(InputInfo* data_info);
void download_or_read_file(Job* jobInfo, InputInfo* dataInfo);
int task_executor(Job* jobInfo);


void plusOneActiveCore();
void minusOneActiveCore();

int my_on_exit(void* ignored1, void *ignored2);
int copy_tape_disk_process(int argc, char* argv[]);

extern map<std::string, double> cumulative_input_site;
extern map<std::string, double> cumulative_output_site;

XBT_LOG_NEW_DEFAULT_CATEGORY(executor, "messages specific for executor");

int executor(int argc, char* argv[]){
    //MSG_process_on_exit(my_on_exit, NULL);

    plusOneActiveCore();

    Job* job = (Job*) MSG_process_get_data(MSG_process_self());
    switch (job->type){
        case MCSIMULATION:
            break;
        default:
            DataInfo* dataInfo = get_input_file_path(job);
            copy_from_tape_to_disk(dataInfo);
            download_or_read_file(job, dataInfo);
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

    std::string storage; // From which storage I should download data

    std::vector<std::string> const& inputFiles = jobInfo->InputFiles;

    for (size_t i = 0; i < inputFiles.size(); ++i) {
        std::vector<std::string> const& fileStorages = (FILES_DATABASE->at(inputFiles.at(i)))->Storages;

        bool disk = false;
        bool tape = false;
        for (size_t j = 0; j < fileStorages.size(); ++j) {
            size_t len_stor = fileStorages.at(i).size();
            const char *last_four = &fileStorages.at(i).c_str()[len_stor-4];

            if (!strncmp(hostName.c_str(), fileStorages.at(i).c_str(), hostName.size()) && !strcmp(last_four, "DISK")){
                storage = fileStorages.at(i);
                disk = true;
                break;
            }
            else if (!strcmp(last_four, "DISK")){
                storage = fileStorages.at(i);
                disk = true;
                continue;
            }
            else if (!strncmp(hostName.c_str(), fileStorages.at(i).c_str(), hostName.size()) && !strcmp(last_four, "TAPE") && !disk) {
                storage = fileStorages.at(i);
                tape = true;
            }
            else if (!disk && !tape){
                storage = fileStorages.at(i);
                tape = true;
            }
        }

        InputInfo* inputInfo = new InputInfo;
        inputInfo->localInputFilePath = inputFiles.at(i);
        inputInfo->storage = storage;
        inputInfo->storageType = disk;
        fullPathVector->push_back(inputInfo);
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
    MSG_file_close(file);

    create_file_label(diskFileName);

    //trace
    dataset_number_change(diskStorageName, 1);

    return 0;
}

int copy_from_tape_to_disk(std::vector<InputInfo*>* inputInfoVector){


    size_t fileAmount = inputInfoVector->size();
    for (size_t i = 0; i < fileAmount; ++i) {
        InputInfo* inputInfo = inputInfoVector->at(i);

        if (!inputInfo->storageType){
            MSG_process_create("copydisk", copy_tape_disk_process, inputInfo, MSG_host_self());
        }
    }

    return 0;
}


void download_or_read_file(Job* jobInfo, DataInfo* dataInfo){

    string host_name = MSG_host_get_name(MSG_host_self());
    string storage_name = host_name + "1";
    double clock = MSG_get_clock();
    msg_file_t file;
    if (dataInfo->destination_name.compare(host_name)){

        // DOWNLOADING FILE FROM ANOTHER TIER
        file = MSG_file_open(dataInfo->input_file_path.c_str(), NULL);

        file_usage_counter(dataInfo->input_file_path);

        plusLinkCounter(dataInfo->destination_name, host_name);
        msg_error_t error = MSG_file_rcopy(file, MSG_host_self(), dataInfo->copy_file_path.c_str());
        create_file_label(dataInfo->copy_file_path);

        //tracing number of dataset
        dataset_number_change(storage_name, 1);
        cumulative_input_per_site(host_name, (double) MSG_file_get_size(file));

        if (error == MSG_OK){
            tracer_traffic(dataInfo->destination_name, host_name, (double) MSG_file_get_size(file));
            //tracer_storage(host_name, dataInfo->storage_type.c_str());
        } else{
            minusLinkCounter(dataInfo->destination_name, host_name);
            minusOneActiveCore();
            jobInfo->stExecClock = 0;
            jobInfo->endExecClock = 0;
            writeToFile(fp, jobInfo);
            MSG_file_close(file);
            MSG_process_kill(MSG_process_self());
        }
        minusLinkCounter(dataInfo->destination_name, host_name);
        msg_file_t d_file = MSG_file_open(dataInfo->copy_file_path.c_str(), NULL);
        file_usage_counter(dataInfo->copy_file_path);
        MSG_file_read(d_file, (sg_size_t) jobInfo->inputSize);

        MSG_file_close(file);
        MSG_file_close(d_file);
    }

    //If I have data, I open and read it

    msg_file_t i_data = MSG_file_open(dataInfo->input_file_path.c_str(), NULL);
    file_usage_counter(dataInfo->input_file_path);
    MSG_file_read(i_data, (sg_size_t) jobInfo->inputSize);
    MSG_file_close(i_data);

    delete dataInfo;
}


int task_executor(Job* jobInfo){
    string host_name = string(MSG_host_get_name(MSG_host_self()));
    string storage_name = host_name + "1";
    string outputFilePath;
    msg_task_t task;
    msg_file_t outFile;

    outputFilePath = "/" + host_name + "1" + "/" + jobInfo->outputName;

    // CREATING AND EXECUTION OF TASK
    task = MSG_task_create(jobInfo->name.c_str(), jobInfo->compSize, 0, NULL);
    jobInfo->stExecClock = MSG_get_clock();
    addActiveCoreT();
    msg_error_t b = MSG_task_execute(task);
    subActiveCoreT();
    jobInfo->endExecClock = MSG_get_clock();
    minusOneActiveCore();
    jobInfo->successExecuted = 1;
    jobInfo->tier = host_name;

    //Anomalies of tier host
    if (b == MSG_OK){
        XBT_INFO("%s has successfully executed", jobInfo->name.c_str());
        MSG_task_destroy(task);
        task = NULL;
    }else{
        XBT_INFO("Error has occurred while executing %s", MSG_task_get_name(task));
        MSG_task_destroy(task);
        task = NULL;
    }

    //Write output to file
    outFile = MSG_file_open(outputFilePath.c_str(), NULL);
    MSG_file_write(outFile, (sg_size_t) (jobInfo->outputFileSize));
    create_file_label(outputFilePath);
    MSG_file_close(outFile);

    // tracing: one new file
    dataset_number_change(storage_name, 1);

    writeToFile(fp, jobInfo);
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
    writeToFile(fp, jobInfo);
    return 0;
}

