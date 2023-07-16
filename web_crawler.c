// web_crawler.c

#include "web_crawler.h"    // gets our web_crawler struct
#include <stdlib.h>   
#include <string.h>
#include "queue.h"  // gets our queue struct and functions
#include <pthread.h> // for pthread_* functions
#include <unistd.h>
#include <stdlib.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

#define OUTPUT_PATH "processed_urls.txt"

struct Queue* url_frontier;
pthread_mutex_t url_frontier_lock = PTHREAD_MUTEX_INITIALIZER;  // initialize the mutex


// File for saving processed URLs
FILE *url_file;
int c = 0;


// function to handle HTTP response data
size_t handle_response(char *ptr, size_t size, size_t nmemb, TidyBuffer* tb) {
    // Enqueue the links from the HTML content
    tidyBufAppend(tb, ptr, (size * nmemb));

    // Return the total size processed
    return (size * nmemb);
}

<<<<<<< HEAD
void write(char** output){
	for(int i = 0; i < 10; i++){
		if(output[i]){
			printf("Writing %d: %s\n", i, output[i]);
			openFileAndWrite(OUTPUT_PATH, output[i]);
		}
=======
void getLinks(xmlDocPtr d){
	xmlInitParser();
	xmlXPathContextPtr x_context;
	xmlXPathObjectPtr x_obj;
	xmlNodeSetPtr n;
	int i;

	x_context = xmlXPathNewContext(d);

	if(x_context == NULL){
		fprintf(stderr, "Unable to create xPath\n");
		return;
>>>>>>> parent of bb1cf52 (Added comments)
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
void getLinks(TidyNode* n, char** output) {
	TidyNode child;

	for(child = tidyGetChild(n); child != NULL; child = tidyGetChild(n)){
		TidyAttr href = tidyAttrGetById(child, TidyAttr_HREF);
		if(href){
			if(c < 10){
				strcpy(output[c], tidyAttrValue(href));
				c++;
=======
=======
>>>>>>> parent of bb1cf52 (Added comments)
	const xmlChar* x_expr = (xmlChar*) "//a";
	x_obj = xmlXPathEvalExpression(x_expr, x_context);

	if(x_obj == NULL){
		fprintf(stderr, "Failed to evaluate xpath expression\n");
		xmlXPathFreeContext(x_context);
		return;
	}
	

	n = x_obj->nodesetval;
	printf("WORKING: %d\n", n->nodeNr);
	for(i = 0; i < n->nodeNr; ++i){
		xmlNodePtr a = n->nodeTab[i];
		xmlChar* href = xmlGetProp(a, (xmlChar*) "href");
		if(href != NULL){
			char* href_copy = (char*)xmlStrdup(href);
			while(*href_copy != '\0'){
				printf("%c", *href_copy++);
			}
			if(href_copy != NULL){
				pthread_mutex_lock(&url_frontier_lock);
				enqueue(url_frontier, href_copy);
				pthread_mutex_unlock(&url_frontier_lock);
>>>>>>> parent of bb1cf52 (Added comments)
			}
		}
		if(tidyAttrValue(href)) printf("URL Found: %s\n", tidyAttrValue(href));
	}
	getLinks(child, output);
}


// The web_crawler thread function
void *web_crawler_thread(void *arg) {
	int i = 0;
	char* content = NULL;
	content[0] = '\0';
	char** parsedURLS;


	CURL* curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Failed to initialize CURL\n");
		return NULL;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_response);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	const char *url = NULL;

		// Dequeue the next URL
		pthread_mutex_lock(&url_frontier_lock);
        if (!empty(url_frontier)) {
            url = dequeue(url_frontier);
        }
        pthread_mutex_unlock(&url_frontier_lock);

		printf("Currently Crawling: %s\n", url);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, url_frontier);

		curl_easy_setopt(curl, CURLOPT_URL, url);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
		}
		printf("%d", i);
<<<<<<< HEAD
<<<<<<< HEAD
		//Proper format for HTML parsing
		/*TidyBuffer o = {0};
		TidyDoc tdoc = tidyCreate();
		tidyOptSetBool(tdoc, TidyForceOutput, yes);
		tidyOptSetInt(tdoc, TidyWrapLen, 0);
		tidySetErrorBuffer(tdoc, &o);
		tidyParseString(tdoc, content);
		tidyCleanAndRepair(tdoc);
		tidySaveBuffer(tdoc, &o);
		tidyRelease(tdoc);*/
=======
=======
>>>>>>> parent of bb1cf52 (Added comments)
		xmlDocPtr doc = htmlReadDoc((xmlChar*) url, NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
>>>>>>> parent of bb1cf52 (Added comments)

		TidyDoc doc = tidyCreate();
		TidyBuffer o = {0};
		TidyBuffer err = {0};
		tidyBufInit(&o);
		tidyOptSetInt(doc, TidyWrapLen, 2048);
		tidyOptSetBool(doc, TidyForceOutput, yes);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &o);
		if (res == CURLE_OK) {
			printf("Crawled %s\n", url);
			tidyParseBuffer(doc, &o);

			for(int i = 0; i < 10; i++){
				parsedURLS[i] = (char*)malloc(2048*sizeof(char*));
			}

			for(int i = 0; i < 10; i++){
				getLinks(tidygetBody(doc), parsedURLS);
			}
		}	

		if(content != NULL){
			getLinks(content, url_frontier);
			fprintf(url_file, "%s\n", url);
			free(content);
			content = NULL;
		}

		//tidyBufFree(&o);

		//xmlDocPtr doc = htmlReadDoc((xmlChar*) url, NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

		/*if(doc == NULL){
			fprintf(stderr, "Unable to parse: %s\n", url);
			free((char*)url);
			continue;
		}*/

<<<<<<< HEAD
<<<<<<< HEAD
		printf("Getting links from the response...\n");
		getLinks(content, url_frontier); //Entry for parser
		printf("Links extracted from the response.\n");

		//xmlFreeDoc(doc);
=======
=======
>>>>>>> parent of bb1cf52 (Added comments)
		getLinks(doc);
		xmlFreeDoc(doc);
>>>>>>> parent of bb1cf52 (Added comments)

		// Save processed URL
		fprintf(url_file, "%s\n", url);

		free((char*)url);
		i++;
<<<<<<< HEAD
<<<<<<< HEAD
		usleep(1000000); //Sleeping script to not potentially overwhelm webpage with requests

=======
=======
>>>>>>> parent of bb1cf52 (Added comments)
		usleep(1000000);
	}
>>>>>>> parent of bb1cf52 (Added comments)
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
