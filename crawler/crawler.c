#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../common/pagedir.h"


/***** Functions *****/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

int main (const int argc, char* argv[]) {
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
}

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    if (argc == 4) {

        *seedURL = normalizeURL(argv[1]);
        if (*seedURL == NULL) {
            fprintf(stderr, "Error - cannot normalize URL \n");
            exit(1);
        }

        if (!isInternalURL(*seedURL)) {
            fprintf(stderr, "Error - URL is not internal\n");
            exit(1);
        }
        
        *maxDepth = atoi(argv[3]);

        if (*maxDepth < 0 || *maxDepth > 10) {
            fprintf(stderr, "Error - maximum depth needs to be in the range [0, 10]\n");
            exit(1);
        }

        *pageDirectory = argv[2];
        if (!pagedir_init(*pageDirectory)) {
            fprintf(stderr, "Error - cannot initiate page directory\n");
            exit(1);
        } 
    } else {
        fprintf(stderr, "Error - invalid number of arguments");
        exit(1);
    }
}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    int pos = 0;
    char* url;

    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        char* normalized = normalizeURL(url);
        printf("%d Found: %s\n", webpage_getDepth(page), normalized);
        if (isInternalURL(normalized)) {
            if (hashtable_insert(pagesSeen, normalized, "")) {
                webpage_t* newWebpage = webpage_new(normalized, webpage_getDepth(page) + 1, NULL);
                bag_insert(pagesToCrawl, newWebpage);
                printf("%d Added: %s\n", webpage_getDepth(page), normalized);
            } else {
                free(normalized);
            }
        } else {
            free(normalized);
        }
        free(url);
    }
}

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    hashtable_t* hashtable = hashtable_new(200);
    hashtable_insert(hashtable, seedURL, "");

    bag_t* bag = bag_new(); 
    bag_insert(bag, webpage_new(seedURL, 0, NULL));

    webpage_t* page;
    int id = 1;

    while ((page = bag_extract(bag)) != NULL) {
        if (webpage_fetch(page)) {
            printf("%d Fetched: %s\n", webpage_getDepth(page), webpage_getURL(page));
            pagedir_save(page, pageDirectory, id);

            if (webpage_getDepth(page) < maxDepth) {
                printf("%d Scanning: %s\n", webpage_getDepth(page), webpage_getURL(page));
                pageScan(page, bag, hashtable);
            }
        } 
        id++;
        webpage_delete(page);
    }

    hashtable_delete(hashtable, NULL);
    bag_delete(bag, webpage_delete);
}

