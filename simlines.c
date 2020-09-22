#include "simlines.h"

#define SPACE ' '

/*
 * Main
 */
int main(int argc, char *argv[]) 
{
    (void) argv;
    if (argc == 1) {
        exit(EXIT_FAILURE);
    } else {
        List_T allfiles = List_list(NULL);
        int count = 0;
        for (int i = 1; i < argc; i++) {
                struct filedata *toadd = malloc(sizeof(struct filedata));
                toadd->filename = argv[i];
                toadd->identifier = count;
                process_File(*toadd);
                count = count + 1;
                allfiles = List_push(allfiles, (void *) toadd);
        }
        /* Initialize database of matches*/
        Table_T matches = Table_new(0, NULL, NULL);
        /* Reverse list of files so that the first */
        /* inputted file is first in the list */
        //printf("files\n");
        //allfiles = List_reverse(allfiles);

        //int t = List_length(allfiles);
        //printf("%s""%d\n", "number of files: ", t);
       
        /* Compare the two files */
        compare_Files(allfiles, matches);

        /* Print the database of matches */
        printf("Print round 1\n");
        int x = Table_length(matches);
        Table_map(matches, print_Table, (void *) &x);
        //printf("\nPrint round 2\n");
        //int y = Table_length(matches);
        //Table_map(matches, print_Table, (void *) &y);

        /* End simlines by freeing all memory */
        end_Simlines(matches, allfiles);
    }

    return 0;
}


/* Compare_files
 * Gets: List of files, database of matches
 * Returns; Nothing
 * Does: Compares the lines of two files
 */
void compare_Files(List_T allfiles, Table_T matches) 
{
    List_T parse = allfiles;
    while (parse != NULL) {
        struct filedata *current = parse->first; 
        //printf("primary file: ");
        //printf("%s\n", current->filename);
        List_T compare_parse = allfiles;
        /* Parse through list of files while there are files left */
        while (compare_parse != NULL) {
            struct filedata *secondary = compare_parse->first;
            //printf("secondary file: ");
            //printf("%s\n", secondary->filename);

            /* Compare the two files to check if they are the same */
            if (filedata_Compare(*current, *secondary) == 0) {
                //printf("Aren't the same, comparing primary and secondary\n");
                file_Compare(*current, *secondary, matches);
            }
            compare_parse = compare_parse->rest;
        }
        parse = parse->rest;
        }
}

/* filedata_compare
 * Gets: two individual file structs
 * Returns: 1 if the two structs are exactly the same, 0 if not
 * Does: Checks if the same line from 
 *       the same file has been read multiple times
 */
int filedata_Compare(struct filedata file1, struct filedata file2) 
{
    /* Checks if the two file structs are exactly the same */
    if (file1.identifier == file2.identifier 
            && file1.filename == file2.filename) {
        return 1;
    }
    /* If the two structs are not exactly the same, return 0 */
    return 0;
}







/* file_compare
 * Gets: Two filedata structs to compare, database of matches
 * Returns: 0
 * Does: Passes every line in both files to line_compare
 */
int file_Compare(struct filedata file, struct filedata secondaryfile, 
        Table_T matches) 
{
    FILE *Ffile = fopen(file.filename, "r");
    /* If file cannot be opened, exit with EXIT_FAILURE */
    if(Ffile == NULL){
        exit(EXIT_FAILURE);
    }
    
    /* Call readaline on first file */
    int linecount = 0;
    char *line;
    int linelength = readaline(Ffile, &line);
    //while (line[0] != '\0') {
    while(line != NULL) {
        FILE *Fsecondaryfile = fopen(secondaryfile.filename, "r");
        /* If file cannot be opened, exit with EXIT_FAILURE */
        if(Fsecondaryfile == NULL) {
            exit(EXIT_FAILURE);
        }

        linecount = linecount + 1;
        char* secondaryline;
        int secondarylength = readaline(Fsecondaryfile, &secondaryline);

        /* Call readaline on second file */
        while (secondaryline != NULL) {
            /* Call line_compare on constant line and every line in secondary file */
            //printf("line: %s\n", line);
            //printf("secondaryline: %s\n", secondaryline);
            const char *cline = cleanLine(&line, linelength);
            const char *csecondary = cleanLine(&secondaryline, secondarylength);
            //printf("CLEAN line: %s\n", cline);
            //printf("CLEAN secondaryline: %s\n", csecondary);
            if((cline && csecondary) && line_Compare(cline, csecondary, linelength) == 0) {
                /* Add to Database if match exists */
                /////const char *aline = Atom_string(cline);
                //printf("adding: ");
                //printf("%s\n", aline);
                addToData(linecount, file, cline, matches);
            }
            /* Continue reading secondary file */
            free(secondaryline);
            secondarylength = readaline(Fsecondaryfile, &secondaryline);
        }
        /* Continue reading primary file */
        free(line);
        //free(secondaryline); //NOT WORKING
        linelength = readaline(Ffile, &line);
        /* Close secondary file */
        fclose(Fsecondaryfile);
    }
   /* Close primary file */
    fclose(Ffile);
    //free(line); //NOT WORKING
    return 0;
}



/* cleanLine
 * Gets: Line to be cleaned
 * Returns: Nothing
 * Does: Cleans line so that it can be formatted the same as others
 *
 *   ADD COMMENTS WITHIN
 */
const char *cleanLine(char **line, int length) 
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
    //printf("%d\n", put);
    //printf("\"" "%s" "\"\n", clean);
    //printf("%d\n", put);
    //free(*line);
    //clean = realloc(sizeof(*line)(length - )
    const char * aline = Atom_string(clean);
    free(clean);
    if (put == 0) {
        return NULL;
    }
    return aline;
}




/* addtodata
 * Gets: Line number, filedata struct for line, line to be added to database,
 *       match database
 * Returns: Nothing
 * Does: Adds line to database of matches. If it already exists within the database,
 *       the filedata struct that pertains to the line is added to the list of other
 *      matches to that line. 
 */
void addToData(int linecount, struct filedata file, const char *aline, Table_T matches) 
{
    /* Call cleanline so that line can be searched for in database of matches */
    //cleanLine(&line);
    /* Line is converted to an Atom so it can be stored in table easily */
    //const char *aline = Atom_string(line);
    //free(line);
    /* If the line does not already exist within the database */
    if (Table_get(matches, aline) == NULL) {
        //printf("adding new\n");
        /* Initialize list member */
        //List_T locations = malloc(sizeof(*locations)); //mem leak
        List_T locations = List_list(NULL);
        struct location *newloc_1 = malloc(sizeof(struct location));
        newloc_1->filename = file.filename;
        newloc_1->linenumber = linecount;
        newloc_1->identifier = file.identifier;
        /* Push element onto list of matches */
        locations = List_append(locations, List_list(newloc_1, NULL));
        //locations = List_push(locations, newloc_1);
        /* Put new list onto table */
        Table_put(matches, aline, locations); 
        //printf("constructed1\n");
    } else {
        /* If Match already exists in database: */
        //printf("adding already existing\n");
            if (check_Repeat(matches, file, aline, linecount) == 1) {
            /* Get pre-existing list of matches */
            List_T locations = Table_get(matches, aline);
            struct location *newloc_1 = malloc(sizeof(struct location));
            newloc_1->filename = file.filename;
            newloc_1->linenumber = linecount;
            newloc_1->identifier = file.identifier;
            /* Push new map onto list of matches */
            locations = List_append(locations, List_list(newloc_1, NULL)); //mem leak
            //locations = List_push(locations, newloc_1); //mem leak
        
            Table_put(matches, aline, locations);
            //printf("constructed2\n");
        }
    }
}






/* check_Repeat
 * Gets: Database of matches, filedata struct to be checked, line and linenumber
 * Returns: 0 if the exact instance of the line has already been added to the database,
 *          1 if it has not. 
 * Does: Checks if the exact instance of the line has already been added to the database
 */
int check_Repeat(Table_T matches, struct filedata file, const char *line, int linenumber) 
{
    /* Initialize List_T object for parsing through list */
    List_T locations = Table_get(matches, line);
    List_T parse = locations;
    while (parse != NULL) {
        struct location *current = parse->first;
        /* Check if everything within each file already exists in list */
        if (file.identifier == current->identifier) {
            if (file.filename == current->filename) {
                if (linenumber == current->linenumber) {
                    return 0;
                }
            }
        }
        parse = parse->rest;
    } 
    /* If match does not already exist: */
    return 1;
}








/* line_Compare
 * Gets: two linea to be processed
 * Returns: 0 if the lines are the same, 1 if not
 * Does: Checks if two lines  are the same
 */
int line_Compare(const char *line, const char *secondaryline, int length) 
{
    (void) length;
    /* Sanitize lines so that they can be compared
      in a constant format */
    //cleanLine(line);
    //cleanLine(secondaryline);
    /* Compare strings using string_comp function */
    if (strcmp(line, secondaryline) == 0 ) {
        /* They are the same: */
        return 0;
    } else {
        /* They are not the same: */
        return 1;
    }
    return 0;
}

/* string_Comp
 * Gets: Two char arrays
 * Returns: 0 if the char arrays are the same, 0 otherwise
 * Does: Checks if two char arrays are the same
 */
int string_Comp(char *str1, char *str2)
{
    while( ( *str1 != '\0' && *str2 != '\0' ) && *str1 == *str2 && 
        (str1 != NULL && str2 != NULL)) {
        str1++;
        str2++;
    }
    if(*str1 == *str2) {
        return 0; 
    } else {
        return 1;
    }
}














/* isacceptablecharacter
 *Gets: character to be processed
 * Returns: Yes if the character is 
 * Does:
 */ 
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





/* char_length
 * Gets: Pointer to character array
 * Returns: Length of char array
 * Does: Calculates length of char array
 */
int char_Length(char *c) 
{
    int i;
    for (i = 0; (int) c[i] != (int)'\0'; ++i);
    return i;
}




/* MakeLocation
 * Gets: Filename, line number
 * Returns: Pointer to the newly made location
 * Does: Initializes new location to be added to list
 */
struct location makeLocation(char *filename, int linenumber)
{
    struct location newloc;
    newloc.filename = filename;
    newloc.linenumber = linenumber;
    return newloc;
}


/* Print_Table
 * Gets: Key (line) to list to be printed, the list, length of list
 * Returns: Nothing
 * Does: Prints table
 */
void print_Table(const void *key, void **value, void *left) 
{
    printf("%s\n", (char *) key);
    List_T parse = *value;
    /* Parses through table, printing each with padded strings */
    while (parse != NULL) {
        struct location *currentlocation = parse->first;
        printf("%-20s" "%7d\n", currentlocation->filename, currentlocation->linenumber);
        parse = parse->rest;
    }
    /* If not the final match, print newline between next match */
    if (*(int *)left != 1) {
        printf("\n");
    }
    *(int *)left = *(int *)left - 1;
}



/* process_file
 * Gets: a filedata struct
 * Returns: Nothing
 * Does: Checks if a file can be opened. If not, program exits with exit_failure
 */
void process_File(struct filedata file)
{
    FILE *Ffile = fopen(file.filename, "r");
    // If file can't be opened, exit with EXIT_FAILURE
    if(Ffile == NULL) {
        exit(EXIT_FAILURE);
    }
    fclose(Ffile);
}


/* end_Simlines
 * Gets: database of matches
 * Returns: Nothing
 * Does: Frees memory allocated for table
 */
void end_Simlines(Table_T matches, List_T allfiles)
{
    /* Calls remove_List on every element of the 
        database using Table_map */
    List_map(allfiles, applyFree, NULL);
    List_free(&allfiles);

    Table_map(matches, remove_List, NULL);

    /* Free memory allocated for table */
    List_free(&allfiles);
    Table_free(&matches);
}


/* remove_List
 * Gets: key, value, NULL
 * Returns: Nothing
 * Does: Frees memory allocated for list at key
 */
void remove_List(const void *key,void **value, void *cl)
{
    (void) key;
    (void) cl;

    List_T parse = *value;
    List_T head = parse;
    while (parse != NULL) {
        printf("FREED LOC\n");
        List_T temp = parse->rest;
        struct location *loc = parse->first;
        printf("%s\n", loc->filename);
        free(parse->first);
        parse = temp;            
    }
    printf("Freed Match\n");
    List_free(&head);
}

void applyFree(void **ptr, void *cl) {
    //printf("FREED:\n");
    (void) cl;
    (void) ptr;
    free(*ptr);
}
