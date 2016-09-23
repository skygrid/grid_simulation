//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"

FILE* storage_fp;

XBT_LOG_NEW_DEFAULT_CATEGORY(dataset, "messages specific for dataset");

int tracer_storage(char* hostname, char* storage_type){
    char* storage_name = malloc(15);
    sprintf(storage_name, "%s%s", hostname, storage_type);
    msg_storage_t st = MSG_storage_get_by_name(storage_name);

    TRACE_host_variable_set("CERN", storage_name, MSG_storage_get_used_size(st));

    free(storage_name);
    return 0;
}

void cumulativeInputPerSiteT(const char* host_name, double size){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(host_name, "inputData", size);
    MSG_sem_release(sem_link);
    return;
}

void cumulativeOutputPerSiteT(const char* host_name, double size){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(host_name, "outputData", size);
    MSG_sem_release(sem_link);
    return;
}

int addDatasetAmountT(const char* host_name, char* type){

    MSG_sem_acquire(sem_link);
    // O -- Tape
    if (!strcmp(type, "0")){
        //TRACE_host_variable_add(host_name, "datasetOnTape", 1);
        TRACE_host_variable_set(host_name, "datasetOnTape", dataset_number(host_name, "0"));
    }else{
        //TRACE_host_variable_add(host_name, "datasetOnDisk", 1);
        TRACE_host_variable_set(host_name, "datasetOnDisk", dataset_number(host_name, "1"));
    }

    MSG_sem_release(sem_link);
    return 0;
}

int minusDatasetAmountT(char* host_name, char* type){
    MSG_sem_acquire(sem_link);
    // O -- Tape
    if (!strcmp(type, "0")){
        TRACE_host_variable_set(host_name, "datasetOnTape", dataset_number(host_name, "0"));
        //TRACE_host_variable_sub(host_name, "datasetOnTape", 1);
    }else{
        TRACE_host_variable_set(host_name, "datasetOnDisk", dataset_number(host_name, "1"));
        //TRACE_host_variable_sub(host_name, "datasetOnDisk", 1);
    }
    free(host_name);
    MSG_sem_release(sem_link);
    return 0;

}

// Returns a number of dataset on a given storage
long dataset_number(char* host_name, char* storage_type){

    char* storage_name = malloc(15);
    sprintf(storage_name, "%s%s", host_name, storage_type);
    msg_storage_t st = MSG_storage_get_by_name(storage_name);

    xbt_dict_cursor_t cursor = NULL;
    char *key;
    double data;
    long amount = 0;

    xbt_dict_t storage_content = MSG_storage_get_content(st);
    xbt_dict_foreach(storage_content, cursor, key, data){
        amount++;
    }
    free(storage_name);
    xbt_dict_free(&storage_content);
    xbt_dict_cursor_free(&cursor);
    return amount;

}





