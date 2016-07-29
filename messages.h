//
// Created by ken on 23.05.16.
//

#ifndef CSIM2SIM_MESSAGES_H
#define CSIM2SIM_MESSAGES_H

#include <simgrid/msg.h>

#define MESSAGES_SIZE_INSTRUCTION 1
#define MESSAGES_SIZE_GIVEMEDATA 1
#define MESSAGES_SIZE_FINALIZE 1
#define MESSAGES_SIZE 1

typedef enum {
    DATARECONSTRUCTION,
    DATASTRIPPING,
    MCMERGE,
    MCRECONSTRUCTION,
    MCSIMULATION,
    MERGE,
    TURBO,
    WGPRODUCTION,
    TEST,
    UNKNOWN,
    USER
} message_type;


msg_task_t task_message_new(char *name, message_type type, const char *downloadfrom, double size);
msg_task_t give_me_data(const char *name, double flops_amount, double size);
msg_task_t send_data(const char *name, double flops_amount, double size);

void task_message_free(void *);
int task_message_size(message_type type);


typedef struct{
    char* name;
    message_type type;
    double compSize;
    double commSize;
    char* inputFileName;
    int locationInputNum;
    msg_host_t* locationInputHosts;
    char* outputNumber;
    char * outputHost1;
    char * outputHost2;
    char * outputHost3;
    char* downloadSite;
    char* outputName;
    double outputFileSize;
}job, *jobPtr;

typedef struct{
    int replicaAmount;
    char* fileName;
    char* currentLoc;
    char* outLoc1;
    char* outLoc2;
    char* outLoc3;
}replicatorData, *replicatorDataPtr;

typedef struct {
    char* filename;
    char* dest;
}uploadData, *uploadDataPtr;

typedef struct {
    long coreAmount; // Number of requested jobs
}jobBatchRequest, *jobBatchRequestPtr;


typedef struct{
    int jobAmount;
    jobPtr jobArray;
}jobBatch, *jobBatchPtr;

#endif //CSIM2SIM_MESSAGES_H