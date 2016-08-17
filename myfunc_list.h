//
// Created by ken on 29.07.16.
//

#ifndef CSIM2SIM_MYFUNC_LIST_H
#define CSIM2SIM_MYFUNC_LIST_H

#include <stdio.h>
#include "messages.h"
extern FILE* fp;
extern msg_sem_t sem_link;

int data_replicator(int argc, char* argv[]);
int job_requester();
jobType charToEnum(char *sval);
int writeToFile(FILE* fpx, jobPtr jobInfo);
int clearFile();
int writeAnomaly(double clock);
int set_0_all_routes();

int plusLinkCounter(const char* src, const char* dst);
int minusLinkCounter(const char* src, const char* dst);
int tracer_traffic(const char* src, const char* dst, double size);

int addActiveCoreT();
int subActiveCoreT();

int addCorruptedCoreT();
int subCorruptedCoreT();

int tracer_storage(char* storage_name, char* storage_type);
#endif //CSIM2SIM_MYFUNC_LIST_H
