//
// Created by ken on 09.08.16.
//
#include <string>
#include <simgrid/msg.h>
#include "myfunc_list.h"


XBT_LOG_NEW_DEFAULT_CATEGORY(dataset, "messages specific for dataset");

int tracer_storage(string hostname, string storage_type){
    string storage_name = hostname + storage_type;
    msg_storage_t st = MSG_storage_get_by_name(storage_name.c_str());

    TRACE_host_variable_set("CERN", storage_name.c_str(), MSG_storage_get_used_size(st));

    return 0;
}

void cumulativeInputPerSiteT(string host_name, double size){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(host_name.c_str(), "inputData", size);
    MSG_sem_release(sem_link);
    return;
}

void cumulativeOutputPerSiteT(string host_name, double size){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(host_name.c_str(), "outputData", size);
    MSG_sem_release(sem_link);
    return;
}

int addDatasetAmountT(string host_name, string type){

    MSG_sem_acquire(sem_link);
    // O -- Tape
    if (!type.compare("0")){
        TRACE_host_variable_add(host_name.c_str(), "datasetOnTape", 1);
        //TRACE_host_variable_set(host_name, "datasetOnTape", dataset_number(host_name, "0"));
    }else{
        TRACE_host_variable_add(host_name.c_str(), "datasetOnDisk", 1);
        //TRACE_host_variable_set(host_name, "datasetOnDisk", dataset_number(host_name, "1"));
    }

    MSG_sem_release(sem_link);
    return 0;
}

int minusDatasetAmountT(string host_name, string type){
    MSG_sem_acquire(sem_link);
    // O -- Tape
    if (!type.compare("0")){
        TRACE_host_variable_sub(host_name.c_str(), "datasetOnTape", 1);
        //TRACE_host_variable_set(host_name, "datasetOnTape", dataset_number(host_name, "0"));
    }else{
        TRACE_host_variable_sub(host_name.c_str(), "datasetOnDisk", 1);
        //TRACE_host_variable_set(host_name, "datasetOnDisk", dataset_number(host_name, "1"));
    }
    MSG_sem_release(sem_link);
    return 0;

}

// Returns a number of dataset on a given storage
long dataset_number(string host_name, string storage_type){

    string storage_name = host_name + storage_type;
    msg_storage_t st = MSG_storage_get_by_name(storage_name.c_str());

    xbt_dict_cursor_t cursor = NULL;
    char *key;
    double data;
    long amount = 0;

    xbt_dict_t storage_content = MSG_storage_get_content(st);
    xbt_dict_foreach(storage_content, cursor, key, data){
        amount++;
    }

    xbt_dict_free(&storage_content);
    xbt_dict_cursor_free(&cursor);
    return amount;

}





