//
// Created by ken on 08.08.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

msg_sem_t sem_link;

int addActiveCoreT(){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "activeCore", 1);
    MSG_sem_release(sem_link);
    return 0;
}

int subActiveCoreT(){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "activeCore", 1);
    MSG_sem_release(sem_link);
    return 0;
}

