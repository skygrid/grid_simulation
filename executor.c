//
// Created by ken on 11.08.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

dataInfoPtr get_input_file_path(jobPtr jobInfo);
int copy_from_tape_to_disk(dataInfoPtr data_info);
void download_or_read_file(jobPtr jobInfo, dataInfoPtr dataInfo);
int task_executor(jobPtr jobInfo);


void plusOneActiveCore();
void minusOneActiveCore();
int my_on_exit();

msg_sem_t sem;

XBT_LOG_NEW_DEFAULT_CATEGORY(executor, "messages specific for executor");

int executor(int argc, char* argv[]){
    MSG_process_on_exit(my_on_exit, NULL);

    dataInfoPtr dataInfo;
    plusOneActiveCore();

    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());

    switch (jobInfo->type){
        case MCSIMULATION:
            break;
        default:
            dataInfo = get_input_file_path(jobInfo);
            copy_from_tape_to_disk(dataInfo);
            download_or_read_file(jobInfo, dataInfo);
            break;
    }
    addActiveCoreT();
    task_executor(jobInfo);
    subActiveCoreT();
    MSG_process_create("dataRep", data_replicator, jobInfo, MSG_host_self());
    MSG_process_kill(MSG_process_self());
    return 0;
}

dataInfoPtr get_input_file_path(jobPtr jobInfo){
    /*Where should I download data
     *return input_file_path and host_name (where data is located)
     */
    char* input_file_path = malloc(50);
    char* copy_file_path = malloc(50);
    char* copy_from_tape_to_disk_name = malloc(50);
    char* dest;
    char* storageType;

    char *dataLocations[] = {jobInfo->dataLocHost1, jobInfo->dataLocHost2, jobInfo->dataLocHost3, jobInfo->dataLocHost4};
    char *storageTypes[] = {jobInfo->storageType1, jobInfo->storageType2, jobInfo->storageType3, jobInfo->storageType4};
    int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);

    // Checks does tier have data on the own storage
    for (int i = 0; i < n; ++i) {
        if (!strcmp(MSG_host_get_name(MSG_host_self()), dataLocations[i])){
            sprintf(input_file_path, "/%s%s/%s", dataLocations[i], storageTypes[i], jobInfo->inputFileName);
            sprintf(copy_from_tape_to_disk_name, "/%s1/%s", dataLocations[i], jobInfo->inputFileName);
            dest = (char *) MSG_host_get_name(MSG_host_self());
            storageType = storageTypes[i];
            break;
        }
        // If tier doesn't have storage on the own
        if (i == (n-1)){
            dest = jobInfo->dataLocHost1;
            storageType = jobInfo->storageType1;
            sprintf(input_file_path, "/%s%s/%s", jobInfo->dataLocHost1, jobInfo->storageType1, jobInfo->inputFileName);
            sprintf(copy_from_tape_to_disk_name, "/%s1/%s", jobInfo->dataLocHost1, jobInfo->inputFileName);
            sprintf(copy_file_path, "/%s1/%s", MSG_host_get_name(MSG_host_self()), jobInfo->inputFileName);
        }
    }

    dataInfoPtr data_info = xbt_new(dataInfo, 1);
    data_info->destination_name = dest;
    data_info->input_file_path = input_file_path;
    data_info->copy_from_tape_to_disk_name = copy_from_tape_to_disk_name;
    data_info->copy_file_path = copy_file_path;
    data_info->storage_type = storageType;

    return data_info;
}

int copy_from_tape_to_disk(dataInfoPtr data_info){
    msg_file_t file;

    // Open, copy and close file    "0" means TAPE
    if (!strcmp(data_info->storage_type, "0")){
        file = MSG_file_open(data_info->input_file_path, NULL);
        MSG_file_rcopy(file, MSG_host_by_name(data_info->destination_name), data_info->copy_from_tape_to_disk_name);
        MSG_file_close(file);

        tracer_storage(data_info->destination_name, data_info->storage_type);

        // So we have new name of input file on the disk
        sprintf(data_info->input_file_path, data_info->copy_from_tape_to_disk_name);
    }

    return 0;
}


void download_or_read_file(jobPtr jobInfo, dataInfoPtr dataInfo){
    msg_file_t file;
    if (strcmp(dataInfo->destination_name, MSG_host_get_name(MSG_host_self()))){

        // DOWNLOADING FILE FROM ANOTHER TIER
        file = MSG_file_open(dataInfo->input_file_path, NULL);
        plusLinkCounter(dataInfo->destination_name, MSG_host_get_name(MSG_host_self()));
        msg_error_t error = MSG_file_rcopy(file, MSG_host_self(), dataInfo->copy_file_path);

        tracer_traffic(dataInfo->destination_name, MSG_host_get_name(MSG_host_self()), (double) MSG_file_get_size(file));

        tracer_storage((char*)MSG_host_get_name(MSG_host_self()), dataInfo->storage_type);

        if (error != MSG_OK){
            minusLinkCounter(dataInfo->destination_name, MSG_host_get_name(MSG_host_self()));
            minusOneActiveCore();
            jobInfo->stExecClock = 0;
            jobInfo->endExecClock = 0;
            writeToFile(fp, jobInfo);
            MSG_file_close(file);
            MSG_process_kill(MSG_process_self());
        }
        minusLinkCounter(dataInfo->destination_name, MSG_host_get_name(MSG_host_self()));
        msg_file_t d_file = MSG_file_open(dataInfo->copy_file_path, NULL);
        MSG_file_read(d_file, (sg_size_t) jobInfo->inputSize);
        MSG_file_close(file);
        MSG_file_close(d_file);
    }

    //If I have data, I open and read it

    msg_file_t i_data = MSG_file_open(dataInfo->input_file_path, NULL);
    MSG_file_read(i_data, (sg_size_t) jobInfo->inputSize);
    MSG_file_close(i_data);

    // Clean data information about files
    free(dataInfo->input_file_path);
    free(dataInfo->copy_from_tape_to_disk_name);
    free(dataInfo->copy_file_path);
    xbt_free(dataInfo);


}

int task_executor(jobPtr jobInfo){
    char outputFilePath[50];
    msg_task_t task;
    msg_file_t outFile;

    sprintf(outputFilePath, "/%s1/%s", MSG_host_get_name(MSG_host_self()), jobInfo->outputName);

    // CREATING AND EXECUTION OF TASK
    task = MSG_task_create(jobInfo->name, jobInfo->compSize, 0, NULL);
    jobInfo->stExecClock = MSG_get_clock();
    addActiveCoreT();
    msg_error_t b = MSG_task_execute(task);
    subActiveCoreT();
    jobInfo->endExecClock = MSG_get_clock();
    minusOneActiveCore();
    jobInfo->successExecuted = 1;

    //Anomalies of tier host
    if (b == MSG_OK){
        XBT_INFO("%s has successfully executed", jobInfo->name);
        MSG_task_destroy(task);
        task = NULL;
    }else{
        XBT_INFO("Error has occurred while executing %s", MSG_task_get_name(task));
        MSG_task_destroy(task);
        task = NULL;
    }

    //Write output to file
    outFile = MSG_file_open(outputFilePath, NULL);
    MSG_file_write(outFile, (sg_size_t) (jobInfo->outputFileSize));
    MSG_file_close(outFile);


    return 0;
}


void plusOneActiveCore(){
    MSG_sem_acquire(sem);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
    MSG_sem_release(sem);
}

void minusOneActiveCore(){
    MSG_sem_acquire(sem);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
    MSG_sem_release(sem);
}


int my_on_exit(){
    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    writeToFile(fp, jobInfo);
    return 0;
}