//
// Created by ken on 22.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

#define number 5

XBT_LOG_NEW_DEFAULT_CATEGORY(tier2, "Messages specific for tier2");

long current_core_number;
long total_core_number;
msg_host_t* get_free_cores(int required_amount, char* tier2name);

msg_host_t* get_free_cores(int required_amount, char* tier2name){
    int i;
    char hostname[50];

    msg_host_t* required_cores = xbt_new0(msg_host_t, required_amount);
    for (i = 0; i < required_amount; i++){
        sprintf(hostname, "%s%ld", tier2name, (i+current_core_number+1));
        required_cores[i] = MSG_host_by_name(hostname);
        current_core_number++;
    }
    return required_cores;
}



int tier2(int agrc, char* argv[]){
    XBT_INFO("I'am tier2 and ready to work");
    MSG_process_set_kill_time(MSG_process_self(), 100.0);
    msg_task_t task = NULL;
    char mailbox[80];
    int i;
    char *output_name;
    msg_file_t

    int id = xbt_str_parse_int(argv[1], "Invalid argument %s");
    int num = xbt_str_parse_int(argv[2], "Invalid argument %s");
    char* tier2name =  argv[3];

    sprintf(mailbox, "Tier2_%i_ST", id);

    while (1){
        int res = MSG_task_receive(&task, mailbox);
        XBT_INFO("Receive %s after matching", MSG_task_get_name(task));
        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        jobPtr jobInfo= MSG_task_get_data(task);



        // What is needed for data replication?
        char** argx = xbt_new(char*, 6);
        argx[0] = jobInfo->outputName; // file name
        argx[1] = jobInfo->outputHost1; //name of destination1
        argx[2] = jobInfo->outputHost2; //name of destination2
        argx[3] = jobInfo->outputHost3; //name of destination3
        argx[4] = (char*) MSG_host_get_name(MSG_host_self());
        argx[5] = jobInfo->outputNumber; // number of replicas


        // Data replication
        MSG_process_create_with_arguments("dataRep", data_replicator, NULL, MSG_host_self(), 6, argx);

        xbt_free(MSG_task_get_data(task));

        MSG_task_destroy(task);

    }



}