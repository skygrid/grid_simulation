//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include "messages.h"

long currentJobInQueue;
int* jobQueueReschedulingHelper;
jobPtr* jobQueueHelper;

int rescheduling(int failedReqJobs){
    currentJobInQueue -= failedReqJobs;
    return 0;
}

int rescheduling_DAM(int failedReqJobs){
    int num = failedReqJobs;
    for (int i = 0; i < num; ++i) {
        jobQueueHelper[jobQueueReschedulingHelper[i]] = 0;
    }
    return 0;
}
