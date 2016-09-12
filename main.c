//
// Created by ken on 23.05.16.
//

#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

int scheduler(int argc, char *argv[]);
int tier1(int argc, char *argv[]);
int evil(int argc, char* argv[]);

FILE* fp;
FILE* storage_fp;

char* path_to_output;

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");

int main(int argc, char *argv[]){
    MSG_init(&argc, argv);

    MSG_create_environment(argv[1]);

    declare_trace_variables();

    MSG_function_register("evil", evil);
    MSG_function_register("scheduler", scheduler);
    MSG_function_register("tier1", tier1);
    MSG_launch_application(argv[2]);
    path_to_output = argv[3];

    msg_error_t res = MSG_main();
    XBT_INFO("Simulation time %g", MSG_get_clock());
    MSG_process_killall(-1);
    fclose(fp);
    fclose(storage_fp);
    return res != MSG_OK;
}