#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"

size_t readaline(FILE *inputfd, char **datapp) {
    int initialsize = 200;
    char *line = malloc(initialsize);
    
    fgets(line, initialsize, inputfd);
    
    *datapp = line;
    return sizeof(line);
}
