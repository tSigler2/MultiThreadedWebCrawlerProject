#include <string.h>
#include <limits.h>
#include <stdlib.h>

struct Queue{
    int front, rear, size;
    unsigned cap;
    int *array;
};

struct Queue* createQueue(unsigned cap){
    struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));

    q->cap = cap;
    q->front = q->size = 0;
    q->rear = cap-1;
    q->array = (int*)malloc(q->cap * sizeof(int));
    return q;
}

int full(struct Queue* q){
    return (q->size == q->cap);
}

int empty(struct Queue* q){
    return (q->size == 0);
}

void enqueue(struct Queue* q, int item){
    if(full(q)) return;

    q->rear = (q->rear + 1) % q->cap;
    q->array[q->rear] = item;
    q->size = q->size + 1;
}

int dequeue(struct Queue* q){
    if(full(q)) return INT_MIN;

    int item = q->array[q->front];
    q->size = q->size - 1;

    return item;
}

int front(struct Queue* q){
    if(empty(q)) return INT_MIN;

    return q->array[q->front];
}

int rear(struct Queue* q){
    if(empty(q)) return INT_MIN;

    return q->array[q->rear];
}