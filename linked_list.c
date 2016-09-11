//
// Created by ken on 10.09.16.
//

#include <stdbool.h>
#include "messages.h"
#include "myfunc_list.h"



struct node *head = NULL;
struct node *end = NULL;

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





//is list empty
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
    link->next = head;

    if (head == NULL){
        end = link;
    }

    //point first to new first node
    head = link;
}

void localInsertLast(jobPtr jobX){
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




//delete a link with given key

