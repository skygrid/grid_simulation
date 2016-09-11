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

        local_previous = local_current;
        local_current = local_current->next;
        previous = current;
        current = current->next;
    }

    free_local_queue();
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
        if (local_current->jobX->type == MCSIMULATION){
            //XBT_INFO("MC; for %s matched name: %s", host, local_current->jobX->name);
            local_current->jobX->startSchedulClock = MSG_get_clock();
            local_current->jobX->successExecuted = 0;
            jobBatch[amount_of_matched_jobs] = local_current->jobX;
            amount_of_matched_jobs++;

            delete_job_from_queue(local_current, local_previous, current, previous);

            local_previous = local_current;
            local_current = local_current->next;
            previous = current;
            current = current->next;

            continue;
        }

        char* dataLocations[] = {local_current->jobX->dataLocHost1, local_current->jobX->dataLocHost2, local_current->jobX->dataLocHost3, local_current->jobX->dataLocHost4};
        int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);
        for (int j = 0; j < n; ++j) {
            if (!strcmp(dataLocations[j], host)){
                local_current->jobX->startSchedulClock = MSG_get_clock();
                jobBatch[amount_of_matched_jobs] = local_current->jobX;
                amount_of_matched_jobs++;

                delete_job_from_queue(local_current, local_previous, current, previous);

                break;
            }
        }

        local_previous = local_current;
        local_current = local_current->next;
        previous = current;
        current = current->next;
    }

    // If there are no more matched jobs, but job queue still exists
    /*if ((amountRequestedJob - amount_of_matched_jobs) < localLength()){
        long remained_jobs = amountRequestedJob - amount_of_matched_jobs;
        jobBatch_AmountPtr batch = matcher(remained_jobs);

        for (int i = 0; i < batch->jobsAmount; ++i) {
            jobBatch[i+amount_of_matched_jobs] = batch->jobBatch[i];
        }
        amount_of_matched_jobs += batch->jobsAmount;
    }*/
    if (MSG_get_clock() == 50.012164948453609){
        printList();
        XBT_INFO("\n\n");
        for (int i = 0; i < amount_of_matched_jobs; ++i) {
            XBT_INFO("%s", jobBatch[i]->name);
        }
    }


    jobBatchInfoX->jobBatch = jobBatch;
    jobBatchInfoX->jobsAmount = amount_of_matched_jobs;

    return jobBatchInfoX;
}

