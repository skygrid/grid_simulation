//
// Created by ken on 12.08.16.
//
#include <simgrid/msg.h>
#include <csvparser.h>
#include "myfunc_list.h"
#include "messages.h"

#define QUEUE_SIZE 280

double end;
jobPtr* jobQueue;
int* jobQueueHelper;


int input(){
    fp = fopen ("/home/ken/PycharmProjects/GridAnalysis/out2.txt", "a");
    clearFile();
    int i = 0;
    jobQueue = xbt_new(jobPtr, QUEUE_SIZE);
    jobQueueHelper = xbt_new(int , QUEUE_SIZE);

    CsvParser *csvparser = CsvParser_new("input.csv", ",", 1);
    CsvRow *header;
    CsvRow *row;

    while ((row = CsvParser_getRow(csvparser))){
        const char **rowFields = CsvParser_getFields(row);
        jobPtr jobX = xbt_new(job, 1);

        //Parsing
        jobX->name = strdup((char*) rowFields[0]);
        jobX->type = charToEnum((char*) rowFields[1]);
        jobX->compSize = xbt_str_parse_double(rowFields[2], "kotok1");
        jobX->inputFileName = strdup((char*) rowFields[3]);
        jobX->inputSize = xbt_str_parse_double(rowFields[4], "kotok2");
        jobX->NRep = (int) xbt_str_parse_double(rowFields[5], "kotok3");
        jobX->storageType = strdup((char*) rowFields[6]);
        jobX->dataLocHost1 = strdup((char*) rowFields[7]);
        jobX->dataLocHost2 = strdup((char*) rowFields[8]);
        jobX->dataLocHost3 = strdup((char*) rowFields[9]);
        jobX->outputName = strdup((char*) rowFields[10]);
        jobX->outputFileSize = xbt_str_parse_double(rowFields[11], "kotok4");
        jobX->outputNumber = (int) xbt_str_parse_double(rowFields[12], "kotok5");
        jobX->outputHost1 = strdup((char*) rowFields[13]);
        jobX->outputHost2 = strdup((char*) rowFields[14]);
        jobX->outputHost3 = strdup((char*) rowFields[15]);
        jobX->outputHost4 = strdup((char*) rowFields[16]);
        jobX->outputHost5 = strdup((char*) rowFields[17]);
        jobX->outputHost6 = strdup((char*) rowFields[18]);
        jobX->successExecuted = 0;
        jobX->startClock = 0;
        jobX->scheduled = 0;
        jobQueue[i] = jobX;
        //jobQueueHelper[i] = 0;
        CsvParser_destroy_row(row);
        i++;
    }
    CsvParser_destroy(csvparser);
    return 0;
}