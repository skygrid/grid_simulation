//
// Created by ken on 14.08.16.
//

#include <simgrid/msg.h>

int switch_off_all_hosts(){
    unsigned int cur;
    msg_host_t host;

    xbt_dynar_t hosts = MSG_hosts_as_dynar();

    xbt_dynar_foreach(hosts, cur, host){
        msg_task_t task = MSG_task_create("finalize", 0.0, 0.0, NULL);
        MSG_task_send(task, MSG_host_get_name(host));
    }

    return 0;
}
