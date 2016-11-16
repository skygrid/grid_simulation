//
// Created by ken on 11.08.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

DataInfo* get_input_file_path(Job* job);
int copy_from_tape_to_disk(DataInfo* data_info);
void download_or_read_file(Job* jobInfo, DataInfo* dataInfo);
int task_executor(Job* jobInfo);


void plusOneActiveCore();
void minusOneActiveCore();

int my_on_exit(void* ignored1, void *ignored2);

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

DataInfo* get_input_file_path(Job* jobInfo){

    /*Where should I download data
     *return input_file_path and host_name (where data is located)
     */
    check_files_availability(jobInfo);
    string host_name = string(MSG_host_get_name(MSG_host_self()));

    string input_file_path;
    string copy_file_path;
    string copy_from_tape_to_disk_name;
    string dest;
    string storageType;

    string dataLocations[] = {jobInfo->dataLocHost1, jobInfo->dataLocHost2, jobInfo->dataLocHost3, jobInfo->dataLocHost4,
                             jobInfo->dataLocHost5, jobInfo->dataLocHost6, jobInfo->dataLocHost7, jobInfo->dataLocHost8,
                             jobInfo->dataLocHost9, jobInfo->dataLocHost10};
    string storageTypes[] = {jobInfo->storageType1, jobInfo->storageType2, jobInfo->storageType3, jobInfo->storageType4,
                            jobInfo->storageType5, jobInfo->storageType6, jobInfo->storageType7, jobInfo->storageType8,
                            jobInfo->storageType9, jobInfo->storageType10};


    int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);

    // Checks does tier have data on the own storage
    for (int i = 0; i < n; ++i) {
        if (!dataLocations[i].compare(host_name) & !storageTypes[i].compare("1")){
            input_file_path = "/" + dataLocations[i] + storageTypes[i] + "/" + jobInfo->inputFileName;
            copy_file_path = "/" + dataLocations[i] + "1" + "/" + jobInfo->inputFileName;
            dest = host_name;
            storageType = storageTypes[i];
            break;
        }
        // If tier doesn't have storage on the own, find available data on another tier
        if (i == (n-1)){
            for (int j = 0; j < n; ++j) {
                if (dataLocations[j].compare("0") && storageTypes[j].compare("0")){
                    storageType = storageTypes[j];
                    dest = dataLocations[j];

                    input_file_path = "/" + dest + storageType + "/" + jobInfo->inputFileName;
                    copy_file_path = "/" + host_name + "1" + "/" + jobInfo->inputFileName;
                    copy_from_tape_to_disk_name = "/" + dest + "1" + "/" + jobInfo->inputFileName;
                    break;
                }

                // Find input data at tape
                if (j == (n-1)){
                    for (int k = 0; k < n; ++k) {
                        if (dataLocations[k].compare("0")){
                            storageType = storageTypes[k];
                            dest = dataLocations[k];

                            input_file_path = "/" + dest + storageType + "/" + jobInfo->inputFileName;
                            copy_file_path = "/" + host_name + "1" + "/" + jobInfo->inputFileName;
                            copy_from_tape_to_disk_name = "/" + dest + "1" + "/" + jobInfo->inputFileName;
                            break;
                        }
                    }
                }
            }
        }
    }

    DataInfo* data_info = new DataInfo;
    data_info->destination_name = dest;
    data_info->input_file_path = input_file_path;
    data_info->copy_from_tape_to_disk_name = copy_from_tape_to_disk_name;
    data_info->copy_file_path = copy_file_path;
    data_info->storage_type = storageType;
    return data_info;
}



int copy_from_tape_to_disk(DataInfo* data_info){
    msg_file_t file;

    // Open, copy and close file    "0" means TAPE
    if (!data_info->storage_type.compare("0")){
        file = MSG_file_open(data_info->input_file_path.c_str(), NULL);

        file_usage_counter(data_info->input_file_path);

        MSG_file_rcopy(file, MSG_host_by_name(data_info->destination_name.c_str()), data_info->copy_from_tape_to_disk_name.c_str());
        create_file_label(data_info->copy_from_tape_to_disk_name);
        MSG_file_close(file);

        // trace storage and dataset amount to disk space
        // tracer_storage(data_info->destination_name, data_info->storage_type.c_str());
        string storage_name = data_info->destination_name + "1";
        dataset_number_change(storage_name, 1);


        // So we have new name of input file on the disk
        data_info->input_file_path = data_info->copy_from_tape_to_disk_name;
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

