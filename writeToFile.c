//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include <stdlib.h>
#include "messages.h"

int writeToFile(jobPtr jobInfo)
{
    FILE * fp;

    fp = fopen ("/home/ken/LHCb/grid_simulation.txt", "a");
    fprintf(fp, "%s,%f,%f,%f,%f", jobInfo->name, jobInfo->startClock, jobInfo->startSchedulClock, jobInfo->stExecClock, jobInfo->endExecClock);
    fclose(fp);
    return(0);
}

int clearFile()
{
    FILE * fp;
    fp = fopen ("/home/ken/LHCb/grid_simulation.txt", "w");
    fprintf(fp, "%s", "JobName, Time start, time schedule, Time start Exec, Time End Exec");
    fclose(fp);
    return(0);
}