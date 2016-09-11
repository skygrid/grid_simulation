//
// Created by ken on 10.09.16.
//

#include <stdbool.h>
#include "messages.h"
#include "myfunc_list.h"



struct node *head = NULL;
struct node *end = NULL;

struct node *local_head = NULL;
struct node *local_end = NULL;

//display the list
void printList()
{
    struct node *ptr = head;
    printf("\n[ ");

    //start from the beginning
    while(ptr != NULL)
    {
        printf("(%s)\n", ptr->jobX->name);
        ptr = ptr->next;
    }

    printf(" ]");
}

//insert link at the first location
void insertFirst(jobPtr jobX) {
    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->jobX = jobX;

    //point it to old first node
    link->next = head;

    if (head == NULL){
        end = link;
    }

    //point first to new first node
    head = link;
}

void insertLast(jobPtr jobX){
    if (head == NULL){
        insertFirst(jobX);
        return;
    }

    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->jobX = jobX;

    link->next = NULL;
    end->next = link;
    end = link;
}

bool isEmpty() {
    return head == NULL;
}

int length() {
    int length = 0;
    struct node *current;

    for(current = head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}

void localInsertFirst(jobPtr jobX) {
    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->jobX = jobX;

    //point it to old first node
    link->next = local_head;

    if (local_head == NULL){
        local_end = link;
    }

    //point first to new first node
    local_head = link;
}

void localInsertLast(jobPtr jobX){
    if (local_head == NULL){
        localInsertFirst(jobX);
        return;
    }

    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->jobX = jobX;

    link->next = NULL;
    local_end->next = link;
    local_end = link;
}

int localLength() {
    int length = 0;
    struct node *current;

    for(current = local_head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}

void delete_job_from_queue(struct node* loc_current, struct node* loc_previous, struct node* current, struct node* previous){
    if(current == head) {
        head = head->next;
    }else {
        previous->next = current->next;
    }
    if (loc_current == local_head){
        local_head = local_head->next;
    } else{
        loc_previous->next = loc_current->next;
    }
    free(current);
    free(loc_current);
}

struct node* create_current_queue(){
    double waiting_time = 50.;
    long jobs = 0;

    local_head = NULL;
    local_end = NULL;

    // wait until job will be created in the queue
    while (MSG_get_clock() < head->jobX->submissionTime){
        if (MSG_get_clock() > head->jobX->submissionTime){
            break;
        }
        MSG_process_sleep(waiting_time);
    }

    struct node* current_job = head;

    while ((current_job) && current_job->jobX->submissionTime < MSG_get_clock()){
        localInsertLast(current_job->jobX);
        current_job = current_job->next;
        jobs++;
    }

    return local_head;
}

