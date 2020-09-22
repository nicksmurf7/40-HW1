/*
 * file: brightness.c
 * Names: Nick Murphy and Reed Kass-Mullet, 
 * nmurph03 and rkassm01
 *
 * brightness.c has one function that is used to
 * read all of the pixels from a .pgm file
 * and calculate the average brightness
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>


int main(int argc, char *argv[])
{
    FILE *file = NULL;
    if (argc == 1) {
        //open from stdin
        file = stdin;
    } else if (argc == 2) {
        //open from filename
        file = fopen(argv[1], "r");
    } else {
        //throw exception
        fprintf(stderr, "Error: too many files were inputted\n");
        exit(EXIT_FAILURE);
    }
    //check file
    if (file == NULL) {
        printf("Error: failed to open file\n");
        exit(EXIT_FAILURE);
    }

    Pnmrdr_T rdr = Pnmrdr_new(file);
    Pnmrdr_mapdata data = Pnmrdr_data(rdr);
    int total_pix = data.width * data.height;
    if(total_pix == 0){
        fprintf(stderr, "Error: supplied pgm has pixel count of 0\n");
        exit(EXIT_FAILURE);
    }
    double sum = 0;
    double average = 0;
    for(double i = 0; i < total_pix; i++){
        sum = sum + Pnmrdr_get(rdr);
    }
    double denom = data.denominator;
    average = (sum)/(denom * total_pix);
    printf("Average Brightness: %.3f\n", average);
    //close file here TODO
    Pnmrdr_free(&rdr);
    fclose(file);
    return 0;
}


