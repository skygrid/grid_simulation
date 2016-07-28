//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(tier2, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);

int data_replicator(int argc, char* argv[]){
    replicatorDataPtr replica = MSG_process_get_data(MSG_process_self());
    int replica_number = replica->replicaAmount;

    if (replica_number == 1){
        uploadDataPtr data1 = xbt_new(uploadData, 1);
        data1->filename = replica->fileName;
        data1->dest = replica->outLoc1;

        MSG_process_create("up1", uploader, data1, MSG_host_self());

    } else if (replica_number == 2){
        uploadDataPtr data1 = xbt_new(uploadData, 1);
        data1->filename = replica->fileName;
        data1->dest = replica->outLoc1;

        uploadDataPtr data2 = xbt_new(uploadData, 1);
        data2->filename = replica->fileName;
        data2->dest = replica->outLoc2;

        MSG_process_create("up1", uploader, data1, MSG_host_self());
        MSG_process_create("up2", uploader, data2, MSG_host_self());

    } else if (replica_number == 3){
        uploadDataPtr data1 = xbt_new(uploadData, 1);
        data1->filename = replica->fileName;
        data1->dest = replica->outLoc1;

        uploadDataPtr data2 = xbt_new(uploadData, 1);
        data2->filename = replica->fileName;
        data2->dest = replica->outLoc2;

        uploadDataPtr data3 = xbt_new(uploadData, 1);
        data3->filename = replica->fileName;
        data3->dest = replica->outLoc3;

        MSG_process_create("up1", uploader, data1, MSG_host_self());
        MSG_process_create("up2", uploader, data2, MSG_host_self());
        MSG_process_create("up3", uploader, data3, MSG_host_self());
    }

    return 0;
}

int uploader(int argc, char* argv[]){
    char curFilePath[50];
    char pathAtDest[50];

    uploadDataPtr data = MSG_process_get_data(MSG_process_self());

    sprintf(curFilePath, "%s/%s", MSG_host_get_name(MSG_host_self()), data->filename);
    sprintf(pathAtDest, "%s/%s", data->dest, data->filename);

    msg_file_t file = MSG_file_open(curFilePath, NULL);
    msg_host_t dest = MSG_host_by_name(data->dest);

    msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest);

    if (a == MSG_OK){
        XBT_INFO("Creating replica completed", "%s");
    }else if (a == MSG_HOST_FAILURE){
        XBT_INFO("Host fail occurred", "%s");
    } else if (a == MSG_TRANSFER_FAILURE){
        XBT_INFO("Transfer fail occurred", "%s");
    }
    MSG_file_close(file);
    return 0;

}
