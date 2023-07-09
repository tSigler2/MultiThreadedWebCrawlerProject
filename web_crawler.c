// web_crawler.c

#include "web_crawler.h"    // gets our web_crawler struct
#include <stdlib.h>    
#include <string.h>
#include "queue.h"  // gets our queue struct and functions
#include <pthread.h> // for pthread_* functions


struct Queue* url_frontier;
pthread_mutex_t url_frontier_lock = PTHREAD_MUTEX_INITIALIZER;  // initialize the mutex


// File for saving processed URLs
FILE *url_file;


// function to handle HTTP response data
size_t handle_response(char *ptr, size_t size, size_t nmemb, void *userdata) {
	// for now, do nothing with the data
	// later, you will want to parse the data and add new URLs to the queue
	return size * nmemb;
}


// The web_crawler thread function
void *web_crawler_thread(void *arg) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Failed to initialize CURL\n");
		return NULL;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_response);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	char* url = NULL;

	for (;;) {
		// Dequeue the next URL
		pthread_mutex_lock(&url_frontier_lock);
		if (!empty(url_frontier)) {
			url = dequeue(url_frontier);
		}
		pthread_mutex_unlock(&url_frontier_lock);

		if (url == NULL) {
			break;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
		}
		// Save processed URL
		fprintf(url_file, "%s\n", url);

		free(url);
		url = NULL;
	}
	curl_easy_cleanup(curl);
	return NULL;
}


// Creates new web_crawler instance
web_crawler *web_crawler_create(char *start_url, int max_threads) {
	// initialize the url_frontier
	url_frontier = createQueue();
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
		free(dequeue(url_frontier));
	}
	free(url_frontier);
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
