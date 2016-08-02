//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
#include "tier1.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(executor, "messages specific for executor");

int hostFailAmount = 0;
msg_sem_t sem;
void plusOneActiveCore();
void minusOneActiveCore();

int executor(int argc, char* argv[]){
    sem = MSG_sem_init(1);
    plusOneActiveCore();
    msg_file_t file, outFile;
    msg_task_t task;
    sg_size_t inputSize, outputSize;

    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    char inputFilePath[80];
    char outputFilePath[80];
    sprintf(inputFilePath, "%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType, jobInfo->inputFileName);
    sprintf(outputFilePath, "%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType, jobInfo->outputName);

    file = MSG_file_open(inputFilePath, NULL);
    inputSize = MSG_file_read(file, (sg_size_t) jobInfo->inputSize);
    MSG_file_close(file);

    // CREATING AND EXECUTION OF TASK
    task = MSG_task_create(jobInfo->name, jobInfo->compSize, 0, NULL);
    msg_error_t a = MSG_task_execute(task);
    minusOneActiveCore();

    //Anomalies
    if (a == MSG_OK){
        XBT_INFO("%s has successfully executed", jobInfo->name);
        MSG_task_destroy(task);
        task = NULL;
    }else{
        hostFailAmount++;
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
    MSG_process_kill(MSG_process_self());
    return 0;
}

void plusOneActiveCore(){
    MSG_sem_acquire(sem);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), NULL);
    XBT_INFO("Active core amount is %s", MSG_host_get_property_value(MSG_host_self(), "activeCore"));
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
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), NULL);
    //XBT_INFO("Active core amount is %s", MSG_host_get_property_value(MSG_host_self(), "activeCore"));
    memset(kot, '\0', 50);
    MSG_sem_release(sem);
}
