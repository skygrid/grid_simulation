//
// Created by ken on 23.05.16.
//

#ifndef CSIM2SIM_MESSAGES_H
#define CSIM2SIM_MESSAGES_H

#include <simgrid/msg.h>

#define MESSAGES_SIZE 100000

typedef enum {
    USER,
    DATASTRIPPING,
    MERGE,
    MCStripping,
    DATARECONSTRUCTION,
    TURBO,
    MCRECONSTRUCTION,
    WGPRODUCTION,
    MCMERGE,
    UNKNOWN,
    MCSIMULATION,
    TEST
} jobType;


typedef struct{
    char* name;
    jobType type;
    char* typeChar;
    double compSize;
    char* inputFileName;
    double inputSize;
    int NRep;
    char* dataLocHost1;
    char* dataLocHost2;
    char* dataLocHost3;
    int outputNumber;
    char* outputHost1;
    char* outputHost2;
    char* outputHost3;
    char* outputHost4;
    char* outputHost5;
    char* outputHost6;
    char* downloadSite;
    char* outputName;
    double outputFileSize;
    char* storageType;

    int scheduled;
    double startClock;
    double startSchedulClock;
    double stExecClock;
    double endExecClock;
    int successExecuted;
}job, *jobPtr;


typedef struct{
    char* input_file_path;
    char* copy_file_path;
    char* destination_name;
}dataInfo, *dataInfoPtr;


typedef struct{
    int replicaAmount;
    char* fileName;
    char* currentLoc;
    char* storageType;
    char* outLoc1;
    char* outLoc2;
    char* outLoc3;
    double size;
}replicatorData, *replicatorDataPtr;


typedef struct {
    char* filename;
    int numberOfReplica;
    char* dest;
    char* storageType;
    double size;
}uploadData, *uploadDataPtr;


typedef struct {
    long coreAmount; // Number of requested jobs
}jobBatchRequest, *jobBatchRequestPtr;


#endif //CSIM2SIM_MESSAGES_H