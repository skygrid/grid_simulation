//
// Created by ken on 29.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(core_monitoring, "messsages specific for cm");

long core_monitoring(){
    msg_task_t task;
    double timeout = 10.0;
    long freeCoreAmount;
    long fullCoreAmount = MSG_host_get_core_number(MSG_host_self());
    MSG_process_sleep(timeout);

    while (1){
        freeCoreAmount = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
        if (freeCoreAmount > fullCoreAmount / 2){
            jobBatchRequestPtr jobRequest = xbt_new(jobBatchRequest, 1);
            jobRequest->coreAmount = freeCoreAmount;

            task = MSG_task_create("request", 0.0, MESSAGES_SIZE, jobRequest);
            MSG_task_send(task, "scheduler");
        }
        MSG_process_sleep(timeout);
        if (freeCoreAmount > fullCoreAmount){
            break;
        }
    }
    return 0;
}

