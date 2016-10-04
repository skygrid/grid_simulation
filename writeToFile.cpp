//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include "messages.h"
#include "myfunc_list.h"
#define str(x) #x


int writeToFile(FILE* fpy, jobPtr jobInfo)
{
    const char* typesStr[] = {"USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN",
                        "MCSIMULATION", "TEST", NULL};
    fprintf(fpy, "%s,%d,%f,%f,%f,%f,%s,%s\n", jobInfo->name, jobInfo->successExecuted, jobInfo->submissionTime, jobInfo->startSchedulClock, jobInfo->stExecClock, jobInfo->endExecClock, jobInfo->tier, typesStr[jobInfo->type]);
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

