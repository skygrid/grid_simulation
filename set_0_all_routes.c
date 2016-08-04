//
// Created by ken on 04.08.16.
//

#include <simgrid/msg.h>

int set_0_all_routes(){
    char* hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};
    for (int i = 0; i < 8; ++i) {
        for (int j = i+1; j < 8; ++j) {
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "UserAmount", 0);
        }
    }
    return 0;
}
