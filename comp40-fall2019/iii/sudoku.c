/*
 * HW2
 * Partners: Eli Dow, Daniel Alderman
 * sudoku.c
 * last edited 9/27/2019
 *
 * Implementation for sudoku
 */

#include <uarray2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include <pnmrdr.h>
#include "mem.h"
#include <math.h>
#include <except.h>

/*
* Returns a UArray2 struct filled with all of the information from the
* Pnmrdr_T data from the pgm file
*/
static void* populate(Pnmrdr_T);

/*
* Tests if the sudoku board stored in the UArray2 is solved
*/
static bool tester(A board);

/*
* Tests that no value in the UArray is larger than 9
*/
static void max_intensity(int col, int row, A board, void *element, void *OK);

/*
* Tests that no value in the UArray is smaller than 1
*/
static void min_intensity(int col, int row, A board, void *element, void *OK);

/*
* Tests that no value is repeated in the same row/col in the sudoku board
*/
static void sim_group(int col, int row, A board, void *element, void *curr);

/*
* Frees all memory that was malloc’d for use in sudoku.c
*/
static void sudoku_free(A board, FILE* fp, Pnmrdr_T image);

/*
* Returns a board where each row is a 3by3 chunk from the board passed in
*/
static A sim_3by3(A board);

int main(int argc, char *argv[])
{
    assert(argv != NULL);
    FILE *fp = NULL;
    Pnmrdr_T image = NULL;
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        assert(fp != NULL);
        image = Pnmrdr_new(fp);
    }
    else if (argc == 1) {
        image = Pnmrdr_new(stdin);
    }
    else {
        return EXIT_FAILURE;
    }

    Pnmrdr_mapdata map = Pnmrdr_data(image);
    assert(map.type == Pnmrdr_gray);
    assert(map.width == 9);
    assert(map.height == 9);

    A board = populate(image);
    bool good = tester(board);
    sudoku_free(board, fp, image);
    
    if (!good) {
        return EXIT_FAILURE;
    }
    else {
         return EXIT_SUCCESS;
    }
}

/*
* Takes in an image that is the file that holds the sudoku board.
* Adds each value in the image to the respective slot in the UArray2
*/
static void* populate(Pnmrdr_T image)
{
    assert(image != NULL);
    int curr_val;
    int total_pix = 0;
    A board = UArray2_new(9,9,sizeof(curr_val));
    Pnmrdr_mapdata map = Pnmrdr_data(image);

    for(unsigned j = 0; (j < map.height && total_pix < 82); j++) {
        for(unsigned i = 0; (i < map.width && total_pix < 82); i++) {
            curr_val = Pnmrdr_get(image);
            total_pix++;
            int* slot = (int*)UArray2_at(board, i, j);
            *slot = curr_val;
        }
    }
    return board;
}

/*
* Tests to see if the UArray2 is a solved sudoku puzzle. Tester calls other 
* functions to test individual aspects of a correct sudoku board (like if 
* no value is greater than 9)
*/
static bool tester(A board)
{
    assert(board != NULL);
    bool OK = true;
    int *curr_group = NULL; 
    UArray2_map_row_major(board, max_intensity, &OK);
    if (!OK) {
        return OK;
    }
    UArray2_map_row_major(board, min_intensity, &OK);
    if (!OK) {
        return OK;
    }
    curr_group = malloc(11 * sizeof(int));
    assert(curr_group != NULL);
    for (int i = 0; i < 11; i++) {
        curr_group[i] = 0;
    }   
    UArray2_map_row_major(board, sim_group, curr_group); 
    if (curr_group[10] != 0) {
        OK = false;
    }
    for (int i = 0; i < 11; i++) {
        curr_group[i] = 0;
    }
    UArray2_map_col_major(board, sim_group, curr_group);
    if (curr_group[10] != 0) {
        OK = false;
    }
    for (int i = 0; i < 11; i++) {
        curr_group[i] = 0;
    }
    A three_board = sim_3by3(board); 
    /*creates a new board where each row is a 3by3 chunk*/

    UArray2_map_col_major(three_board, sim_group, curr_group);
    if (curr_group[10] != 0) {
        OK = false;
    }

    free(curr_group);
    UArray2_free(&three_board);

    return OK;
}

/*
* Tests board to see that no element is over the value of 9, and updates OK
*/
static void max_intensity(int col, int row, A board, void *element, void *OK)
{
    assert(board != NULL);
    assert(element != NULL);
    assert(OK != NULL);

    int *element_num = (int *) element;
    element_num = UArray2_at(board, col, row);
    if (*element_num > 9) {
        *((bool *)OK) = false;
    }
}

/*
* Tests board to see that no element is under the value of 1, and updates OK
*/
static void min_intensity(int col, int row, A board, void *element, void *OK)
{
    assert(board != NULL);
    assert(element != NULL);
    assert(OK != NULL);

    int *element_num = (int *) element;
    element_num = UArray2_at(board, col, row);
    if (*element_num < 1) {
        *((bool *)OK) = false;
    } 
}

/*
* Tests board to see that no element is repeated in the row/col, updates OK
*/
static void sim_group(int col, int row, A board, void *element, void *curr)
{
    assert(board != NULL);
    assert(element != NULL);
    assert(curr != NULL);

    int *curr_group = (int *) curr;
    curr_group[9]++;
    if (curr_group[9] > 9) {
        for (int i = 0; i < 10; i++) {
            curr_group[i] = 0;
        }       
        curr_group[9]++;
    }
    int *element_num = (int *) element;
    element_num = UArray2_at(board, col, row);
    curr_group[*element_num-1]++;
    
    if (curr_group[*element_num-1] != 1) {
        curr_group[10] = 1;
    }

}
/*
* Creates and returns a new board where each row is a 3by3 chunk from
* the array passed in.  
*/
static A sim_3by3(A board)
{
    assert(board != NULL);
    int col = 0;
    int row = 0;
    A new_board = UArray2_new(9, 9, sizeof(col));
        
    for (int g = 0; g < 9; g+=3) {
        for (int h = 0; h < 9; h+=3) {
        int newi = g;
        int newj = h;
        int limi = g + 3;
        int limj = h + 3;
        for (int i = newi; i < limi; i++) {
            for (int j = newj; j < limj; j++) {
                int* slot = (int*)UArray2_at(board, j, i);
                int* newslot = (int*)UArray2_at(new_board, col, row);
                *newslot = *slot;
                col++;
            }
        }
        row++;
        col = 0;
        }
      }    

    return new_board;
}

/*
* Frees all data malloc’d and closes files that are opened throughout sudoku
*/
static void sudoku_free(A board, FILE* fp, Pnmrdr_T image)
{
    assert(board != NULL);
    assert(fp != NULL);
    assert(image != NULL);
    UArray2_free(&board);

    if (fp != NULL) {
        fclose(fp);
    }
    if (image != NULL) {
        Pnmrdr_free(&image);
    }
}
