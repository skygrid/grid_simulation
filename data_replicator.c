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
    MSG_process_sleep(0.9);
    xbt_free(replica); // ATTENTION !!!!!!!!!
    MSG_process_kill(MSG_process_self());
    return 0;
}

int uploader(int argc, char* argv[]){
    msg_file_t file = NULL;
    char *curFilePath = malloc(50);
    char *pathAtDest = malloc(50);
    char *destHostName = malloc(15);

    uploadDataPtr data = MSG_process_get_data(MSG_process_self());

    destHostName = strdup(data->dest);
    destHostName[strlen(destHostName)-1] = '\0';

    if (strcmp(MSG_host_get_name(MSG_host_self()), destHostName)) {

        sprintf(curFilePath, "/%s1/%s", MSG_host_get_name(MSG_host_self()), data->filename);
        sprintf(pathAtDest, "/%s/%s", data->dest, data->filename);

        file = MSG_file_open(curFilePath, NULL);
        msg_host_t dest = MSG_host_by_name(destHostName);

        plusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);

        msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest);

        if (a == MSG_OK) {
            tracer_traffic(MSG_host_get_name(MSG_host_self()), destHostName, (double) MSG_file_get_size(file));
            minusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);
            MSG_file_close(file);
            XBT_INFO("Creating replica completed at %s", MSG_host_get_name(dest));
        }  else {
            minusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);
            MSG_file_close(file);
            XBT_INFO("Transfer fail occurred", "%s");
        }
    }

    //trace storage

    char *stor_type = malloc(1);
    strcpy(stor_type, &(data->dest)[strlen(data->dest)-1]);

    tracer_storage(destHostName, stor_type);
    //clearing memory

    free(stor_type);

    free(curFilePath);
    free(pathAtDest);
    free(destHostName);

    free(data->dest);
    free(data->filename);
    xbt_free(data);
    MSG_process_kill(MSG_process_self());
    return 0;

}
