/* 
 * counters.c - CS50 'bag' module
 *
 * see counters.h for more information.
 *
 * Lucija Banovic, FA24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "mem.h"

/**************** local types ****************/
typedef struct counternode {
    int key;                        // counter key
    int value;                      // counter value
    struct counternode *next;       // pointer to the next counter in the set
} counternode_t;

/**************** global types ****************/
typedef struct counters {
    struct counternode *head;       // head of the list of counters in counters set
} counters_t;

/**************** local functions ****************/
static counternode_t* counternode_new(int key) {

    counternode_t* node = mem_malloc(sizeof(counternode_t));

    if (node == NULL) {
        // error allocating memory for node; return error
        return NULL;
    } else {
        node->key = key;
        node->value = 1;
        node->next = NULL;
        return node;
    }
}

static counternode_t* counters_find(counters_t* counterset, int key) {

    if (counterset == NULL) {
        return NULL;
    }

    counternode_t* current = counterset->head;
    while (current != NULL) {
        if (current->key == key) {
            return current; // Key found
        }
        current = current->next;
    }
    return NULL; // Key not found
}

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

counters_t* counters_new(void)
{
  counters_t* counterset = mem_malloc(sizeof(counters_t));

  if (counterset == NULL) {
    return NULL;              // error allocating counter set
  } else {
    // initialize contents of counter set structure
    counterset->head = NULL;
    return counterset;
  }
}

int counters_add(counters_t* ctrs, const int key) {
    if (ctrs == NULL || key < 0) {
        return 0;
    }

    counternode_t* existing_counter = counters_find(ctrs, key);

    if (existing_counter != NULL) {
        existing_counter->value++;
        return existing_counter->value;
    } else {
        counternode_t* new_counter = counternode_new(key);
        if (new_counter == NULL) {
            return 0;
        }


        new_counter->next = ctrs->head;
        ctrs->head = new_counter;
        return new_counter->value;
    }
}

int counters_get(counters_t* ctrs, const int key) {
    if (ctrs == NULL || key < 0) {
        return 0;
    }

    counternode_t* node = counters_find(ctrs, key);

    if (node != NULL) {
        return node->value;
    } else {
        return 0;
    }
}

bool counters_set(counters_t* ctrs, const int key, const int count) {
    if (ctrs == NULL || key < 0 || count < 0) {
        return false;
    }

    counternode_t* node = counters_find(ctrs, key);

    if (node != NULL) {
        node->value = count;
    } else {
        counters_add(ctrs, key);
        node = counters_find(ctrs, key);
        node->value = count;
    }

    return true;
}

void counters_print(counters_t* ctrs, FILE* fp) {
    if (fp == NULL) {
        return;
    }

    if (ctrs == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    fprintf(fp, "{");
    counternode_t* current = ctrs->head;
    bool first = true;

    while (current != NULL) {
        if (!first) {
            fprintf(fp, ", ");
        }
        fprintf(fp, "%d=%d", current->key, current->value);
        current = current->next;
        first = false;
    }
    fprintf(fp, "}\n");
}

void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count)) {
    if (ctrs != NULL && itemfunc != NULL) {
        for (counternode_t* node = ctrs->head; node != NULL; node=node->next) {
            (*itemfunc)(arg, node->key, node->value);
        }
    }
}

void counters_delete(counters_t* ctrs) {
    if (ctrs == NULL) {
        return;
    } else {
        counternode_t* current = ctrs->head;
        while (current != NULL) {
            counternode_t* next = current->next; 
            mem_free(current);                       
            current = next;                
        }
        mem_free(ctrs);
    }
}

