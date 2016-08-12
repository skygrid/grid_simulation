//
// Created by ken on 12.08.16.
//

#include <simgrid/msg.h>

int copy_from_tape_to_disk(char* host_name, char* name){
    msg_file_t file;
    char filename[80];
    char copied_file_name[80];

    sprintf(filename, "/%s1/%s", host_name, name);
    sprintf(copied_file_name, "/%s0/%s", host_name, name);

    // Open, copy and close file
    file = MSG_file_open(filename, NULL);
    MSG_file_rcopy(file, MSG_host_by_name(host_name), copied_file_name);
    MSG_file_close(file);

    memset(filename, '\0', 80);
    memset(copied_file_name, '\0', 80);


}

