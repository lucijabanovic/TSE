#include <stdio.h>
#include <stdlib.h>
#include "../common/index.h"

int main (const int argc, const char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indextest <oldIndexFilename> <newIndexFilename>");
        return 1;
    }

    FILE* oldFile = fopen(argv[1], "r");
    if (oldFile == NULL) {
        fprintf(stderr, "Error - Unable to open file for reading\n");
    }
    fclose(oldFile);

    FILE* newFile = fopen(argv[2], "w");
    if (newFile == NULL) {
        fprintf(stderr, "Error - Unable to open file for writing\n");
    }
    fclose(newFile);

    index_t* index = index_load(argv[1]);
    index_save(index, argv[2]);
    index_delete(index);
}