//
// Created by ken on 04.08.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

msg_sem_t sem_link;
int set_0_all_routes(){
    sem_link = MSG_sem_init(1);
    char* hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};
    TRACE_link_srcdst_variable_set("CERN", "CERN", "UserAmount", 0);
    for (int i = 0; i < 8; ++i) {
        for (int j = i+1; j < 8; ++j) {
            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "UserAmount", 0);
            MSG_sem_release(sem_link);
        }
    }
    MSG_process_killall(-1);
    return 0;
}
