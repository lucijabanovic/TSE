/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Lucija Banovic, FA24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "hashtable.h"
#include "mem.h"

/**************** local types ****************/
typedef struct hashnode {
    void* item;                       
    char* key;                     
    struct hashnode *next;    
} hashnode_t;

/**************** global types ****************/
typedef struct hashtable {
    hashnode_t** table;
    int num_slots;
} hashtable_t;

/**************** global functions ****************/
hashtable_t* hashtable_new(const int num_slots) {
    if (num_slots <= 0) {
        return NULL;
    }

    hashtable_t* new_hashtable = mem_malloc(sizeof(hashtable_t));
    if (new_hashtable == NULL) {
        return NULL; //failed allocation
    }

    new_hashtable->table = mem_calloc(num_slots, sizeof(hashnode_t*));
    if (new_hashtable->table == NULL) {
        mem_free(new_hashtable);
        return NULL;
    }

    new_hashtable->num_slots = num_slots;
    return new_hashtable;
}

bool hashtable_insert(hashtable_t* ht, const char* key, void* item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }

    unsigned long slot = hash_jenkins(key, ht->num_slots);
    
    hashnode_t* current = ht->table[slot];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false;
        }
        current = current->next;
    }

    hashnode_t* new_node = mem_malloc(sizeof(hashnode_t));
    if (new_node == NULL) {
        return false;
    }

    new_node->key = mem_malloc(strlen(key) + 1);
    if (new_node->key == NULL) {
        mem_free(new_node); 
        return false;
    }
    strcpy(new_node->key, key);

    new_node->item = item;
    new_node->next = ht->table[slot];
    ht->table[slot] = new_node;
    return true;
}

void* hashtable_find(hashtable_t* ht, const char* key) {
    if (ht == NULL || key == NULL) {
        return NULL;
    }

    unsigned long slot = hash_jenkins(key, ht->num_slots);

    hashnode_t* current = ht->table[slot];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item;
        }
        current = current->next;
    }

    return NULL;
}

void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)) {
    if (fp == NULL) {
    }

    if (ht == NULL) {
        fprintf(fp, "(null)\n"); 
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        hashnode_t* current = ht->table[i];
        if (current == NULL) {
            fprintf(fp, "[%d]: (empty)\n", i);
        } else {
            fprintf(fp, "[%d]: ", i);
            while (current != NULL) {
                if (itemprint != NULL) {
                    itemprint(fp, current->key, current->item); 
                } else {
                    fprintf(fp, "(%s, %p) ", current->key, current->item);
                }
                current = current->next;
            }
            fprintf(fp, "\n");
        }
    }   
}

void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)) {
    if (ht != NULL && itemfunc != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            hashnode_t* current = ht->table[i];
            while (current != NULL) {
                (*itemfunc)(arg, current->key, current->item);
                current = current->next;
            }
        }
    }       
}

void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item)) {
    if (ht != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            hashnode_t* current = ht->table[i];
            while (current != NULL) {
                hashnode_t* temp = current;
                current = current->next;

                if (itemdelete != NULL) {
                    itemdelete(temp->item);
                }

                free(temp->key);
                free(temp);
            }
        }     
        free(ht->table);
        free(ht); 
    }
}
