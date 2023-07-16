// web_crawler.h

// include guard
#ifndef WEB_CRAWLER_H
#define WEB_CRAWLER_H

#include <curl/curl.h>		// used to make HTTP requests
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h> //Used for link extractions
#include "queue.h"

typedef struct web_crawler {
    char *start_url;	// a pointer to the start URL for the web crawler
    CURL *curl;	  // a pointer to a CURL object, used to make HTTP requests
    pthread_t *threads;	  // a pointer to an array of thread objects
    int max_threads;    // the maximum number of threads that the web crawler should use
} web_crawler;

// creates a new web_crawler and returns a pointer to it. It takes as arguments the start URL and the maximum number of threads
web_crawler *web_crawler_create(char *start_url, int max_threads);
// cleans up resources used by a web_crawler and then frees it. It takes a pointer to the web_crawler to be destroyed
void web_crawler_destroy(web_crawler *crawler);
// starts the web crawler. It takes a pointer to the web_crawler to be run.
void web_crawler_run(web_crawler *crawler);
//Get links from page and enqueue them
void getLinks(xmlDocPtr doc, struct Queue *url_frontier, pthread_mutex_t *frontier_lock);

#endif // web_crawler.h

