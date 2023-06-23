#include "web_crawler.h"
#include "queue.c"

int main(){
    struct Queue* q = createQueue(1000);    // need to clean up this queue file

    web_crawler *crawler = web_crawler_create("http://example.com", 10);
    web_crawler_run(crawler);
    web_crawler_destroy(crawler);

    return 0;
}
