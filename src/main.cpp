//
// Created by ken on 23.05.16.
//

#include <simgrid/msg.h>
#include <chrono>
#include "messages.h"
#include "myfunc_list.h"

int scheduler(int argc, char *argv[]);
int tier1(int argc, char *argv[]);
int evil(int argc, char* argv[]);
int initialize_file_labels(int argc, char* argv[]);
int delete_unpopular_file(int argc, char* argv[]);
int killer(int argc, char* argv[]);
int tracer(int argc, char* argv[]);

FILE* fp;
msg_sem_t sem_requester;

char* path_to_output;
string current_model;

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");

int main(int argc, char *argv[]){

    auto t1 = std::chrono::high_resolution_clock::now();

    MSG_init(&argc, argv);

    MSG_create_environment(argv[1]);

    declare_trace_variables();

    sem_requester = MSG_sem_init(1);

    MSG_function_register("evil", evil);
    MSG_function_register("scheduler", scheduler);
    MSG_function_register("tier1", tier1);
    MSG_function_register("killer", killer);
    MSG_function_register("tracer", tracer);

    MSG_function_register("initialize", initialize_file_labels);
    MSG_function_register("delete_unpop_file", delete_unpopular_file);
    MSG_launch_application(argv[2]);
    path_to_output = argv[3];
    current_model = argv[4];

    msg_error_t res = MSG_main();
    XBT_INFO("Simulation time %f", MSG_get_clock());

    fclose(fp);
    MSG_sem_destroy(sem_requester);
    MSG_sem_destroy(sem_link);

    // Clear info of name node

    std::map<string, FileData*>::iterator file_itr = name_node->begin();
    while (file_itr != name_node->end()){
        delete (*file_itr).second;
        name_node->erase(file_itr);
        file_itr++;
    }

    // delete global_queue and name_node
    delete global_queue;
    delete name_node;

    auto t2 = std::chrono::high_resolution_clock::now();
    XBT_INFO("Real time of simulation: %d seconds", std::chrono::duration_cast<std::chrono::seconds>(t2-t1).count());


    return res != MSG_OK;
}
