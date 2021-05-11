/*
 * HW2
 * Partners: Eli Dow, Daniel Alderman
 * bit2.h
 *
 * Interface for bit2
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <bit.h>

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#define B Bit2_T
typedef struct B *B;
/* 
 * Bit2_new takes in a width, height, in order to allocate the 2D Bit2 array to
 * the heap. It returns the new 2D array.
 */
Bit2_T Bit2_new(int width, int height);


/* 
 * Bit2_free takes in a pointer to a 2D Bit2 array and deallocates all
 * of the memory stored in it. It sets it to null, and the function returns
 * nothing.
 */
void Bit2_free(Bit2_T* array);


/* 
 * Bit2_width takes in a pointer to a 2D Bit2 array and 
 * returns the width of it as an integer.
 */
int Bit2_width(Bit2_T array);

/* 
 * Bit2_height takes in a pointer to a 2D Bit2 array and 
 * returns the height of it as an integer.
 */
int Bit2_height(Bit2_T array);

/* 
 * Bit2_index takes in a pointer to a 2D Bit array, its row and column and uses
 * an algebraic equation to return the corect index in the 1D array.
 */
int Bit2_index(Bit2_T array, int col, int row);

/* 
 * Bit2_map_row_major takes in a pointer to a 2D Bit2 array and a function apply
 * that is applied to each row. *cl is for client discretion. 
 * Returns nothing
 */
void Bit2_map_row_major(Bit2_T array, void apply(int i, int j, Bit2_T array,
                                                int bit, void *p1), void*cl);

/* 
 * Bit2_map_col_major takes in a pointer to a 2D Bit2 array and a function apply
 * that is applied to each column. *cl is for client discretion. 
 * Returns nothing
 */
void Bit2_map_col_major(Bit2_T array, void apply(int i, int j, Bit2_T array,
                                                int bit, void *p1), void*cl);


/* 
 * Bit2_get takes in a pointer to a 2D Bit2 array, an int row, and an int col. 
 * It tests to see if the bit stored at thee indices (row,col) exists in the 
 * array. It returns 1 if n is in the arrray, 0 otherwise.
 */
int Bit2_get(Bit2_T array, int col, int row);

/* 
 * Bit2_put takes in a pointer to a 2D Bit2 array, an int row, an int col, and 
 * an int bit. It replaces the bit stored at the (row,col) indices with
 * int bit. It returns the bit that was just replaced. 
 */
int Bit2_put(Bit2_T array, int col, int row, int bit);

#endif
