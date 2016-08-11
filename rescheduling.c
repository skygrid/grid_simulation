//
// Created by ken on 09.08.16.
//
#include <simgrid/msg.h>
#include "messages.h"

long currentJobInQueue;
int* jobQueueReschedulingHelper;
jobPtr* jobQueueHelper;

int rescheduling(long failedReqJobs){
    currentJobInQueue -= failedReqJobs;
    return 0;
}

int rescheduling_DAM(int failedReqJobs){
    for (int i = 0; i < failedReqJobs; ++i) {
        jobQueueHelper[jobQueueReschedulingHelper[i]] = 0;
    }
    return 0;
}
