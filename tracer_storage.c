//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include <stdio.h>

FILE* storage_fp;
FILE* host_fp;


int tracer_storage(){
    unsigned int cur;
    msg_storage_t st;
    xbt_dynar_t storages = MSG_storages_as_dynar();
    xbt_dynar_foreach(storages, cur, st){
        fprintf(storage_fp, "%s,%f,%f\n", MSG_storage_get_name(st), (double) MSG_storage_get_used_size(st),  MSG_get_clock());
    }
    return 0;
}



