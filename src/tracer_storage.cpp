//
// Created by ken on 09.08.16.
//
#include <string>
#include <simgrid/msg.h>
#include "myfunc_list.h"

map<std::string, double > cumulative_input_site;
map<std::string, double > cumulative_output_site;

XBT_LOG_NEW_DEFAULT_CATEGORY(dataset, "messages specific for dataset");

int tracer_storage(std::string& hostname, std::string storage_type){
	
    std::string storage_name = hostname + storage_type;
    msg_storage_t st = MSG_storage_get_by_name(storage_name.c_str());

    TRACE_host_variable_set("CERN", storage_name.c_str(), MSG_storage_get_used_size(st));

    return 0;
}

void cumulative_input_per_site(const std::string& host_name, double size){
    MSG_sem_acquire(sem_link);
    cumulative_input_site[host_name] += size;
    MSG_sem_release(sem_link);
    return;
}

void cumulative_output_per_site(const std::string& host_name, double size){
    MSG_sem_acquire(sem_link);
    cumulative_output_site[host_name] += size;
    MSG_sem_release(sem_link);
    return;
}

void dataset_number_change(const std::string& storage_name, int change){
    /*
     * -1 delete file
     * +1 add file*/
    MSG_sem_acquire(sem_link);
    switch (change){
        case 1:
            storage_number_map[storage_name]++;
            break;
        case -1:
            storage_number_map[storage_name]--;
            break;
        default:
            break;
    }
    MSG_sem_release(sem_link);
    return;
}


int addDatasetAmountT(std::string& host_name, std::string type){

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

int minusDatasetAmountT(std::string& host_name, std::string type){
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
long dataset_number(std::string& host_name, std::string storage_type){

    std::string storage_name = host_name + storage_type;
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

