// web_crawler.c

#include "web_crawler.h"    // gets our web_crawler struct
#include <stdlib.h>    
#include <string.h>

// Creates new web_crawler instance
web_crawler *web_crawler_create(char *start_url, int max_threads) {
    // dynamically allocates memory for the web_crawler struct
    web_crawler *crawler = malloc(sizeof(web_crawler));
    // initializes the start_url and max_threads fields with the function arguments
    crawler->start_url = strdup(start_url);   // create a copy of the start_url string.
    crawler->curl = curl_easy_init();
    crawler->max_threads = max_threads;
    // dynamically allocates memory for the pthread_t threads
    crawler->threads = malloc(max_threads * sizeof(pthread_t));
    return crawler;
}

// cleans up a web_crawler instance
void web_crawler_destroy(web_crawler *crawler) {
    free(crawler->start_url);
    curl_easy_cleanup(crawler->curl);
    free(crawler->threads);
    free(crawler);
}

// entry point for the worker threads
void *web_crawler_thread(void *arg) {
    web_crawler *crawler = (web_crawler *)arg;
    // TODO: implement the web crawling logic here that takes a 
    // pointer to a web_crawler instance and is supposed to implement 
    // the web crawling logic
    return NULL;
}

// starts the web crawler
void web_crawler_run(web_crawler *crawler) {
    // creates max_threads threads using pthread_create
    for (int i = 0; i < crawler->max_threads; ++i) {
        pthread_create(&crawler->threads[i], NULL, web_crawler_thread, crawler);
    }
    // waits for all of them to finish using pthread_join
    for (int i = 0; i < crawler->max_threads; ++i) {
        pthread_join(crawler->threads[i], NULL);
    }

// Each thread will start execution in the web_crawler_thread function with crawler as argument
}
