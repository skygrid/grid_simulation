//
// Created by ken on 23.05.16.
//

#ifndef CSIM2SIM_MESSAGES_H
#define CSIM2SIM_MESSAGES_H

#include <simgrid/msg.h>
#include <string>
#include <list>
#include <vector>

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


class Job{
public:
    string name;
    double submissionTime;
    jobType type;
    double compSize;
    string inputFileName;
    double inputSize;
    int NRep;
    string dataLocHost1;
    string dataLocHost2;
    string dataLocHost3;
    string dataLocHost4;
    string dataLocHost5;
    string dataLocHost6;
    string dataLocHost7;
    string dataLocHost8;
    string dataLocHost9;
    string dataLocHost10;

    string storageType1;
    string storageType2;
    string storageType3;
    string storageType4;
    string storageType5;
    string storageType6;
    string storageType7;
    string storageType8;
    string storageType9;
    string storageType10;

    int outputNumber;
    string outputHost1;
    string outputHost2;
    string outputHost3;
    string outputHost4;
    string outputHost5;
    string outputHost6;
    string outputHost7;
    string outputHost8;
    string outputHost9;
    string outputHost10;


    char* downloadSite;
    string outputName;
    double outputFileSize;


    int scheduled;
    double startClock;
    double startSchedulClock;
    double stExecClock;
    double endExecClock;
    int successExecuted;
    string tier;


};

class DataInfo{
public:
    string input_file_path;
    string copy_file_path;
    string copy_from_tape_to_disk_name;
    string destination_name;
    string storage_type;
    DataInfo(){};

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
    vector<double> clocks;
};



#endif //CSIM2SIM_MESSAGES_H