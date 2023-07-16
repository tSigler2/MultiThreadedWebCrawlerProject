//queue.c

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>


//Node Structure to make up Queue, carries strings up to 500 characters
struct Node{
	char* address;
	struct Node *next;
	struct Node *last;
};


//Queue structure
struct Queue{
	char** urls;
	int front;
	int rear;
	int cap;
	int size;
};


//Method to create queue
struct Queue* createQueue(int c){
	struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));
	q->urls = (char**)malloc(c*sizeof(char*));
	q->front = 0;
	q->rear = -1;
	q->cap = c;
	q->size = 0;
	return q;
}

//Returns whether queue is empty
int empty(struct Queue* q){
	return (q->size == 0);
}


//Adds to end of queue
void enqueue(struct Queue* q, const char* item){
	if(q->size >= q->cap){
		int new_cap = q->cap*2;
		q->urls = (char**)realloc(q->urls, new_cap*sizeof(char*));
		q->cap = new_cap;
	}

	q->rear = (q->rear+1) % q->cap;
	q->urls[q->rear] = strdup(item);
	q->size++;
}


//Dequeues from front of queue, adjusts queue on dequeue
const char* dequeue(struct Queue* q){
	if(q->size == 0){
		printf("EMPTY");
		return NULL;
	}

	const char* url = q->urls[q->front];
	q->front = (q->front+1) % q->cap;
	q->size--;
	return url;
}

//Deallocate memory being used for queue
void destroyQueue(struct Queue* q){
	for(int i = 0; i < q->size; i++){
		free(q->urls[i]);
	}

	free(q->urls);
	free(q);
} // queue.c