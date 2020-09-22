#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
void add_to_string(char **line, char c, int length);
void reallocate(char **line, int malloc_size);

#define INITIAL_SIZE 200

size_t readaline(FILE *inputfd, char **datapp) {
    char *line;
    int c;
    int length = 0;
    int malloc_size = INITIAL_SIZE;

    line = malloc(INITIAL_SIZE);

    if (feof(inputfd)) {
                *datapp = NULL;
                free(line); //NEW
                return 0;
    }

    c = fgetc(inputfd);

    while (!feof(inputfd)) {
        add_to_string(&line, c, length);
        length = length + 1;

        if (length >= malloc_size) {
            reallocate(&line, malloc_size);
            malloc_size = malloc_size * 2;
        }

        if(c == 10) {
            break;
        }

        c = fgetc(inputfd);
    }

    //printf("readaline read in:");
    //printf("%s\n", line);
    *datapp = line;
    return length;
}

void add_to_string(char **line, char c, int length){
    *(*line + length) = c;
}
void reallocate(char **line, int malloc_size) {
    *line = realloc(*line, malloc_size * 2);
}
