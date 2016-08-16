//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>

FILE* storage_fp;
FILE* host_fp;


int tracer_storage(char* storage_name_1, char* storage_type){
    char* storage_name = malloc(15);
    sprintf(storage_name, "%s%s", storage_name_1, storage_type);
    msg_storage_t st = MSG_storage_get_by_name(storage_name);

    TRACE_host_variable_set("CERN", storage_name, MSG_storage_get_used_size(st));

    free(storage_name);
    return 0;
}


