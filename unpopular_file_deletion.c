//
// Created by ken on 14.09.16.
//

#include <simgrid/msg.h>

int delete_unpopular_file(){
    unsigned int cur;
    msg_storage_t st;

    xbt_dynar_t storages = MSG_storages_as_dynar();
    xbt_dynar_foreach(storages, cur, st){

    }

    xbt_dynar_free_container(&storages);


    return 0;
}