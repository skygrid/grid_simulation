//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "my_structures.h"
#include "myfunc_list.h"
#include <string>

XBT_LOG_NEW_DEFAULT_CATEGORY(datareplica, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);

int data_replicator(int argc, char* argv[]){
    Job* replica = (Job*) MSG_process_get_data(MSG_process_self());
//    size_t output_files = replica->OutputFiles.size();
//
//    for (size_t k = 0; k < output_files; ++k) {
//
//        const vector<std::string>& replica_locations = FILES_DATABASE->at(replica->OutputFiles.at(k))->Storages;
//
//        for (size_t i = 0; i < replica_locations.size(); ++i) {
//            UploadData* data = new UploadData;
//            data->filename = replica->OutputFiles.at(k);
//            data->dest = replica_locations.at(i);
//
//            MSG_process_create("upload", uploader, data, MSG_host_self());
//        }
//    }
//
//
//    MSG_process_sleep(1.1);
    delete replica;
    return 0;
}



int uploader(int argc, char* argv[]){
    string host_name = MSG_host_get_name(MSG_host_self());
    string curFilePath;
    string pathAtDest;
    string destHostName;
    string stor_type;

    msg_file_t file = NULL;

    UploadData* data = (UploadData*) MSG_process_get_data(MSG_process_self());

    //Copy to own tape
    if (!data->dest.compare("+")){

        curFilePath = "/" + host_name + "1/" + data->filename;
        pathAtDest = "/" + host_name + "0/" + data->filename;
        file = MSG_file_open(curFilePath.c_str(), NULL);
        MSG_file_rcopy(file, MSG_host_self(), pathAtDest.c_str());

        destHostName = host_name;
        stor_type = "0";

        // tracing
        dataset_number_change(destHostName+"0", 1);

    }else{
        destHostName = data->dest;
        destHostName.pop_back(); // CERN1 --> CERN
        stor_type = data->dest.back();
    }

    if (destHostName.compare(host_name)) {

        curFilePath = "/" + host_name + "1/" + data->filename;
        pathAtDest = "/" + data->dest + "/" + data->filename;

        file = MSG_file_open(curFilePath.c_str(), NULL);
        msg_host_t dest = MSG_host_by_name(destHostName.c_str());

        //plusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);

        msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest.c_str());
        if (!atoi(stor_type.c_str())) create_file_label(pathAtDest);

        //trace number of datasets and output traffic from site
        dataset_number_change(destHostName + stor_type, 1);
        cumulative_output_per_site(host_name, (double) MSG_file_get_size(file));

        if (a == MSG_OK) {
            tracer_traffic(host_name, destHostName, (double) MSG_file_get_size(file));
            minusLinkCounter(host_name, destHostName);
            MSG_file_close(file);
            //XBT_INFO("Creating replica completed at %s", MSG_host_get_name(dest));
        }  else {
            minusLinkCounter(host_name, destHostName);
            MSG_file_close(file);
            XBT_INFO("Transfer fail occurred");
        }
    }

    delete data;

    return 0;

}
