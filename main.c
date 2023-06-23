#include "web_crawler.h"
#include "queue.c"
#include "scheduler.c"

// This probably needs to move {
pthread_t t[32]; //Thread array, allows us to create up to 32 threads
static struct pcb pList[MAX_P]; //Scheduler Array of process control blocks for functions


//Test Functions for scheduler
void p0(){
    printf("0\n");
}

void p1(){
    printf("1\n");
}
// }

int main(){

    web_crawler *crawler = web_crawler_create("http://example.com", 10);
    web_crawler_run(crawler);
    web_crawler_destroy(crawler);
    struct Queue* q = createQueue();

    pList[0] = processAppend(0, "Print 0", p0);
    schedule(pList, 0);

    return 0;
}
