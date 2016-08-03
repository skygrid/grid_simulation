//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include <stdlib.h>
#include "messages.h"

int writeToFile(jobPtr jobInfo)
{
    FILE * fp;

    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/out_DAMx.txt", "a");
    fprintf(fp, "%s,%f,%f,%f,%f\n", jobInfo->name, jobInfo->startClock, jobInfo->startSchedulClock, jobInfo->stExecClock, jobInfo->endExecClock);
    fclose(fp);
    return(0);
}

int clearFile()
{
    FILE * fp, *fp1;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/out_DAMx.txt", "w");
    fprintf(fp, "%s", "JobName, Time start, time schedule, Time start Exec, Time End Exec");
    fclose(fp);

    fp1 = fopen("/home/ken/PycharmProjects/GridAnalysis/anomaly_DAM.txt", "w");
    fclose(fp1);

    return(0);
}

int writeAnomaly(double clock)
{
    FILE * fp;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/anomaly_DAM.txt", "a");
    fprintf(fp, "%f\n", clock);
    fclose(fp);
    return(0);
}

