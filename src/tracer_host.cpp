//
// Created by ken on 08.08.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(tracer_host, "messages specific for trace");

int addActiveCoreT(){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "corruptedCore", 0);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "activeCore", 1);
    MSG_sem_release(sem_link);
    return 0;
}

int subActiveCoreT(){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "corruptedCore", 0);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "activeCore", 1);
    MSG_sem_release(sem_link);
    return 0;
}



int addCorruptedCoreT(){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "corruptedCore", 1);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "activeCore", -1);
    MSG_sem_release(sem_link);
    return 0;
}

int subCorruptedCoreT(){
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "corruptedCore", 1);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "activeCore", -1);
    MSG_sem_release(sem_link);
    return 0;
}

