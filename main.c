#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.c"
#include "scheduler.c"

pthread_t t[32]; //Thread array, allows us to create up to 32 threads
static struct pcb pList[MAX_P]; //Scheduler Array of process control blocks for functions


//Test Functions for scheduler
void p0(){
    printf("0\n");
}

void p1(){
    printf("1\n");
}

int main(){
    struct Queue* q = createQueue(1000);

    pList[0] = processAppend(0, "Print 0", p0);
    schedule(pList, 0);

    return 0;
}