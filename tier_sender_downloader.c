//
// Created by ken on 23.05.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "tier_sender_downloader.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sender_downloader, "messages specific for sending and downloading");

int tier_sender_downloader(int argc, char *argv[]){

    MSG_process_set_kill_time(MSG_process_self(), 100.0);

    char mailbox[80];
    msg_task_t task = NULL;

    int id = xbt_str_parse_int(argv[1], "Invalid argument %s");

    sprintf(mailbox, "Tier1_%i", id);

    while (1){

        int res = MSG_task_receive(&(task), mailbox);
        xbt_assert(res == MSG_OK, "MSG_get_task failed");

        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        message_t message = MSG_task_get_data(task);
        if (message->type == GIVEMEDATA){
            message_t messx = xbt_new(s_message_t, 1);
            messx->type = DOWNLOADED;
            msg_task_t d_task = MSG_task_create(MSG_task_get_name(task), message->flops_amount, 0.001*message->size_data, messx);
            XBT_INFO("Start to send DATA of %s to %s", MSG_task_get_name(task), MSG_host_get_name(MSG_task_get_source(task)));
            MSG_task_send(d_task, MSG_host_get_name(MSG_task_get_source(task)));
            XBT_INFO("Finished to send DATA of %s to %s", MSG_task_get_name(task), MSG_host_get_name(MSG_task_get_source(task)));
            MSG_task_destroy(task);
            task = NULL;
        }
    }
}
