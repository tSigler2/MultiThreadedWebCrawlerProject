#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

struct Node{
    char address[500];
    struct Node *next;
};

//Queue structure
struct Queue{
    struct Node *head;
    struct Node *tail;
};

//Method to create queue
struct Queue* createQueue(){
    struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));

    return q;
}

//Returns whether queue is empty
int empty(struct Queue* q){
    return (q->tail == NULL);
}

//Adds to end of queue
void enqueue(struct Queue* q, char item[500]){
    struct Node *p = malloc(sizeof(struct Node));
    strcpy(p->address, item);
    p->next = NULL;

    if(q->head == NULL && q->tail == NULL){
        q->head->next = q->tail;
    }
    else{
        q->head->next = p;
        q->head = p;
    }
}

//Dequeues from front of queue, adjusts queue on dequeue
const char* dequeue(struct Queue* q){
    static char ret[500];
    if(q->tail == NULL){
        printf("EMPTY");
        return 0;
    }

    strcpy(ret, q->tail->address);

    if(q->head == q->tail){
        q->head = NULL;
    }
    q->tail = q->tail->next;

    return ret;
}

//Returns front element of queue
const char* front(struct Queue* q){
    if(empty(q)) return 0;

    return q->tail->address;
}

//Returns element at end of queue
const char* rear(struct Queue* q){
    if(empty(q)) return 0;

    return q->head->address;
}