//queue.h

#ifndef QUEUE_H // include guard
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Node Structure to make up Queue, carries strings up to 500 characters
struct Node{
    char address[500];
    struct Node *next;
    struct Node *last;
};

//Queue structure
struct Queue{
    struct Node *head;
    struct Node *tail;
};

//Method to create queue
struct Queue* createQueue();

//Returns whether queue is empty
int empty(struct Queue* q);

//Adds to end of queue
void enqueue(struct Queue* q, char item[500]);

//Dequeues from front of queue, adjusts queue on dequeue
const char* dequeue(struct Queue* q);

//Returns front element of queue
const char* front(struct Queue* q);

//Returns element at end of queue
const char* rear(struct Queue* q);

#endif
