/*
 *  decompression.h
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This file contains our interface for our decompression functions
 * 
 */
#include "pnm.h"
#include "a2methods.h"

void get_words(int i, int j, A2Methods_UArray2, void *elem, void *cl);

void decompress_image(A2Methods_UArray2 array, A2Methods_T methods);

