//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"

void plusOneCorruptedCore();
void minusOneCorruptedCore();

void plusOneActiveCore();
void minusOneActiveCore();

int angel(int argc, char* argv[]);


int evil(int argc, char* argv[]){
	/**
		@type simgrid process
		Creates break downs at @amountCorruptCore at @sleepTime moment.
        Breaking duration is @timeCorruption.

        Simgrid process parameters
        --------------------------
        argv[1] -- @amountCorruptCore
        argv[2] -- @sleepTime
        argv[3] -- @timeCorruption

	*/
    double sleepTime = xbt_str_parse_double(argv[1], "error");
    MSG_process_sleep(sleepTime);
    int i = 0;
    int* amountCorruptCore = (int*) xbt_str_parse_int(argv[2], "error");
    double timeCorruption = xbt_str_parse_double(argv[3], "error");
    msg_process_t proc1, proc2;
    xbt_swag_t swag = MSG_host_get_process_list(MSG_host_self());
    xbt_swag_foreach_safe(proc1, proc2, swag){
        if (i > *amountCorruptCore){
            break;
        }
        if (!strcmp(MSG_process_get_name(proc1), "executor")){
            MSG_process_kill(proc1);

            //For tracing file
            addCorruptedCoreT();

            // For internal usage
            plusOneCorruptedCore();
            minusOneActiveCore();
            i++;
        }
    }
    MSG_process_sleep(timeCorruption);
    MSG_process_create("angel", angel, amountCorruptCore, MSG_host_self());
    MSG_process_kill(MSG_process_self());

    return 0;
}

int angel(int argc, char* argv[]){
	/**
		@type simgrid process
		Repairs cores broken by evil

        Simgrid process parameters
        --------------------------
        @amountCorruptCore -- How many cores this process should repair.
	*/
    int* amountCorruptCore = (int*) MSG_process_get_data(MSG_process_self());
    for (int i = 0; i <= *amountCorruptCore; ++i) {
        subCorruptedCoreT();

        minusOneCorruptedCore();
        plusOneActiveCore();
    }
    return 0;
}



void plusOneCorruptedCore(){
	/**
		@type function
		Increases the number of corrupred (broken) cores by one.
	*/
    MSG_sem_acquire(sem_link);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup(kot));
    MSG_sem_release(sem_link);
}

void minusOneCorruptedCore(){
	/**
		@type function
		Decreases the number of corrupted cores by one.
	*/
    MSG_sem_acquire(sem_link);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup(kot));
    MSG_sem_release(sem_link);
}

