#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>



int main(int argc, char *argv[]){
    FILE *file;
    if (argc == 1) {
        //open from stdin
        file = stdin;
    } else if (argc == 2) {
        //open from filename
        file = fopen(argv[1], "r");
    } else {
        //throw exception
        exit(EXIT_FAILURE);
    }
    //check file
    if (file == NULL) {
        printf("failed to open\n");
    }

    Pnmrdr_T rdr = Pnmrdr_new(file);

    Pnmrdr_mapdata data = Pnmrdr_data(rdr);

    int x = data.width;
    int y = data.height;
    printf("%d\n", x);
    printf("%d\n", y);

    int sum = 0;
    for(int i = 0; i < x*y; i++){
        sum = sum + Pnmrdr_get(rdr);
    }
    printf("Average Brightness: %d\n", sum/(x*y));

    //close file here TODO
    fclose(file);
    return 0;
}
