//
// Created by ken on 14.09.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"
#include "messages.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(delete_unpopular_file, "messages specific for deletion");
xbt_dict_t dict;

int initialize_file_labels(){

    unsigned int cur;
    char* local_name;
    size_t data_size;
    msg_storage_t st;
    char* storage_name;
    double clock = MSG_get_clock();

    dict = xbt_dict_new();
    xbt_dynar_t storages = MSG_storages_as_dynar();
    xbt_dynar_foreach(storages, cur, st){
        storage_name = (char*) MSG_storage_get_name(st);

        xbt_dict_t storage_content = MSG_storage_get_content(st);
        xbt_dict_cursor_t cursor = NULL;

        // break if tape
        if (storage_name[strlen(storage_name)-1] == '0') break;


        xbt_dict_foreach(storage_content, cursor, local_name, data_size){
            char *filename = malloc(40);
            sprintf(filename, "/%s%s", storage_name, local_name);

            fileDataPtr data = xbt_new(fileData, 1);
            xbt_dynar_t dynar = xbt_dynar_new(sizeof(double), NULL); // !!!!
            xbt_dynar_push_as(dynar, double, clock);
            data->number_used = 0;
            data->all_using_clock = dynar;

            xbt_dict_set(dict, filename, data, xbt_free_f);
            free(filename);
        }

        xbt_dict_cursor_free(&cursor);
        xbt_dict_free(&storage_content);
    }

    return 0;
}

int create_file_label(char* filename){
    return 0;

    double clock = MSG_get_clock();

    xbt_dynar_t dynar = xbt_dynar_new(sizeof(double), NULL);
    xbt_dynar_push_as(dynar, double, clock);

    fileDataPtr data = xbt_new(fileData, 1);
    data->number_used = 0;
    data->all_using_clock = dynar;
    xbt_dict_set(dict, filename, data, xbt_free_f);

    return 0;
}

int check_file_availability(char* filename){
    // 0 -- file doesn't exist
    // 1 -- file exists

    msg_file_t file = MSG_file_open(filename, NULL);
    if (MSG_file_get_size(file) == 0){
        return 0;
    } else return 1;
}



int delete_unpopular_file(int argc, char* argv[]){
    double sleep_time = xbt_str_parse_double(argv[1], "error");
    msg_file_t file;
    while (TRUE){
        MSG_process_sleep(sleep_time);
        double delete_time = 5 * 86400;

        double current_time = MSG_get_clock();
        char* filename;
        size_t data_size;

        xbt_dict_cursor_t cursor = NULL;
        xbt_dict_foreach(dict, cursor, filename, data_size){

            fileDataPtr data = xbt_dict_get(dict, filename);
            double last_used_time = xbt_dynar_getlast_as(data->all_using_clock, double);

            if (NULL == data->used) break;

            if ((last_used_time + delete_time) < current_time){
                //XBT_INFO("DELETE FILE");
                file = MSG_file_open(filename, NULL);
                MSG_file_unlink(file);
                //xbt_free(&data->number_used);
                //xbt_dynar_free(&data->all_using_clock);
                //xbt_free(data);
                xbt_dict_remove(dict, filename);
            }
        }

        xbt_dict_cursor_free(&cursor);
        // redundant
        if (sleep_time == delete_time){
            break;
        }
    }
    return 0;
}

char* find_host(char* filename){
    size_t sz = strlen(filename);
    char *x = malloc(sz);
    snprintf(x, sz, filename + 1);
    const char* t = "10";
    int len = (int) strcspn(filename, t);
    x[len-1] = '\0';
    return x;
}


int storage_delete_unpopular_file(int argc, char* argv[]){
    double sleep_time = xbt_str_parse_double(argv[1], "error");

    while (TRUE){
        MSG_process_sleep(sleep_time);
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
                sprintf(filename, "/%s%s", storage_name, local_name);

                fileDataPtr data = xbt_dict_get(dict, filename);
                double last_used_time = xbt_dynar_getlast_as(data->all_using_clock, double);

                if ((last_used_time + delete_time) < current_time){
                    XBT_INFO("DELETE FILE");
                    msg_file_t file = MSG_file_open(filename, NULL);
                    MSG_file_rmove(file, MSG_host_by_name(MSG_storage_get_host(st)), filename);
                    MSG_file_close(file);

                    xbt_free(&data->number_used);
                    xbt_dynar_free(&data->all_using_clock);
                    xbt_dict_remove(dict, filename);
                    xbt_free(data);
                }
                free(filename);
            }

            xbt_dict_cursor_free(&cursor);
            xbt_dict_free(&storage_content);
        }

        xbt_dynar_free_container(&storages);

        // redundant
        if (sleep_time == delete_time){
            break;
        }
    }
    return 0;
}

