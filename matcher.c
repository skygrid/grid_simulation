//
// Created by ken on 11.09.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"
#include "messages.h"

jobBatch_AmountPtr matcher(long amountRequestedJob){
    int amount_of_scheduled_jobs = 0;

    if (isEmpty()){
        MSG_process_kill(MSG_process_self());
    }
    if (length() < amountRequestedJob){
        amountRequestedJob = length();
    }

    jobBatch_AmountPtr jobBatchInfoX = xbt_new(jobBatch_Amount, 1);
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);

    struct node* local_current = create_current_queue();
    struct node* local_previous = NULL;
    struct node* current = head;
    struct node* previous = NULL;

    while (amount_of_scheduled_jobs < amountRequestedJob && (localLength() > 0)) {
        local_current->jobX->startSchedulClock = MSG_get_clock();
        local_current->jobX->stExecClock = 0;
        local_current->jobX->endExecClock = 0;
        local_current->jobX->successExecuted = 0;
        jobBatch[amount_of_scheduled_jobs] = local_current->jobX;
        amount_of_scheduled_jobs++;

        // DELETE JOB FROM QUEUE
        delete_job_from_queue(local_current, local_previous, current, previous);

        local_previous = local_current;
        local_current = local_current->next;
        previous = current;
        current = current->next;
    }

    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_scheduled_jobs;
    return jobBatchInfoX;
}

jobBatch_AmountPtr matcher_DAM(long amountRequestedJob, const char* host){

    double waiting_time = 50.;
    int amount_of_matched_jobs = 0;

    if (isEmpty()){
        MSG_process_kill(MSG_process_self());
    }
    if (length() < amountRequestedJob){
        amountRequestedJob = length();
    }

    jobBatch_AmountPtr jobBatchInfoX = xbt_new(jobBatch_Amount, 1);
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);

    struct node* current = head;
    struct node* previous = NULL;

    while (MSG_get_clock() < current->jobX->submissionTime){
        if (MSG_get_clock() > current->jobX->submissionTime){
            break;
        }
        MSG_process_sleep(waiting_time);
    }

    while(amount_of_matched_jobs < amountRequestedJob){
        if (current->jobX->type == MCSIMULATION){
            current->jobX->startSchedulClock = MSG_get_clock();
            current->jobX->successExecuted = 0;
            jobBatch[amount_of_matched_jobs] = current->jobX;
            amount_of_matched_jobs++;

            delete_job_from_queue(local_current, local_previous, current, previous);

            if (MSG_get_clock() < current->next->jobX->submissionTime){
                jobBatchInfoX->jobBatch = jobBatch;
                jobBatchInfoX->jobsAmount = amount_of_matched_jobs;
                return jobBatchInfoX;
            }
            continue;
        }

        char* dataLocations[] = {current->jobX->dataLocHost1, current->jobX->dataLocHost2, current->jobX->dataLocHost3, current->jobX->dataLocHost4};
        int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);
        for (int j = 0; j < n; ++j) {
            if (!strcmp(dataLocations[j], host)){
                current->jobX->startSchedulClock = MSG_get_clock();
                jobBatch[amount_of_matched_jobs] = current->jobX;
                amount_of_matched_jobs++;

                delete_queue_job(current, previous);

                if (MSG_get_clock() < current->next->jobX->submissionTime){
                    jobBatchInfoX->jobBatch = jobBatch;
                    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;
                    return jobBatchInfoX;
                }
                break;
            }
        }
        previous = current;
        current = current->next;
    }

    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;

    return jobBatchInfoX;
}

