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
    long JobId;
    double EndExecTime;
    std::string Federation;
    std::vector<std::string> InputFiles;
    std::string JobGroup;
    std::string JobType;
    jobType type;
    long LocalJobID;
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
    std::string localInputFilePath;
    std::string fullCopyFilePath;
    std::string fullCopyFromTapeToDiskName;
    std::string storage;
    std::string hostName;
    bool storageType;
};

struct InputAndJobInfo{
    InputInfo* inputInfo;
    Job* job;
};

class ReplicatorData{
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
public:
    string filename;
    int numberOfReplica;
    string dest;
    string storageType;
    double size;
};



class JobBatchRequest{
public:
    long coreAmount;
    JobBatchRequest(){}
};

class FileData{
public:
    int number_used;
    string used;
    vector<double>* clocks;

    ~FileData(){
        delete clocks;
    }
};



#endif //CSIM2SIM_MESSAGES_H