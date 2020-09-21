#include "simlines.h"

int main(int argc, char *argv[]) {

    (void) argv;
    if (argc == 1) {
        //Exit Quitely
    } else {
        List_T allfiles = List_list(NULL);
        int count = 0;
        for (int i = 1; i < argc; i++){
               struct filedata *toadd = malloc(sizeof(struct filedata));
               toadd->filename = argv[i];
               toadd->identifier = count;
               count = count + 1;
               allfiles = List_push(allfiles, (void *) toadd);
        }

        Table_T matches = Table_new(0, NULL, NULL);

        allfiles = List_reverse(allfiles);
       
        compare_files(allfiles, matches);

        int x = Table_length(matches);
        Table_map(matches, print_table, (void *) &x);
    }
    return 0;
}

void compare_files(List_T allfiles, Table_T matches) {
    List_T parse = allfiles;
    while (parse != NULL) {
        struct filedata *current = parse->first; 
        List_T compare_parse = allfiles;

        while (compare_parse != NULL) {
            struct filedata *secondary = compare_parse->first;
            if (filedata_compare(*current, *secondary) == 0) {
                file_compare(*current, *secondary, matches);
            }
            compare_parse = compare_parse->rest;
        }
        parse = parse->rest;
        }
}

int filedata_compare(struct filedata file1, struct filedata file2) {
    //printf("comparingfiles\n");
    if (file1.identifier == file2.identifier && file1.filename == file2.filename){
        //printf("same\n");
        return 1;
    }
    //printf("different\n");
    return 0;
}

int file_compare(struct filedata file, struct filedata secondaryfile, Table_T matches) {
    //(void) constantfile;
    //(void) comparedfile;
    //(void) matches;
    FILE *Ffile = fopen(file.filename, "r");
    
    char *line;
    readaline(Ffile, &line);
    int linecount = 0;
    int loopcheck1 = 0;
    int loopcheck2 = 0;
    while (line[0] != '\0') {
        FILE *Fsecondaryfile = fopen(secondaryfile.filename, "r");
        loopcheck1++;
        //printf("%s" "%d\n", "primary check line #: ", loopcheck1);
        linecount = linecount + 1;
        char* secondaryline;
        readaline(Fsecondaryfile, &secondaryline);
        while (secondaryline[0] != '\0') {
            loopcheck2++;
            //printf("%s" "%d\n", "secondary check line #: ", loopcheck2);
            if(line_compare(line, secondaryline) == 0) {
                //ADD TO DB
                addtodata(linecount, file, line, matches);
                //printf("should add\n");
            }
            readaline(Fsecondaryfile, &secondaryline);
            //printf("%s" "%s\n", "next secondary compare ", compareline);
        }
        readaline(Ffile, &line);
        fclose(Fsecondaryfile);
    }
    //printf("file_compare\n");
    fclose(Ffile);
    
    return 0;
}

void addtodata(int linecount, struct filedata file, char *line, Table_T matches) {
    //printf("%s" "%s\n", "adding to data ", line);
    //printf("%s\n",file.filename);
    cleanline(&line);
    const char *aline = Atom_string(line);
    if (Table_get(matches, aline) == NULL) {
        //printf("Adding a new match to db\n");
        List_T locations  = List_list(NULL);
        struct location *newloc_1 = malloc(sizeof(struct location));
        //struct location *newloc_2 = malloc(sizeof(struct location));
        newloc_1->filename = file.filename;
        newloc_1->linenumber = linecount;
        newloc_1->identifier = file.identifier;
        //newloc_2->filename = comparedfile;
        //newloc_2->linenumber = linecount;
        locations = List_push(locations, newloc_1);
        //locations = List_push(locations, newloc_2);
        Table_put(matches, aline, locations); 
    } else {
            if (check_repeat(matches, file, line, linecount) == 1) {
            List_T locations  = Table_get(matches, aline);
            //Already in the lsit
            //printf("adding to already existing part\n");
            //List_T locations  = Table_get(matches, aline);
            struct location *newloc_1 = malloc(sizeof(struct location));
            newloc_1->filename = file.filename;
            newloc_1->linenumber = linecount;
            newloc_1->identifier = file.identifier;
            locations = List_push(locations, newloc_1);
            Table_put(matches, aline, locations);
            } else {
                //printf("did not add because it is a repeat\n");
            }
        
    }
    //printf("%d\n", List_length(Table_get(matches, Atom_string(line))));
}

int check_repeat(Table_T matches, struct filedata file, char *line, int linenumber) {

    List_T locations = Table_get(matches, Atom_string(line));
    List_T parse = locations;
    //printf("checking for repeat\n");
    ///printf("%s""%d", "identifier: " , file.identifier);
    while (parse != NULL) {
        
        struct location *current = parse->first;
        //printf("%s""%d\n", " versus identifier: " , current->identifier);
        if (file.identifier == current->identifier) {
            if (file.filename == current->filename) {
                if (linenumber == current->linenumber) {
                    //printf("%s", line);
                    //printf("was a repeat\n");
                    return 0;
                }
            }
        }
        parse = parse->rest;
    } 
    return 1;
}

int line_compare(char *line, char *secondaryline) {
    //printf("comparing: ");
    //printf("%s" "%s" "%s", line, " and ", secondaryline);
    //printf("\n");
    cleanline(&line);
    cleanline(&secondaryline);
    //printf("comparing (clean): ");
    //printf("%s" "%s" "%s\n", line, " and ", secondaryline);
    //printf("%s" "%d\n", "length1: ", char_length(line));
    //printf("%s" "%d\n", "length2: ", char_length(secondaryline));
    if (strcmp(line, secondaryline) == 0 ){
        //They are the same
        //printf("returning that they are the same\n");
        return 0;
    } else {
        //they are not the same
        //printf("returning that they are NOT the same\n");
        return 1;
    }

    /*
    if(char_length(line) != char_length(secondaryline)){
        printf("(length) returning that they are not the same\n");
        return 1;
    }

    
    for (int x = 0; x <= (int) sizeof(line); x++)
    {
        if(line[x] != secondaryline[x])
        {
            printf("returning that they are not the same\n");
            return 1;
        }
    }
    printf("returning that they are the same\n");
    */
    return 0;
}

void cleanline(char **line) {
    //Print checks
    //printf("cleaning\n");
    //printf("%s\n", *line);
    //int length = char_length(*line);
    char c;
    char *clean = malloc(sizeof(*line)*char_length(*line));
    //int shift = 0;

    int place = 0;
    int put = 0;
    c = *(*line + place);
    //printf("%s""%c\n", "c: ", c);
    int protect = 0;
    while(c != '\0') {
        c = *(*line + place);
        if (isacceptablecharacter(c) == 1) {
            //printf("adding put ");
            //printf("%c\n", c);
            clean[put] = c;
            place = place + 1;
            put = put + 1;
        } else {
            clean[put] = 32;
            put = put + 1;
            while(isacceptablecharacter(c) == 0 && c != '\0') {
                //printf("skipping put");
                //printf("%c\n", c);
                place = place + 1;
                c = *(*line + place);
            }
        }
        protect = protect + 1;
    }

    /*
    for (int i = 0; i < length; i++) {
        //printf("C4\n");
        //printf("%s" "%d", "i = ", i);
        c = *(*line + i);
        //printf("%s" "%c" "%d\n","character: ", c , c);
        //printf("C4.5\n");
        //1 = acceptable
        //0 = unacceptable
        if (isacceptablecharacter(c) == 1) {
            //printf("%s" "%d\n", "acceptable char input at: ", i - shift);
            //printf("%d\n", shift);
            clean[i - shift] = (*line)[i];
            //printf("C6\n");
        } else {
            //the current char is not a-z or '_'
            //printf("the char is unacceptable\n");
            if(i != 0 && !(i == (length - 1))) {
                if (isacceptablecharacter(*(*line + i - 1)) == 1 && isacceptablecharacter(*(*line + i + 1))){
                    //printf("C6.3\n");
                    //printf("%s" "%d\n", "space input at: ", i - shift);
                    clean[i - shift] = (*line)[i];
                    //printf("C6.7\n");
                } else {
                    //printf("shift+\n");
                    //printf("shifting and not copying because double letter\n");
                    shift = shift + 1;
                    //do not add the character
                }
            } else {
                //Is the first character
                //printf("shifting and not copying because at front or end\n");
                shift = shift + 1;
            }
        }
        
    }
    */
    //printf("%s" "%s\n", "Clean:", clean);
    *line = clean;
}

int isacceptablecharacter(char c) {
    //printf("%s" "%c\n","c = ", c);
    if (c == 95){
            //printf("B2\n");
            return 1;
        } else if (c >= 65 && c <= 90) {
            //printf("B3\n");
            return 1;
        } else if (c >= 97 && c <=122) {
            //printf("B4\n");
            return 1;
        } else {
            //printf("B55555555555555\n");
            return 0;
        }
}

int char_length(char *c) {
    int i = 0;
    for (i = 0; (int) c[i] != (int)'\0'; ++i);
    return i;
}

struct location makelocation(char *filename, int linenumber){
    struct location newloc;
    newloc.filename = filename;
    newloc.linenumber = linenumber;
    return newloc;
}

void add_match(List_T *matches) {
    (void) matches;
}

void print_table(const void *key, void **value, void *left) {
    printf("%s\n", (char *) key);
    List_T parse = *value;
    parse = List_reverse(parse);
    //printf("%d\n", x);
    while (parse != NULL) {
        struct location *currentlocation = parse->first;
        printf("%-20s" "%7d\n", currentlocation->filename, currentlocation->linenumber);
        
        parse = parse->rest;
    }
    if (*(int *)left != 1) {
        printf("\n");
    }
    *(int *)left = *(int *)left - 1;
}
