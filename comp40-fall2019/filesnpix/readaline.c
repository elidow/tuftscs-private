/*
 * HW1
 * Partners: Eli Dow, Daniel Alderman
 * readaline.c
 *
 * This program contains a single function that 
 * a single line of input from fiile inputfd without
 * using getline or getdelim
 */

#include "readaline.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


//handles edge case in readaline when eof is reached
static bool readaline_end(char**, char*, char*, size_t);

//doubles the size of linesread and returns it. 
static char* expand(char* linesread, int bitesnum, int capacity);

/*
 * Function: readaline
 * Parameters: FILE * (pointer to the file at a specific spot, 
 * char** is a pointer to the string/char* of the line being read
 * 
 * Does: Reads a line from FILE* and returns the 
 * number of bites or the line. It does this by 
 * retrieving the next character (as an int) at the file pointer and
 * casting it to a char while adding the number of bites in bitnum
 * It also calls readaline_end in the case of the file ending. If the line
 * is longer than linesread capacity, it calls expand and doubles the size
 * of linesread. 
 * It returns the number of bites in the line.
 */
size_t readaline(FILE *inputfd, char** datapp)
{
    assert(inputfd != NULL);
    assert(datapp != NULL);
    int capacity = 200;

    if(*datapp != NULL)
        free(*datapp);
    size_t bitesnum = 0;
    char *linesread = malloc(200 * sizeof(char));
    char *curr = malloc(sizeof(char));
    *curr = ' ';
    int temp = 72;

    while((*curr != 10))
    {
        temp = fgetc(inputfd);
        if(temp >= 0 && temp < 256)
        {
            *curr = (char) temp;
            bitesnum++;
            if((int) bitesnum > capacity)
            {
                linesread = expand(linesread, ((int) bitesnum - 1), capacity);
                capacity = capacity * 2;
            }
            linesread[bitesnum-1] = *curr;
        }
        else if(temp == -1)
        {
            bool empty_line = readaline_end(datapp, linesread, curr, bitesnum);
            if(empty_line)
                return 0;
            else
                return bitesnum;
        }
    }   
    *datapp = linesread;
    if(curr != NULL)
        free(curr);

    return bitesnum;
}


/*
 * Function: readaline_end
 * Parameters: char** is a pointer to the string/char* of the line being
 * read in the file. char* linesread, which mallocs a char* of size 200
 * and points to the string/char* of the line being read. char* curr, 
 * which is a pointer to the current character the file points at. size_t 
 * bitesnum, which is the size in bytes of the line being read 
 * Does: Handles the case when it reaches the end of the file. If the line is 
 * empty, then iit frees linesread and curr and returns true meaning its 
 * empty. If not, (bitesnum > 0) then it sets curr to the last spot in 
 * linesread and returns false.
 * The function returns a bool that signifies what case the file is ending
 */
static bool readaline_end(char ** datapp, char * linesread, char * curr,
                          size_t bitesnum)
{
    if(bitesnum == 0)   
    {
        if(linesread != NULL) 
            free(linesread);
        if(curr != NULL)
            free(curr);

        *datapp = NULL;
        datapp = NULL;
        return true;
    }
    if(bitesnum > 0) 
    {
        linesread[bitesnum-1] = *curr;
        *datapp = linesread;
        if(curr != NULL)
            free(curr);
        return false;
    }
    return true;
}


/*
 * Function: expand
 * Parameters: char* linesread is the array of chars to be expanded. int 
 * bitesnum is the number of chars currently in linesread. int capacity is the
 *  current size of linesread, this is half of what the new size will be. 
 * Does: Mallocs a new array that is twice the size of linesread, it then 
 * assigns all the old values from linesread to temp. We then free linesread,
 * and then assign linesread to temp. Finally, we return linesread in its
 * expanded form. 
 */
static char* expand(char* linesread, int bitesnum, int capacity)
{

    char* temp = malloc(2 * capacity * sizeof(char));
    for (int i = 0; i < bitesnum; i++)
    {
        temp[i] = linesread[i];
    }

    free(linesread);

    linesread = temp;

    return linesread;

}

