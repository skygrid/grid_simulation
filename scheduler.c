//
// Created by ken on 24.05.16.
//

#include <stdio.h>
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

FILE* fp; // global variables


#define QUEUE_SIZE 9999
jobBatch_AmountPtr matcher(long amountRequestedJob);
jobBatch_AmountPtr matcher_DAM(long amountRequestedJob, const char* host);
int rescheduling(long failedReqJobs);
int input();

long currentJobInQueue = 0;
long amountOfScheduledJobs = 0;

jobPtr* jobQueue;
int jobQueueHelper[QUEUE_SIZE] = {0};

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

            //jobBatch_AmountPtr batch = matcher(batchRequest->coreAmount);
            jobBatch_AmountPtr batch = matcher_DAM(batchRequest->coreAmount, MSG_host_get_name(MSG_task_get_source(task)));
            taskB = MSG_task_create("", batch->jobsAmount, MESSAGES_SIZE, batch->jobBatch);
            //Add new user to link
            plusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));

            switch(MSG_task_send(taskB, MSG_host_get_name(MSG_task_get_source(task)))){
                case MSG_OK:
                    minusLinkCounter(MSG_host_get_name(MSG_host_self()), MSG_host_get_name(MSG_task_get_source(task)));
                    XBT_INFO("Send %s after matching %s", batch->jobBatch[0]->name, MSG_host_get_name(MSG_task_get_source(task)));
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
jobBatch_AmountPtr matcher(long amountRequestedJob){
    double waiting_time = 50.;
    int amount_of_scheduled_jobs = 0;
    long cjiq = currentJobInQueue; // currentJobInQueue

    if (currentJobInQueue + amountRequestedJob > QUEUE_SIZE){
        MSG_process_kill(MSG_process_self());
        amountRequestedJob = QUEUE_SIZE - currentJobInQueue;
    }
    XBT_INFO("Amount is %ld", amountRequestedJob);

    jobBatch_AmountPtr jobBatchInfoX = xbt_new(jobBatch_Amount, 1);
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);

    // wait until job will be created in the queue
    while (MSG_get_clock() < jobQueue[currentJobInQueue]->submissionTime){
        if (MSG_get_clock() > jobQueue[currentJobInQueue]->submissionTime){
            break;
        }
        MSG_process_sleep(waiting_time);
    }

    for (long i = currentJobInQueue; i < (cjiq+amountRequestedJob); ++i) {
        jobQueue[i]->startSchedulClock = MSG_get_clock();
        jobQueue[i]->stExecClock = 0;
        jobQueue[i]->endExecClock = 0;
        jobQueue[i]->successExecuted = 0;
        jobBatch[i-cjiq] = jobQueue[i];
        amount_of_scheduled_jobs++;
        currentJobInQueue++;

        // if (i+1) job isn't created then we leave the scheduler
        if (MSG_get_clock() < jobQueue[currentJobInQueue+1]->submissionTime){
            jobBatchInfoX->jobBatch = jobBatch;
            jobBatchInfoX->jobsAmount = amount_of_scheduled_jobs;
            return jobBatchInfoX;
        }
    }
    if (currentJobInQueue > QUEUE_SIZE){
        XBT_INFO("QUEUE is run out");
    }

    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_scheduled_jobs;
    return jobBatchInfoX;
}

jobBatch_AmountPtr matcher_DAM(long amountRequestedJob, const char* host){
    currentJobInQueue = 0;

    double waiting_time = 50.;
    int amount_of_matched_jobs = 0;


    if (amountOfScheduledJobs + amountRequestedJob >= QUEUE_SIZE){
        XBT_INFO("QUEUE is run out");
        MSG_process_kill(MSG_process_self());
    }
    jobBatch_AmountPtr jobBatchInfoX = xbt_new(jobBatch_Amount, 1);
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);

    /*while (MSG_get_clock() < jobQueue[currentJobInQueue]->submissionTime){
        if (MSG_get_clock() > jobQueue[currentJobInQueue]->submissionTime){
            break;
        }
        MSG_process_sleep(waiting_time);
    }*/

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
                amount_of_matched_jobs++;

                if (MSG_get_clock() < jobQueue[currentJobInQueue+1]->submissionTime){
                    jobBatchInfoX->jobBatch = jobBatch;
                    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;
                    return jobBatchInfoX;
                }


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
                    amount_of_matched_jobs++;

                    if (MSG_get_clock() < jobQueue[currentJobInQueue+1]->submissionTime){
                        jobBatchInfoX->jobBatch = jobBatch;
                        jobBatchInfoX->jobsAmount = amount_of_matched_jobs;
                        return jobBatchInfoX;
                    }

                    break;
                }
            }
            currentJobInQueue++;
        }else currentJobInQueue++;
        int k = 5;
    }


    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;

    return jobBatchInfoX;
}


int give_any_jobs(long amount_matched, long amount_requested){
    long current_job_queue = 0;
    long jobs = amount_requested - amount_matched;

    while ()
}






