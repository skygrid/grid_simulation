//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"
void plusOneCorruptedCore();
void minusOneCorruptedCore();
int angel();
msg_sem_t sem_link;


int evil(int argc, char* argv[]){
    double sleepTime = xbt_str_parse_double(argv[1], "error");
    MSG_process_sleep(sleepTime);
    int i = 0;
    int amountCorruptCore = (int) xbt_str_parse_int(argv[2], "error");
    double timeCorruption = xbt_str_parse_double(argv[3], "error");
    msg_process_t proc1, proc2;
    xbt_swag_t swag = MSG_host_get_process_list(MSG_host_self());
    xbt_swag_foreach_safe(proc1, proc2, swag){
        if (i > amountCorruptCore){
            break;
        }
        if (!strcmp(MSG_process_get_name(proc1), "executor")){
            MSG_process_kill(proc1);
            addCorruptedCoreT();
            plusOneCorruptedCore();
            i++;
        }
    }
    MSG_process_sleep(timeCorruption);
    MSG_process_create("angel", angel, amountCorruptCore, MSG_host_self());
    MSG_process_kill(MSG_process_self());

    return 0;
}

int angel(){
    int amountCorruptCore = (int) MSG_process_get_data(MSG_process_self());
    for (int i = 0; i <= amountCorruptCore; ++i) {
        subCorruptedCoreT();
        minusOneCorruptedCore();
    }
    return 0;
}



void plusOneCorruptedCore(){
    MSG_sem_acquire(sem_link);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
    MSG_sem_release(sem_link);
}

void minusOneCorruptedCore(){
    MSG_sem_acquire(sem_link);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
    MSG_sem_release(sem_link);
}

