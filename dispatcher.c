//
// Created by ken on 23.05.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "tier_sender_downloader.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(dispatcher, "messsages specific for dispatching");


static int from_scheduler_to_tier(int argc, char *argv[]){
    MSG_process_set_kill_time(MSG_process_self(), 100.0);

    msg_task_t task = NULL;
    char mailbox[80]; char node_name[80];
    xbt_dict_t map;
    xbt_dict_cursor_t cursor = NULL; int i;

    int id = xbt_str_parse_int(argv[1], "Invalid argument %s");
    int num = xbt_str_parse_int(argv[2], "Invalid argument %s");

    sprintf(mailbox, "Tier1_%i_ST", id);

    while (1){
        int res = MSG_task_receive(&task, mailbox);
        XBT_INFO("Receive after matching");
        XBT_INFO("%s", MSG_task_get_name(task));

        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        map = MSG_task_get_data(task);
        char* host;
        msg_task_t task_t;

        xbt_dict_foreach(map, cursor, host, task_t){
            XBT_INFO("Start to send %s to %s", MSG_task_get_name(task_t), host);
            MSG_task_send(task_t, host);
        }

        //xbt_dict_free(map);
        //MSG_task_destroy(task);
        task = NULL;
    }
    return 0;
}

static int from_tier_to_scheduler(int argc, char **argv){

    MSG_process_set_kill_time(MSG_process_self(), 100.0);

    char mailbox[80];
    msg_task_t task;
    msg_task_t m_pilot;

    int id = xbt_str_parse_int(argv[1], "Invalid argument %s");


    sprintf(mailbox, "dispatcher1_%i_TS", id);

    while (1){
        int res = MSG_task_receive(&task, mailbox);
        if (res == MSG_OK){

        }
        XBT_INFO("Received message from %s", MSG_host_get_name(MSG_task_get_source(task)));

        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            MSG_process_kill(MSG_process_self());
            break;
        }
        xbt_dynar_t dynar = xbt_dynar_new(sizeof(char*), NULL);
        const char * host_name = MSG_host_get_name(MSG_task_get_source(task));
        xbt_dynar_push(dynar, &host_name);

        if (xbt_dynar_length(dynar) > 0){
            XBT_INFO("Start send a pilot message to megascheduler");
            m_pilot = MSG_task_create("mega_pilot", 0, MESSAGES_SIZE, dynar);
            MSG_task_send(m_pilot, "scheduler");
        }
        MSG_task_destroy(task);
        task = NULL;
    }

    return 0;
}

int dispatcher(int argc, char *argv[]){

    int num = xbt_str_parse_int(argv[1], "Invalid argument %s");
    int number_tiers = xbt_str_parse_int(argv[2], "Invalid argument %s");

    char** newargv = xbt_new(char*, 3);  char** newargvx = xbt_new(char*, 3); char** newargvy = xbt_new(char*, 2);

    newargv[0] = xbt_strdup("TS"); newargvx[0] = xbt_strdup("ST"); newargvy[0] = xbt_strdup("SD");
    newargv[1] = xbt_strdup(argv[1]); newargvx[1] = xbt_strdup(argv[1]); newargvy[1] = xbt_strdup(argv[1]);
    newargv[2] = xbt_strdup(argv[2]); newargvx[2] = xbt_strdup(argv[2]);

    MSG_process_create_with_arguments("TS", from_tier_to_scheduler, NULL, MSG_host_self(), 3, newargv);
    MSG_process_create_with_arguments("ST", from_scheduler_to_tier, NULL, MSG_host_self(), 3, newargvx);
    MSG_process_create_with_arguments("SD", tier_sender_downloader, NULL, MSG_host_self(), 2, newargvy);

    return 0;
}

