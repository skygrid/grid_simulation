//
// Created by ken on 12.09.16.
//

#include <string>
#include <simgrid/msg.h>
#include "myfunc_list.h"


msg_sem_t sem_link;

int declare_trace_variables(){
    string variable_name;
    string hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};

    //Declare storage variables for tracing
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 2; ++j) {
            variable_name = hosts[i] + to_string(j);
            TRACE_host_variable_declare(variable_name.c_str());
        }
    }

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

    string storage_variable;

    sem_link = MSG_sem_init(1);
    string hosts[8] = {"CERN", "CNAF", "IN2P3", "RRCKI", "PIC", "RAL", "GRIDKA", "SARA"};
    TRACE_link_srcdst_variable_set("CERN", "CERN", "directUserAmount", 0);
    TRACE_link_srcdst_variable_set("CERN", "CERN", "indirectUserAmount", 0);
    for (int i = 0; i < 8; ++i) {

        //Set all variables of hosts to initial value
        TRACE_host_variable_set(hosts[i].c_str(), "activeCore", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "corruptedCore", 0);

        TRACE_host_variable_set(hosts[i].c_str(), "inputData", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "outputData", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnDisk", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnTape", 0);


        //Set all variables of links to initial value
        for (int j = i+1; j < 8; ++j) {
            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_set(hosts[i].c_str(), hosts[j].c_str(), "traffic", 0);
            TRACE_link_srcdst_variable_set(hosts[i].c_str(), hosts[j].c_str(), "directUserAmount", 0);
            TRACE_link_srcdst_variable_set(hosts[i].c_str(), hosts[j].c_str(), "indirectUserAmount", 0);
            MSG_sem_release(sem_link);
        }

        //Set all variables of storage to initial value
        for (int k = 0; k < 2; ++k) {
            storage_variable = hosts[i] + to_string(k);
            msg_storage_t storage = MSG_storage_get_by_name(storage_variable.c_str());
            TRACE_host_variable_set("CERN", storage_variable.c_str(), MSG_storage_get_used_size(storage));
        }
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnDisk", dataset_number(hosts[i], "1"));
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnTape", dataset_number(hosts[i], "0"));
    }
    return 0;
}

int tracer(int argc, char* argv[]){
    double day = 86400;
    msg_storage_t storage;
    unsigned int cursor;
    xbt_dynar_t storages = MSG_storages_as_dynar();

    while (global_queue.size() > 0){

        xbt_dynar_foreach(storages, cursor, storage){
            string host_name = MSG_storage_get_host(storage);
            string st_name = MSG_storage_get_name(storage);
            xbt_dict_t storage_content = MSG_storage_get_content(storage);

            if (st_name.back() == '1'){
                TRACE_host_variable_set(host_name.c_str(), "datasetOnDisk", xbt_dict_size(storage_content));
            }else{
                TRACE_host_variable_set(host_name.c_str(), "datasetOnTape", xbt_dict_size(storage_content));
            }

            TRACE_host_variable_set("CERN", st_name.c_str(), MSG_storage_get_used_size(storage));
            xbt_dict_free(&storage_content);

        }

        MSG_process_sleep(day);
    }
    xbt_dynar_free(&storages);


    return 0;
}