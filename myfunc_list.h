//
// Created by ken on 29.07.16.
//

#ifndef CSIM2SIM_MYFUNC_LIST_H
#define CSIM2SIM_MYFUNC_LIST_H

int data_replicator(int argc, char* argv[]);
int job_requester();
jobType charToEnum(char *sval);
int writeToFile(jobPtr jobInfo);
int clearFile();
int writeAnomaly(double clock);
#endif //CSIM2SIM_MYFUNC_LIST_H
