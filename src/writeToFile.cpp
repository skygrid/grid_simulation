//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include "messages.h"
#include "myfunc_list.h"
#define str(x) #x


int writeToFile(FILE* fpy, Job* jobInfo)
{
    const char* typesStr[] = {"USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN",
                        "MCSIMULATION", "TEST", NULL};
    fprintf(fpy, "%lu,%d,%f,%f,%f,%f,%s,%s\n", jobInfo->JobId, jobInfo->successExecuted, jobInfo->SubmissionTime, jobInfo->startSchedulClock, jobInfo->StartExecTime, jobInfo->EndExecTime, jobInfo->Federation.c_str(), jobInfo->JobType.c_str()); //typesStr[jobInfo->type]
    return(0);
}

int clearFile()
{
    FILE * fp, *fp1;
    fp = fopen (path_to_output, "w");
    fprintf(fp, "%s", "JobName, Success, Time start, time schedule, Time start Exec, Time End Exec, Tier, Type\n");
    fclose(fp);
    //fp1 = fopen("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "w");
    //fclose(fp1);

    return(0);
}

int writeAnomaly(double clock)
{
    /*FILE * fp;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "a");
    fprintf(fp, "%f\n", clock);
    fclose(fp);*/
    return(0);
}

