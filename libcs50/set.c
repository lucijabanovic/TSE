/* 
 * set.c - CS50 'set' module
 *
 * see hashtable.h for more information.
 *
 * Lucija Banovic, FA24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "mem.h"

/**************** local types ****************/
typedef struct setnode {
    void* item;
    char* key;
    struct setnode *next;
} setnode_t;

/**************** global types ****************/
typedef struct set {
    struct setnode *head;
} set_t;

/**************** global functions ****************/
set_t* set_new(void) {
    set_t* set = mem_malloc(sizeof(set_t));

    if (set == NULL) {
        return NULL;
    } else {
        set->head = NULL;
        return set;
    }
}

bool set_insert(set_t* set, const char* key, void* item) {
    if (set == NULL || key == NULL || item == NULL) {
        return false;
    }

    setnode_t* current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false; //key already exists
        }
        current = current->next;
    }

    setnode_t* new_node = mem_malloc(sizeof(setnode_t));
    if (new_node == NULL) {
        return false; // memory allocation failed
    }

    new_node->key = mem_malloc(strlen(key) + 1);
    if (new_node->key == NULL) {
        mem_free(new_node);
        return false; // memory allocation failed
    }

    strcpy(new_node->key, key);

    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;

    return true;
}

void* set_find(set_t* set, const char* key) {
    if (set == NULL || key == NULL) {
        return NULL;
    }

    setnode_t* current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item; // key found
        }
        current = current->next;
    }

    return NULL; // key not found
}

void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)) {
    if (fp == NULL) {
        return;
    }

    if (set == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    fprintf(fp, "{");

    setnode_t* current = set->head;
    bool first = true;

    while (current != NULL) {
        if (!first) {
            fprintf(fp, ", ");
        }

        if (itemprint != NULL) {
            (*itemprint)(fp, current->key, current->item);
        } else {
            fprintf(fp, "%s", current->key); // if itemprint==NULL, print set with no items
        }

        first = false;
        current = current->next;
    }

    fprintf(fp, "}\n");
}

void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)) {
    if (set != NULL && itemfunc != NULL) {
        for (setnode_t* node = set->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->item);
        }
    }   
}

void set_delete(set_t* set, void (*itemdelete)(void* item)) {
    if (set == NULL) {
        return;
    }
    
    setnode_t* current = set->head;
    while (current != NULL) {
        setnode_t* next = current->next;
        if (itemdelete != NULL) {
            (*itemdelete)(current->item);
        }

        mem_free(current->key);
        mem_free(current);
        current = next;
    }
    mem_free(set);
}