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
        printf("(%d, %s)\n",ptr->key, ptr->jobX->name);
        ptr = ptr->next;
    }

    printf(" ]");
}

//insert link at the first location
void insertFirst(jobPtr jobX, int key) {
    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->jobX = jobX;
    link->key = key;

    //point it to old first node
    link->next = head;

    if (head == NULL){
        end = link;
    }

    //point first to new first node
    head = link;
}

void insertLast(jobPtr jobX, int key){
    if (head == NULL){
        insertFirst(jobX, key);
        return;
    }

    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->key = key;
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


//delete a link with given key
int delete(int key){

    //start from the first link
    struct node* current = head;
    struct node* previous = NULL;

    //if list is empty
    if(head == NULL){
        return NULL;
    }

    //navigate through list
    while(current->key != key){
        if(current->next == NULL){
            return NULL;
        }else {
            previous = current;
            current = current->next;
        }
    }

    if(current == head) {
        head = head->next;
    }else {
        previous->next = current->next;
    }
    return 0;
}

