//
// Created by ken on 24.05.16.
//

#include <stdio.h>
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

FILE* fp; // global variables


#define QUEUE_SIZE 10000
jobPtr* matcher(long amountRequestedJob);
jobPtr* matcher_DAM(long amountRequestedJob, const char* host);
int rescheduling(long failedReqJobs);
int input();

long currentJobInQueue = 0;
long amountOfScheduledJobs = 0;

jobPtr* jobQueue;
int* jobQueueHelper;

XBT_LOG_NEW_DEFAULT_CATEGORY(scheduler, "messages specific for scheduler");

int scheduler(int argc, char* argv[]){
    input();
    char mailbox[30];
    msg_task_t task = NULL;
    msg_task_t taskB = NULL;
    sprintf(mailbox, "scheduler");

    while (1){
        int res = MSG_task_receive(&task, mailbox);
        // Anomalies
        if (res == MSG_OK){
            XBT_INFO("Get job request from %s", MSG_host_get_name(MSG_task_get_source(task)));
            jobBatchRequestPtr batchRequest = MSG_task_get_data(task);

            //jobPtr* batch = matcher(batchRequest->coreAmount);
            jobPtr* batch = matcher_DAM(batchRequest->coreAmount, MSG_host_get_name(MSG_task_get_source(task)));
            taskB = MSG_task_create("", batchRequest->coreAmount, MESSAGES_SIZE, batch);
            //Add new user to link
            plusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));

            switch(MSG_task_send(taskB, MSG_host_get_name(MSG_task_get_source(task)))){
                case MSG_OK:
                    minusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));
                    XBT_INFO("Send %s after matching %s", batch[0]->name, MSG_host_get_name(MSG_task_get_source(task)));
                    break;
                case MSG_TRANSFER_FAILURE:
                    rescheduling(batchRequest->coreAmount);
                    MSG_task_destroy(taskB);
                    taskB = NULL;
                    break;
                case MSG_HOST_FAILURE:
                    MSG_task_destroy(taskB);
                    taskB = NULL;
                    break;
            }

            if(!strcmp(MSG_task_get_name(task), "finalize")){
                MSG_task_destroy(task);
                break;
            }
            xbt_free(batchRequest);
            MSG_task_destroy(task);
            task = NULL;

        }else if (res == MSG_TRANSFER_FAILURE){
            XBT_INFO("transfer failure occur");
        }
    }
}
jobPtr* matcher(long amountRequestedJob){

    if (currentJobInQueue + amountRequestedJob > QUEUE_SIZE){
        MSG_process_kill(MSG_process_self());
        amountRequestedJob = QUEUE_SIZE - currentJobInQueue;
    }
    XBT_INFO("Amount is %ld", amountRequestedJob);
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);
    for (long i = currentJobInQueue; i < (currentJobInQueue+amountRequestedJob); ++i) {
        jobQueue[i]->startSchedulClock = MSG_get_clock();
        jobQueue[i]->stExecClock = 0;
        jobQueue[i]->endExecClock = 0;
        jobQueue[i]->successExecuted = 0;
        jobBatch[i-currentJobInQueue] = jobQueue[i];
    }
    currentJobInQueue += amountRequestedJob;
    if (currentJobInQueue > QUEUE_SIZE){
        XBT_INFO("QUEUE is run out");
    }
    return jobBatch;
}

jobPtr* matcher_DAM(long amountRequestedJob, const char* host){
    currentJobInQueue = 0;

    if (amountOfScheduledJobs + amountRequestedJob >= QUEUE_SIZE){
        XBT_INFO("QUEUE is run out");
        MSG_process_kill(MSG_process_self());
    }
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);
    long i = 0;
    while(i < amountRequestedJob){
        if (currentJobInQueue >= QUEUE_SIZE){
            XBT_INFO("QUEUE is run out");
            MSG_process_kill(MSG_process_self());
            break;
        }

        if (jobQueueHelper[currentJobInQueue] == 0) {
            if (jobQueue[currentJobInQueue]->type == MCSIMULATION){
                jobQueueHelper[currentJobInQueue] = 1;
                jobQueue[currentJobInQueue]->startSchedulClock = MSG_get_clock();
                jobBatch[i] = jobQueue[currentJobInQueue];
                i++;
                currentJobInQueue++;
                amountOfScheduledJobs++;
                continue;
            }

            char* dataLocations[] = {jobQueue[currentJobInQueue]->dataLocHost1, jobQueue[currentJobInQueue]->dataLocHost2, jobQueue[currentJobInQueue]->dataLocHost3, jobQueue[currentJobInQueue]->dataLocHost4};
            int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);
            for (int j = 0; j < n; ++j) {
                if (!strcmp(dataLocations[j], host)){
                    jobQueueHelper[currentJobInQueue] = 1;
                    jobQueue[currentJobInQueue]->startSchedulClock = MSG_get_clock();
                    jobBatch[i] = jobQueue[currentJobInQueue];
                    i++;
                    amountOfScheduledJobs++;
                    break;
                }
            }
            currentJobInQueue++;
        }else currentJobInQueue++;
    }
    return jobBatch;
}






