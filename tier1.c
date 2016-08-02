#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(tier1, "messsages specific for tier1");
//
// Created by ken on 29.07.16.
//
int executorLauncher();
int job_requester();

// MAIN TIER1 FUNCTION
int tier1(int argc, char* argv[]){
    char* tierMailbox = argv[1];
    char** argx = xbt_new(char*, 1);
    argx[0] = tierMailbox;

    // LAUNCH PROCESS
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup("0"), NULL);
    MSG_process_create("tier1_executor", executorLauncher, argx, MSG_host_self());
    MSG_process_create("job_requester", job_requester, NULL, MSG_host_self());

    return 0;
}


int executorLauncher(){
    int i;
    msg_task_t task;
    char* tierMailbox = strdup(((char**) MSG_process_get_data(MSG_process_self()))[0]);

    while (1){
        int a = MSG_task_receive(&task, tierMailbox);
        if (a == MSG_OK){
        }
        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        int jobAmount = 1;// (int) MSG_task_get_flops_amount(task);
        jobPtr* jobPtrBatchData = MSG_task_get_data(task);
        XBT_INFO("Successfully receive %s", jobPtrBatchData[0]->name);
        //LAUNCH PROCESS TO EXECUTE TASKS
        for (i = 0; i < jobAmount; ++i) {
            MSG_process_create("executor", executor, jobPtrBatchData[i], MSG_host_self());
        }
        MSG_task_destroy(task);
        task = NULL;
    }
    return 0;
}


