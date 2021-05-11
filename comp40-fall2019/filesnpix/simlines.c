/*
 * HW1
 * Partners: Eli Dow, Daniel Alderman
 * simlines.c
 *
 * This purpose of this program is to read any number of files
 * and detect lines in the files that are similar.
 */

#include "readaline.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <atom.h>
#include "table.h"
#include "seq.h"
#include <stdbool.h>

// calls readaline to retrieve data and passes it to helper function
static void pass_info(char** files, int filenum, Table_T**);

// helper function for pass_info, makes table out of sequences
static void table_maker(char *line, int file_name, int line_num, Table_T**);

// cleans all lines by getting rid of non-aplhanumeric characters(not '_')
static char** clean_line(char** line, int total);

//prints similar lines output with key and value
static void apply_print(const void *key, void **value, void *cl);

//frees memory of key and value
static void apply_delete(const void *key, void **value, void *cl);

/*
 * struct line_T
 * This struct stores information about a line from a file, which
 * includes the line as a char*, the file_name as a number, and 
 * the line number
 */
struct line_T
{
    char* line;
    int file_name;
    int line_number;
};

/*
 * Function: main
 * Parameters: int argc, char *argv[]
 * Does: Reads file names and calls data structure functions to
 * create a list of similar lines in all files.
 */
int main(int argc, char *argv[])
{

    char *file_names[argc-1];

    for(int i = 1; i < argc; i++)
        file_names[i-1] = argv[i];

    Table_T hash = Table_new(1000, NULL, NULL);
    Table_T* hash_pointer = &hash;

    pass_info(file_names, argc - 1, &hash_pointer);

    Table_map(*hash_pointer, apply_print, &file_names);
    Table_map(*hash_pointer, apply_delete, file_names);
    assert(hash != NULL);
    assert(hash_pointer != NULL);
    Table_free(hash_pointer);

    return EXIT_SUCCESS;
}

/*
 * Function: pass_info
 * Parameters: char** file_names, which stores a pointer to a pointer of 
 * chars (an array of strings) that holds all filenames. int filenum, which is
 * an int which stores the number of files. Table_T** hash, which is a 
 * pointer to a pointer of the hanson table where everything will be added.
 * Does: This function is responsible for reading a file num & passing its info
 * to table_maker to store in the hash table. We loop through the files while 
 * making sure the files exist (stderr if it doesnt). We call readaline in order
 * to store the line and clean it. Then, we pass that info including the table 
 * to table maker.
 * It returns nothing
 */
static void pass_info(char ** file_names, int filenum, Table_T** hash)
{
    char* data = NULL;
    char** datapp = &data;

    for (int i = 0; i < filenum; i++)
    {
        int linenum = 0;
        FILE *fp = fopen(file_names[i], "r");
        if(fp == NULL)
            fprintf(stderr, "File: %s could not be opened\n", file_names[i]);
        else
        {
            while(true)
            {
                linenum++;
                size_t total = readaline(fp, datapp);
                
                if(total == 0)
                    break;

                char* c_line = NULL;
                char** curr_line = &c_line;
                curr_line = clean_line(datapp, total);
                assert(curr_line != NULL);

                table_maker(*curr_line, i, linenum, hash);
            }
        }
        if(fp != NULL)
            fclose(fp);
    }
    
    if(datapp != NULL)
    {
        if(*datapp != NULL)
            free(*datapp);
    }
}

/*
 * Function: table_maker
 * Parameters: char* line, which is the string of the line to be added to the 
 * table. int file_name, which is an integer which corresponds to the file name
 * in file_names array to be added to the table. Int line_num, which is the 
 * line number of the line to be added to the table. Table_T** hash, which is a 
 * pointer to a pointer of the hanson table where everything will be added. 
 * Does: Creates and mallocs a new line_T struct and populates it values with 
 * the info passed in. The line is then made into an atom which is then used as
 * a key to be added to the correct sequence key, and the table is a table of 
 * sequences so the atom is added to the sequence that is stored in the table.
 * Returns nothing
 */
static void table_maker(char *line, int file_name, int line_num, Table_T** hash)
{
    struct line_T *this_line = malloc(sizeof(struct line_T));

    this_line->line = line;
    this_line->file_name = file_name;
    this_line->line_number = line_num;

    const char *string_pointer = this_line->line;
    const char *atom_string = Atom_string(string_pointer);

    Seq_T curr_seq = Table_get(**hash, atom_string);

    if(curr_seq == NULL)
    {
        Seq_T newseq = Seq_new(1);
        Seq_addhi(newseq, this_line);
        Table_put(**hash, atom_string, newseq);
    }
    else
        curr_seq = Seq_addhi(curr_seq, this_line);
}

/*
 * Function: clean_line
 * Parameters: char** line, which is the line taken directly from datapp which 
 * needs to be "cleaned" into spaces, underscores, and alphanumeric characters.
 * Int size, which is the size of the char** line passed in, taken directly 
 * from readaline return value.
 * Does: Strips all non-alphanumeric values that aren’t _ and adds spaces where
 * appropriate. It also adds a null terminator to the end of each char* so it is
 * a string that is ready to print. 
 * It returns a pointer to the fully cleaned and ready to go char* or string.
 */
static char** clean_line(char** line, int size)
{
    char* dirty = *line;
    char* cleanboi = NULL;
    cleanboi = malloc(size);
    assert(cleanboi != NULL);
    bool end_space = false;
    int index = 0;
    char prev = '-';
    for(int i = 0; i < size; i++)
    {
        if(dirty[i] == '\n')
            break;
        else if(!((isalnum(dirty[i])) || (dirty[i] == '_')))
        {
            if((isalnum(prev)) || (prev == '_'))
            {
                cleanboi[index] = ' ';
                index++;
            }
        }
        else if((isalnum(dirty[i])) || (dirty[i] == '_'))
        {
            cleanboi[index] = dirty[i];
            index++;
        }
        prev = dirty[i];
    }
    if((index > 1) && cleanboi[index-1] == ' '){
        cleanboi[index-1] = '\0';
        end_space = true;
    }
    free(*line);
    if(!end_space)
    {
        char* temp = realloc(cleanboi, index+1);
        temp[index] = '\0';
        *line = temp;
    }
    else 
        *line = cleanboi;

    return line;
}

/*
 * Function: apply_print
 * Parameters: void* key, which is the key of the sequence stored in the table,
 * so it is the char* that holds the line. void** value, which is the sequence 
 * which is stored in the table at this key. void*cl, which stores a pointer to 
 * a pointer of chars (an array of strings) that holds all the filenames.
 * Does: Checks the size of the sequence, and if it is greater than 1 it prints
 * out the *key which is the similar line and then all of the filenames and 
 * line numbers for each line_T stored in the sequence. When we call it through
 * table_map it goes through and prints out all of the match groups.
 * returns nothing
 */
static void apply_print(const void *key, void **value, void *cl)
{
    Seq_T seq = *value;
    int length = Seq_length(seq);
    char** file_names = (char** ) cl;
    char* matchword = (char*) key;

    if(length > 1)
    {
        if(strlen(matchword) < 1)
            return;
        //(void) matchword;
        printf("\n%s\n", (char*) key);

        for(int i = 0; i < length; i++)
        {
            struct line_T *curr = (struct line_T *) Seq_get(seq, i);
            char *file_name = file_names[curr->file_name];
            printf("%-20s ", file_name);
            printf("%7u\n", curr->line_number);
        }
    }
}

/*
 * Function: apply_delete
 * Parameters: void* key, which is the key of the sequence stored in the table,
 * so it is the char* that holds the line. void** value, which is the sequence 
 * which is stored in the table at this key. void*cl stores nothing and we void
 * it, but we must include it for table_map.
 * Does: Deletes every line_T stored in the sequence and then deletes the 
 * sequence itself. When we call it through the table_map it successfully 
 * deallocates all data stored in the table. 
 */
static void apply_delete(const void *key, void **value, void *cl)
{
    Seq_T seq = *value;
    int length = Seq_length(seq);
    (void) cl;
    (void) key;
    if(length > 0)
    {
        assert(seq != NULL);
        for(int i = 0; i < length; i++)
        {
            struct line_T *curr = Seq_get(seq, i);
            free(curr);
        }
        Seq_free(&seq);
    }
}

