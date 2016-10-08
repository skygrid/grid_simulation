//
// Created by ken on 12.08.16.
//
#include <simgrid/msg.h>
#include <csvparser.h>
#include "myfunc_list.h"
#include "messages.h"
#include <string>
#include <boost/algorithm/string.hpp>

jobType charToEnum(char *sval);

char* typesStr[] = {"USER", "DATASTRIPPING", "MERGE", "MCStripping", "DATARECONSTRUCTION", "TURBO",  "MCRECONSTRUCTION", "WGPRODUCTION", "MCMERGE", "UNKNOWN",
                    "MCSIMULATION", "TEST", NULL};

int input(){
    fp = fopen (path_to_output, "a");
    clearFile();
    int i = 0;

    CsvParser *csvparser = CsvParser_new("input.csv", ",", 1);
    CsvRow *header;
    CsvRow *row;

    while ((row = CsvParser_getRow(csvparser))){
        const char **rowFields = CsvParser_getFields(row);
        Job* jobX = new Job();
        jobX->name = rowFields[0];
        jobX->type = charToEnum((char*) rowFields[1]);
        jobX->submissionTime = xbt_str_parse_double(rowFields[2], "kotok1");
        jobX->compSize = xbt_str_parse_double(rowFields[3], "kotok4");
        jobX->inputFileName = rowFields[4];
        jobX->inputSize = xbt_str_parse_double(rowFields[5], "kotok2");
        jobX->NRep = (int) xbt_str_parse_double(rowFields[6], "kotok3");

        jobX->dataLocHost1 = rowFields[7];
        jobX->dataLocHost2 = rowFields[8];
        jobX->dataLocHost3 = rowFields[9];
        jobX->dataLocHost4 = rowFields[10];
        jobX->dataLocHost5 = rowFields[11];
        jobX->dataLocHost6 = rowFields[12];
        jobX->dataLocHost7 = rowFields[13];
        jobX->dataLocHost8 = rowFields[14];
        jobX->dataLocHost9 = rowFields[15];
        jobX->dataLocHost10 = rowFields[16];

        jobX->storageType1 = rowFields[17];
        jobX->storageType2 = rowFields[18];
        jobX->storageType3 = rowFields[19];
        jobX->storageType4 = rowFields[20];
        jobX->storageType5 = rowFields[21];
        jobX->storageType6 = rowFields[22];
        jobX->storageType7 = rowFields[23];
        jobX->storageType8 = rowFields[24];
        jobX->storageType9 = rowFields[25];
        jobX->storageType10 = rowFields[26];

        jobX->outputName = rowFields[27];
        jobX->outputFileSize = xbt_str_parse_double(rowFields[28], "kotok4");
        jobX->outputNumber = (int) xbt_str_parse_double(rowFields[29], "kotok5");
        jobX->outputHost1 = rowFields[30];
        jobX->outputHost2 = rowFields[31];
        jobX->outputHost3 = rowFields[32];
        jobX->outputHost4 = rowFields[33];
        jobX->outputHost5 = rowFields[34];
        jobX->outputHost6 = rowFields[35];
        jobX->outputHost7 = rowFields[36];
        jobX->outputHost8 = rowFields[37];
        jobX->outputHost9 = rowFields[38];
        jobX->outputHost10 = rowFields[39];
        jobX->successExecuted = 0;
        jobX->startClock = 0;
        jobX->scheduled = 0;

        global_queue.push_back(jobX);

        CsvParser_destroy_row(row);
        i++;
    }
    CsvParser_destroy(csvparser);
    return 0;
}


jobType charToEnum(string sval)
{
    //jobType result=USER;
    //int i=0;
    for (int enumInt = USER; enumInt != LAST; enumInt++)
        if (boost::iequals(sval, typesStr[enumInt])){
            jobType type = static_cast<jobType>(enumInt);
            return type;
        }
    return UNKNOWN;
}