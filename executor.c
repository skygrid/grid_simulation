//
// Created by ken on 11.08.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
void plusOneActiveCore();
void minusOneActiveCore();
void my_on_exit();
msg_sem_t sem;



XBT_LOG_NEW_DEFAULT_CATEGORY(executor, "messages specific for executor");

int executor(int argc, char* argv[]){
    MSG_process_on_exit(my_on_exit, NULL);
    plusOneActiveCore();
    msg_file_t file, outFile;
    msg_task_t task;
    sg_size_t inputSize, outputSize;
    char* dest;

    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    char inputFilePath[80];
    char copyFilePath[80];
    char outputFilePath[80];
    sprintf(copyFilePath, "/%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType, jobInfo->inputFileName);
    //find location of input file

    switch (jobInfo->type){
        case MCSIMULATION:
            break;
        default:
    }

    if (jobInfo->type != MCSIMULATION) {


        if (strcmp(dest, "none")){
            // DOWNLOADING FILE FROM ANOTHER TIER
            file = MSG_file_open(inputFilePath, NULL);
            plusLinkCounter(dest, MSG_host_get_name(MSG_host_self()));
            msg_error_t errorx = MSG_file_rcopy(file, MSG_host_self(), copyFilePath);

            if (errorx != MSG_OK){
                minusLinkCounter(dest, MSG_host_get_name(MSG_host_self()));
                minusOneActiveCore();
                jobInfo->stExecClock = 0;
                jobInfo->endExecClock = 0;
                writeToFile(fp, jobInfo);
                MSG_file_close(file);
                MSG_process_kill(MSG_process_self());

            }
            minusLinkCounter(dest, MSG_host_get_name(MSG_host_self()));
            msg_file_t d_file = MSG_file_open(copyFilePath, NULL);
            MSG_file_read(d_file, (sg_size_t) jobInfo->inputSize);
            MSG_file_close(file);
            MSG_file_close(d_file);
        }
        // IF I HAVE DATA
        msg_file_t i_data = MSG_file_open(inputFilePath, NULL);
        MSG_file_read(i_data, (sg_size_t) jobInfo->inputSize);
        MSG_file_close(i_data);

    }
    sprintf(outputFilePath, "/%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType,
            jobInfo->outputName);

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

    MSG_process_create("dataRep", data_replicator, jobInfo, MSG_host_self());

    memset(inputFilePath, '\0', 80);
    memset(outputFilePath, '\0', 80);
    memset(copyFilePath, '\0', 80);
    MSG_process_kill(MSG_process_self());
    return 0;
}

dataInfoPtr get_input_file_path(jobPtr jobInfo){
    /*Where should I download data
     *return input file name and host_name (where data is located)
     */
    char* input_file_path = malloc(50);
    char* dest;
    char* copy_file_path = malloc(50);

    if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost1)) {
        sprintf(input_file_path, "/%s%s/%s", jobInfo->dataLocHost1, jobInfo->storageType, jobInfo->inputFileName);
        dest = "self";
    } else if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost2)) {
        sprintf(input_file_path, "/%s%s/%s", jobInfo->dataLocHost2, jobInfo->storageType, jobInfo->inputFileName);
        dest = "self";
    } else if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost3)) {
        sprintf(input_file_path, "/%s%s/%s", jobInfo->dataLocHost3, jobInfo->storageType, jobInfo->inputFileName);
        dest = "self";
    } else {
        //msg_host_t file
        dest = jobInfo->dataLocHost1;
        sprintf(input_file_path, "/%s1/%s", jobInfo->dataLocHost1, jobInfo->inputFileName);
        sprintf(copy_file_path, "/%s1/%s", MSG_host_get_name(MSG_host_self()), jobInfo->inputFileName);
    }
    dataInfoPtr data_info = xbt_new(dataInfo, 1);
    data_info->destination_name = dest;
    data_info->input_file_path = input_file_path;
    data_info->copy_file_path = copy_file_path;

    return data_info;
}

void


void download_or_read_file(jobPtr jobInfo, dataInfoPtr dataInfo){
    msg_file_t file;
    if (strcmp(dataInfo->destination_name, "self")){

        // DOWNLOADING FILE FROM ANOTHER TIER
        file = MSG_file_open(dataInfo->input_file_path, NULL);
        plusLinkCounter(dataInfo->destination_name, MSG_host_get_name(MSG_host_self()));
        msg_error_t error = MSG_file_rcopy(file, MSG_host_self(), dataInfo->copy_file_path);

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


void my_on_exit(){
    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    writeToFile(fp, jobInfo);
}