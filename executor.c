//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
XBT_LOG_NEW_DEFAULT_CATEGORY(dispatcher, "messages specific for executor");

int hostFailAmount = 0;
void plusOneActiveCore();
void minusOneActiveCore();

int executor(int argc, char* argv[]){

    msg_file_t file, outFile;
    msg_task_t task;
    sg_size_t inputSize, outputSize;

    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    char inputFilePath[80];
    char outputFilePath[80];
    sprintf(inputFilePath, "%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->inputFileName);
    sprintf(outputFilePath, "%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->outputName);

    file = MSG_file_open(inputFilePath, NULL);
    inputSize = MSG_file_read(file, (sg_size_t) jobInfo->commSize);

    // CREATING AND EXECUTION OF TASK
    task = MSG_task_create(jobInfo->name, jobInfo->compSize, 0, NULL);
    plusOneActiveCore();
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
    MSG_file_write(outFile, (sg_size_t) jobInfo->outputFileSize);

    //Launch replicas creator
    replicatorDataPtr replica = xbt_new(replicatorData, 1);
    replica->fileName = jobInfo->outputName;
    replica->currentLoc = (char*) MSG_host_get_name(MSG_host_self());
    replica->outLoc1 = jobInfo->outputHost1;
    replica->outLoc2 = jobInfo->outputHost2;
    replica->outLoc3 = jobInfo->outputHost3;

    //Clear memory
    MSG_file_close(file);
    MSG_file_close(outFile);
    memset(inputFilePath, 0, 80);
    memset(outputFilePath, 0, 80);
    xbt_free(jobInfo);
    MSG_process_kill(MSG_process_self());
    return 0;
}

void plusOneActiveCore(){
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), NULL);
    memset(kot, 0, 50);
}

void minusOneActiveCore(){
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), NULL);
    memset(kot, 0, 50);
}
