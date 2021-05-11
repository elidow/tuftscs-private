/*
 *  decompression.c
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This file contains our implementation for our decompression functions
 * 
 */


#include "decompression.h"
#include "structs.h"
#include "arith40.h"
#include "bitpack.h"
#include "colorspace.h"
#include "pack.h"


#define BLOCK 2
#define P_WIDTH 4
#define INT 5
#define UINT 9
#define DENOM 255


/*
 * get_words
 * 
 * Parameters:
 * Returns:
 * Purpose: An apply function to read in words from file passed in
 * CRE:
 */
void get_words(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    uint32_t *word = (uint32_t *)elem;

    *word = 0;
    FILE *input = (FILE *)cl;
    /* can we make this more modular? */

    for (int i = 24; i >= 0; i -= 8) {
        int c = getc(input);
        if (c == EOF) {
            assert(0); /* could also do assert(c != EOF) */
        } 
        *word = Bitpack_newu(*word, 8, i, (uint32_t)c);
    }
    
}

/*
 * print_image
 *
 * Parameters: Pnm_ppm 
 * Returns: Nothing (void)     
 * Purpose: Reads in a Pnm_ppm type and prints out the image held within
 * the pixmap to stdout. Will use Pnm_ppmwrite()
 * CRE: Pnm_ppm cannot be NULL  
 */
static void print_image(Pnm_ppm image)
{
    assert(image != NULL);
    Pnm_ppmwrite(stdout, image);
}



/*
 * decompress_image()
 * 
 * Parameters: 2D array of the compressed image
 * Returns: void
 * Purpose: will call the rest of the implementaion for the decompression
 *          functionality
 * CRE: if the array and methods are non-existent
 */
void decompress_image(A2Methods_UArray2 array, A2Methods_T methods)
{
    int width = (methods->width(array)) * BLOCK;
    int height = (methods->height(array)) * BLOCK;
    Pnm_rgb temp;
    A2Methods_UArray2 pixels = methods->new(width, height, sizeof(*temp));

    struct Pnm_ppm pixmap = 
    {   
        .width = width, 
        .height = height, 
        .denominator = DENOM, 
        .pixels = pixels, 
        .methods = methods
    };
 
    /* array now holds packed words
        we want to call a helper function to call a mapping funciton to 
        unpack words and convert into colorspace pixels */

    

    Colorspace_pixel temp2;
    A2Methods_UArray2 colorspace = methods->new(width, height, sizeof(*temp2));
    find_colorspace(array, methods, colorspace);
    Closure *cl = malloc(sizeof(*cl));
    cl->array = colorspace;
    cl->methods = methods;

    cl->array = pixmap.pixels;

    /* colorspace pixels to rgb */
    methods->map_default(colorspace, get_rgb, cl);

    print_image(&pixmap);

    methods->free(&colorspace);
    methods->free(&pixels);
    free(cl);
}