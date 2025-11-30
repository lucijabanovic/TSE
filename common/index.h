#ifndef INDEX_H
#define INDEX_H

#include "../libcs50/hashtable.h"  
#include "../libcs50/counters.h"  

typedef struct index index_t;

index_t* index_new(const int num_slots);

void index_insert(index_t *index, const char* word, const int docID);

void index_save(index_t* index, const char* filename);

void write_to_file(const char* word, void* counter, FILE* file);

void fprintf_counter(int docID, int count, FILE* file);

void index_delete(index_t* index);

index_t* index_load (const char* indexFilename);

counters_t* index_get (index_t* index, char* word);

#endif // INDEX_H
