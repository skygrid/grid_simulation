#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
#include <stdio.h>

XBT_LOG_NEW_DEFAULT_CATEGORY(tier1, "messsages specific for tier1");
//
// Created by ken on 29.07.16.
//

FILE* fp;

int executorLauncher();
int job_requester();
int executor(int argc, char* argv[]);
void plusOneActiveCore();
void minusOneActiveCore();
msg_sem_t sem;

// MAIN TIER1 FUNCTION
int tier1(int argc, char* argv[]){
    char* tierMailbox = argv[1];
    char** argx = xbt_new(char*, 1);
    argx[0] = tierMailbox;

    // LAUNCH PROCESS
    sem = MSG_sem_init(1);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup("0"), NULL);
    MSG_process_create("tier1_executor", executorLauncher, argx, MSG_host_self());
    MSG_process_create("job_requester", job_requester, NULL, MSG_host_self());

    return 0;
}


int executorLauncher(){
    int i;
    msg_task_t task;
    char* tierMailbox = strdup(((char**) MSG_process_get_data(MSG_process_self()))[0]);

    while (1){
        int a = MSG_task_receive(&task, tierMailbox);
        if (a == MSG_OK){
        } else if (a == MSG_HOST_FAILURE){
            MSG_task_destroy(task);
            task = NULL;
            writeAnomaly(MSG_get_clock());
        } else if (a == MSG_TRANSFER_FAILURE){
            writeAnomaly(MSG_get_clock());
            MSG_task_destroy(task);
            task = NULL;
        }
        if(!strcmp(MSG_task_get_name(task), "finalize")){
            MSG_task_destroy(task);
            break;
        }
        int jobAmount = (int) MSG_task_get_flops_amount(task);
        jobPtr* jobPtrBatchData = MSG_task_get_data(task);
        XBT_INFO("Successfully receive %s", jobPtrBatchData[0]->name);
        //LAUNCH PROCESS TO EXECUTE TASKS
        for (i = 0; i < jobAmount; ++i) {
            MSG_process_create("executor", executor, jobPtrBatchData[i], MSG_host_self());
        }
        MSG_task_destroy(task);
        task = NULL;
    }
    return 0;
}

int executor(int argc, char* argv[]){
    plusOneActiveCore();
    msg_file_t file, outFile;
    msg_task_t task;
    sg_size_t inputSize, outputSize;

    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    char inputFilePath[80];
    char outputFilePath[80];

    //find location of input file
    if (jobInfo->type != MCSIMULATION) {
        if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost1)) {
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost1, jobInfo->storageType, jobInfo->inputFileName);
        } else if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost2)) {
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost2, jobInfo->storageType, jobInfo->inputFileName);
        } else if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost3)) {
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost3, jobInfo->storageType, jobInfo->inputFileName);
        } else {
            //msg_host_t file
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost1, jobInfo->storageType,
                    jobInfo->inputFileName);
        }
        file = MSG_file_open(inputFilePath, NULL);
        sg_size_t a = MSG_file_read(file, (sg_size_t) jobInfo->inputSize);
        if (a == -1){
            writeAnomaly(MSG_get_clock());
        }
        MSG_file_close(file);
    }
    sprintf(outputFilePath, "/%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType,
            jobInfo->outputName);

    // CREATING AND EXECUTION OF TASK
    task = MSG_task_create(jobInfo->name, jobInfo->compSize, 0, NULL);
    jobInfo->stExecClock = MSG_get_clock();
    msg_error_t b = MSG_task_execute(task);
    jobInfo->endExecClock = MSG_get_clock();
    minusOneActiveCore();


    //Anomalies
    if (b == MSG_OK){
        XBT_INFO("%s has successfully executed", jobInfo->name);
        MSG_task_destroy(task);
        task = NULL;
    }else{
        writeAnomaly(MSG_get_clock());
        XBT_INFO("Error has occurred while executing %s", MSG_task_get_name(task));
        MSG_task_destroy(task);
        task = NULL;
    }

    //Write output to file
    outFile = MSG_file_open(outputFilePath, NULL);
    MSG_file_write(outFile, (sg_size_t) (jobInfo->outputFileSize));
    MSG_file_close(outFile);

    MSG_process_create("dataRep", data_replicator, jobInfo, MSG_host_self());

    memset(inputFilePath, '\0', 80);
    memset(outputFilePath, '\0', 80);
    writeToFile(fp, jobInfo);
    MSG_process_kill(MSG_process_self());
    return 0;
}

void plusOneActiveCore(){
    MSG_sem_acquire(sem);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), NULL);
    //XBT_INFO("Active core amount is %s", MSG_host_get_property_value(MSG_host_self(), "activeCore"));
    memset(kot, '\0', 50);
    MSG_sem_release(sem);
}

void minusOneActiveCore(){
    MSG_sem_acquire(sem);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number--;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), NULL);
    //XBT_INFO("Active core amount is %s", MSG_host_get_property_value(MSG_host_self(), "activeCore"));
    memset(kot, '\0', 50);
    MSG_sem_release(sem);
}
