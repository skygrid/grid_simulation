//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include <stdio.h>

FILE* storage_fp;
FILE* host_fp;

int tracer_storage();
int tracer_active_core();

double end;

int main_tracer(){
    double timeout = 15;
    storage_fp = fopen("/home/ken/PycharmProjects/GridAnalysis/Storage/storage2", "w");
    host_fp = fopen("/home/ken/PycharmProjects/GridAnalysis/Host/host2", "w");

    fprintf(storage_fp, "Storage_name,Used_size,time\n");
    fprintf(host_fp, "Host_name,ActiveCore,CorrCore,time\n");

    while (1){
        if (MSG_get_clock() > 55687){
            break;
        }
        MSG_process_create("", tracer_storage, NULL, MSG_host_self());
        MSG_process_create("", tracer_active_core, NULL, MSG_host_self());
        MSG_process_sleep(timeout);
        printf("x");
    }
    return 0;

}


int tracer_storage(){
    unsigned int cur;
    msg_storage_t st;
    xbt_dynar_t storages = MSG_storages_as_dynar();
    xbt_dynar_foreach(storages, cur, st){
        fprintf(storage_fp, "%s,%f,%f\n", MSG_storage_get_name(st), (double) MSG_storage_get_used_size(st),  MSG_get_clock());
    }
    return 0;
}

int tracer_active_core(){
    unsigned int cur;
    msg_host_t host;
    xbt_dynar_t hosts = MSG_hosts_as_dynar();
    xbt_dynar_foreach(hosts, cur, host){
        fprintf(host_fp, "%s,%s,%s,%f\n", MSG_host_get_name(host), MSG_host_get_property_value(host, "activeCore"), MSG_host_get_property_value(host, "corruptedCore"), MSG_get_clock());
    }
    return 0;
}

