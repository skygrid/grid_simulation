//
// Created by ken on 30.05.16.
//

#include <simgrid/msg.h>
#include "messages.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(node, "messages specific for nodes");

int tier1(int argc, char *argv[]){
    MSG_process_set_kill_time(MSG_process_self(), 100.0);

    XBT_INFO("I'am ready to work");
    msg_task_t task = NULL;
    msg_task_t data = NULL;
    char mailbox[80];
    char dispatcher_mail[80];

    int num = xbt_str_parse_int(argv[1], "Invalid argument %s");
    int id = xbt_str_parse_int(argv[2], "Invalid argument %s");

    sprintf(mailbox, "Tier1_%i_%i", num, id);

    //At first moment nodes get to dispatcher
    sprintf(dispatcher_mail, "dispatcher1_%i_TS", num);
    msg_task_t first_task = MSG_task_create("Firstrequest", 0, MESSAGES_SIZE, NULL);
    XBT_INFO("Send a pilot message to dispatcher");
    MSG_task_send(first_task, dispatcher_mail);

    while(1){

        int res = MSG_task_receive(&task, mailbox);

        if (res == MSG_OK) {

            if (!strcmp(MSG_task_get_name(task), "finalize")) {
                MSG_task_destroy(task);
                break;
            }
            message_t message = MSG_task_get_data(task);
            switch (message->type) {
                case MONTE_CARLO:

                    XBT_INFO("Receive %s", MSG_task_get_name(task));
                    XBT_INFO("Start to execute Monte-Carlo %s", MSG_task_get_name(task));
                    message_t sms_from_task = MSG_task_get_data(task);
                    msg_task_t mc_task = MSG_task_create("", sms_from_task->flops_amount, 0, NULL);
                    int errcode = MSG_task_execute(mc_task);
                    if (errcode == MSG_OK){
                        XBT_INFO("Finished execute Monte-Carlo %s", MSG_task_get_name(task));
                        MSG_task_destroy(mc_task);
                        MSG_task_destroy(task);
                        task = NULL;
                        XBT_INFO("Send a pilot message to dispatcherxx");
                        MSG_task_send(MSG_task_create("Request", 0, MESSAGES_SIZE, NULL), dispatcher_mail);
                    } else if(errcode == MSG_HOST_FAILURE){
                        XBT_INFO("Gloups. The cpu on which I'm running just turned off!. See you!");
                        MSG_task_destroy(task);
                        task = NULL;
                        return 0;
                    } else{
                        XBT_INFO("Hey ?! What's up ? ");
                        xbt_die("Unexpected behavior");
                    }
                    break;
                case INSTRUCTION:

                    XBT_INFO("Receive %s", MSG_task_get_name(task));
                    if (!strcmp(message->downloadfrom, MSG_host_get_name(MSG_host_self()))) {
                        XBT_INFO("Start to execute %s.I have all data", MSG_task_get_name(task));
                        int errcode = MSG_task_execute(task);

                        if (errcode == MSG_OK){
                            XBT_INFO("Finished to execute %s", MSG_task_get_name(task));
                            MSG_task_destroy(task);
                            task = NULL;
                            XBT_INFO("Send a pilot message to dispatcher");
                            MSG_task_send(MSG_task_create("Request", 0, MESSAGES_SIZE, NULL), dispatcher_mail);
                        }
                        else if (errcode == MSG_HOST_FAILURE){
                            XBT_INFO("Gloups. The cpu on which I'm running just turned off!. See you!");
                            MSG_task_destroy(task);
                            task = NULL;
                            return 0;
                        } else{
                            XBT_INFO("Hey ?! What's up ? ");
                            xbt_die("Unexpected behavior");
                        }

                    } else {
                        msg_task_t gtask = give_me_data(MSG_task_get_name(task), message->flops_amount,
                                                        message->size_data);
                        XBT_INFO("Hey %s, give me data for %s", message->downloadfrom, MSG_task_get_name(task));
                        MSG_task_send(gtask, message->downloadfrom);
                        XBT_INFO("yyyyyyyyyy");

                        MSG_task_destroy(task);
                        task = NULL;
                    }
                    break;
            }
        } else if (res == MSG_TIMEOUT) {
            XBT_INFO("Anomaly occurred:Timeout Exception");
            MSG_task_destroy(task);
            task = NULL;
        } else if (res == MSG_HOST_FAILURE) {
            XBT_INFO("Anomaly occurred: HostFailure Exception");
            MSG_task_destroy(task);
            task = NULL;
        } else if (res == MSG_TRANSFER_FAILURE){
            XBT_INFO("Anomaly occurred: Transfer failure Exception");
            MSG_task_destroy(task);
            task = NULL;
            break;
        }
    }

 }
