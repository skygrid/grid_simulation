#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"
#include <stdio.h>

XBT_LOG_NEW_DEFAULT_CATEGORY(tier1, "messsages specific for tier1");
//
// Created by ken on 29.07.16.
//

FILE* fp;

int anomalyLinkTracer(const char* src, const char* dst);
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
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup("0"), xbt_free_f);
    MSG_host_set_property_value(MSG_host_self(), "corruptedCore", xbt_strdup("0"), xbt_free_f);
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
    char* dest;

    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    char inputFilePath[80];
    char copyFilePath[80];
    char outputFilePath[80];
    sprintf(copyFilePath, "/%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType, jobInfo->inputFileName);
    //find location of input file
    if (jobInfo->type != MCSIMULATION) {

        if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost1)) {
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost1, jobInfo->storageType, jobInfo->inputFileName);

            dest = jobInfo->dataLocHost1;
        } else if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost2)) {
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost2, jobInfo->storageType, jobInfo->inputFileName);
            dest = jobInfo->dataLocHost2;
        } else if (!strcmp(MSG_host_get_name(MSG_host_self()), jobInfo->dataLocHost3)) {
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost3, jobInfo->storageType, jobInfo->inputFileName);
            dest = jobInfo->dataLocHost3;
        } else {
            //msg_host_t file
            sprintf(inputFilePath, "/%s%s/%s", jobInfo->dataLocHost1, jobInfo->storageType, jobInfo->inputFileName);
            dest = jobInfo->dataLocHost1;
        }

        plusLinkCounter(dest, MSG_host_get_name(MSG_host_self()));

        file = MSG_file_open(inputFilePath, NULL);
        msg_error_t error = MSG_file_rcopy(file, MSG_host_self(), copyFilePath);
        //sg_size_t a = MSG_file_read(file, (sg_size_t) jobInfo->inputSize);
        //XBT_INFO("READ from %s  at %f, %s", dest, MSG_get_clock(), jobInfo->name);

        minusLinkCounter(dest, MSG_host_get_name(MSG_host_self()));

        MSG_file_close(file);
        //Handling anomalies
        if (error != MSG_OK){
            writeAnomaly(MSG_get_clock());
            char* reserv_loc[3] = {jobInfo->dataLocHost1, jobInfo->dataLocHost2, jobInfo->dataLocHost3};
            for (int i = 0; i < 3; ++i) {
                if (reserv_loc[i] != dest & reserv_loc[i] != "0")
                    sprintf(inputFilePath, "/%s%s/%s", reserv_loc[i], jobInfo->storageType, jobInfo->inputFileName);

                    plusLinkCounter(reserv_loc[i], MSG_host_get_name(MSG_host_self()));
                    file = MSG_file_open(inputFilePath, NULL);
                    msg_error_t err1 = MSG_file_rcopy(file, MSG_host_self(), copyFilePath);
                    //sg_size_t a = MSG_file_read(file, (sg_size_t) jobInfo->inputSize);
                    MSG_file_close(file);

                    if (err1 != MSG_OK){
                        anomalyLinkTracer(reserv_loc[i], MSG_host_get_name(MSG_host_self()));
                        writeAnomaly(MSG_get_clock());
                    } else{
                        minusLinkCounter(reserv_loc[i], MSG_host_get_name(MSG_host_self()));
                        break;
                    }
            }
        }

    }
    sprintf(outputFilePath, "/%s%s/%s", MSG_host_get_name(MSG_host_self()), jobInfo->storageType,
            jobInfo->outputName);

    // CREATING AND EXECUTION OF TASK
    task = MSG_task_create(jobInfo->name, jobInfo->compSize, 0, NULL);
    jobInfo->stExecClock = MSG_get_clock();
    addActiveCoreT();
    msg_error_t b = MSG_task_execute(task);
    subActiveCoreT();
    jobInfo->endExecClock = MSG_get_clock();
    minusOneActiveCore();
    jobInfo->success_or_anom = 1;


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
    memset(copyFilePath, '\0', 80);
    writeToFile(fp, jobInfo);
    MSG_process_kill(MSG_process_self());
    return 0;
}



void my_on_exit(){
    jobPtr jobInfo = MSG_process_get_data(MSG_process_self());
    jobInfo->success_or_anom = 0;
    jobInfo->endExecClock = MSG_get_clock();
    writeToFile(fp, jobInfo);
}



void plusOneActiveCore(){
    MSG_sem_acquire(sem);
    char kot[50];
    long number;
    number = xbt_str_parse_int(MSG_host_get_property_value(MSG_host_self(), "activeCore"), "error");
    number++;
    sprintf(kot, "%ld", number);
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), xbt_free_f);
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
    MSG_host_set_property_value(MSG_host_self(), "activeCore", xbt_strdup(kot), xbt_free_f);
    memset(kot, '\0', 50);
    MSG_sem_release(sem);
}
