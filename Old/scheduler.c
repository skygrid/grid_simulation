//
// Created by ken on 24.05.16.
//

#include <simgrid/msg.h>
#include <csvparser.h>
#include "messages.h"

xbt_dynar_t task_names;
xbt_dynar_t flops_sizes;
xbt_dynar_t data_sizes;
xbt_dynar_t types;
xbt_dynar_t data_location;

int current_task_n = 0;

XBT_LOG_NEW_DEFAULT_CATEGORY(scheduler, "messsages specific for scheduler");

int killAllProcess(){
    int i = 0;
    int dispatcher_amount = 7;
    char destination_TS[80];
    char destination_ST[80];
    char SD_name[80];

    for (i = 0; i < dispatcher_amount; ++i) {
        sprintf(destination_TS, "dispatcher1_%i_TS", i+1);
        sprintf(destination_ST, "Tier1_%i_ST", i+1);
        sprintf(SD_name, "Tier1_%i", i+1);

        msg_task_t task_TS = MSG_task_create("finalize", 0, 0, 0);
        msg_task_t task_ST = MSG_task_create("finalize", 0, 0, 0);
        msg_task_t task_SD = MSG_task_create("finalize", 0, 0, 0);
        MSG_task_dsend(task_TS, destination_TS, NULL);
        MSG_task_dsend(task_ST, destination_ST, NULL);
        MSG_task_dsend(task_SD, SD_name, NULL);
        task_SD, task_ST, task_TS = NULL;
    }
    return 0;
}

int comparator(void *a, void *b){
    XBT_INFO("urochiy");
    msg_host_t hostA = *(msg_host_t*) a;
    msg_host_t hostB = *(msg_host_t*) b;

    double valA = MSG_host_get_speed(hostA);
    double valB = MSG_host_get_speed(hostB);
    return (valA > valB) - (valA < valB);
}

int contains(xbt_dynar_t* dynar, char* tier1_name){
    int cpt;

    for (cpt = 0; cpt < xbt_dynar_length(*dynar); ++cpt) {
        char* string = * (char **) xbt_dynar_get_ptr(*dynar, cpt);
        if (!memcmp(string, tier1_name, strlen(string))){
            return 1;
        }
    }
    return 0;
}

int input(){
    task_names = xbt_dynar_new(sizeof(char**), NULL);
    flops_sizes = xbt_dynar_new(sizeof(double), NULL);
    data_sizes = xbt_dynar_new(sizeof(double), NULL);
    types = xbt_dynar_new(sizeof(char**), NULL);
    data_location = xbt_dynar_new(sizeof(xbt_dynar_t), NULL);

    CsvParser *csvparser = CsvParser_new("data.csv", ",", 1);
    CsvRow *header;
    CsvRow *row;

    while ((row = CsvParser_getRow(csvparser))){
        const char **rowFields = CsvParser_getFields(row);

        double flops = xbt_str_parse_double(rowFields[1], "error");
        double bytes = xbt_str_parse_double(rowFields[2], "error");
        char* task_name_copy = strdup(rowFields[0]);
        char* type_copy = strdup(rowFields[3]);

        xbt_dynar_push(task_names, &task_name_copy);
        xbt_dynar_push(types, &type_copy);
        xbt_dynar_push_as(flops_sizes, double, flops);
        xbt_dynar_push_as(data_sizes, double, bytes);


        int number_fields = CsvParser_getNumFields(row);

        xbt_dynar_t locations = xbt_dynar_new(sizeof(char**), NULL);

        if (number_fields == 5){
            char * location1 = strdup(rowFields[4]);
            xbt_dynar_push(locations, &location1);
        }
        else if (number_fields == 6){
            char * location1 = strdup(rowFields[4]);
            char * location2 = strdup(rowFields[5]);

            xbt_dynar_push(locations, &location1);
            xbt_dynar_push(locations, &location2);
        }
        else if (number_fields == 7){
            char * location1 = strdup(rowFields[4]);
            char * location2 = strdup(rowFields[5]);
            char * location3 = strdup(rowFields[6]);

            xbt_dynar_push(locations, &location1);
            xbt_dynar_push(locations, &location2);
            xbt_dynar_push(locations, &location3);
        }


        xbt_dynar_push(data_location, &locations);
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);
}

xbt_dict_t match_task(xbt_dynar_t* dynar_host){
    unsigned long i = 0;
    msg_task_t task;
    xbt_dict_t map = xbt_dict_new();
    char** name;
    char** type;
    double flops;
    double size;

    for (i = 0; i < xbt_dynar_length(*dynar_host); ++i) {

        name = xbt_dynar_get_ptr(task_names, current_task_n + i);
        flops = xbt_dynar_get_as(flops_sizes, current_task_n + i, double);
        size = xbt_dynar_get_as(data_sizes, current_task_n + i, double);
        type = xbt_dynar_get_ptr(types, current_task_n + i);

        message_t message = xbt_new(s_message_t, 1);
        message->size_data = size;
        message->flops_amount = flops;

        if (!strcmp(*type, "Monte-Carlo")){
            message->type = MONTE_CARLO;
            message->downloadfrom = "None";

        } else if (!strcmp(*type, "Production")){
            message->type = INSTRUCTION;

            xbt_dynar_t* cur_data_loc = xbt_dynar_get_ptr(data_location, current_task_n+i); // (!)

            char** host_name_string = xbt_dynar_get_ptr(*dynar_host, i);

            if (contains(cur_data_loc, *host_name_string)){
                message->downloadfrom = * (char**) xbt_dynar_get_ptr(*dynar_host, i);
            } else{
                message->downloadfrom = * (char**) xbt_dynar_get_ptr(*cur_data_loc, 0);
            }
        } else if (!strcmp(*type, "User")){
        }

        task = MSG_task_create(*name, 0, MESSAGES_SIZE, message);
        xbt_dict_set(map, *(char**) xbt_dynar_get_ptr(*dynar_host, i), task, NULL);
        current_task_n++;
    }
    return map;
}


int scheduler(int argc, char* argv[]){

    MSG_process_set_kill_time(MSG_process_self(), 100.0);

    int count = 0;
    input();
    char mailbox[30];
    char destination[50];
    msg_task_t task = NULL;
    int id = (int) xbt_str_parse_int(argv[1], "Invalid argument %s");
    sprintf(mailbox, "scheduler");

    while (1){
        if (count >= 100){
            MSG_process_suspend(MSG_process_self());
            break;
        }
        int res = MSG_task_receive(&task, mailbox);
        XBT_INFO("Receive a pilot from %s", MSG_host_get_name(MSG_task_get_source(task)));

        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        // matching procedure; sending to dispatcher
        sprintf(destination, "%s_%s", MSG_host_get_name(MSG_task_get_source(task)), "ST");
        xbt_dynar_t dynar = MSG_task_get_data(task);
        //xbt_dynar_sort(dynar, comparator);

        xbt_dict_t map = match_task(&dynar);
        XBT_INFO("Start send after matching to %s", destination);
        msg_task_t taskB = MSG_task_create("", 0, MESSAGES_SIZE, map);
        msg_error_t a = MSG_task_send(taskB, destination);

        if (a == MSG_OK){
            XBT_INFO("Send completed");
        }
        else if (a == MSG_HOST_FAILURE){
            XBT_INFO("Problems with host");
            free(map);
            MSG_task_destroy(taskB);
            return 0;
        }
        else if (a == MSG_TRANSFER_FAILURE){
            XBT_INFO("Mmh. Something went wrong with '%s'. Nevermind. Let's keep going!", mailbox);
            free(map);
            MSG_task_destroy(taskB);
        }
        MSG_task_destroy(task);
        task = NULL;
        map = NULL;
        count++;
    }

    return 0;
}

int kotok(){
    int i; jobPtr j1;
    jobPtr* jobBatchData = xbt_new(jobPtr , 10);
    for (i = 0; i < 10; ++i) {
        jobBatchData[i] = j1;
    }
}










































