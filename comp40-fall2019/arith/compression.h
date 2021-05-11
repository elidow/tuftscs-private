/*
 *  compression.h
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This file contains the interface for all of the compression functions
 * 
 */
#include "pnm.h"
#include "a2methods.h"

/* 
 * takes in an image from a file and stores it in a Pnm_ppm
 * will trim image if necessary and checks for valid file types
 */
void read_image(A2Methods_T methods, FILE *input, Pnm_ppm *image);

/*
 * takes in a Pnm_ppm image and an A2Methods_T in order to compress the image
 * into an array of 32 bit words and passes it to the print function
 */
void compress_image(Pnm_ppm image, A2Methods_T methods);