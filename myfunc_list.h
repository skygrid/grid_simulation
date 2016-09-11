//
// Created by ken on 29.07.16.
//

#ifndef CSIM2SIM_MYFUNC_LIST_H
#define CSIM2SIM_MYFUNC_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include "messages.h"

int input();

extern FILE* fp;
extern msg_sem_t sem_link;

int data_replicator(int argc, char* argv[]);
int job_requester();
jobType charToEnum(char *sval);
int writeToFile(FILE* fpx, jobPtr jobInfo);
int clearFile();
int writeAnomaly(double clock);
int set_0_all_routes();

int plusLinkCounter(const char* src, const char* dst);
int minusLinkCounter(const char* src, const char* dst);
int tracer_traffic(const char* src, const char* dst, double size);

int addActiveCoreT();
int subActiveCoreT();

int addCorruptedCoreT();
int subCorruptedCoreT();

int tracer_storage(char* storage_name, char* storage_type);

//LINKED LIST FUNCTIONS
extern struct node *head;
extern struct node *end;
extern struct node *local_head;
extern struct node *local_end;

void insertFirst(jobPtr jobX);
void insertLast(jobPtr jobX);
int length();
bool isEmpty();

void localInsertFirst(jobPtr jobX);
void localInsertLast(jobPtr jobX);
int localLength();

struct node* create_current_queue();
void delete_job_from_queue(struct node* loc_current, struct node* loc_previous, struct node* current, struct node* previous);

jobBatch_AmountPtr matcher(long amountRequestedJob);
jobBatch_AmountPtr matcher_DAM(long amountRequestedJob, const char* host);

void free_local_queue();
void printList();

#endif //CSIM2SIM_MYFUNC_LIST_H
