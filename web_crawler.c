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


// New struct to handle the response data
typedef struct MemoryStruct{
    char *memory;
    size_t size;
} MemoryStruct;


// handle_response function to write data to a MemoryStruct
size_t handle_response(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


void getLinks(xmlDocPtr d){
  //Initial an html parser
  xmlInitParser();
  xmlXPathContextPtr x_context;
  xmlXPathObjectPtr x_obj;
  xmlNodeSetPtr n;
  int i;

  x_context = xmlXPathNewContext(d); //Get path through html

  if(x_context == NULL){
    fprintf(stderr, "Unable to create xPath\n");
    return;
  }
  const xmlChar* x_expr = (xmlChar*) "//a"; //Context for how to find links in html
  x_obj = xmlXPathEvalExpression(x_expr, x_context); //Try to find links

  if(x_obj == NULL){
    fprintf(stderr, "Failed to evaluate xpath expression\n");
    xmlXPathFreeContext(x_context);
    return;
  }
  n = x_obj->nodesetval; //Number of links found
  printf("WORKING: %d\n", n->nodeNr);
  for(i = 0; i < n->nodeNr; ++i){ //Supposed to enqueue new links
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
      }
    }
    xmlFree(href);
  }
  xmlXPathFreeObject(x_obj);
  xmlXPathFreeContext(x_context);
}


// The web_crawler thread function
void *web_crawler_thread(void *arg) {
  web_crawler *crawler = (web_crawler*) arg;
  int i = 0;

  printf("Thread starting\n");
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
      printf("Dequeued URL: %s\n", url);
    }
    pthread_mutex_unlock(&url_frontier_lock);
    // check if paused
    pthread_mutex_lock(&crawler->pause_lock);
    while (crawler->paused) {
      pthread_cond_wait(&crawler->pause_cond, &crawler->pause_lock);
    }
    pthread_mutex_unlock(&crawler->pause_lock);
    printf("Currently Crawling: %s\n", url);
    
    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    if (url != NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("HTML content fetched from URL: %s\n", url);

            // Call htmlReadDoc with the fetched HTML content instead of the URL
            xmlDocPtr doc = htmlReadDoc((xmlChar*)chunk.memory, NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
            if (doc == NULL) {
                fprintf(stderr, "Unable to parse HTML content from URL: %s\n", url);
            } else {
                getLinks(doc); // Entry for parser
                xmlFreeDoc(doc);
            }
        }
    }
    if(chunk.memory) {
        free(chunk.memory);
    }    
    printf("%d", i);
    //Proper format for HTML parsing
    xmlDocPtr doc = htmlReadDoc((xmlChar*) url, NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

    if(doc == NULL){
      fprintf(stderr, "Unable to parse: %s\n", url);
      free((char*)url);
      continue;
    }
    getLinks(doc); //Entry for parser
    xmlFreeDoc(doc);

    // Save processed URL
    fprintf(url_file, "%s\n", url);

    //free((char*)url);
    i++;
    usleep(10000000); //Sleeping script to not potentially overwhelm webpage with requests
  }
  printf("Thread finishing\n");
  curl_easy_cleanup(curl);
  return NULL;
}


// Creates new web_crawler instance
web_crawler *web_crawler_create(char *start_url, int max_threads) {
  // initialize the url_frontier
  url_frontier = createQueue();
  // add the start_url to the url_frontier
  enqueue(url_frontier, start_url);
  // dynamically allocates memory for the web_crawler struct
  web_crawler *crawler = malloc(sizeof(web_crawler));
  // initializes pause related fields
  crawler->paused = 0;
  pthread_mutex_init(&crawler->pause_lock, NULL);
  pthread_cond_init(&crawler->pause_cond, NULL);
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
  pthread_cond_destroy(&crawler->pause_cond);
  pthread_mutex_destroy(&crawler->pause_lock);
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
}


// pause the webcrawler
void web_crawler_pause(web_crawler *crawler) {
  pthread_mutex_lock(&crawler->pause_lock);
  crawler->paused = 1;
  pthread_mutex_unlock(&crawler->pause_lock);
}


// resume from pause
void web_crawler_resume(web_crawler *crawler) {
  pthread_mutex_lock(&crawler->pause_lock);
  crawler->paused = 0;
  pthread_cond_broadcast(&crawler->pause_cond);
  pthread_mutex_unlock(&crawler->pause_lock);
}
//web_crawler.c
