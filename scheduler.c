#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_P 32
#define MAX_LEN 32

//Process control block structure
struct pcb{
    int pid, on, *function;
    char name[32];
};

//Adding functions to array of process control blocks
struct pcb processAppend(int pid, char* name, void *function){
    struct pcb cur;
    cur.pid = pid;
    strcpy(cur.name, name);
    cur.function = function;
    cur.on = 1;

    return cur;
}


//Removes ability of processes to run
int processRemove(struct pcb* pList, int pid){
    pList[pid].on = 0;
    return 0;
}


//Basic scheduler that runs function at specified index in list of processes to be scheduled
void schedule(struct pcb* pList, int index){
    void (*p)(void);

    if(pList[index].on){
        p = (void *)pList[index].function;
        (*p)();
    }

    processRemove(pList, index);
}