//
// Created by ken on 08.08.16.
//

#include <simgrid/msg.h>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(tracer_host, "messages specific for trace");

int addActiveCoreT(){
	/**
		@type function
		 Increases number of running cores by one on host which calls this function.
		 Then writes value to trace file.
	*/
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "corruptedCore", 0);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "activeCore", 1);
    MSG_sem_release(sem_link);
    return 0;
}

int subActiveCoreT(){
	/**
		@type function
		Decreases number of running cores by one on host which calls this function.
		Then writes value to trace file.			
	*/
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "corruptedCore", 0);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "activeCore", 1);
    MSG_sem_release(sem_link);
    return 0;
}



int addCorruptedCoreT(){
	/**
	    @type function
		Increases number of broken cores by one on host which calls this function.
		Then writes value to trace file.			
	*/
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "corruptedCore", 1);
    TRACE_host_variable_add(MSG_host_get_name(MSG_host_self()), "activeCore", -1);
    MSG_sem_release(sem_link);
    return 0;
}

int subCorruptedCoreT(){
	/**
		@type function
		Decreases number of broken cores by one on host which calls this function.
		Then writes value to trace file.			
	*/
    MSG_sem_acquire(sem_link);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "corruptedCore", 1);
    TRACE_host_variable_sub(MSG_host_get_name(MSG_host_self()), "activeCore", -1);
    MSG_sem_release(sem_link);
    return 0;
}

