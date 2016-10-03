//
// Created by ken on 12.08.16.
//
#include <simgrid/msg.h>
#include <csvparser.h>
#include "myfunc_list.h"
#include "messages.h"

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
        jobPtr jobX = xbt_new(job, 1);

        jobX->name = strdup((char*) rowFields[0]);
        jobX->type = charToEnum((char*) rowFields[1]);
        jobX->submissionTime = xbt_str_parse_double(rowFields[2], "kotok1");
        jobX->compSize = xbt_str_parse_double(rowFields[3], "kotok4");
        jobX->inputFileName = strdup((char*) rowFields[4]);
        jobX->inputSize = xbt_str_parse_double(rowFields[5], "kotok2");
        jobX->NRep = (int) xbt_str_parse_double(rowFields[6], "kotok3");

        jobX->dataLocHost1 = strdup((char*) rowFields[7]);
        jobX->dataLocHost2 = strdup((char*) rowFields[8]);
        jobX->dataLocHost3 = strdup((char*) rowFields[9]);
        jobX->dataLocHost4 = strdup((char*) rowFields[10]);
        jobX->dataLocHost5 = strdup((char*) rowFields[11]);
        jobX->dataLocHost6 = strdup((char*) rowFields[12]);
        jobX->dataLocHost7 = strdup((char*) rowFields[13]);
        jobX->dataLocHost8 = strdup((char*) rowFields[14]);
        jobX->dataLocHost9 = strdup((char*) rowFields[15]);
        jobX->dataLocHost10 = strdup((char*) rowFields[16]);

        jobX->storageType1 = strdup((char*) rowFields[17]);
        jobX->storageType2 = strdup((char*) rowFields[18]);
        jobX->storageType3 = strdup((char*) rowFields[19]);
        jobX->storageType4 = strdup((char*) rowFields[20]);
        jobX->storageType5 = strdup((char*) rowFields[21]);
        jobX->storageType6 = strdup((char*) rowFields[22]);
        jobX->storageType7 = strdup((char*) rowFields[23]);
        jobX->storageType8 = strdup((char*) rowFields[24]);
        jobX->storageType9 = strdup((char*) rowFields[25]);
        jobX->storageType10 = strdup((char*) rowFields[26]);

        jobX->outputName = strdup((char*) rowFields[27]);
        jobX->outputFileSize = xbt_str_parse_double(rowFields[28], "kotok4");
        jobX->outputNumber = (int) xbt_str_parse_double(rowFields[29], "kotok5");
        jobX->outputHost1 = strdup((char*) rowFields[30]);
        jobX->outputHost2 = strdup((char*) rowFields[31]);
        jobX->outputHost3 = strdup((char*) rowFields[32]);
        jobX->outputHost4 = strdup((char*) rowFields[33]);
        jobX->outputHost5 = strdup((char*) rowFields[34]);
        jobX->outputHost6 = strdup((char*) rowFields[35]);
        jobX->outputHost7 = strdup((char*) rowFields[36]);
        jobX->outputHost8 = strdup((char*) rowFields[37]);
        jobX->outputHost9 = strdup((char*) rowFields[38]);
        jobX->outputHost10 = strdup((char*) rowFields[39]);
        jobX->successExecuted = 0;
        jobX->startClock = 0;
        jobX->scheduled = 0;

        insertLast(jobX);

        CsvParser_destroy_row(row);
        i++;
    }
    CsvParser_destroy(csvparser);
    return 0;
}



jobType charToEnum(char *sval)
{
    //jobType result=USER;
    //int i=0;
    for (int enumInt = USER; enumInt != LAST; enumInt++)
        if (0==strcasecmp(sval, typesStr[enumInt])){
            jobType type = static_cast<jobType>(enumInt);
            return type;
        }
    return UNKNOWN;
}