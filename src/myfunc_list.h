//
// Created by ken on 29.07.16.
//

#ifndef CSIM2SIM_MYFUNC_LIST_H
#define CSIM2SIM_MYFUNC_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include "my_structures.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

int input();

extern std::map<std::string, long> storage_number_map; // name of storage and amount of files st_num_map;
extern std::map<std::string, double > cumulative_input_site;
extern std::map<std::string, double > cumulative_output_site;

void dataset_number_change(const string& storage_name, int change);

extern std::map<std::string, InputFile*>* FILES_DATABASE;
extern std::list<Job*>* GLOBAL_QUEUE;

extern std::map<string, FileData*>* name_node;
extern FILE* FP;
extern msg_sem_t sem_link;
extern msg_sem_t sem_requester;
extern std::string current_model;

extern char* path_to_output;

int data_replicator(int argc, char* argv[]);
int job_requester();
int writeToFile(Job* jobInfo);
int clearFile();
int writeAnomaly(double clock);
int set_0_all_routes();

int declare_trace_variables();
long dataset_number(std::string& host_name, std::string storage_type);

int plusLinkCounter(std::string& src, std::string& dst);
int minusLinkCounter(std::string& src, std::string& dst);
int tracer_traffic(std::string& src, std::string& dst, double size);

int addActiveCoreT();
int subActiveCoreT();

int addCorruptedCoreT();
int subCorruptedCoreT();

int addDatasetAmountT(std::string& host_name, std::string type);
int minusDatasetAmountT(std::string& host_name, std::string type);
void cumulative_input_per_site(const std::string& host_name, double size);
void cumulative_output_per_site(const std::string& host_name, double size);
int tracer_storage(std::string& storage_name, std::string storage_type);

std::list<Job*>* create_current_queue();

std::vector<Job*>* matcher(long amountRequestedJob);
std::vector<Job*>* matcher_DAM(long amountRequestedJob, const std::string& host);
std::vector<Job*>* matcher_tier2(long amountRequestedJob, const std::string& host);
std::vector<Job*>* matcher_calibration(long amountRequestedJob, const std::string& hostname);

int create_file_label(std::string& filename);
void file_usage_counter(std::string& filename);

void check_files_availability(Job* jobInfo);

#endif //CSIM2SIM_MYFUNC_LIST_H
