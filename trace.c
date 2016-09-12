//
// Created by ken on 12.09.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

int declare_trace_variables(){
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

    // Input and output data per site
    TRACE_host_variable_declare("inputData");
    TRACE_host_variable_declare("outputData");

    TRACE_host_variable_declare("datasetOnDisk");
    TRACE_host_variable_declare("datasetOnTape");
    set_0_all_routes();

    return 0;
}

int set_0_all_routes(){

    char* storage_variable = malloc(15);

    sem_link = MSG_sem_init(1);
    char* hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};
    TRACE_link_srcdst_variable_set("CERN", "CERN", "directUserAmount", 0);
    TRACE_link_srcdst_variable_set("CERN", "CERN", "indirectUserAmount", 0);
    for (int i = 0; i < 8; ++i) {

        //Set all variables of hosts to initial value
        TRACE_host_variable_set(hosts[i], "activeCore", 0);
        TRACE_host_variable_set(hosts[i], "corruptedCore", 0);

        TRACE_host_variable_set(hosts[i], "inputData", 0);
        TRACE_host_variable_set(hosts[i], "outputData", 0);
        TRACE_host_variable_set(hosts[i], "datasetOnDisk", 0);
        TRACE_host_variable_set(hosts[i], "datasetOnTape", 0);


        //Set all variables of links to initial value
        for (int j = i+1; j < 8; ++j) {
            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "traffic", 0);
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "directUserAmount", 0);
            TRACE_link_srcdst_variable_set(hosts[i], hosts[j], "indirectUserAmount", 0);
            MSG_sem_release(sem_link);
        }

        //Set all variables of storage to initial value
        for (int k = 0; k < 2; ++k) {
            sprintf(storage_variable, "%s%d", hosts[i], k);
            msg_storage_t storage = MSG_storage_get_by_name(storage_variable);
            TRACE_host_variable_set("CERN", storage_variable, MSG_storage_get_used_size(storage));
        }

        TRACE_host_variable_set(hosts[i], "datasetOnDisk", dataset_number(hosts[i], "1"));
        TRACE_host_variable_set(hosts[i], "datasetOnTape", dataset_number(hosts[i], "0"));
    }

    return 0;
}

