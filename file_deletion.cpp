//
// Created by ken on 14.09.16.
//

#include <simgrid/msg.h>
#include <string>
#include <vector>
#include "myfunc_list.h"
#include "messages.h"

map<string, FileData*> name_node;

XBT_LOG_NEW_DEFAULT_CATEGORY(delete_unpopular_file, "messages specific for deletion");

int initialize_file_labels(){

    unsigned int cur;
    char* local_name;
    size_t data_size;
    msg_storage_t st;
    char* storage_name;
    double clock = MSG_get_clock();

    xbt_dynar_t storages = MSG_storages_as_dynar();


    xbt_dynar_foreach(storages, cur, st){
        storage_name = (char*) MSG_storage_get_name(st);

        xbt_dict_t storage_content = MSG_storage_get_content(st);
        xbt_dict_cursor_t cursor = NULL;

        // break if tape
        if (storage_name[strlen(storage_name)-1] == '0') continue;


        xbt_dict_foreach(storage_content, cursor, local_name, data_size){
            string filename = (string) storage_name + (string) local_name;

            FileData* fileData = new FileData();
            vector<double> vec_clock;
            vec_clock.push_back(MSG_get_clock());
            fileData->number_used = 0;
            fileData->clocks = vec_clock;

            name_node.insert(make_pair(filename, fileData));
        }
        xbt_dict_cursor_free(&cursor);
        xbt_dict_free(&storage_content);
    }

    xbt_dynar_free(&storages);

    return 0;
}

int create_file_label(string& filename){
    return 0;
    /*When new file created*/
    FileData *fileData = new FileData;
    fileData->number_used = 0;
    vector<double> vec_clock;
    vec_clock.push_back(MSG_get_clock());
    fileData->clocks = vec_clock;
    name_node.insert(make_pair(filename, fileData));
    return 0;
}

void file_usage_counter(string& filename){
    return;
    /* When file is been using*/
    string type;
    unsigned long len = (int) strcspn(filename.c_str(), "10");
    type = filename.at(len);
    if (!type.compare("0")){
        return;
    } else{
        FileData* fileData = name_node[filename];
        fileData->number_used += 1;
        fileData->used = "1";
        fileData->clocks.push_back(MSG_get_clock());
        return;
    }
}


int delete_unpopular_file(int argc, char* argv[]){

    double sleep_time = xbt_str_parse_double(argv[1], "error");
    while (TRUE){
        MSG_process_sleep(sleep_time);
        double delete_time = 5*86400;
        string filename;

        double current_time = MSG_get_clock();

        for (auto &file_map: name_node) {
            if ( (file_map.second->clocks.back()) && (current_time - file_map.second->clocks.back()) > delete_time){
                filename = file_map.first;
                msg_file_t file = MSG_file_open(filename.c_str(), NULL);
                //unlink file
                MSG_file_unlink(file);
                string host_name = filename.substr(1, strcspn(filename.c_str(), "10") -1);
                minusDatasetAmountT(host_name, "1");

                //delete data
                delete file_map.second;
                name_node.erase(filename);
            }
        }

        if (global_queue.size() == 0){
            break;
        }
    }
    return 0;
}
