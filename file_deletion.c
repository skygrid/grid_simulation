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
        if (storage_name[strlen(storage_name)-1] == '0') continue;


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
    double clock = MSG_get_clock();

    xbt_dynar_t dynar = xbt_dynar_new(sizeof(double), NULL);
    xbt_dynar_push_as(dynar, double, clock);

    fileDataPtr data = xbt_new(fileData, 1);
    data->number_used = 0;
    data->all_using_clock = dynar;
    xbt_dict_set(dict, filename, data, xbt_free_f);

    return 0;
}

void file_usage_counter(char* filename){
    char* type = malloc(30);
    int len = (int) strcspn(filename, "10");
    sprintf(type, "%s", filename+len);
    type[1] = '\0';
    if (!strcmp(type, "0")){
        return;
    } else{
        double clock = MSG_get_clock();
        fileDataPtr data = xbt_dict_get(dict, filename);
        data->number_used += 1;
        data->used = "1";
        xbt_dynar_push_as(data->all_using_clock, double, clock);
        return;
    }
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

int delete_unpopular_file(int argc, char* argv[]){
    double sleep_time = xbt_str_parse_double(argv[1], "error");
    msg_file_t file;
    while (TRUE){
        MSG_process_sleep(sleep_time);
        double delete_time = 5*86400;

        double current_time = MSG_get_clock();
        char* filename;
        fileDataPtr data;

        xbt_dict_cursor_t cursor = NULL;

        xbt_dict_foreach(dict, cursor, filename, data){
            double last_used_time = xbt_dynar_getlast_as(data->all_using_clock, double);

            if (NULL == data->used) continue;

            if ((last_used_time + delete_time) < current_time){
                XBT_INFO("delete %s", filename);
                file = MSG_file_open(filename, NULL);
                minusDatasetAmountT(find_host(filename), "1");
                MSG_file_unlink(file);
                xbt_dict_remove(dict, filename);
            }
        }

        xbt_dict_cursor_free(&cursor);
        if (length() == 0){
            break;
        }
    }
    return 0;
}
