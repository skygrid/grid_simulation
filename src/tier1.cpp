#include <simgrid/msg.h>
#include "my_structures.h"
#include "myfunc_list.h"
#include <vector>

XBT_LOG_NEW_DEFAULT_CATEGORY(tier1, "messsages specific for tier1");
//
// Created by ken on 29.07.16.
//


int executorLauncher(int argc, char* argv[]);
int job_requester(int argc, char* argv[]);
int executor(int argc, char* argv[]);


// MAIN TIER1 FUNCTION
int tier1(int argc, char* argv[]){
    // LAUNCH PROCESS
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup("0"), xbt_free_f);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup("0"), xbt_free_f);
    MSG_process_create("tier1_executor", executorLauncher, NULL, MSG_host_self());
    MSG_process_create("job_requester", job_requester, NULL, MSG_host_self());

    return 0;
}


int executorLauncher(int agrc, char* argv[]){
    msg_task_t task = NULL;
    const char* tierMailbox = MSG_host_get_name(MSG_host_self());

    while (1){
        int a = MSG_task_receive(&task, tierMailbox);
        // ONLY IF YOU'VE RECEIVED
        if (a == MSG_OK){
            if(!strcmp(MSG_task_get_name(task), "finalize")){
                MSG_task_destroy(task);
                break;
            }
            int jobAmount = (int) MSG_task_get_flops_amount(task);
            vector<Job*>* jobBatch = (vector<Job*>*) MSG_task_get_data(task);
            //XBT_INFO("Successfully receive jobBatch");

            //LAUNCH PROCESS TO EXECUTE TASKS
            for (unsigned long i = 0; i < jobAmount; ++i) {
                MSG_process_create("executor", executor, jobBatch->at(i), MSG_host_self());
            }

            MSG_task_destroy(task);
            delete jobBatch;
            task = NULL;
        } else if (a == MSG_HOST_FAILURE){
            task = NULL;
            writeAnomaly(MSG_get_clock());
        } else if (a == MSG_TRANSFER_FAILURE){
            writeAnomaly(MSG_get_clock());
        }

    }

    return 0;
}

