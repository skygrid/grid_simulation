//
// Created by ken on 05.08.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"

char* get_direction(const char* src, const char* dst);
char* find_transit(const char* src, const char* dst);
int direct(const char* src, const char* dst);
char* get_opposite_direction(char* direction);

msg_sem_t sem_link;

int plusLinkCounter(const char* src, const char* dst){
    MSG_sem_acquire(sem_link);
    char* transit = NULL;
    if (strcmp(src, dst)){

        if (direct(src, dst)){
            TRACE_link_srcdst_variable_add(src, dst, get_direction(src, dst), 1);
            TRACE_link_srcdst_variable_add(src, dst, get_opposite_direction(get_direction(src, dst)), 0);
        } else{
            transit = find_transit(src, dst);
            TRACE_link_srcdst_variable_add(src, transit, get_direction(src, transit), 1);
            TRACE_link_srcdst_variable_add(transit, dst, get_direction(transit, dst), 1);

            TRACE_link_srcdst_variable_add(src, transit, get_opposite_direction(get_direction(src, transit)), 0);
            TRACE_link_srcdst_variable_add(transit, dst, get_opposite_direction(get_direction(transit, dst)), 0);

        }
    }
    MSG_sem_release(sem_link);
    return 0;
}

int minusLinkCounter(const char* src, const char* dst){
    MSG_sem_acquire(sem_link);
    char* transit = NULL;
    if (strcmp(src, dst)){

        if (direct(src, dst)){
            TRACE_link_srcdst_variable_sub(src, dst, get_direction(src, dst), 1);
            TRACE_link_srcdst_variable_sub(src, dst, get_opposite_direction(get_direction(src, dst)), 0);
        } else{
            transit = find_transit(src, dst);
            TRACE_link_srcdst_variable_sub(src, transit, get_direction(src, transit), 1);
            TRACE_link_srcdst_variable_sub(transit, dst, get_direction(transit, dst), 1);

            TRACE_link_srcdst_variable_sub(src, transit, get_opposite_direction(get_direction(src, transit)), 0);
            TRACE_link_srcdst_variable_sub(transit, dst, get_opposite_direction(get_direction(transit, dst)), 0);

        }
    }
    MSG_sem_release(sem_link);
    return 0;
}




/*int anomalyLinkTracer(const char* src, const char* dst){
    msg_task_t message1, message2;
    MSG_sem_acquire(sem_link);
    char* transit = NULL;
    if (strcmp(src, dst)){
        if (direct(src, dst)){
            TRACE_link_srcdst_variable_set(src, dst, "directUserAmount", 0);
            TRACE_link_srcdst_variable_set(src, dst, "indirectUserAmount", 0);
        }else{
            transit = find_transit(src, dst);
            message1 = MSG_task_create("", 0, MESSAGES_SIZE, NULL);
            msg_error_t err1 = MSG_task_send(message1, transit);
            if (err1 == MSG_TRANSFER_FAILURE){
                TRACE_link_srcdst_variable_set(src, transit, "directUserAmount", 0);
                TRACE_link_srcdst_variable_set(src, transit, "indirectUserAmount", 0);
            }else {
                TRACE_link_srcdst_variable_set(transit, dst, "directUserAmount", 0);
                TRACE_link_srcdst_variable_set(transit, dst, "indirectUserAmount", 0);
            }
        }

    }



    MSG_sem_release(sem_link);
    return 0;
}*/



int direct(const char* src, const char* dst){
    if ((!strcmp(src, "CERN") || !strcmp(dst, "CERN")) || (!strcmp(src, "GRIDKA") &&  (!strcmp(dst, "CNAF") || !strcmp(dst, "IN2P3") || !strcmp(dst, "SARA"))) || ((!strcmp(src, "SARA") || !strcmp(src, "IN2P3") || !strcmp(src, "CNAF")) && !strcmp(dst, "GRIDKA"))){
        return 1;
    }
    return 0;
}

char* get_direction(const char* src, const char* dst){
    char* direction;

    if ((!strcmp(src, "CERN") || (!strcmp(src, "GRIDKA") &&  (!strcmp(dst, "CNAF") || !strcmp(dst, "IN2P3") || !strcmp(dst, "SARA"))))){
        direction = "directUserAmount";
    }
    else if (!strcmp(dst, "CERN")  || ((!strcmp(src, "SARA") || !strcmp(src, "IN2P3") || !strcmp(src, "CNAF")) && !strcmp(dst, "GRIDKA"))){
        direction = "indirectUserAmount";
    }

    return direction;
}


char* find_transit(const char* src, const char* dst){
    char* transit;
    if ((!strcmp(src,  "IN2P3") || !strcmp(src, "SARA") || !strcmp(src, "CNAF")) && (!strcmp(dst, "IN2P3") || !strcmp(dst, "SARA") || !strcmp(dst, "CNAF"))){
        transit = "GRIDKA";
    } else{
        transit = "CERN";
    }
    return transit;
}

char* get_opposite_direction(char* direction){
    char* opposit_dir = NULL;
    if (!strcmp(direction, "directUserAmount")){
        opposit_dir = "indirectUserAmount";
    } else{
        opposit_dir = "directUserAmount";
    }
    return opposit_dir;
}