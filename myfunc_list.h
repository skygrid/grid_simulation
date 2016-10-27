//
// Created by ken on 29.07.16.
//

#ifndef CSIM2SIM_MYFUNC_LIST_H
#define CSIM2SIM_MYFUNC_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include "messages.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

int input();

extern list<Job*>* global_queue;
extern map<string, FileData*>* name_node;
extern FILE* fp;
extern msg_sem_t sem_link;
extern msg_sem_t sem_requester;
extern string current_model;

extern char* path_to_output;

int data_replicator(int argc, char* argv[]);
int job_requester();
int writeToFile(FILE* fpx, Job* jobInfo);
int clearFile();
int writeAnomaly(double clock);
int set_0_all_routes();

int declare_trace_variables();
long dataset_number(string& host_name, string storage_type);

int plusLinkCounter(string& src, string& dst);
int minusLinkCounter(string& src, string& dst);
int tracer_traffic(string& src, string& dst, double size);

int addActiveCoreT();
int subActiveCoreT();

int addCorruptedCoreT();
int subCorruptedCoreT();

int addDatasetAmountT(string& host_name, string type);
int minusDatasetAmountT(string& host_name, string type);
void cumulativeInputPerSiteT(string& host_name, double size);
void cumulativeOutputPerSiteT(string& host_name, double size);
int tracer_storage(string& storage_name, string storage_type);

list<Job*>* create_current_queue();

vector<Job*>* matcher(long amountRequestedJob);
vector<Job*>* matcher_DAM(long amountRequestedJob, const string host);


int create_file_label(string& filename);
void file_usage_counter(string& filename);

void check_files_availability(Job* jobInfo);

#endif //CSIM2SIM_MYFUNC_LIST_H
