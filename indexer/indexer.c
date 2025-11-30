#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

void indexPage (index_t* index, webpage_t* webpage, int docID);
void indexBuild (char* pageDirectory, char* indexFilename);

int main (int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indexer <pageDirectory> <indexFilename>\n");
        return 1;
    }

    char pathname[200];
    snprintf(pathname, sizeof(pathname), "%s/.crawler", argv[1]);

    // Check if the page directory was produced by the Crawler
    if (access(pathname, F_OK) != 0) {
        fprintf(stderr, "Error - page directory was not produced by the Crawler\n");
        return 1;
    }

    // Check if the index file can be created or written
    FILE* testFile = fopen(argv[2], "w");
    if (testFile == NULL) {
        fprintf(stderr, "Error - index file cannot be written\n");
        return 1;
    }
    fclose(testFile);

    indexBuild(argv[1], argv[2]);
    return 0;
}

void indexBuild (char* pageDirectory, char* indexFilename) {
    index_t* index = index_new(500);
    if (index == NULL) {
        fprintf(stderr, "Error - failed to create index\n");
        return;
    }
    int docID = 1;
    while (true) {

        webpage_t* webpage = pagedir_load(pageDirectory, docID);
        if (webpage == NULL) {
            break;
        }

        indexPage(index, webpage, docID);
        webpage_delete(webpage);
        docID++;
    }      

    index_save(index, indexFilename);
    index_delete(index);
}

void indexPage (index_t* index, webpage_t* webpage, int docID) {
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
        if (strlen(word) >= 3 && word[0] != '<' && word[strlen(word) - 1] != '>') {
            normalize_word(word);
            index_insert(index, word, docID);
        }
        free(word);
    }
}