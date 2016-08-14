//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(datareplica, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);
msg_sem_t sem_link;

int data_replicator(int argc, char* argv[]){
    jobPtr replica = MSG_process_get_data(MSG_process_self());
    int replica_number = replica->outputNumber;

    char* outputLocations[] = {replica->outputHost1, replica->outputHost2, replica->outputHost3, replica->outputHost4, replica->outputHost5, replica->outputHost6};

    for (int i = 0; i < replica_number; ++i) {
        uploadDataPtr data = xbt_new(uploadData, 1);
        data->filename = xbt_strdup(replica->outputName);
        data->dest = xbt_strdup(outputLocations[i]);
        MSG_process_create("upload", uploader, data, MSG_host_self());
    }

    //xbt_free(replica);
    MSG_process_kill(MSG_process_self());
    return 0;
}

int uploader(int argc, char* argv[]){
    char curFilePath[50];
    char pathAtDest[50];
    msg_file_t file = NULL;

    uploadDataPtr data = MSG_process_get_data(MSG_process_self());
    if (strcmp(MSG_host_get_name(MSG_host_self()), data->dest)) {

        sprintf(curFilePath, "/%s1/%s", MSG_host_get_name(MSG_host_self()), data->filename);
        sprintf(pathAtDest, "/%s/%s", data->dest, data->filename);

        file = MSG_file_open(curFilePath, NULL);
        msg_host_t dest = MSG_host_by_name(data->dest);

        plusLinkCounter(MSG_host_get_name(MSG_host_self()), data->dest);

        msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest);

        if (a == MSG_OK) {
            minusLinkCounter(MSG_host_get_name(MSG_host_self()), data->dest);
            MSG_file_close(file);
            XBT_INFO("Creating replica completed at %s", MSG_host_get_name(dest));
        }  else {
            minusLinkCounter(MSG_host_get_name(MSG_host_self()), data->dest);
            MSG_file_close(file);
            XBT_INFO("Transfer fail occurred", "%s");
        }

        // Clear memory
        memset(curFilePath, '\0', 255);
        memset(pathAtDest, '\0', 255);
    }

    xbt_free(data);
    MSG_process_kill(MSG_process_self());
    return 0;

}
