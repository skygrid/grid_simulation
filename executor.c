//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
XBT_LOG_NEW_DEFAULT_CATEGORY(dispatcher, "messages specific for executor");

int hostFailAmount = 0;

int executor(int argc, char* argv[]){
    /** argc is always 1
     * argv in jobPtr
    */
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
    msg_error_t a = MSG_task_execute(task);
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

    MSG_file_close(file);
    MSG_file_close(outFile);



    return 0;
}
