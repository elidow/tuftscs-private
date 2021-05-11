/*
 *  structs.h
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This file contains all of our stucts
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "bitpack.h"
#include "assert.h"
#include "range.h"

/* 
 * struct Closure
 * contains an A2Methods_T and an A2Methods_UArray2 pointer
 */
typedef struct 
{
    A2Methods_T methods;
    A2Methods_UArray2 array;
    float denom;
} Closure;

typedef struct 
{
    A2Methods_T methods;
    A2Methods_UArray2 *array;
} Input_Image;


/* 
 * struct Colorspace_pixel
 * contains floating points for component video color spaces Y, Pb, and Pr
 */
typedef struct
{
    float Y;
    float Pb;
    float Pr;
} *Colorspace_pixel;


typedef struct 
{
    float Pb;
    float Pr;
    float a;
    float b;
    float c;
    float d;
} *Codeword;



