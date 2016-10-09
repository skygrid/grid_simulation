//
// Created by ken on 09.10.16.
//

#include <simgrid/msg.h>
#include <string>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(killer, "messages specific for killer");

int killer(int argc, char* argv[]){
    string hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};
    double timeout = 1000;
    msg_task_t task = NULL;

    while (TRUE){
        if (global_queue.empty()){
            MSG_process_sleep(100.);

            for (int i = 0; i < hosts->length(); ++i) {
                task = MSG_task_create("finalize", 0, MESSAGES_SIZE, NULL);
                MSG_task_send(task, hosts[i].c_str());
                XBT_INFO("Send to: %s", hosts[i].c_str());
            }

            break;
        }
        MSG_process_sleep(timeout);
    }
    return 0;
}