/* Querier.c file for the tiny search engine
 * Reads through the input lines and the provided index file and ranks the documents containing queried words
 * Lucija Banovic FA24 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include "../common/index.h"
#include "../libcs50/counters.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"


/*
 * a struct that contains the set we want to intersect with another set (set2)
 * and a set that is the result of that intersection (result)
 */
typedef struct twosets {
    counters_t* set2;
    counters_t* result;
} twosets_t;

int parseQuery (char line[], char* words[]);
void tokenize (char line[], char* words[]);
int literalCheck (char* words[]);
counters_t* getDocSet (index_t* index, char* words[]);
void intersect (counters_t* set1, twosets_t* set2);
void merge (counters_t* set1, counters_t* set2); 
void intersectHelper (void* arg, const int docID, const int count);
void unionHelper (void* arg, const int docID, const int count);
int maxScore (counters_t* docSet);
void scoreHelper (void* arg, const int docID, const int count);
void docPrint (counters_t* docSet, const char* pageDirectory);
int getKey (counters_t* docSet);
void getKeyHelper (void* arg, const int docID, const int count);

int main (const int argc, const char* argv[]) {
    
    // Check if it received exactly two command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./querier <pageDirectory> <indexFilename>\n");
        return 1;
    }

    // Checks if the page directory was produced by the crawler
    // Creates a pathname of a format pageDirectory/.crawler
    char pathname[100];
    snprintf(pathname, sizeof(pathname), "%s/.crawler", argv[1]);

    // Checks if a file named "pageDirectory/.crawler" exists
    if ((access(pathname, F_OK)) != 0) {
        fprintf(stderr, "Error: page directory not produced by the crawler\n");
        return 1;
    } 

    // Checks if the indexFilename is the pathname of a file that can be read
    FILE* fp = fopen(argv[2], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: unable to read file\n");
        return 1;
    }
    fclose(fp);

    index_t* index = index_load(argv[2]);
    
    char line[100];
    char* words[100];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (parseQuery(line, words) == 1) {
            fprintf(stderr, "Error: invalid query\n");
        } else {
            counters_t* docs = getDocSet(index, words);

            docPrint(docs, argv[1]);

            counters_delete(docs);

            for (int i = 0; words[i] != NULL; i++) {
                free(words[i]);
            }
        }
    }

    index_delete(index);

    return 0;
}

/*
 * Makes sure all the characters are letters of the alphabet
 * Makes sure that all the words are lowercase
 * Gets rid of all the extra spaces at the beginning, the end, and in between words
 */
int parseQuery (char line[], char* words[]) {
    // Checks if the query only contains letters and spaces
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isalpha(line[i]) && !isspace(line[i])) {
            fprintf(stderr, "Error: query contains an invalid character\n");
            return 1;
        }
    }

    tokenize(line, words);
    if (literalCheck(words) != 0) {
        fprintf(stderr, "Error: literal check failed\n");
        return 1;
    }

    return 0;
}

/* Tokenizes a string into an array of words
 * Without using strtok() as suggested in the lab instructions 
 * Make sure to free memory from the array after using it
 */
void tokenize (char line[], char* words[]) {
    int i = 0;
    int j = 0;
    while (line[i] != '\0') {
        // if character is a space, ignore it and move onto the next character
        if (isspace(line[i])) {
            i++;
            continue;
        }

        // once we come accross a letter
        if (isalpha(line[i])) {
            char letter = tolower(line[i]);
            // create a placeholder for a soon-to-be-created token
            char word[50] = "";
            int k = 0;
            // keep iterating through following letters
            while (isalpha(letter) && letter != '\0') {
                // append those letters onto the token
                word[k] = letter;
                i++;
                k++;
                letter = tolower(line[i]);
            }
            word[k] = '\0';
            
            // Allocate space for the token in the array
            words[j] = malloc(strlen(word) + 1);
            if (words[j] != NULL) {
                strcpy(words[j], word);
                j++;
            }
        } else {
            i++;
        }
    }

    // set the last element in words[] to NULL as an indicator of when the array ends
    words[j] = NULL;
}

int literalCheck (char* words[]) {
    // calculate the lenght of array words[]
    int length = 0;
    while (words[length] != NULL) {
        length++;
    }

    // check if there's an "or" or "and" operator at the beginning/end of the array
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0 || 
    strcmp(words[length-1], "and") == 0 || strcmp(words[length-1], "or") == 0) {
        fprintf(stderr, "Error: an operator cannot be at the beginning or the end of the query\n");
        return 1;
    }

    // loop through the array and check for any adjacent operators
    for (int i = 0; i < length-1; i++) {
        if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
            if (strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0) {
                fprintf(stderr, "Error: two operators cannot be adjacent\n");
                return 1;
            }
        }
    }

    return 0;
}
/*
 * Loops through the parsed query
 * Returns a counters set of documents that satisfy the query as keys and their scores as items
 */
counters_t* getDocSet (index_t* index, char* words[]) {
    // create a counters set that will store all documents that satisfy the query
    counters_t* docSet = counters_new();

    int i = 0;

    // indicator of the last word in query
    bool last = false;
    
    // loop through all the words in the query
    while (words[i] != NULL) {

        // create a counters set that will store all documents in the current andsequence
        counters_t* andSet = counters_new();

        // indicator that we are working with the first word in the andsequence
        bool first = true;

        // loop through words until we reach "or" operator
        while (words[i] != NULL && strcmp(words[i], "or") != 0) {

            // get the set of documents in which the current word appears
            counters_t* counterset = index_get(index, words[i]);

            // if the word doesn't appear in any document (counterset = NULL), skip onto the next word
            if (strcmp(words[i], "and") != 0 && counterset == NULL) {
                printf("Warning: word '%s' cannot be found\n", words[i]);
                i++;
                continue;
            }

            // if the word is the first word in andsequence, merge it with andSet
            if (first) {
                merge(andSet, counterset);
                first = false;
            }

            // if the word is an "and" operator, continue onto the next word
            if (strcmp(words[i], "and") == 0) {
                i++;
                continue;
            }

            // create a twosets_t struct and set the merging set set2 to counterset
            // create a new counters set result
            twosets_t twoset = { .set2 = counterset, .result = counters_new() };

            // intersect sets andSet and counterset and store the result in twoset.result
            intersect(andSet, &twoset);
            counters_delete(andSet);
            andSet = twoset.result;

            // if the word is the last word in query, merge the last andSet with the docSet
            if (words[i+1] == NULL) {
                merge(docSet, andSet);
                counters_delete(andSet);
                // indicate that this is the last word
                last = true;
            }
            
            i++;
        }

        // if we're at the last word in query, exit the while loop
        if (last) break;

        if (words[i+1] == NULL || words[i] == NULL) {
            break;
        }

        // if we come accross an "or" operator, merge the last andSet with docSet
        if (docSet != NULL) {
            merge(docSet, andSet);
        }

        if (andSet != NULL) {
            counters_delete(andSet);
        }
        i++;      
    }

    return docSet;
}

/* Intersect two sets using intersectHelper */
void intersect (counters_t* set1, twosets_t* twoset) {
    counters_iterate(set1, twoset, intersectHelper);
}

/* Helper function for intersect() */
void intersectHelper (void* arg, const int docID, const int count) {
    // using our twosets_t struct as an arbitrary *arg pointer
    // this way, we can work with two sets in a helper function, instead of only one
    twosets_t* twoset = arg;

    // if there are two matching keys in these sets
    if (counters_get(twoset->set2, docID) != 0) {
        // check which key has a smaller value
        // set the key to docID and set the minimum value as the document score in the twoset->result set
        if (count < counters_get(twoset->set2, docID)) {
            counters_set(twoset->result, docID, count);
        } else {
            counters_set(twoset->result, docID, counters_get(twoset->set2, docID));
        }
    }
}
/* Create a union of two sets using unionHelper */
void merge (counters_t* set1, counters_t* set2) {
    counters_iterate(set2, set1, unionHelper);
}

/* Helper function for merge () */
void unionHelper (void* arg, const int docID, const int count) {
    counters_t* set1 = arg;
    // "append" set2 to onto set1
    // set the score of docID to a sum of docID counts of both sets
    counters_set(set1, docID, count + counters_get(set1, docID));
}

/* Print the set of documents satisfying the query
 * Format:
 *      Score: <document score>
 *      ID: <document id>
 *      URL: <document url>
 */
void docPrint (counters_t* docSet, const char* pageDirectory) {
    int max;
    // loop through the set of documents until all the document scores are zero
    while ((max = maxScore(docSet)) != 0) {
        // get document ID and score using helper functions
        int docID = getKey(docSet);
        int score = counters_get(docSet, docID);

        printf("Score: %d\n", score);
        printf("ID: %d\n", docID);

        // create a file pathname using pageDirectory and document ID
        char pathname[100];
        snprintf(pathname, sizeof(pathname), "%s/%d", pageDirectory, docID);

        FILE* fp = fopen(pathname, "r");
        if (fp != NULL) {
            char* url = file_readLine(fp);
            fclose(fp);
            printf("URL: %s\n", url);
            free(url);
        } else {
            fprintf(stderr, "Error: unable to open file for reading\n");
            return;
        }

        printf("\n");

        // set the visited document score to zero
        counters_set(docSet, docID, 0);
    }
}

/* Get a maximum score from a documents set */
int maxScore (counters_t* docSet) {
    int max = 0;
    counters_iterate(docSet, &max, scoreHelper);

    return max;
}

/* Helper function for maxScore() */
void scoreHelper (void* arg, const int docID, const int count) {
    int* maxpt = arg;
    // *maxpt is initially set to 0
    // iterate through all document scores
    // if the score is greater than *maxpt, set *maxpt to that score
    // in the end we end up with the greatest score in the set
    if (count > *maxpt) {
        *maxpt = count;
    }
}

/* Get the key containing max score */
int getKey (counters_t* docSet) {
    int max = maxScore(docSet);
    int key;
    counters_iterate(docSet, &max, getKeyHelper);
    key = max;

    return key;
}

/* Helper function for getKey() */
void getKeyHelper (void* arg, const int docID, const int count) {
    int* maxpt = arg;
    // *maxpt is initially set to 0
    // iterate through document scores
    // when we come across the biggest score, set the *maxpt to the key containing that score
    // this might be a weird approach because we are changing the count to a key value
    // however, this is the easiest way to do it because I can only pass one void* arg to a helper function
    if (count == *maxpt) {
        *maxpt = docID;
    }
}

