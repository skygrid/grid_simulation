//
// Created by ken on 24.05.16.
//

#include <simgrid/msg.h>
#include <csvparser.h>
#include "messages.h"
#include "myfunc_list.h"

#define QUEUE_SIZE 100000

int current_task_n = 0;

XBT_LOG_NEW_DEFAULT_CATEGORY(scheduler, "messages specific for scheduler");

int input(){
    int i = 0;

    jobPtr* jobQueue = xbt_new(jobPtr, QUEUE_SIZE);

    CsvParser *csvparser = CsvParser_new("data.csv", ",", 1);
    CsvRow *header;
    CsvRow *row;

    while ((row = CsvParser_getRow(csvparser))){
        const char **rowFields = CsvParser_getFields(row);

        //Parsing
        jobQueue[i]->name = (char*) rowFields[0];
        jobQueue[i]->type = charToEnum((char*) rowFields[1]);
        jobQueue[i]->compSize = xbt_str_parse_double(rowFields[2], "error");
        jobQueue[i]->inputFileName = (char*) rowFields[3];
        jobQueue[i]->inputSize = xbt_str_parse_double(rowFields[4], "error");
        jobQueue[i]->NRep = (int) xbt_str_parse_double(rowFields[5], "error");
        jobQueue[i]->storageType = (char*) rowFields[6];
        jobQueue[i]->dataLocHost1 = (char*) rowFields[7];
        jobQueue[i]->dataLocHost2 = (char*) rowFields[8];
        jobQueue[i]->dataLocHost3 = (char*) rowFields[9];
        jobQueue[i]->outputName = (char*) rowFields[10];
        jobQueue[i]->outputFileSize = xbt_str_parse_double(rowFields[11], "error");
        jobQueue[i]->outputNumber = (int) xbt_str_parse_double(rowFields[12], "error");
        jobQueue[i]->outputHost1 = (char*) rowFields[13];
        jobQueue[i]->outputHost2 = (char*) rowFields[14];
        jobQueue[i]->outputHost3 = (char*) rowFields[15];
        CsvParser_destroy_row(row);
        i++;
    }
    CsvParser_destroy(csvparser);
    return 0;
}


int scheduler(int argc, char* argv[]){

    MSG_process_set_kill_time(MSG_process_self(), 100.0);

    int count = 0;
    input();
    char mailbox[30];
    char destination[50];
    msg_task_t task = NULL;
    int id = (int) xbt_str_parse_int(argv[1], "Invalid argument %s");
    sprintf(mailbox, "scheduler");

    while (1){
        if (count >= 100){
            MSG_process_suspend(MSG_process_self());
            break;
        }
        int res = MSG_task_receive(&task, mailbox);
        XBT_INFO("Receive a pilot from %s", MSG_host_get_name(MSG_task_get_source(task)));

        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        // matching procedure; sending to dispatcher
        sprintf(destination, "%s_%s", MSG_host_get_name(MSG_task_get_source(task)), "ST");
        xbt_dynar_t dynar = MSG_task_get_data(task);
        //xbt_dynar_sort(dynar, comparator);

        xbt_dict_t map = match_task(&dynar);
        XBT_INFO("Start send after matching to %s", destination);
        msg_task_t taskB = MSG_task_create("", 0, MESSAGES_SIZE, map);
        msg_error_t a = MSG_task_send(taskB, destination);

        if (a == MSG_OK){
            XBT_INFO("Send completed");
        }
        else if (a == MSG_HOST_FAILURE){
            XBT_INFO("Problems with host");
            free(map);
            MSG_task_destroy(taskB);
            return 0;
        }
        else if (a == MSG_TRANSFER_FAILURE){
            XBT_INFO("Mmh. Something went wrong with '%s'. Nevermind. Let's keep going!", mailbox);
            free(map);
            MSG_task_destroy(taskB);
        }
        MSG_task_destroy(task);
        task = NULL;
        map = NULL;
        count++;
    }

    return 0;
}

int kotok(){
    int i; jobPtr j1;
    jobPtr* jobBatchData = xbt_new(jobPtr , 10);
    for (i = 0; i < 10; ++i) {
        jobBatchData[i] = j1;
    }
}










































