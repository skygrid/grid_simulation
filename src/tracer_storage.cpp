//
// Created by ken on 09.08.16.
//
#include <string>
#include <simgrid/msg.h>
#include "myfunc_list.h"

std::map<std::string, double > cumulative_input_site;
std::map<std::string, double > cumulative_output_site;

XBT_LOG_NEW_DEFAULT_CATEGORY(dataset, "messages specific for dataset");

int tracer_storage(std::string& hostname, std::string& storage_type){
    /**
        Adds used size of @storage_name to trace file. 

        Parameters:
        -----------
        @hostname -- host name
        @storage_type -- TAPE or DISK
    */
	
    std::string storage_name = hostname + storage_type;
    msg_storage_t st = MSG_storage_get_by_name(storage_name.c_str());

    TRACE_host_variable_set("CERN", storage_name.c_str(), MSG_storage_get_used_size(st));

    return 0;
}

void cumulative_input_per_site(const std::string& host_name, double size){
    /**
        Add @size to cumulative input traffic of site.

        Parameters:
        -----------
        @host_name -- name of host which downloads file
        @size -- size of downloaded file
    */
    MSG_sem_acquire(sem_link);
    cumulative_input_site[host_name] += size;
    MSG_sem_release(sem_link);
    return;
}

void cumulative_output_per_site(const std::string& host_name, double size){
    /**
        Add @size to cumulative output traffic of site.
        
        Parameters:
        -----------
        @host_name -- name of host which uploads file
        @size -- file size
    */
    MSG_sem_acquire(sem_link);
    cumulative_output_site[host_name] += size;
    MSG_sem_release(sem_link);
    return;
}

void dataset_number_change(const std::string& storage_name, int change){
    /**
        Changes number of datasets on @storage_name.
        * -1 -- delete file
        * +1 -- add file

        Parameters:
        -----------
        @storage_name -- name of storage
        @change -- number of new datasets (or deleted datasets)
    */
    
    MSG_sem_acquire(sem_link);
    switch (change){
        case 1:
            ++storage_number_map[storage_name];
            break;
        case -1:
            --storage_number_map[storage_name];
            break;
        default:
            break;
    }
    MSG_sem_release(sem_link);
    return;
}


int addDatasetAmountT(std::string& host_name, std::string& type){
    /**
        Increases number of datasets on @host_name by one.

        Parameters:
        ----------
        @host_name -- name of host
        @type -- DISK or TAPE
    */
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
    /**
        Decreases number of datasets on @host_name by one.
         
        Parameters:
        ----------
        @host_name -- name of host
        @type -- DISK or TAPE
    */
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

long dataset_number(std::string& host_name, std::string storage_type){
    /**
        Returns a number of dataset on a given storage.
         
        Parameters:
        ----------
        @host_name -- name of host
        @storage_type -- DISK or TAPE
    */
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

