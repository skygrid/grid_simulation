//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
void plusOneCorruptedCore();


int fullCoreAmount;
int evil(){
    int i;
    int amountCorruptCore = 50; //
    msg_process_t proc1, proc2;
    xbt_swag_t swag = MSG_host_get_process_list(MSG_host_self());
    xbt_swag_foreach_safe(proc1, proc2, swag){
        if (i < amountCorruptCore){
            break;
        }
        if (!strcmp(MSG_process_get_name(proc1), "executor")){
            MSG_process_kill(proc1);
            plusOneCorruptedCore();
            i++;
        }
    }
    return 0;
}


void plusOneCorruptedCore(){
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
}

