//
// Created by ken on 24.05.16.
//

#include <stdio.h>
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

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
            //XBT_INFO("Get job request from %s", MSG_host_get_name(MSG_task_get_source(task)));
            jobBatchRequestPtr batchRequest = (jobBatchRequestPtr) MSG_task_get_data(task);

            jobBatch_AmountPtr batch = matcher(batchRequest->coreAmount);
            //jobBatch_AmountPtr batch = matcher_DAM(batchRequest->coreAmount, MSG_host_get_name(MSG_task_get_source(task)));
            taskB = MSG_task_create("", batch->jobsAmount, MESSAGES_SIZE, batch->jobBatch);
            //Add new user to link
            plusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));

            switch(MSG_task_send(taskB, MSG_host_get_name(MSG_task_get_source(task)))){
                case MSG_OK:
                    minusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));
                    //XBT_INFO("Send %d jobs after matching to %s", batch->jobsAmount ,MSG_host_get_name(MSG_task_get_source(task)));
                    break;
                case MSG_TRANSFER_FAILURE:
                    //rescheduling(batchRequest->coreAmount);
                    MSG_task_destroy(taskB);
                    taskB = NULL;
                    break;
                case MSG_HOST_FAILURE:
                    MSG_task_destroy(taskB);
                    taskB = NULL;
                    break;
            }

            if(!strcmp(MSG_task_get_name(task), "finalize")){
                MSG_task_destroy(task);
                break;
            }
            xbt_free(batchRequest);
            MSG_task_destroy(task);
            task = NULL;

        }else if (res == MSG_TRANSFER_FAILURE){
            XBT_INFO("transfer failure occur");
        }
    }
}
