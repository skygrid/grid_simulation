//
// Created by ken on 24.05.16.
//

#include <simgrid/msg.h>
#include <csvparser.h>
#include "messages.h"
#include "myfunc_list.h"

#define QUEUE_SIZE 10000
jobPtr* matcher(long amountRequestedJob);
int input();

long currentJobInQueue = 0;
jobPtr* jobQueue;

XBT_LOG_NEW_DEFAULT_CATEGORY(scheduler, "messages specific for scheduler");

int scheduler(int argc, char* argv[]){

    input();
    char mailbox[30];
    msg_task_t task = NULL;
    msg_task_t taskB = NULL;
    sprintf(mailbox, "scheduler");

    while (1){
        int res = MSG_task_receive(&task, mailbox);
        // Anomalies
        if (res == MSG_OK){
            XBT_INFO("Get job request from %s", MSG_host_get_name(MSG_task_get_source(task)));
        }else if (res == MSG_TRANSFER_FAILURE){
            MSG_task_destroy(task);
            task = NULL;
        } else if (res == MSG_HOST_FAILURE){
            MSG_task_destroy(task);
            task = NULL;
        }
        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }

        jobBatchRequestPtr batchRequest = MSG_task_get_data(task);
        // !!!!!111
        jobPtr* batch = matcher(1);
        taskB = MSG_task_create("", 1.0, MESSAGES_SIZE, batch);
        msg_error_t res1 = MSG_task_send(taskB, MSG_host_get_name(MSG_task_get_source(task)));

        // Anomalies
        if (res1 == MSG_OK){
            XBT_INFO("Send jobs after matching %s", MSG_host_get_name(MSG_task_get_source(task)));
        }else if (res1 == MSG_TRANSFER_FAILURE){
            MSG_task_destroy(taskB);
            taskB = NULL;
        } else if (res1 == MSG_HOST_FAILURE){
            MSG_task_destroy(taskB);
            taskB = NULL;
        }
        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }

        xbt_free(batchRequest);
        MSG_task_destroy(task);
        task = NULL;
        taskB = NULL;
    }
}

jobPtr* matcher(long amountRequestedJob){

    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);
    for (long i = currentJobInQueue; i < (currentJobInQueue+amountRequestedJob); ++i) {
        jobBatch[i-currentJobInQueue] = jobQueue[i];
    }
    return jobBatch;
}

int input(){
    int i = 0;
    jobQueue = xbt_new(jobPtr, QUEUE_SIZE);

    CsvParser *csvparser = CsvParser_new("input.csv", ",", 1);
    CsvRow *header;
    CsvRow *row;

    while ((row = CsvParser_getRow(csvparser))){
        const char **rowFields = CsvParser_getFields(row);
        jobPtr jobX = xbt_new(job, 1);

        //Parsing
        jobX->name = strdup((char*) rowFields[0]);
        jobX->type = charToEnum((char*) rowFields[1]);
        jobX->compSize = xbt_str_parse_double(rowFields[2], "kotok1");
        jobX->inputFileName = strdup((char*) rowFields[3]);
        jobX->inputSize = xbt_str_parse_double(rowFields[4], "kotok2");
        jobX->NRep = (int) xbt_str_parse_double(rowFields[5], "kotok3");
        jobX->storageType = strdup((char*) rowFields[6]);
        jobX->dataLocHost1 = strdup((char*) rowFields[7]);
        jobX->dataLocHost2 = strdup((char*) rowFields[8]);
        jobX->dataLocHost3 = strdup((char*) rowFields[9]);
        jobX->outputName = strdup((char*) rowFields[10]);
        jobX->outputFileSize = xbt_str_parse_double(rowFields[11], "kotok4");
        jobX->outputNumber = (int) xbt_str_parse_double(rowFields[12], "kotok5");
        jobX->outputHost1 = strdup((char*) rowFields[13]);
        jobX->outputHost2 = strdup((char*) rowFields[14]);
        jobX->outputHost3 = strdup((char*) rowFields[15]);
        jobQueue[i] = jobX;
        CsvParser_destroy_row(row);
        i++;
    }
    CsvParser_destroy(csvparser);
    return 0;
}





