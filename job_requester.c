//
// Created by ken on 29.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
msg_sem_t sem_link;
int anomalyLinkTracer(const char* src, const char* dst);

XBT_LOG_NEW_DEFAULT_CATEGORY(job_requester, "messages specific for cm");


int job_requester(){
    msg_task_t task;
    double timeout = 20.0;
    long freeCoreAmount;
    int fullCoreAmount = MSG_host_get_core_number(MSG_host_self());
    MSG_process_sleep(0.01);

    while (1){
        freeCoreAmount = fullCoreAmount - xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error") -
                xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
        if (freeCoreAmount > 20){
            jobBatchRequestPtr jobRequest = xbt_new(jobBatchRequest, 1);
            jobRequest->coreAmount = freeCoreAmount;

            task = MSG_task_create("request", 0.0, MESSAGES_SIZE, jobRequest);

            plusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");

            msg_error_t a = MSG_task_send(task, "scheduler");

            if (a == MSG_OK){
                minusLinkCounter(MSG_host_get_name(MSG_host_self()), "CERN");
            }else if (a == MSG_TRANSFER_FAILURE){
                anomalyLinkTracer(MSG_host_get_name(MSG_host_self()), "CERN");
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


