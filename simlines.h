#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include <list.h>
#include <table.h>
#include <atom.h>

struct location {
    char *filename;
    int linenumber;
    int identifier;
};

struct match {
    char *line;
    struct location *match; //this is a list of all matches found
};

struct filedata {
    char* filename;
    int identifier;
};

void compare_files(List_T allfiles, Table_T matches);
int file_compare(struct filedata constantfile, struct filedata comparedfile, Table_T matches);
int line_compare(char *line, char *secondaryline);
void cleanline(char **line);
int isacceptablecharacter(char c);
int char_length(char *c);
struct location makelocation(char *filename, int linenumber);
void addtodata(int linecount, struct filedata file, char *line, Table_T matches);
void print_table(const void *key, void **value, void *left);
int check_repeat(Table_T matches, struct filedata file, char *aline, int linenumber);
int filedata_compare(struct filedata file1, struct filedata file2);
