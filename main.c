//
// Created by ken on 23.05.16.
//

#include <simgrid/msg.h>
#include "messages.h"


int scheduler(int argc, char *argv[]);
int tier1(int argc, char *argv[]);
int set_0_all_routes();

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");


int main(int argc, char *argv[]){
    MSG_init(&argc, argv);

    MSG_create_environment(argv[1]);

    TRACE_link_variable_declare("UserAmount");
    set_0_all_routes();

    MSG_function_register("scheduler", scheduler);
    MSG_function_register("tier1", tier1);
    MSG_launch_application(argv[2]);

    msg_error_t res = MSG_main();

    XBT_INFO("Simulation time %g", MSG_get_clock());

    return res != MSG_OK;
}