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


void run_Simlines(List_T allfiles, int argc, char *argv);
void compare_Files(List_T allfiles, Table_T matches);
void process_File(struct filedata file);
int file_Compare(struct filedata constantfile, struct filedata comparedfile, Table_T matches);
int line_Compare(char *line, char *secondaryline);
void cleanLine(char **line);
int isAcceptableChar(char c);
int char_Length(char *c);
struct location makeLocation(char *filename, int linenumber);
void addToData(int linecount, struct filedata file, char *line, Table_T matches);
void print_Table(const void *key, void **value, void *left);
int check_Repeat(Table_T matches, struct filedata file, char *aline, int linenumber);
int filedata_Compare(struct filedata file1, struct filedata file2);


int string_Comp(char *str1, char *str2);
void end_Simlines(Table_T matches);
void remove_List(const void *key,void **value, void *cl);
