//
// Created by ken on 29.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
msg_sem_t sem_link;

XBT_LOG_NEW_DEFAULT_CATEGORY(job_requester, "messages specific for cm");

int job_requester(){
    msg_task_t task;
    double timeout = 20.0;
    long freeCoreAmount;
    long fullCoreAmount = MSG_host_get_core_number(MSG_host_self());

    while (1){
        freeCoreAmount = fullCoreAmount - xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
        if (freeCoreAmount > fullCoreAmount / 2){
            jobBatchRequestPtr jobRequest = xbt_new(jobBatchRequest, 1);
            jobRequest->coreAmount = freeCoreAmount;

            task = MSG_task_create("request", 0.0, MESSAGES_SIZE, jobRequest);


            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_add(MSG_host_get_name(MSG_host_self()), "CERN", "UserAmount", 1);
            MSG_sem_release(sem_link);

            msg_error_t a = MSG_task_send(task, "scheduler");

            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_sub(MSG_host_get_name(MSG_host_self()), "CERN", "UserAmount", 1);
            MSG_sem_release(sem_link);

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

