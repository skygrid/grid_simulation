//
// Created by ken on 23.05.16.
//

#ifndef CSIM2SIM_MESSAGES_H
#define CSIM2SIM_MESSAGES_H

#include <simgrid/msg.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>

#define MESSAGES_SIZE 100000
using namespace std;


typedef enum {
	/**
		@type enum
		Defines all job types
	*/
    USER,
    DATASTRIPPING,
    MERGE,
    MCStripping,
    DATARECONSTRUCTION,
    TURBO,
    MCRECONSTRUCTION,
    WGPRODUCTION,
    MCMERGE,
    UNKNOWN,
    MCSIMULATION,
    TEST,
    LAST
} jobType;


struct Job{
	/**
		@type struct
		Job object (plain old data).
		Information about jobs stored in `jobs.yml` 
		will be cast to Job object.
	*/
    long JobId;
    double EndExecTime;
    std::string Federation;
    std::vector<std::string> InputFiles;
    std::string JobGroup;
    std::string JobType;
    jobType type;
    std::string LocalJobID;
    std::vector<std::string> OutputFiles;
    std::string RescheduleTime;
    std::string Site;
    double StartExecTime;
    std::string Status;
    double SubmissionTime;
    float SystemPriority;
    double TotalCPUTime; // in seconds
    float UserPriority;
    double WallClockTime;
    int successExecuted;
    double startSchedulClock;
    std::string tier;
};

struct InputFile{
	/**
		@type struct
		Information about input files stored in `bkk.yml`
		will be cast to this object.
	*/
    std::string name;
    std::string events;
    std::vector<std::string> BKKPath;
    std::string DataQuality;
    std::string Replica;
    std::string RunNumber;
    double Size;
    std::vector<std::string> Storages;
};


struct InputInfo{
	/**
		@type struct
		Contains information needed when tier starts to copy file.
		@localInputFilePath -- experiment.dst
		@fullCopyFilePath -- /RRCKI-DISK/experiment.dst
		@fullCopyFromTapeToDiskName -- /CERN-PROD-DISK/experiment.dst
		@storage -- TAPE or DISK
		@hostName -- RRCKI-DISK
	*/
    std::string localInputFilePath;
    std::string fullCopyFilePath;
    std::string fullCopyFromTapeToDiskName;
    std::string storage;
    std::string hostName;
    bool storageType;
};

struct InputAndJobInfo{
	/**
		@type struct
		Combines job and inputinfo pointers.
	*/
    InputInfo* inputInfo;
    Job* job;
    msg_bar_t barrier;
};

class ReplicatorData{
	/**
		@type struct
		Info needed for starting output files replication.
	*/
public:
    int replicaAmount;
    string fileName;
    string currentLoc;
    string storageType;
    string outLoc1;
    string outLoc2;
    string outLoc3;
    double size;
};

class UploadData{
	/**
		@type struct 
		Info needed for data replication of single file.
	*/
public:
    string filename;
    int numberOfReplica;
    string dest;
    string storageType;
    double size;
};


class JobBatchRequest{
	/**
		@type struct 
		Amount of jobs host wants to get.
	*/
public:
    long coreAmount;
    JobBatchRequest(){}
};

class FileData{
	/**
		@type 
		Contains file attributes:
		 time of creation
		 all times when file was accessed
	*/
public:
    int number_used;
    std::string used;
    std::vector<double>* clocks;

    ~FileData(){
        delete clocks;
    }
};



#endif //CSIM2SIM_MESSAGES_H
