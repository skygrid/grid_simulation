#include <simgrid/msg.h>

//
// Created by ken on 29.07.16.
//

int busyCoreAmount;

int tier1(int argc, char* argv[]){
    char* tierMailbox = argv[1];
    int coreAmount = (int) xbt_str_parse_int(argv[2], "Invalid argument %s");

    while (1){

    }



    return 0;
}
