// web_crawler.c

#include "web_crawler.h"    // gets our web_crawler struct
#include <stdlib.h>   
#include <string.h>
#include "queue.h"  // gets our queue struct and functions
#include <pthread.h> // for pthread_* functions
#include <unistd.h>
#include <stdlib.h>


struct Queue* url_frontier;
pthread_mutex_t url_frontier_lock = PTHREAD_MUTEX_INITIALIZER;  // initialize the mutex


// File for saving processed URLs
FILE *url_file;

struct MemoryStruct{
	char* m;
	size_t s;
};


// function to handle HTTP response data
size_t handle_response(char *ptr, size_t size, size_t nmemb, void *userdata) {
	// for now, do nothing with the data
	// later, you will want to parse the data and add new URLs to the queue
	return size * nmemb;
}

void getLinks(xmlDocPtr doc, struct Queue *url_frontier, pthread_mutex_t *frontier_lock) {
    xmlInitParser();
    xmlXPathContextPtr x_context;
    xmlXPathObjectPtr x_obj;
    xmlNodeSetPtr n;
    int i;

    x_context = xmlXPathNewContext(doc);

    if (x_context == NULL) {
        fprintf(stderr, "Unable to create xPath\n");
        return;
    }

    const xmlChar *x_expr = (xmlChar *) "//a/@href";
    x_obj = xmlXPathEvalExpression(x_expr, x_context);

    if (x_obj == NULL) {
        fprintf(stderr, "Failed to evaluate xpath expression\n");
        xmlXPathFreeContext(x_context);
        return;
    }

    n = x_obj->nodesetval;

    for (i = 0; i < n->nodeNr; ++i) {
        xmlNodePtr a = n->nodeTab[i];
		//printf("%s", &a);
        xmlChar *href = xmlNodeListGetString(doc, a->children, 1);
        if (href != NULL) {
            char *href_copy = (char *)xmlStrdup(href);
            printf("Found link: %s\n", href_copy);
            pthread_mutex_lock(frontier_lock);
            enqueue(url_frontier, href_copy);
            pthread_mutex_unlock(frontier_lock);
            xmlFree(href);
        }
    }

    xmlXPathFreeObject(x_obj);
    xmlXPathFreeContext(x_context);
}

// The web_crawler thread function
void *web_crawler_thread(void *arg) {
	int i = 0;
	CURL* curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Failed to initialize CURL\n");
		return NULL;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_response);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	const char *url = NULL;

	while (!empty(url_frontier) && i < 100) {
		// Dequeue the next URL
		pthread_mutex_lock(&url_frontier_lock);
		if (!empty(url_frontier)) {
			url = dequeue(url_frontier);
		}
		pthread_mutex_unlock(&url_frontier_lock);

		printf("Currently Crawling: %s\n", url);

		if (url == NULL) {
			break;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
		}
		printf("%d", i);
		//Proper format for HTML parsing
		xmlDocPtr doc = htmlReadMemory(url, strlen(url), "", NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

		if(doc == NULL){
			fprintf(stderr, "Unable to parse: %s\n", url);
			free((char*)url);
			continue;
		}

		getLinks(doc, url_frontier, &url_frontier_lock); //Entry for parser
		xmlFreeDoc(doc);
		printf("Segmentation Fault?\n");

		// Save processed URL
		fprintf(url_file, "%s\n", url);

		//free((char*)url);
		i++;
		free((char *)url);
		usleep(1000000); //Sleeping script to not potentially overwhelm webpage with requests
	}
	curl_easy_cleanup(curl);
	return NULL;
}


// Creates new web_crawler instance
web_crawler *web_crawler_create(char *start_url, int max_threads) {
	// initialize the url_frontier
	url_frontier = createQueue(100);
	enqueue(url_frontier, start_url);  // add the start_url to the url_frontier

	// File for reading/writing processed URLs
	url_file = fopen("processed_urls.txt", "a+");
	if (!url_file) {
		fprintf(stderr, "Failed to open file\n");
		return NULL;
	}
	// Read processed URLs from file and enqueue them
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, url_file)) != -1) {
		enqueue(url_frontier, strdup(line));
	}
	free(line);
	// dynamically allocates memory for the web_crawler struct
	web_crawler *crawler = malloc(sizeof(web_crawler));
	// initializes the start_url and max_threads fields with the function arguments
	crawler->start_url = strdup(start_url);   // create a copy of the start_url string.
	crawler->curl = curl_easy_init();
	curl_easy_setopt(crawler->curl, CURLOPT_WRITEFUNCTION, handle_response);
	curl_easy_setopt(crawler->curl, CURLOPT_FOLLOWLOCATION, 1L); // follow redirects
	crawler->max_threads = max_threads;
	// dynamically allocates memory for the pthread_t threads
	crawler->threads = malloc(max_threads * sizeof(pthread_t));
	return crawler;
}


// cleans up a web_crawler instance
void web_crawler_destroy(web_crawler *crawler) {
	fclose(url_file);  // Close the file here
	free(crawler->start_url);
	curl_easy_cleanup(crawler->curl);
	free(crawler->threads);
	// clean up the url_frontier
	while(!empty(url_frontier)) {
		dequeue(url_frontier);
	}
	destroyQueue(url_frontier);
	free(crawler);
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
} //web_crawler.c
