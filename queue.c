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
	return (q->head == NULL);
}


//Adds to end of queue
void enqueue(struct Queue* q, const char* item){
	struct Node *p = malloc(sizeof(struct Node));
	p->address = malloc((strlen(item) + 1) * sizeof(char));
	strncpy(p->address, item, strlen(item)+1);
	p->next = NULL;

	if(q->head == NULL && q->tail == NULL){
		q->head = p;
		q->tail = p;
	}
	else if(q->head == q->tail){
		p->next = q->tail;
		q->tail = p;
		q->head->last = q->tail;
	}
	else{
		p->next = q->tail;
		q->tail->last = p;
		q->tail = p;
	}
}


//Dequeues from front of queue, adjusts queue on dequeue
char* dequeue(struct Queue* q){
	if(q->head == NULL){
		printf("EMPTY");
		return NULL;
	}

	struct Node *temp = q->head;
	char* ret = strdup(q->head->address);

	if(q->head == q->tail){
		q->head = NULL;
		q->tail = NULL;
	}
	else{
		q->head = q->head->last;
	}
	return ret;
}


//Returns front element of queue
const char* front(struct Queue* q){
	if(empty(q)) return NULL;

	return q->head->address;
}


//Returns element at end of queue
const char* rear(struct Queue* q){
	if(empty(q)) return NULL;

	return q->tail->address;
}

//Deallocate memory being used for queue
void destroyQueue(struct Queue* q){
	struct Node* curr = q->head;

	while(curr != NULL){
		struct Node* next = curr->last;
		//free(curr->address);
		free(curr);

		curr = next;
	}

	free(q);
} // queue.c