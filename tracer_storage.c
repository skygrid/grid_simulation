//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>

int tracer_storage(){
    unsigned int cur, cur1;
    msg_storage_t st, st1;
    double timeout = 50;

    xbt_dynar_t storages = MSG_storages_as_dynar();
    xbt_dynar_foreach(storages, cur, st){
        TRACE_host_variable_declare(MSG_storage_get_name(st));
        TRACE_host_variable_set(MSG_host_get_name(MSG_host_self()), MSG_storage_get_name(st), MSG_storage_get_used_size(st));
    }

    xbt_dynar_foreach(storages, cur1, st1){
        TRACE_host_variable_set(MSG_host_get_name(MSG_host_self()), MSG_storage_get_name(st), MSG_storage_get_used_size(st));
    }

}

