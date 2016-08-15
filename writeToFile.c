//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include "messages.h"

int writeToFile(FILE* fpy, jobPtr jobInfo)
{
    fprintf(fpy, "%s,%d,%f,%f,%f,%f\n", jobInfo->name, jobInfo->successExecuted, jobInfo->startClock, jobInfo->startSchedulClock, jobInfo->stExecClock, jobInfo->endExecClock);
    return(0);
}

int clearFile()
{
    FILE * fp, *fp1;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/out2.txt", "w");
    fprintf(fp, "%s", "JobName, Success, Time start, time schedule, Time start Exec, Time End Exec\n");
    fclose(fp);
    fp1 = fopen("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "w");
    fclose(fp1);

    return(0);
}

int writeAnomaly(double clock)
{
    FILE * fp;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "a");
    fprintf(fp, "%f\n", clock);
    fclose(fp);
    return(0);
}

