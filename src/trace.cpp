//
// Created by ken on 12.09.16.
//

#include <string>
#include <simgrid/msg.h>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(ttrace, "messages specific for trace");

msg_sem_t sem_link;

int declare_trace_variables(){
	/**
		@type function
		Declares variables which simgrid will trace
		(active cores, broken cores, traffic via link, input traffic to host, 
		output traffic from host, number of datasets on tapes and disks)

	*/

    msg_host_t host;
    unsigned int cursor;
    xbt_dynar_t hosts_dynar = MSG_hosts_as_dynar();

    //Declare storage variables for tracing
    std::string storage_name;
    std::string storage_types[2] = {"-DISK", "-TAPE"};
    xbt_dynar_foreach(hosts_dynar, cursor, host){
        for (int i = 0; i < 2; ++i) {
            storage_name = MSG_host_get_name(host) + storage_types[i];
            TRACE_host_variable_declare(storage_name.c_str());
        }
    }

    xbt_dynar_free(&hosts_dynar);

    TRACE_host_variable_declare("activeCore");
    TRACE_host_variable_declare("corruptedCore");
    TRACE_link_variable_declare("traffic");
    TRACE_link_variable_declare("directUserAmount");
    TRACE_link_variable_declare("indirectUserAmount");

    // Input and output data per site
    TRACE_host_variable_declare("inputData");
    TRACE_host_variable_declare("outputData");

    TRACE_host_variable_declare("datasetOnDisk");
    TRACE_host_variable_declare("datasetOnTape");
    set_0_all_routes();

    return 0;
}

int set_0_all_routes(){
	/**
		@type function
		Initializes variables(declared in declare_trace_variable()) by start value.
	*/

    std::string storage_variable;

    sem_link = MSG_sem_init(1);

    std::string host_name;
    std::string dst_host_name;
    const char* host_name_char_arr;
    const char* dst_host_name_char_arr;

    xbt_dynar_t hosts_dynar = MSG_hosts_as_dynar();
    size_t tiers_amount = xbt_dynar_length(hosts_dynar);

    TRACE_link_srcdst_variable_set("CERN-PROD", "CERN-PROD", "directUserAmount", 0);
    TRACE_link_srcdst_variable_set("CERN-PROD", "CERN-PROD", "indirectUserAmount", 0);

    for (unsigned long i = 0; i < tiers_amount; ++i) {
        host_name = MSG_host_get_name(xbt_dynar_get_as(hosts_dynar, i, msg_host_t));
        host_name_char_arr = host_name.c_str();

        //Set all variables of hosts to initial value
        TRACE_host_variable_set(host_name_char_arr, "activeCore", 0);
        TRACE_host_variable_set(host_name_char_arr, "corruptedCore", 0);

        TRACE_host_variable_set(host_name_char_arr, "inputData", 0);
        TRACE_host_variable_set(host_name_char_arr, "outputData", 0);
        TRACE_host_variable_set(host_name_char_arr, "datasetOnDisk", 0);
        TRACE_host_variable_set(host_name_char_arr, "datasetOnTape", 0);

        //Set all variables of links to initial value
        for (unsigned long j = i+1; j < tiers_amount; ++j) {
            dst_host_name = MSG_host_get_name(xbt_dynar_get_as(hosts_dynar, j, msg_host_t));
            dst_host_name_char_arr = dst_host_name.c_str();

            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_set(host_name_char_arr, dst_host_name_char_arr, "traffic", 0);
            TRACE_link_srcdst_variable_set(host_name_char_arr, dst_host_name_char_arr, "directUserAmount", 0);
            TRACE_link_srcdst_variable_set(host_name_char_arr, dst_host_name_char_arr, "indirectUserAmount", 0);
            MSG_sem_release(sem_link);
        }

        //Set all variables of storage to initial value
        std::string s[] = {"-TAPE", "-DISK"};
        for (int k = 0; k < 2; ++k) {
            storage_variable = host_name + s[k];
            msg_storage_t storage = MSG_storage_get_by_name(storage_variable.c_str());
            TRACE_host_variable_set("CERN-PROD", storage_variable.c_str(), MSG_storage_get_used_size(storage));
        }
        TRACE_host_variable_set(host_name_char_arr, "datasetOnDisk", dataset_number(host_name, "-DISK"));
        TRACE_host_variable_set(host_name_char_arr, "datasetOnTape", dataset_number(host_name, "-TAPE"));
    }

    xbt_dynar_free(&hosts_dynar);
    return 0;
}

int tracer(int argc, char* argv[]){
	/**
		@type simgrid process
		Writes amount of dataset on each host (disk and tape)  to trace file every @day.

        Simgrid process parameters:
        ---------------------------
        None
	*/
    //return 0;
    double day = 86400;

    while (!GLOBAL_QUEUE->empty()){

        for(auto& storage_name: storage_number_map){

            const char* hostname = storage_name.first.substr(0, storage_name.first.size()-5).c_str();
            if (storage_name.first.back() == 'K'){
                TRACE_host_variable_set(hostname, "datasetOnDisk", storage_name.second);
            }else{
                TRACE_host_variable_set(hostname, "datasetOnTape", storage_name.second);
            }
        }

        for (auto& hostname_map: cumulative_input_site) {
            TRACE_host_variable_set(hostname_map.first.c_str(), "inputData", hostname_map.second);
        }

        for (auto& hostname_map: cumulative_output_site) {
            TRACE_host_variable_set(hostname_map.first.c_str(), "outputData", hostname_map.second);
        }

        MSG_process_sleep(day);
    }

    return 0;
}

