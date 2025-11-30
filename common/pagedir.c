#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

bool pagedir_init(const char* pageDirectory) {
    char pathname[1024];
    strcpy(pathname, pageDirectory);
    strcat(pathname, "/.crawler");

    FILE *file = fopen(pathname, "w");
    if (file == NULL) {
        fprintf(stderr, "Error creating the .crawler file: %s\n", pathname);
        return false;
    }
    fclose(file);
    return true;
}

bool pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    char pathname[1024];
    
    sprintf(pathname, "%s/%d", pageDirectory, docID);

    FILE *file = fopen(pathname, "w");
    if (file == NULL) {
        fprintf(stderr, "Error creating the file: %s\n", pathname);
        return false;
    }

    fprintf(file, "%s\n", webpage_getURL(page));  
    fprintf(file, "%d\n", webpage_getDepth(page));  
    fprintf(file, "%s\n", webpage_getHTML(page));  

    fclose(file);
    return true;
}

webpage_t* pagedir_load(const char* pageDirectory, const int docID) {
    char pathname[200];
    snprintf(pathname, sizeof(pathname), "%s/%d", pageDirectory, docID);
    FILE* fp = fopen(pathname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error - unable to read file (Crawler file)\n");
        return NULL;
    }

    char* url = file_readLine(fp);

    char* depthChar = file_readLine(fp);
    int depth = 0;
    if (depthChar != NULL) {
        depth = atoi(depthChar);
        free(depthChar);
    }

    size_t htmlSize = 1000;
    char* html = malloc(htmlSize);
    if (html == NULL) {
        free(url);
        fclose(fp);
        fprintf(stderr, "Error - unable to allocate memory for HTML\n");
        return NULL;
    }
    html[0] = '\0'; // Initialize to an empty string

    char* line;
    while ((line = file_readLine(fp)) != NULL) {
        size_t lineLen = strlen(line);
        if (strlen(html) + lineLen >= htmlSize) {
            htmlSize += lineLen + 100; // Increase buffer size (e.g., add 100 bytes)
            char* newHtml = realloc(html, htmlSize);
            if (newHtml == NULL) {
                free(line);
                free(url);
                free(html);
                fclose(fp);
                fprintf(stderr, "Error - unable to reallocate memory for HTML\n");
                return NULL;
            }
            html = newHtml;
        }
        strcat(html, line);
        free(line);
    }

    fclose(fp);

    webpage_t* webpage = webpage_new(url, depth, html);

    return webpage;
}



