//
// Created by ken on 11.09.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"
#include "messages.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(matcher, "messages specific for matcher");

jobBatch_AmountPtr matcher(long amountRequestedJob){
    int amount_of_matched_jobs = 0;

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

    while (amount_of_matched_jobs < amountRequestedJob && (local_current != NULL)) {
        local_current->jobX->startSchedulClock = MSG_get_clock();
        local_current->jobX->stExecClock = 0;
        local_current->jobX->endExecClock = 0;
        local_current->jobX->successExecuted = 0;
        jobBatch[amount_of_matched_jobs] = local_current->jobX;
        amount_of_matched_jobs++;

        // DELETE JOB FROM QUEUE
        delete_job_from_queue(local_current, local_previous, current, previous);

        local_current = local_current->next;
        current = current->next;
    }

    //free_local_queue();
    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;
    return jobBatchInfoX;
}

jobBatch_AmountPtr matcher_DAM(long amountRequestedJob, const char* host){

    int amount_of_matched_jobs = 0;

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
    while(amount_of_matched_jobs < amountRequestedJob && (local_current != NULL)){
        int k = 0;
        if (local_current->jobX->type == MCSIMULATION){
            local_current->jobX->startSchedulClock = MSG_get_clock();
            local_current->jobX->successExecuted = 0;
            jobBatch[amount_of_matched_jobs] = local_current->jobX;
            amount_of_matched_jobs++;

            delete_job_from_queue(local_current, local_previous, current, previous);

            local_current = local_current->next;
            current = current->next;

            continue;
        }

        char* dataLocations[] = {local_current->jobX->dataLocHost1, local_current->jobX->dataLocHost2, local_current->jobX->dataLocHost3, local_current->jobX->dataLocHost4,
                                 local_current->jobX->dataLocHost5, local_current->jobX->dataLocHost6, local_current->jobX->dataLocHost7, local_current->jobX->dataLocHost8,
                                 local_current->jobX->dataLocHost9, local_current->jobX->dataLocHost10};
        char* storage_types[] = {local_current->jobX->storageType1, local_current->jobX->storageType2, local_current->jobX->storageType3, local_current->jobX->storageType4,
                                 local_current->jobX->storageType5, local_current->jobX->storageType6, local_current->jobX->storageType7, local_current->jobX->storageType8,
                                 local_current->jobX->storageType9, local_current->jobX->storageType10};

        int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);
        for (int j = 0; j < n; ++j) {
            if ((!strcmp(dataLocations[j], host)) & (!strcmp(storage_types[j], "1"))){
                k = 5;
                local_current->jobX->startSchedulClock = MSG_get_clock();
                jobBatch[amount_of_matched_jobs] = local_current->jobX;
                amount_of_matched_jobs++;

                delete_job_from_queue(local_current, local_previous, current, previous);

                local_current = local_current->next;
                current = current->next;
                break;
            }
        }
        if (k == 0){
            local_previous = local_current;
            local_current = local_current->next;
            previous = current;
            current = current->next;
        }
    }

    // If there are no more matched jobs, but job queue still exists
    long remained_jobs = amountRequestedJob - amount_of_matched_jobs;
    if ((remained_jobs > 0) && (localLength() > 0)){
        remained_jobs = (remained_jobs < localLength()) ? remained_jobs : (remained_jobs - localLength());

        jobBatch_AmountPtr batch = matcher(remained_jobs);

        for (int i = 0; i < batch->jobsAmount; ++i) {
            jobBatch[i+amount_of_matched_jobs] = batch->jobBatch[i];
        }
        amount_of_matched_jobs += batch->jobsAmount;
    }

    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;

    return jobBatchInfoX;
}

