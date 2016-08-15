#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(tier1, "messsages specific for tier1");
//
// Created by ken on 29.07.16.
//

FILE* fp;

int executorLauncher();
int job_requester();
int executor(int argc, char* argv[]);
msg_sem_t sem;

// MAIN TIER1 FUNCTION
int tier1(int argc, char* argv[]){
    char* tierMailbox = argv[1];
    char** argx = xbt_new(char*, 1);
    argx[0] = tierMailbox;

    // LAUNCH PROCESS
    sem = MSG_sem_init(1);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup("0"), xbt_free_f);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup("0"), xbt_free_f);
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
        // ONLY IF YOU'VE RECEIVED
        if (a == MSG_OK){
            if(!strcmp(MSG_task_get_name(task), "finalize")){
                MSG_task_destroy(task);
                break;
            }
            int jobAmount = (int) MSG_task_get_flops_amount(task);
            jobPtr* jobPtrBatchData = MSG_task_get_data(task);
            XBT_INFO("Successfully receive %s", jobPtrBatchData[0]->name);

            //LAUNCH PROCESS TO EXECUTE TASKS
            for (i = 0; i < jobAmount; ++i) {
                MSG_process_create("executor", executor, jobPtrBatchData[i], MSG_host_self());
            }

            MSG_task_destroy(task);
            task = NULL;
        } else if (a == MSG_HOST_FAILURE){
            task = NULL;
            writeAnomaly(MSG_get_clock());
        } else if (a == MSG_TRANSFER_FAILURE){
            writeAnomaly(MSG_get_clock());
        }

    }
    free(tierMailbox);
    return 0;
}

