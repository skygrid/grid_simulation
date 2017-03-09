//
// Created by ken on 05.08.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"

char* get_direction(const char* src, const char* dst);
char* find_transit(const char* src, const char* dst);
int direct(const char* src, const char* dst);
char* get_opposite_direction(char* direction);


int plusLinkCounter(std::string& src, std::string& dst){
	/**
	@type function
	   Increases by one the number of "users" who transfer data from @src and @dst hosts.
	   Writes number to trace file.		
	*/
    MSG_sem_acquire(sem_link);
    /*char* transit = NULL;
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
    }*/
    MSG_sem_release(sem_link);
    return 0;
}

int tracer_traffic(std::string& src, std::string& dst, double size){
	/**
	   @type function
		Adds @size bytes to link's cumulative sum of transferred data between
		@dst and @src. 
		Writes value to trace file. 		
	*/
    MSG_sem_acquire(sem_link);
    TRACE_link_srcdst_variable_add(src.c_str(), dst.c_str(), "traffic", size);
    MSG_sem_release(sem_link);
    return 0;
}


int minusLinkCounter(std::string& src, std::string& dst){
	/**
	   @type function
		Decreases by one the number of "users" who transfer data from @src and @dst hosts.
		Writes number to trace file.		
	*/
    MSG_sem_acquire(sem_link);


    /*char* transit = NULL;
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
    }*/
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


/*
int direct(string& src, string& dst){
    if ((!strcmp(src, "CERN") || !strcmp(dst, "CERN")) || (!strcmp(src, "GRIDKA") &&  (!strcmp(dst, "CNAF") || !strcmp(dst, "IN2P3") || !strcmp(dst, "SARA"))) || ((!strcmp(src, "SARA") || !strcmp(src, "IN2P3") || !strcmp(src, "CNAF")) && !strcmp(dst, "GRIDKA"))){
        return 1;
    }
    return 0;
}

char* get_direction(string& src, string& dst){
    char* direction;

    if ((!src.compare("CERN") || (!strcmp(src, "GRIDKA") &&  (!strcmp(dst, "CNAF") || !strcmp(dst, "IN2P3") || !strcmp(dst, "SARA"))))){
        direction = "directUserAmount";
    }
    else if (!strcmp(dst, "CERN")  || ((!strcmp(src, "SARA") || !strcmp(src, "IN2P3") || !strcmp(src, "CNAF")) && !strcmp(dst, "GRIDKA"))){
        direction = "indirectUserAmount";
    }

    return direction;
}


string find_transit(string& src, string& dst){
    string transit;
    if ((!src.compare("IN2P3") || !strcmp(src, "SARA") || !strcmp(src, "CNAF")) && (!strcmp(dst, "IN2P3") || !strcmp(dst, "SARA") || !strcmp(dst, "CNAF"))){
        transit = "GRIDKA";
    } else{
        transit = "CERN";
    }
    return transit;
}

string get_opposite_direction(string& direction){
    string opposit_dir;
    if (!direction.compare("directUserAmount")){
        opposit_dir = "indirectUserAmount";
    } else{
        opposit_dir = "directUserAmount";
    }
    return opposit_dir;
}*/
