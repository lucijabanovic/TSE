#ifndef PAGEDIR_H
#define PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"

// Initializes the page directory by creating a hidden .crawler file.
bool pagedir_init(const char* pageDirectory);

// Saves the webpage content to a file in the specified directory.
bool pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

//Loads the webpage content from a file in the specified directory
webpage_t* pagedir_load (const char* pageDirectory, const int docID);

#endif // PAGEDIR_H
