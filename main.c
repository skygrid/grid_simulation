//
// Created by ken on 23.05.16.
//

#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"


int scheduler(int argc, char *argv[]);
int tier1(int argc, char *argv[]);
int evil(int argc, char* argv[]);
int main_tracer();

XBT_LOG_NEW_DEFAULT_CATEGORY(msg_app_masterworker, "Messages specific for this msg example");

FILE* fp;
FILE* storage_fp;


int main(int argc, char *argv[]){
    MSG_init(&argc, argv);

    MSG_create_environment(argv[1]);

    TRACE_host_variable_declare("activeCore");
    TRACE_host_variable_declare("corruptedCore");
    TRACE_link_variable_declare("directUserAmount");
    TRACE_link_variable_declare("indirectUserAmount");
    set_0_all_routes();

    MSG_function_register("main_tracer", main_tracer);
    MSG_function_register("evil", evil);
    MSG_function_register("scheduler", scheduler);
    MSG_function_register("tier1", tier1);
    MSG_launch_application(argv[2]);

    msg_error_t res = MSG_main();
    XBT_INFO("Simulation time %g", MSG_get_clock());
    MSG_process_killall(-1);
    fclose(fp);
    fclose(storage_fp);
    return res != MSG_OK;
}