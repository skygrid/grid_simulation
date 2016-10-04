//
// Created by ken on 28.07.16.
//
#include <simgrid/msg.h>
#include "messages.h"
#include "myfunc_list.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(datareplica, "Messages specific for creating replicas");
int uploader(int argc, char* argv[]);
int data_replicator(int argc, char* argv[]);
int free_job_memory(jobPtr job);
int clear_uploader_data(uploadDataPtr data);

int data_replicator(int argc, char* argv[]){
    jobPtr replica = (jobPtr) MSG_process_get_data(MSG_process_self());
    int replica_number = replica->outputNumber;

    char* outputLocations[] = {replica->outputHost1, replica->outputHost2, replica->outputHost3, replica->outputHost4, replica->outputHost5,
                               replica->outputHost6, replica->outputHost7, replica->outputHost8, replica->outputHost9, replica->outputHost10};

    for (int i = 0; i < replica_number; ++i) {
        uploadDataPtr data = xbt_new(uploadData, 1);
        data->filename = xbt_strdup(replica->outputName);
        data->dest = xbt_strdup(outputLocations[i]);
        MSG_process_create("upload", uploader, data, MSG_host_self());
    }
    MSG_process_sleep(0.9);
    free_job_memory(replica);
    MSG_process_kill(MSG_process_self());
    return 0;
}

int free_job_memory(jobPtr job){
    free(job->name);
    //free(&job->submissionTime);
    //free(&job->compSize);
    free(job->inputFileName);
    free(job->outputName);

    free(job->dataLocHost1);
    free(job->dataLocHost2);
    free(job->dataLocHost3);
    free(job->dataLocHost4);
    free(job->dataLocHost5);
    free(job->dataLocHost6);
    free(job->dataLocHost7);
    free(job->dataLocHost8);
    free(job->dataLocHost9);
    free(job->dataLocHost10);

    free(job->outputHost1);
    free(job->outputHost2);
    free(job->outputHost3);
    free(job->outputHost4);
    free(job->outputHost5);
    free(job->outputHost6);
    free(job->outputHost7);
    free(job->outputHost8);
    free(job->outputHost9);
    free(job->outputHost10);
    free(job->storageType1);
    free(job->storageType2);
    free(job->storageType3);
    free(job->storageType4);
    free(job->storageType5);
    free(job->storageType6);
    free(job->storageType7);
    free(job->storageType8);
    free(job->storageType9);
    free(job->storageType10);

    //free((char*)job->tier);

    free(job);
    return 0;
}

int uploader(int argc, char* argv[]){
    msg_file_t file = NULL;
    char *curFilePath = (char*) malloc(50);
    char *pathAtDest = (char*) malloc(50);
    char *destHostName = (char*) malloc(15);
    char *stor_type = (char*) malloc(1);

    uploadDataPtr data = (uploadDataPtr) MSG_process_get_data(MSG_process_self());

    //Copy to own tape
    if (!strcmp(data->dest, "+")){
        sprintf(curFilePath, "/%s1/%s", MSG_host_get_name(MSG_host_self()), data->filename);
        sprintf(pathAtDest, "/%s0/%s", MSG_host_get_name(MSG_host_self()), data->filename);
        file = MSG_file_open(curFilePath, NULL);
        MSG_file_rcopy(file, MSG_host_self(), pathAtDest);

        sprintf(destHostName, "%s", (char*) MSG_host_get_name(MSG_host_self()));
        sprintf(stor_type, "%s", "0");

        addDatasetAmountT(MSG_host_get_name(MSG_host_self()), "0");
        cumulativeOutputPerSiteT(MSG_host_get_name(MSG_host_self()), (double) MSG_file_get_size(file));

    }else{
        destHostName = strdup(data->dest);
        destHostName[strlen(destHostName)-1] = '\0';
        strcpy(stor_type, &(data->dest)[strlen(data->dest)-1]);
    }

    if (strcmp(MSG_host_get_name(MSG_host_self()), destHostName)) {

        sprintf(curFilePath, "/%s1/%s", MSG_host_get_name(MSG_host_self()), data->filename);
        sprintf(pathAtDest, "/%s/%s", data->dest, data->filename);

        file = MSG_file_open(curFilePath, NULL);
        msg_host_t dest = MSG_host_by_name(destHostName);

        plusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);

        msg_error_t a = MSG_file_rcopy(file, dest, pathAtDest);
        create_file_label(pathAtDest);

        //trace number of datasets and output traffic from site
        addDatasetAmountT(destHostName, &data->dest[strlen(data->dest)-1]);
        cumulativeOutputPerSiteT(MSG_host_get_name(MSG_host_self()), (double) MSG_file_get_size(file));

        if (a == MSG_OK) {
            tracer_traffic(MSG_host_get_name(MSG_host_self()), destHostName, (double) MSG_file_get_size(file));
            minusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);
            MSG_file_close(file);
            //XBT_INFO("Creating replica completed at %s", MSG_host_get_name(dest));
        }  else {
            minusLinkCounter(MSG_host_get_name(MSG_host_self()), destHostName);
            MSG_file_close(file);
            XBT_INFO("Transfer fail occurred");
        }
    }

    //trace storage
    tracer_storage(destHostName, stor_type);

    //clearing memory
    free(stor_type);
    free(curFilePath);
    free(pathAtDest);
    free(destHostName);

    clear_uploader_data(data);
    MSG_process_kill(MSG_process_self());
    return 0;

}

int clear_uploader_data(uploadDataPtr data){
    free(data->dest);
    free(data->filename);
    free(data);
    return 0;
}
