/*
 *  compress40.c
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 *  edow01 and echo02
 * 
 *  This file contains our implementation for the compress40 and decompress40
 *  functions for our arith program
 * 
 */

#include "compress40.h"
#include "compression.h"
#include "decompression.h"
#include "structs.h"
#include "assert.h"

#define BLOCK 2

/************************* FUNCTIONS FOR COMPRESSING *************************/

/*
 * compress40()
 * 
 * Parameters: Pointer to the FILE element of an image
 * Returns: Nothing (void)     
 * Purpose: Takes in a pointer to a file element of an image and
 *          and compresses the data of a ppm image to a binary image
 * CRE: FILE *input cannot be NULL        
 */
void compress40(FILE *input)
{
    assert(input != NULL);
    Pnm_ppm image;
    
    /* default A2Methods, which would be row major */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods != NULL);

    read_image(methods, input, &image);
    assert(image != NULL);
    compress_image(image, methods);


    Pnm_ppmfree(&image);
 
}

/*****************************************************************************/

/************************ FUNCTIONS FOR DECOMPRESSING ************************/

/*
 * decompress40()
 * 
 * Parameters:  Pointer to the FIlE element to read the image from
 * Returns:     void
 * Purpose:     Will take in a bit-packed file ordered in Big-Endian and will
 *              decompress the data from that file into a ppm image
 * CRE:         FILE *input cannot be NULL
 */
void decompress40(FILE *input)
{
    assert(input != NULL);

    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, 
                                                                    &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    /* read in data big_endian */

    A2Methods_T methods = uarray2_methods_plain;

    uint32_t temp;
    int size = sizeof(temp);

    A2Methods_UArray2 words = methods->new(width / BLOCK, height / BLOCK, size);

    /* now need to fill the array */
    methods->map_default(words, get_words, input);
    

    decompress_image(words, methods);

    methods->free(&words);
}

/*****************************************************************************/