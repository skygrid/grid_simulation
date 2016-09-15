//
// Created by ken on 14.09.16.
//

#include <simgrid/msg.h>
#include "messages.h"

int initialize_file_labels(){

    unsigned int cur;
    char* local_name;
    size_t data_size;
    msg_storage_t st;
    char* storage_name;

    xbt_dynar_t storages = MSG_storages_as_dynar();
    xbt_dynar_foreach(storages, cur, st){
        storage_name = (char*) MSG_storage_get_name(st);

        xbt_dict_t storage_content = MSG_storage_get_content(st);
        xbt_dict_cursor_t cursor = NULL;

        xbt_dict_foreach(storage_content, cursor, local_name, data_size){
            char *filename = malloc(40);
            sprintf("%s%s", filename, storage_name, local_name);
            msg_file_t file = MSG_file_open(filename, NULL);

            fileDataPtr data = xbt_new(fileData, 1);
            xbt_dynar_t dynar = xbt_dynar_new(sizeof(double), NULL); // !!!!!!!!!!!!!!!!!!!!!!!!
            data->number_used = 0;
            data->all_using_clock = &dynar;

            MSG_file_set_data(file, data);
            MSG_file_close(file);
            free(filename);
        }

        xbt_dict_cursor_free(&cursor);
        xbt_dict_free(&storage_content);
    }


    return 0;
}





int delete_unpopular_file(int argc, char* argv[]){
    double sleep_time = xbt_str_parse_double(argv[1], "error");

    while (TRUE){
        double delete_time = 5 * 86400;

        double current_time = MSG_get_clock();
        unsigned int cur;
        char* local_name;
        size_t data_size;
        msg_storage_t st;
        char* storage_name;


        xbt_dynar_t storages = MSG_storages_as_dynar();
        xbt_dynar_foreach(storages, cur, st){
            storage_name = (char*) MSG_storage_get_name(st);

            xbt_dict_t storage_content = MSG_storage_get_content(st);
            xbt_dict_cursor_t cursor = NULL;

            xbt_dict_foreach(storage_content, cursor, local_name, data_size){
                char *filename = malloc(40);
                sprintf("%s%s", filename, storage_name, local_name);
                msg_file_t file = MSG_file_open(filename, NULL);
                fileDataPtr data = MSG_file_get_data(file);
                double last_used_time = xbt_dynar_getlast_as(*(data->all_using_clock), double);
                if ((last_used_time + delete_time) < current_time){
                    MSG_file_rmove(file, MSG_host_by_name(MSG_storage_get_host(st)), filename);
                    xbt_dynar_free(data->all_using_clock);
                    xbt_free(data);
                }
                MSG_file_close(file);
                free(filename);
            }

            xbt_dict_cursor_free(&cursor);
            xbt_dict_free(&storage_content);
        }

        xbt_dynar_free_container(&storages);

        MSG_process_sleep(sleep_time);

        // redundant
        if (sleep_time == delete_time){
            break;
        }
    }

    return 0;
}