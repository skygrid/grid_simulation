//
// Created by ken on 12.09.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

int trace(){
    char* variable_name = malloc(10);
    char* hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};

    //Declare storage variables for tracing
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 2; ++j) {
            sprintf(variable_name, "%s%d", hosts[i], j);
            TRACE_host_variable_declare(variable_name);
        }
    }
    free(variable_name);

    TRACE_host_variable_declare("activeCore");
    TRACE_host_variable_declare("corruptedCore");
    TRACE_link_variable_declare("traffic");
    TRACE_link_variable_declare("directUserAmount");
    TRACE_link_variable_declare("indirectUserAmount");
    set_0_all_routes();

    return 0;
}

