/*
 *  colorspace.h
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 *  edow01 and echo02
 * 
 *  This file contains our interface for our functions dealing with
 *  conversions between our Colorspace_pixel struct and Pnm_rgb structs. 
 * 
 */

#include "structs.h"

/************************* FUNCTIONS FOR COMPRESSING *************************/

/* 
 * apply function to iterate through array of Pnm_ppm elements and find and 
 * store their corresponding Colorspace_pixel types in a new array.                           
 */
void get_colorspace(int i, int j, A2Methods_UArray2 array, void *elem, 
                        void *cl);


/*****************************************************************************/

/************************ FUNCTIONS FOR DECOMPRESSING ************************/

/* 
 * apply function to iterate through array of Colorspace_pixel types and
 * access their corresponding Pnm_ppm types in a new array
 */
void get_rgb(int i, int j, A2Methods_UArray2 array, void *elem, 
                    void *cl);

/*****************************************************************************/