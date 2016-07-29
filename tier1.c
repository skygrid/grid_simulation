#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(tier1, "messsages specific for tier1");
//
// Created by ken on 29.07.16.
//

int busyCoreAmount;

int tier1(int argc, char* argv[]){
    int i;
    msg_task_t task;
    char* tierMailbox = argv[1];
    int coreAmount = (int) xbt_str_parse_int(argv[2], "Invalid argument %s");
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup("0"), NULL);

    while (1){
        msg_error_t a = MSG_task_receive(&task, tierMailbox);
        if (a == MSG_OK){
            XBT_INFO("Successfully receive task");
        }
        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        int jobAmount = (int) MSG_task_get_flops_amount(task);
        jobPtr* jobPtrBatchData = MSG_task_get_data(task);

        //LAUNCH PROCESS TO EXECUTE TASKS
        for (i = 0; i < jobAmount; ++i) {
            MSG_process_create(tierMailbox, executor, jobPtrBatchData[i], MSG_host_self());
        }
    }

    return 0;
}


