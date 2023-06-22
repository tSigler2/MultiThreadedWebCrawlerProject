#include <string.h>
#include <limits.h>
#include <stdlib.h>

//Queue structure
struct Queue{
    int front, rear, size;
    unsigned cap;
    int *array;
};

//Method to create queue
struct Queue* createQueue(unsigned cap){
    struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));

    q->cap = cap;
    q->front = q->size = 0;
    q->rear = cap-1;
    q->array = (int*)malloc(q->cap * sizeof(char) * 1000);
    return q;
}

//Returns whether the queue is full
int full(struct Queue* q){
    return (q->size == q->cap);
}

//Returns whether queue is empty
int empty(struct Queue* q){
    return (q->size == 0);
}

//Adds to end of queue
void enqueue(struct Queue* q, char* item){
    if(full(q)) return;

    q->rear = (q->rear + 1) % q->cap;
    strcpy(q->array[q->rear], item);
    q->size = q->size + 1;
}

//Dequeues from front of queue, adjusts queue on dequeue
char* dequeue(struct Queue* q){
    char item[200];

    if(full(q)) return '\0';
    strcpy(item, q->array[q->front]);
    q->size = q->size - 1;

    return item;
}

//Returns front element of queue
int front(struct Queue* q){
    if(empty(q)) return INT_MIN;

    return q->array[q->front];
}

//Returns element at end of queue
int rear(struct Queue* q){
    if(empty(q)) return INT_MIN;

    return q->array[q->rear];
}