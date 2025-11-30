#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void normalize_word(char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}