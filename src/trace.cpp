//
// Created by ken on 12.09.16.
//

#include <string>
#include <simgrid/msg.h>
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(ttrace, "messages specific for trace");

msg_sem_t sem_link;

int declare_trace_variables(){
    string variable_name;
    std::string hosts[8] = {"CERN-PROD", "INFN-T1", "IN2P3-CC", "NRC-KI-T1", "pic", "RAL-LCG2", "FZK-LCG2", "NIKHEF-ELPROD"};

    //Declare storage variables for tracing
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 2; ++j) {
            variable_name = hosts[i] + to_string(j);
            TRACE_host_variable_declare(variable_name.c_str());
        }
    }

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

    std::string storage_variable;

    sem_link = MSG_sem_init(1);
    std::string hosts[8] = {"CERN-PROD", "INFN-T1", "IN2P3-CC", "NRC-KI-T1", "pic", "RAL-LCG2", "FZK-LCG2", "NIKHEF-ELPROD"};
    TRACE_link_srcdst_variable_set("CERN-PROD", "CERN-PROD", "directUserAmount", 0);
    TRACE_link_srcdst_variable_set("CERN-PROD", "CERN-PROD", "indirectUserAmount", 0);
    for (int i = 0; i < 8; ++i) {

        //Set all variables of hosts to initial value
        TRACE_host_variable_set(hosts[i].c_str(), "activeCore", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "corruptedCore", 0);

        TRACE_host_variable_set(hosts[i].c_str(), "inputData", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "outputData", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnDisk", 0);
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnTape", 0);


        //Set all variables of links to initial value
        for (int j = i+1; j < 8; ++j) {
            MSG_sem_acquire(sem_link);
            TRACE_link_srcdst_variable_set(hosts[i].c_str(), hosts[j].c_str(), "traffic", 0);
            TRACE_link_srcdst_variable_set(hosts[i].c_str(), hosts[j].c_str(), "directUserAmount", 0);
            TRACE_link_srcdst_variable_set(hosts[i].c_str(), hosts[j].c_str(), "indirectUserAmount", 0);
            MSG_sem_release(sem_link);
        }

        //Set all variables of storage to initial value
        std::string s[] = {"-TAPE", "-DISK"};
        for (int k = 0; k < 2; ++k) {
            storage_variable = hosts[i] + s[k];
            msg_storage_t storage = MSG_storage_get_by_name(storage_variable.c_str());
            TRACE_host_variable_set("CERN-PROD", storage_variable.c_str(), MSG_storage_get_used_size(storage));
        }
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnDisk", dataset_number(hosts[i], "-DISK"));
        TRACE_host_variable_set(hosts[i].c_str(), "datasetOnTape", dataset_number(hosts[i], "-TAPE"));
    }
    return 0;
}

int tracer(int argc, char* argv[]){
    //return 0;
    double day = 86400;

    while (!GLOBAL_QUEUE->empty()){

        for(auto& storage_name: storage_number_map){
            XBT_INFO(storage_name.first.c_str());

            const char* hostname = storage_name.first.substr(0, storage_name.first.size()-1).c_str();
            if (storage_name.first.back() == '1'){
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

