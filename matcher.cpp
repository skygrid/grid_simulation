//
// Created by ken on 11.09.16.
//
#include <simgrid/msg.h>
#include "myfunc_list.h"
#include "messages.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(matcher, "messages specific for matcher");

void check_files_availability(jobPtr jobInfo){
    return;
    char* input_file_path = (char*) malloc(50);
    msg_file_t file;

    char** dataLocations[] = {&jobInfo->dataLocHost1, &jobInfo->dataLocHost2, &jobInfo->dataLocHost3, &jobInfo->dataLocHost4,
                             &jobInfo->dataLocHost5, &jobInfo->dataLocHost6, &jobInfo->dataLocHost7, &jobInfo->dataLocHost8,
                             &jobInfo->dataLocHost9, &jobInfo->dataLocHost10};
    char* storageTypes[] = {jobInfo->storageType1, jobInfo->storageType2, jobInfo->storageType3, jobInfo->storageType4,
                            jobInfo->storageType5, jobInfo->storageType6, jobInfo->storageType7, jobInfo->storageType8,
                            jobInfo->storageType9, jobInfo->storageType10};

    int n = (int) sizeof(dataLocations) / sizeof(dataLocations[0]);

    for (int i = 0; i < n; ++i) {
        if (!strcmp(*dataLocations[i], "0")){
            continue;
        }
        sprintf(input_file_path, "/%s%s/%s", *dataLocations[i], storageTypes[i], jobInfo->inputFileName);
        file = MSG_file_open(input_file_path, NULL);
        if (MSG_file_get_size(file) == 0){
            *dataLocations[i] = "0";
        }

    }
}

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

        //check file availability
        //check_files_availability(local_current->jobX);

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
        //check_files_availability(local_current->jobX);

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

