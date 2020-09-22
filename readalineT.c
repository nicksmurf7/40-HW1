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
