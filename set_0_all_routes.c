//
// Created by ken on 04.08.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

msg_sem_t sem_link;
int set_0_all_routes(){
    sem_link = MSG_sem_init(1);
    char* hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};
    TRACE_link_srcdst_variable_set("CERN", "CERN", "directUserAmount", 0);
    TRACE_link_srcdst_variable_set("CERN", "CERN", "indirectUserAmount", 0);
    for (int i = 0; i < 8; ++i) {

        TRACE_host_variable_set(hosts[i], "activeCore", 0);
        TRACE_host_variable_set(hosts[i], "corruptedCore", 0);

        for (int j = i+1; j < 8; ++j) {
            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "directUserAmount", 0);
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "indirectUserAmount", 0);
            MSG_sem_release(sem_link);
        }
    }

    return 0;
}
