//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
#include <string>

XBT_LOG_NEW_DEFAULT_CATEGORY(datareplica, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);

int data_replicator(int argc, char* argv[]){

    Job* replica = (Job*) MSG_process_get_data(MSG_process_self());
    int replica_number = replica->outputNumber;

    string outputLocations[] = {replica->outputHost1, replica->outputHost2, replica->outputHost3, replica->outputHost4, replica->outputHost5,
                               replica->outputHost6, replica->outputHost7, replica->outputHost8, replica->outputHost9, replica->outputHost10};

    for (int i = 0; i < replica_number; ++i) {

        UploadData* data = new UploadData;
        data->filename = replica->outputName;
        data->dest = outputLocations[i];

        MSG_process_create("upload", uploader, data, MSG_host_self());
    }

    MSG_process_sleep(1.1);
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

        //addDatasetAmountT(host_name, "0");
        cumulativeOutputPerSiteT(host_name, (double) MSG_file_get_size(file));

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
        create_file_label(pathAtDest);

        //trace number of datasets and output traffic from site
        //addDatasetAmountT(destHostName, stor_type);
        cumulativeOutputPerSiteT(host_name, (double) MSG_file_get_size(file));

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

    //trace storage
    //tracer_storage(destHostName, stor_type);

    //clearing memory
    delete data;

    return 0;

}
