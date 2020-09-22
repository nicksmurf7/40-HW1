#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"


void add_to_string(char **line, char c, int length);
void reallocate(char **line, int malloc_size);
char *cleanLine(char **line, int length); 
int isAcceptableChar(char c);

#define INITIAL_SIZE 200

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    FILE *file = fopen("test.txt", "r");
    FILE *Sfile = fopen("test2.txt", "r");
    char *fline;
    char *sline;
    int x = readaline(file, &fline);
    int y = readaline(Sfile,&sline);
    printf("FLINE LENGTH == %d\n", x);
    printf("SLINE LENGTH == %d\n", y);
    printf("FLINE  == %s\n", fline);    
    printf("SLINE  == %s\n", sline);  
    char *fclean = cleanLine(&fline,x);
    char *sclean = cleanLine(&sline,y);
    printf("CLEAN FLINE == %s\n", fclean);
    printf("CLEAN SLINE == %s\n", sclean);
}


size_t readaline(FILE *inputfd, char **datapp) {
    char *line;
    int c;
    int length = 0;
    int malloc_size = INITIAL_SIZE;

    line = (char *) malloc(INITIAL_SIZE);

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

    printf("readaline read in:");
    printf("%s\n", line);
    *datapp = line;
    return length;
}

void add_to_string(char **line, char c, int length){
    *(*line + length) = c;
}
void reallocate(char **line, int malloc_size) {
    *line = realloc(*line, malloc_size * 2);
}


/* cleanLine
 * Gets: Line to be cleaned
 * Returns: Nothing
 * Does: Cleans line so that it can be formatted the same as others
 *
 *   ADD COMMENTS WITHIN
 */
char *cleanLine(char **line, int length) 
{
    char c = 0;
    char *clean = (char *) malloc(sizeof(*line)*length);
    int place = 0;
    int put = 0;
    c = *(*line + place);
    //printf("%c\n", c);
    while(place < length) {
        c = *(*line + place);
        //printf("%c\n", c);
        //printf("%d\n", c);
        if (isAcceptableChar(c) == 1) {
            clean[put] = c;
            place = place + 1;
            put = put + 1;
        } else {
            while(place < length && isAcceptableChar(c) == 0) {
                place = place + 1;
                c = *(*line + place);
            }
            if (place < length) {
                clean[put] = 32;
                put = put + 1;
            }
        }
        
    }
    clean[put] = '\0';
    printf("%d\n", put);
    //free(*line);
    //clean = realloc(sizeof(*line)(length - )
    return clean;
}



int isAcceptableChar(char c) 
{
    if (c == 95) {
        /* If c is an underline */
            return 1;
        } else if (c >= 65 && c <= 90) {
            /* If c is  */
            return 1;
        } else if (c >= 97 && c <=122) {
            /* If c is */
            return 1;
        } else if (c >= 48 && c <= 57) {
            /* If c is */
            return 1;
        } else {
            /* If c is unacceptable */
            return 0;
        }

}
