//
// Created by ken on 28.07.16.
// Puprose: data replication
//

#include <simgrid/msg.h>
#include <string>
#include "my_structures.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(datareplica, "Messages specific for creating replicas");

int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);

int data_replicator(int argc, char* argv[]){
	/**
		@type simgrid process
		Launches `uploader` processes to replicate output files. 
	*/

    Job* replica = (Job*) MSG_process_get_data(MSG_process_self());
    size_t output_files = replica->OutputFiles.size();

    for (size_t k = 0; k < output_files; ++k) {

        InputFile* infl;
        try {
            infl = FILES_DATABASE->at(replica->OutputFiles.at(k));
        }catch (std::out_of_range& e){
            continue;
        }

        const std::vector<std::string>& replica_locations = infl->Storages;

        for (size_t i = 0; i < replica_locations.size(); ++i) {
            UploadData* data = new UploadData;
            data->filename = replica->OutputFiles.at(k);
            data->dest = replica_locations.at(i);

            MSG_process_create("upload", uploader, data, MSG_host_self());
        }
    }

    MSG_process_sleep(1.1);
    delete replica;
    return 0;
}

int uploader(int argc, char* argv[]){
	/**
		@type simgrid process
		Uploads output file to another host.
	*/

    std::string host_name = MSG_host_get_name(MSG_host_self());
    std::string curFilePath;
    std::string pathAtDest;
    std::string destHostName;
    std::string stor_type;

    msg_file_t file = NULL;

    UploadData* data = (UploadData*) MSG_process_get_data(MSG_process_self());

    destHostName = data->dest;
    destHostName.erase(destHostName.length()-5);
    stor_type = data->dest.back();

    if (destHostName.compare(host_name)) {

        curFilePath = "/" + host_name + "-DISK" + data->filename;
        pathAtDest = "/" + data->dest + data->filename;

        file = MSG_file_open(curFilePath.c_str(), NULL);
        msg_host_t dest = MSG_host_by_name(destHostName.c_str());

        //plusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);

        msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest.c_str());
        //if (!atoi(stor_type.c_str())) create_file_label(pathAtDest);

        //trace number of datasets and output traffic from site
        dataset_number_change(data->dest, 1);
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
