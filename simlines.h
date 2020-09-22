#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include <list.h>
#include <table.h>
#include <atom.h>
#include <string.h>

struct location {
    char *filename;
    int linenumber;
    int identifier;
    const int timewaster;
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
void get_files(List_T allfiles, int numfiles, char *files[]);
void compare_Files(List_T allfiles, Table_T matches);
void process_File(struct filedata file);
int file_Compare(struct filedata constantfile, struct filedata comparedfile, Table_T matches);
int line_Compare(const char *line, const char *secondaryline, int length);
const char *cleanLine(char **line, int length);
int isAcceptableChar(char c);
int char_Length(char *c);
void addToData(int linecount, struct filedata file, const char *aline, Table_T matches);
void print_Table(const void *key, void **value, void *left);
int check_Repeat(Table_T matches, struct filedata file, const char *aline, int linenumber);
int filedata_Compare(struct filedata file1, struct filedata file2);
void applyFree(void **ptr, void *cl);
void printvalue(void **ptr, void *cl);
void end_Simlines(Table_T matches, List_T allfiles);
void remove_List(const void *key,void **value, void *cl);
