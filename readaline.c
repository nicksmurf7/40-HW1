#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
void add_to_string(char **line, char c, int length);
void reallocate(char **line, int malloc_size);

#define INITIAL_SIZE 200



/* readaline
 * Gets: User-inputted file, pointer to char array to hold line
 * Returns: Size of line that was just read
 * Does: Reads a line from inputfd, returns the size of 
 * the read line
 */
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


/* add_to_string
 * Gets: Line to add char from, char to add, index of char
 * Returns: Nothing
 * Does: Adds char to string
 */
void add_to_string(char **line, char c, int length){
    *(*line + length) = c;
}



/* reallocate
 * Gets: Line that is too big, previously allocated size
 * Returns: Nothing
 * Does: Reallocs space for line when it is too long
 */
void reallocate(char **line, int malloc_size) {
    *line = realloc(*line, malloc_size * 2);
}
