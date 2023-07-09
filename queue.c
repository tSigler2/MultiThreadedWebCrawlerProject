//queue.c

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>


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
struct Queue* createQueue(){
	struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));
	q->head = NULL;
	q->tail = NULL;
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
		q->head = p;
		q->tail = p;
	}
	else{
		p->last = q->tail;
		q->tail->next = p;
		q->tail = p;
	}
}


//Dequeues from front of queue, adjusts queue on dequeue
const char* dequeue(struct Queue* q){
	static char ret[500];
	if(q->head == NULL){
		printf("EMPTY");
		return NULL;
	}

	strcpy(ret, q->head->address);
	struct Node *temp = q->head;

	if(q->head == q->tail){
		q->head = NULL;
		q->tail = NULL;
	}
	else{
		q->head = q->head->next;
		q->head->last = NULL;
	}

	free(temp);
	return ret;
}


//Returns front element of queue
const char* front(struct Queue* q){
	if(empty(q)) return 0;

	return q->head->address;
}


//Returns element at end of queue
const char* rear(struct Queue* q){
	if(empty(q)) return 0;

	return q->tail->address;
} // queue.c
