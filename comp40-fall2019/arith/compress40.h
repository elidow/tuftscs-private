/*
 *  compress40.h
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This is the header file for our compress40 implementation
 * 
 */

#include <stdio.h>

/************************* FUNCTIONS FOR COMPRESSING *************************/

/* compress40 
 * should take an image and compress it using the bitpack 
 * Will print to stdout the 32 bit word format of the image
 */
extern void compress40  (FILE *input);  /* reads PPM, writes compressed image */

/* decompress40
 * Takes in data in the form of 32 bit words, will decompress into an image
 */
extern void decompress40(FILE *input);  /* reads compressed image, writes PPM */

/*****************************************************************************/
