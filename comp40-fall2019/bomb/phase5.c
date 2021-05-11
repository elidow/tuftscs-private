#include <stdlib.h>
#include <stdio.h>

void phase_5(char *input);
void explode_bomb();
int string_length(char *input);
int strings_not_equal(char *inputOne, char *inputTwo);


int main()
{
    char input[] = {'b', 'e', 'c', 'd', 'k', 'a'};
    phase_5(input);
    fprintf(stderr, "done\n");
    return(1);
}

/*
 * phase_5
 * Parameters: char* or string for our input
 * Returns: Nothing
 * Does: Maps each character of a six character string to one of the elements
 *       in an array of integers. Compares the new string to "ravens" and 
 *       explodes the bomb if they are not equal
 */
void phase_5(char *input)
{
    int mapArray[] = {105, 115, 114, 118, 101, 97, 119, 104, 111, 98, 112,
                        110, 117, 116, 102, 103};

    if (string_length(input) != 6) {
        explode_bomb();
    }

    /* Maps oinginal string to new values from mapArray */
    for (int i = 0; i < 6; i++) {
        char curr = input[i];
        int map_index = (int)curr % 16;
        input[i] = (char)mapArray[map_index];
    }
    
    /* Checks if input is equal to ravens */
    if (strings_not_equal(input, "ravens")) {
        explode_bomb();
    }
}

void explode_bomb()
{
    fprintf(stderr, "rip, your bomb exploded\n");
}

int string_length(char *input)
{
    (void)input;
    return 6;
}

int strings_not_equal(char *inputOne, char *inputTwo)
{
    (void)inputOne;
    (void)inputTwo;
    return 0;
}
