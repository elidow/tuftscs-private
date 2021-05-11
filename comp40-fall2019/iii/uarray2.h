/*
 * HW2
 * Partners: Eli Dow, Daniel Alderman
 * uarray2.h
 * last edited 9/27/2019
 *
 * Interface for uarray2
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <uarray.h>

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define A UArray2_T
typedef struct A *A;



/* 
 * UArray2_new takes in a width, height, and a size in bites for each element
 * in order to allocate the 2D UArray to the heap.
 * It returns the new 2D array.
 */
A UArray2_new(int width, int height, int size);


/* 
 * UArray2_free takes in a pointer to a 2D UArray and deallocates all
 * of the memory stored in it. It sets it to null, and the function returns
 * nothing.
 */
void UArray2_free(A *array);


/* 
 * UArray2_width takes in a pointer to a 2D UArray and 
 * returns the width of it as an integer.
 */
int UArray2_width(A array);

/* 
 * UArray2_height takes in a pointer to a 2D UArray and 
 * returns the height of it as an integer.
 */
int UArray2_height(A array);

/* 
 * UArray2_size takes in a pointer to a 2D UArray and 
 * returns the size in bites of it as an integer.
 */
int UArray2_size(A array);

/* 
 * UArray2_at takes in a pointer to a 2D UArray and a row and column of an 
 * index and returns a pointer to the element stored there.
 */
void* UArray2_at(A array, int col, int row);

/* 
 * UArray2_index takes in a pointer to a 2D UArray, its row and column and uses
 * an algebraic equation to return the correct index in the 1D array.
 */
int UArray2_index(A array, int col, int row);

/* 
 * UArray2_map_row_major takes in a pointer to a 2D UArray and a function apply
 * that is applied to each row. *cl is for client discretion. 
 * Returns nothing
 */
void UArray2_map_row_major(A array, void apply(int i, int j, UArray2_T a,
                                        void *p1, void *p2), void*cl);

/* 
 * UArray2_map_col_major takes in a pointer to a 2D UArray and a function apply
 * that is applied to each column. *cl is for client discretion. 
 * Returns nothing
 */
void UArray2_map_col_major(A array, void apply(int i, int j, UArray2_T a,
                                        void *p1, void *p2), void*cl);

#endif

