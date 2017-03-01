//
// Created by ken on 03.08.16.
//

#include <stdio.h>
#include "my_structures.h"
#include "myfunc_list.h"
#define str(x) #x
FILE * FP;

int writeToFile(Job* jobInfo){
	/**
		@type function
		Write to output file information about job's metricas
		(time of pushing into queue, scheduling, starting of execution, anomalies, etc.)
		@jobInfo job object which fields contains neccesary information.
	*/
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
	/**
		@type function
		Simply clear file where metricas about jobs will be written.
	*/
    FP = fopen (path_to_output.c_str(), "w");
    fprintf(FP, "%s", "JobName, Success, Time start, time schedule, Time start Exec, Time End Exec, Tier, Type\n");
    //fclose(FP);
    //fp1 = fopen("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "w");
    //fclose(fp1);

    return 0;
}

int writeAnomaly(double clock)
{
	/**
		@type function
		@clock time when anomaly occurred
		Writes to anomaly.txt file info about anomalies have occurred in simulation. 
	*/
    /*FILE * fp;
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/anomaly.txt", "a");
    fprintf(fp, "%f\n", clock);
    fclose(fp);*/
    return(0);
}

