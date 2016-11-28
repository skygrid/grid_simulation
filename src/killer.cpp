//
// Created by ken on 09.10.16.
//

#include <simgrid/msg.h>
#include <string>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(killer, "messages specific for killer");

int killer(int argc, char* argv[]){
    int tier2_amount = 16;
    string hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};

    string tier2s[16];
    for (int i = 1; i < tier2_amount+1; ++i) {
        tier2s[i-1] = "Tier2_" + to_string(i);
    }

    double timeout = 1000;
    msg_task_t task = NULL;
    msg_task_t scheduler_task = NULL;

    while (TRUE){
        if (global_queue->empty()){
            MSG_process_sleep(100.);

            // Kill tier0 and tier1s
            for (int i = 0; i < 8; ++i) {
                task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
                MSG_task_send(task, hosts[i].c_str());
            }

            // kill tier2s
            for (int j = 1; j < tier2_amount+1; ++j) {
                task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
                MSG_task_send(task, tier2s[j-1].c_str());
            }

            scheduler_task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
            MSG_task_send(scheduler_task, "scheduler");
            break;
        }
        MSG_process_sleep(timeout);
    }
    return 0;
}

int die_or_not(){
    MSG_sem_acquire(sem_requester);
    if (global_queue->empty()){
        MSG_process_kill(MSG_process_self());
        MSG_sem_release(sem_requester);
    }
    MSG_sem_release(sem_requester);
    return 0;
}