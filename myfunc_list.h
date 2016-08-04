//
// Created by ken on 29.07.16.
//

#ifndef CSIM2SIM_MYFUNC_LIST_H
#define CSIM2SIM_MYFUNC_LIST_H

#include <stdio.h>
extern FILE* fp;

int data_replicator(int argc, char* argv[]);
int job_requester();
jobType charToEnum(char *sval);
int writeToFile(FILE* fpx, jobPtr jobInfo);
int clearFile();
int writeAnomaly(double clock);
int int pileOfFailedTask();
#endif //CSIM2SIM_MYFUNC_LIST_H
