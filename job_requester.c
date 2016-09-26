//
// Created by ken on 29.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(job_requester, "messages specific for cm");


int job_requester(){
    msg_task_t task;
    double timeout = 1000;
    long freeCoreAmount;
    int fullCoreAmount = MSG_host_get_core_number(MSG_host_self());
    MSG_process_sleep(0.01);

    while (1){
        freeCoreAmount = fullCoreAmount - xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error") -
                xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
        if (freeCoreAmount > 4){
            jobBatchRequestPtr jobRequest = xbt_new(jobBatchRequest, 1);
            jobRequest->coreAmount = freeCoreAmount;

            task = MSG_task_create("request", 0.0, MESSAGES_SIZE, jobRequest);

            plusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");

            switch(MSG_task_send(task, "scheduler")){
                case MSG_OK:
                    minusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");
                    break;
                case MSG_TRANSFER_FAILURE:
                    minusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");
                    MSG_task_destroy(task);
                    task = NULL;
                    break;
                case MSG_HOST_FAILURE:
                    MSG_task_destroy(task);
                    task = NULL;
                    break;
            }
        }
        MSG_process_sleep(timeout);
        if (freeCoreAmount > fullCoreAmount){
            break;
        }
    }
    return 0;
}


