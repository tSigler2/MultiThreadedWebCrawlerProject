#include "web_crawler.h"  // gets our web_crawler struct and functions
#include "queue.h"  // gets our queue struct and functions


int main() {
    web_crawler *crawler = web_crawler_create("http://example.com", 10);
    if (!crawler) {
        fprintf(stderr, "Failed to create web crawler\n");
        return 1;
    }

    web_crawler_run(crawler);

    web_crawler_destroy(crawler);

    return 0;
}
