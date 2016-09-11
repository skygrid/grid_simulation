//
// Created by ken on 10.09.16.
//

#include <stdio.h>
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

jobBatch_AmountPtr matcher(long amountRequestedJob);
jobBatch_AmountPtr matcher_DAM(long amountRequestedJob, const char* host);
void delete_queue_job(struct node* current, struct node* previous);
struct node* create_current_queue();


jobBatch_AmountPtr matcher(long amountRequestedJob){
    double waiting_time = 50.;
    int amount_of_scheduled_jobs = 0;

    if (isEmpty()){
        MSG_process_kill(MSG_process_self());
    }
    if (length() < amountRequestedJob){
        amountRequestedJob = length();
    }

    jobBatch_AmountPtr jobBatchInfoX = xbt_new(jobBatch_Amount, 1);
    jobPtr* jobBatch = xbt_new(jobPtr, amountRequestedJob);

    struct node* current = create_current_queue();
    struct node* previous = NULL;

    // wait until job will be created in the queue
    while (MSG_get_clock() < current->jobX->submissionTime){
        if (MSG_get_clock() > current->jobX->submissionTime){
            break;
        }
        MSG_process_sleep(waiting_time);
    }

    while (amount_of_scheduled_jobs < amountRequestedJob) {
        current->jobX->startSchedulClock = MSG_get_clock();
        current->jobX->stExecClock = 0;
        current->jobX->endExecClock = 0;
        current->jobX->successExecuted = 0;
        jobBatch[amount_of_scheduled_jobs] = current->jobX;
        amount_of_scheduled_jobs++;

        // DELETE JOB FROM QUEUE
        delete_queue_job(current, previous);

        // if (i+1) job isn't created then we leave the scheduler
        if (MSG_get_clock() < current->next->jobX->submissionTime){
            jobBatchInfoX->jobBatch = jobBatch;
            jobBatchInfoX->jobsAmount = amount_of_scheduled_jobs;
            return jobBatchInfoX;
        }
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

            delete_queue_job(current, previous);

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

void delete_queue_job(struct node* current, struct node* previous){
    if(current == head) {
        head = head->next;
    }else {
        previous->next = current->next;
    }
    free(current);
}

struct node* create_current_queue(){
    struct node* local_head = NULL;
    struct node* local_end = NULL;

    if (head->jobX->submissionTime < MSG_get_clock()){
        struct node* current_job = head;
        local_head = head;

        while (current_job->jobX->submissionTime > MSG_get_clock()){
            localInsertLast(current_job->jobX, local_head, local_end);
            current_job = current_job->next;
        }
    }

    return local_head;
}
