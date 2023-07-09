# **MultiThreadedWebCrawlerProject**

## **Web Crawler OS Project**

### terminal commands for use:
 $ make           #compiles all files
 $ ./web_crawler  #run project
 $ make clean     #remove compile files

### main.c
- creates the web_crawler and runs it

### web_crawler.c
- implements the functions declared in 'web_crawler.h' 

### web_crawler.h
- declares our 'web_crawler' structure and its associated functions

### queue.c
- the multi-threading management  (task queue, visited set)

### TODOs
- implement web crawling logic in web_crawler.c
- the logic to fetch and parse web page: 'libxml2' library

