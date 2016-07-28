//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
XBT_LOG_NEW_DEFAULT_CATEGORY(tier2, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);

int data_replicator(int argc, char* argv[]){
    int replica_number = (int) xbt_str_parse_int(argv[5], "Error parsing replicas");

    if (replica_number == 1){
        char** argx1 = xbt_new(char*, 3);
        argx1[0] = argv[0]; // filename
        argx1[1] = argv[1]; // destination
        argx1[2] = argv[4]; // current host

        MSG_process_create_with_arguments("up1", uploader, NULL, MSG_host_self(), 3, argx1);

    } else if (replica_number == 2){
        char** argx1 = xbt_new(char*, 3);
        argx1[0] = argv[0]; // filename
        argx1[1] = argv[1]; // destination
        argx1[2] = argv[4]; // current host

        char** argx2 = xbt_new(char*, 3);
        argx2[0] = argv[0]; // filename
        argx2[1] = argv[2]; // destination
        argx2[2] = argv[4]; // current host


        MSG_process_create_with_arguments("up1", uploader, NULL, MSG_host_self(), 3, argx1);
        MSG_process_create_with_arguments("up2", uploader, NULL, MSG_host_self(), 3, argx2);

    } else if (replica_number == 3){

        char** argx1 = xbt_new(char*, 3);
        argx1[0] = argv[0]; // filename
        argx1[1] = argv[1]; // destination
        argx1[2] = argv[4]; // current host

        char** argx2 = xbt_new(char*, 3);
        argx2[0] = argv[0]; // filename
        argx2[1] = argv[2]; // destination
        argx2[2] = argv[4]; // current host

        char** argx3 = xbt_new(char*, 3);
        argx3[0] = argv[0]; // filename
        argx3[1] = argv[3]; // destination
        argx3[2] = argv[4]; // current host

        MSG_process_create_with_arguments("up1", uploader, NULL, MSG_host_self(), 3, argx1);
        MSG_process_create_with_arguments("up2", uploader, NULL, MSG_host_self(), 3, argx2);
        MSG_process_create_with_arguments("up3", uploader, NULL, MSG_host_self(), 3, argx3);
    }

    return 0;
}

int uploader(int argc, char* argv[]){
    char path[30];
    char filePath[30];

    sprintf(filePath, "%s/%s", argv[2], argv[0]);
    msg_file_t file = MSG_file_open(filePath, NULL);
    msg_host_t dest = MSG_host_by_name(argv[1]);

    sprintf(path, "%s/%s", argv[1], argv[0]);
    msg_error_t a = MSG_file_rcopy(file, dest, path);

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
