//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include "my_structures.h"
#include "myfunc_list.h"
#define str(x) #x
FILE * FP;

int writeToFile(Job* jobInfo)
{
    MSG_sem_acquire(sem_link);
    static size_t n = 0;
    if (n == 0) clearFile();
    const char* typesStr[] = {"USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN",
                        "MCSIMULATION", "TEST", NULL};
    fprintf(FP, "%lu,%d,", jobInfo->JobId, jobInfo->successExecuted); //typesStr[jobInfo->type]
    fprintf(FP, "%.0f,%f,%f,%f,", jobInfo->SubmissionTime, jobInfo->startSchedulClock, jobInfo->StartExecTime, jobInfo->EndExecTime);
    fprintf(FP, "%s,%s\n", jobInfo->Federation.c_str(), jobInfo->JobType.c_str());
    n++;
    MSG_sem_release(sem_link);
    return(0);
}

int clearFile()
{
    FP = fopen (path_to_output.c_str(), "w");
    fprintf(FP, "%s", "JobName, Success, Time start, time schedule, Time start Exec, Time End Exec, Tier, Type\n");
    //fclose(FP);
    //fp1 = fopen("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "w");
    //fclose(fp1);

    return 0;
}

int writeAnomaly(double clock)
{
    /*FILE * fp;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "a");
    fprintf(fp, "%f\n", clock);
    fclose(fp);*/
    return(0);
}

