//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>

void minusOneCorruptedCore();


int angel(){
    int amountCorruptCore = 50;
    for (int i = 0; i < amountCorruptCore; ++i) {
        minusOneCorruptedCore();
    }
    return 0;
}

void minusOneCorruptedCore(){
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "corruptedCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
}

