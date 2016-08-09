//
// Created by ken on 29.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
#include "global_vabiables.h"
msg_sem_t sem_link;

XBT_LOG_NEW_DEFAULT_CATEGORY(job_requester, "messages specific for cm");

int fullCoreAmount;

int job_requester(){
    msg_task_t task;
    double timeout = 20.0;
    long freeCoreAmount;
    fullCoreAmount = MSG_host_get_core_number(MSG_host_self());

    while (1){
        freeCoreAmount = fullCoreAmount - xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
        if (freeCoreAmount > 20){ //fullCoreAmount / 2
            jobBatchRequestPtr jobRequest = xbt_new(jobBatchRequest, 1);
            jobRequest->coreAmount = freeCoreAmount;

            task = MSG_task_create("request", 0.0, MESSAGES_SIZE, jobRequest);

            plusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");

            msg_error_t a = MSG_task_send(task, "scheduler");

            minusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");

            if (a == MSG_OK){
            }else if (a == MSG_TRANSFER_FAILURE){
                MSG_task_destroy(task);
                task = NULL;
            } else if (a == MSG_HOST_FAILURE){
                MSG_task_destroy(task);
                task = NULL;
            }
        }
        MSG_process_sleep(timeout);
        if (freeCoreAmount > fullCoreAmount){
            break;
        }
    }
    return 0;
}


