//
// Created by ken on 14.09.16.
//

#include <simgrid/msg.h>
#include <string>
#include <vector>
#include "myfunc_list.h"
#include "my_structures.h"

std::map<string, FileData*>* name_node;
std::map<std::string, long> storage_number_map;

XBT_LOG_NEW_DEFAULT_CATEGORY(delete_unpopular_file, "messages specific for deletion");

int initialize_file_labels(int argc, char* argv[]){
	/**
		@type simgrid process
		Takes all available files (datasets) on all storages (disks and tapes) and label them.
		To label means -- create a `label` object associated with dataset
		 where information about file (date of creation, date of all accesses to file, etc.) can be stored. 
		 
	*/
    return 0;
    name_node = new std::map<string, FileData*>;
    unsigned int cur;
    char* local_name;
    size_t data_size;
    msg_storage_t st;
    char* storage_name;

    xbt_dynar_t storages = MSG_storages_as_dynar();

    xbt_dynar_foreach(storages, cur, st){
        storage_name = (char*) MSG_storage_get_name(st);

        xbt_dict_t storage_content = MSG_storage_get_content(st);

        // Find how many datasets are stored on each storage
        storage_number_map[(string) storage_name] = xbt_dict_size(storage_content);

        xbt_dict_cursor_t cursor = NULL;

        // break if tape
        if (storage_name[strlen(storage_name)-1] == '0'){
            xbt_dict_free(&storage_content);
            continue;
        }

        xbt_dict_foreach(storage_content, cursor, local_name, data_size){
            string filename = "/" + (string) storage_name + (string) local_name;
            create_file_label(filename);
        }
        xbt_dict_cursor_free(&cursor);
        xbt_dict_free(&storage_content);
    }

    xbt_dynar_free(&storages);

    return 0;
}

/*When new file created*/
int create_file_label(std::string& filename){
	/**
		@type function
		Creation of auxiliary object `FileData` associated with absolute filepath.

        Parameters:
        ----------
        @filename -- label will be created for this name 
	*/
    return 0;
    if (name_node->find(filename) != name_node->end())
        return 0;
    FileData *fileData = new FileData;
    fileData->number_used = 0;
    fileData->used = "0";
    vector<double> *vec_clock = new vector<double>;
    vec_clock->push_back(MSG_get_clock());
    fileData->clocks = vec_clock;
    name_node->insert(make_pair(filename, fileData));
    return 0;
}

/* When file is used by someone*/
void file_usage_counter(std::string& filename){
	/**
		@type function
		Adds time of access(create, read, write) to file's label object
		 and increases counter of file usages by one.

        Parameters:
        ----------
        @filename -- filename 
	*/
    return;
    std::string type;
    unsigned long len = (int) strcspn(filename.c_str(), "10");
    type = filename.at(len);
    if (!type.compare("0")){
        return;
    } else{
        FileData* fileData = (*name_node)[filename];
        fileData->number_used += 1;
        fileData->used = "1";
        fileData->clocks->push_back(MSG_get_clock());
        return;
    }
}


int delete_unpopular_file(int argc, char* argv[]){
	/**
		@type simgrid process
		Every @sleep_time secs deletes files
         which have not been accessed at last @delete_time secs.

        Simgrid process parameters:
        --------------------------
        @argv[1] -- time (in secs) between file deletions
	*/
    return 0;
    double sleep_time = xbt_str_parse_double(argv[1], "error");

    sleep_time = 15 * 86400;
    double delete_time = 365 * 86400;
    while (TRUE){

        MSG_process_sleep(sleep_time);
        std::string filename;
        double current_time = MSG_get_clock();

        for (auto &file_map: *name_node) {
            if ( (!file_map.second->clocks->back()) && (current_time - file_map.second->clocks->back()) >= delete_time && !file_map.second->used.compare("1")){
                filename = file_map.first;
                msg_file_t file = MSG_file_open(filename.c_str(), NULL);

                //unlink file
                MSG_file_unlink(file);
                string host_name = filename.substr(1, strcspn(filename.c_str(), "10") -1);
                dataset_number_change(host_name + "1", -1);

                //delete data
                delete file_map.second;
                name_node->erase(filename);
            }
        }
        if (GLOBAL_QUEUE->empty()){
            break;
        }
    }
    return 0;
}
