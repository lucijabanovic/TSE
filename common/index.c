/* Module for an index_t type that represents an index in memory
 *
 * Lucija Banovic FA24
 */

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

typedef struct index {
    hashtable_t* ht;
} index_t;

index_t* index_new(const int num_slots) {
    index_t *index = malloc(sizeof(index_t));
    if (index == NULL) {
        return NULL;
    }
    index->ht = hashtable_new(num_slots);
    if (index->ht == NULL) {
        free(index);
        return NULL;
    }
    return index;
}

void index_insert(index_t *index, const char* word, const int docID) {
    counters_t* counter = hashtable_find(index->ht, word);
    if (counter == NULL) {
        counter = counters_new();
        if (counter == NULL) {
            fprintf(stderr, "Error - Failed to create a new counter.\n");
            return;
        }

        hashtable_insert(index->ht, word, counter);

        if (!counters_set(counter, docID, 1)) {
            fprintf(stderr, "Error - Failed to set counter value.\n");
        }
    } else {
        int currentCount = counters_get(counter, docID);
        if (!counters_set(counter, docID, currentCount + 1)) {
            fprintf(stderr, "Error - Failed to update counter value.\n");
        }
    }
}

void free_counter(void* counter) {
    counters_delete(counter);
}

void index_delete(index_t* index) {
    if (index == NULL) return;

    hashtable_delete(index->ht, free_counter);
    free(index);
}

void fprintf_counter(void* arg, const int docID, const int count) {
    FILE* file = arg;
    fprintf(file, "%d %d ", docID, count);
}

void write_to_file(void* arg, const char* word, void* counter) {
    FILE* file = arg;
    fprintf(file, "%s ", word);
    counters_iterate(counter, file, fprintf_counter);  
    fprintf(file, "\n");
}

void index_save(index_t* index, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error - couldn't open file for writing");
        return;
    }

    hashtable_iterate(index->ht, file, write_to_file);

    fclose(file);
}

index_t* index_load (char* indexFilename) {
    FILE* file = fopen(indexFilename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error - unable to load file for reading\n");
        return NULL;
    }

    index_t* index = index_new(100);
    if (index == NULL) {
        fprintf(stderr, "Error - unable to create index\n");
        fclose(file);
        return NULL;
    }

    char* word;

    while ((word = file_readWord(file)) != NULL) {
        if (word == NULL) {
            fprintf(stderr, "Error - unable to read word\n");
            free(word);
            continue;
        }

        int docID, count;
        while (fscanf(file, "%d %d", &docID, &count) == 2) {
            for (int i=0; i < count; i++) {
                index_insert(index, word, docID);
            }
        }

        free(word);
    }

    fclose(file);
    return(index);
}

counters_t* index_get (index_t* index, char* word) {
    hashtable_t* hashtable = index -> ht;
    return hashtable_find(hashtable, word);
}