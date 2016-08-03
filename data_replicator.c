//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(datareplica, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);

int data_replicator(int argc, char* argv[]){
    jobPtr replica = MSG_process_get_data(MSG_process_self());
    int replica_number = replica->outputNumber;

    if (replica_number == 1){
        uploadDataPtr data1 = xbt_new(uploadData, 1);
        data1->filename = xbt_strdup(replica->outputName);
        data1->dest = xbt_strdup(replica->outputHost1);
        data1->storageType = xbt_strdup(replica->storageType);
        data1->numberOfReplica = 1;

        MSG_process_create("up1", uploader, data1, MSG_host_self());

    } else if (replica_number == 2){
        uploadDataPtr data1 = xbt_new(uploadData, 1);
        data1->filename = xbt_strdup(replica->outputName);
        data1->dest = xbt_strdup(replica->outputHost1);
        data1->storageType = xbt_strdup(replica->storageType);
        data1->numberOfReplica = 1;

        uploadDataPtr data2 = xbt_new(uploadData, 1);
        data2->filename = xbt_strdup(replica->outputName);
        data2->dest = xbt_strdup(replica->outputHost2);
        data2->storageType = xbt_strdup(replica->storageType);
        data2->numberOfReplica = 2;

        MSG_process_create("up1", uploader, data1, MSG_host_self());
        MSG_process_create("up2", uploader, data2, MSG_host_self());

    } else if (replica_number == 3){
        uploadDataPtr data1 = xbt_new(uploadData, 1);
        data1->filename = xbt_strdup(replica->outputName);
        data1->dest = xbt_strdup(replica->outputHost1);
        data1->storageType = xbt_strdup(replica->storageType);
        data1->numberOfReplica = 1;

        uploadDataPtr data2 = xbt_new(uploadData, 1);
        data2->filename = xbt_strdup(replica->outputName);
        data2->dest = xbt_strdup(replica->outputHost2);
        data2->storageType = xbt_strdup(replica->storageType);
        data2->numberOfReplica = 2;

        uploadDataPtr data3 = xbt_new(uploadData, 1);
        data3->filename = xbt_strdup(replica->outputName);
        data3->dest = xbt_strdup(replica->outputHost3);
        data3->storageType = xbt_strdup(replica->storageType);
        data3->numberOfReplica = 3;

        MSG_process_create("up1", uploader, data1, MSG_host_self());
        MSG_process_create("up2", uploader, data2, MSG_host_self());
        MSG_process_create("up3", uploader, data3, MSG_host_self());
    }
    xbt_free(replica);
    MSG_process_kill(MSG_process_self());
    return 0;
}

int uploader(int argc, char* argv[]){
    char curFilePath[50];
    char pathAtDest[50];
    msg_file_t file = NULL;

    uploadDataPtr data = MSG_process_get_data(MSG_process_self());
    if (strcmp(MSG_host_get_name(MSG_host_self()), data->dest)) {

        sprintf(curFilePath, "/%s%s/%s", MSG_host_get_name(MSG_host_self()), data->storageType, data->filename);
        sprintf(pathAtDest, "/%s%s/%s", data->dest, data->storageType, data->filename);

        file = MSG_file_open(curFilePath, NULL);
        msg_host_t dest = MSG_host_by_name(data->dest);

        msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest);

        if (a == MSG_OK) {
            MSG_file_close(file);
            //XBT_INFO("Creating replica completed at %s", MSG_host_get_name(dest));
        } else if (a == MSG_HOST_FAILURE) {
            writeAnomaly(MSG_get_clock());
            MSG_file_close(file);
            XBT_INFO("Host fail occurred", "%s");
        } else if (a == MSG_TRANSFER_FAILURE) {
            writeAnomaly(MSG_get_clock());
            MSG_file_close(file);
            XBT_INFO("Transfer fail occurred", "%s");
        }

        // Clear memory
        memset(curFilePath, '\0', 255);
        memset(pathAtDest, '\0', 255);
        xbt_free(data);
        //XBT_INFO("DT number of openfiles is %lu", numberofopenedfiles2);
    }
    MSG_process_kill(MSG_process_self());
    return 0;

}
