//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
long currentJobInQueue;

int rescheduling(int* failedReqJobs){
    currentJobInQueue -= *failedReqJobs;
    return 0;
}
