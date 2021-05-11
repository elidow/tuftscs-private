/*
 *  compression.c
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This file contains our implementation for our compression functions
 * 
 */

#include "compression.h"
#include "structs.h"
#include "arith40.h"
#include "bitpack.h"
#include "range.h"

/* temp inclusion */
#include "decompression.h"

#define BLOCK 2
#define P_WIDTH 4
#define INT 5
#define UINT 9

/*
 * fill_new()
 * 
 * Parameters: indices of the element being accessed, 
 * Returns: void
 * Purpose: this is an apply function to fill the new array with the correct 
 *          pixels fromt the initial array
 * CRE: indices are less than 0, elem and cl are NULL
 */
static void fill_new(int i, int j, A2Methods_UArray2 array, void *elem, 
                        void *cl)
{
    assert(i >= 0 && j >= 0);
    assert(array != NULL);
    assert(elem != NULL && cl != NULL);

    A2Methods_T methods = ((Input_Image *)cl)->methods;
    A2Methods_UArray2 *array2 = ((Input_Image *)cl)->array;

    if (i < methods->width(*array2) && j < methods->height(*array2)) {

        Pnm_rgb color = methods->at(*array2, i, j);
        *color = *(Pnm_rgb) elem;

    }
}

/*
 * RGB_to_Colorspace()
 * 
 * Parameters: Pnm_rgb pixel pointer
 * Returns: Colorspace_pixel pointer
 * Purpose: Takes the rgb elements from the pixel and uses equations outlined
 *          in spec to calculate the colorspace counterparts. 
 * CRE: Pnm_rgb pixel cannot be NULL
 */
static Colorspace_pixel RGB_to_Colorspace(Pnm_rgb pixel, float denom)
{
  
    assert (pixel != NULL);

    Colorspace_pixel cp = malloc(sizeof(*cp));

    (void) denom;

    // fprintf(stderr, "r is %d\n", pixel->red);
    // fprintf(stderr, "g is %d\n", pixel->green);
    // fprintf(stderr, "b is %d\n", pixel->blue);
    float r = (float)(pixel->red) / 255.0;
    float g = (float)(pixel->green) / 255.0;
    float b = (float)(pixel->blue) / 255.0;
    // fprintf(stderr, "r is %f\n", r);
    // fprintf(stderr, "g is %f\n", g);
    // fprintf(stderr, "b is %f\n", b);

    cp->Y = 0.299 * r + 0.587 * g + 0.114 * b;
    cp->Pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
    cp->Pr = 0.5 * r - 0.418688 * g - 0.081312 * b;
    // fprintf(stderr, "Y is %f\n", cp->Y);
    // fprintf(stderr, "Pb is %f\n", cp->Pb);
    // fprintf(stderr, "Pr is %f\n", cp->Pr);
    return cp;
}



/*
 * read_image()
 * 
 * Parameters: A2Methods_T methods to determine how to read in image, FILE * to
 *              read the image from, Pnm_ppm *image to modify with correct new
 *              image.
 * Returns: void, but passes back Pnm_ppm image pointer that is modified
 * Purpose: Will fill the Pnm_ppm *image with the correctly formatted data. 
 * CRE: methods, input or image are NULL
 */
void read_image(A2Methods_T methods, FILE *input, Pnm_ppm *image)
{
    assert(methods != NULL && input != NULL && image != NULL);

    *image = Pnm_ppmread(input, methods);
    assert(image != NULL);

    int width = (*image)->width;
    int height = (*image)->height;
    assert(width > 0 && height > 0);
    int size = methods->size((*image)->pixels);
    fprintf(stderr, "width is %d and height is %d\n", width, height);
    if (width % 2 != 0 || height % 2 != 0){
        fprintf(stderr, "image needs to be trimmed\n");
        width = width - ((*image)->width % 2);
        height = height - ((*image)->height % 2);
        assert(width > 0 && height > 0);

        A2Methods_UArray2 array = methods->new(width, height, size);
        
        Input_Image *value = malloc(sizeof(*value));
        value->array = &array;
        value->methods = methods;

        methods->map_default((*image)->pixels, fill_new, value);

        assert(array != NULL);

        methods->free(&((*image)->pixels));
        (*image)->pixels = array;

        (*image)->width = width;
        (*image)->height = height;
        fprintf(stderr, "new width is %d and height is %d\n", width, height);

        free(value);
    }
}


/*
 * get_colorspace()
 * 
 * Parameters: integers i and j, A2Methods_Uarray2, void* elem and cl
 * Returns: Void
 * Purpose: Apply function that will iterate through each pixel in the array and
 *          store it in a new UArray2 as a Colorspace_pixel type with the 
 *          necessary conversions
 * CRE: When the indexing is out of bounds, any of the other parameters are NULL
 */
static void get_colorspace(int i, int j, A2Methods_UArray2 array, void *elem, 
                    void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    A2Methods_UArray2 array2 = ((Closure *)cl)->array;
    A2Methods_T methods = ((Closure *)cl)->methods;
    
    /* gives us a uarray2 with the colorspace pixels */
    Colorspace_pixel pixel = methods->at(array2, i, j);
    
    Colorspace_pixel holder = RGB_to_Colorspace(elem, ((Closure *)cl)->denom);
    *pixel = *holder;

    free(holder);

    
}


/* 
 * Pixelspace_to_DCT
 * 
 * Parameters: 4 Colorspace_pixel structs with data from one 2 by 2 block
 * Returns: Pointer to codeword struct
 * Purpose: Take in 4 colorspace_pixels, retrieve data and convert to data for
 *          codeword struct
 * CRE: if Colorspace_Pixels are NULL
 */
static Codeword Pixelspace_to_DCT(Colorspace_pixel one, Colorspace_pixel two, 
                                  Colorspace_pixel three, Colorspace_pixel four)
{
    assert (one != NULL && two != NULL && three != NULL && four != NULL);

    Codeword word = malloc(sizeof(*word));
    /* casting to float? */
    word->a = (four->Y + three->Y + two->Y + one->Y) / 4.0;
    word->b = (four->Y + three->Y - two->Y - one->Y) / 4.0;
    word->c = (four->Y - three->Y + two->Y - one->Y) / 4.0;
    word->d = (four->Y - three->Y - two->Y + one->Y) / 4.0;
  
    word->Pb = (four->Pb + three->Pb + two->Pb + one->Pb) / 4.0;
    word->Pr = (four->Pr + three->Pr + two->Pr + one->Pr) / 4.0;


    return word;

}



/* 
 * pack
 * 
 * Parameters: unsigned 32 bit integer that represents the word, Codeword struct
 * Returns: Nothing
 * Purpose: Packs the word from the data in the Codeword struct to the uint32_t*
 * CRE: If word or holder are NULL
 */
 static void pack(uint32_t *word, Codeword holder)
 {
    assert(word != NULL);
    assert(holder != NULL);
    unsigned Pb = compress_p(holder->Pb);
    unsigned Pr = compress_p(holder->Pr);

    *word = Bitpack_newu(*word, P_WIDTH, 0, Pr);
    *word = Bitpack_newu(*word, P_WIDTH, P_WIDTH, Pb);
    *word = Bitpack_news(*word, INT, 8, constrict(holder->d));
    *word = Bitpack_news(*word, INT, 13, constrict(holder->c));
    *word = Bitpack_news(*word, INT, 18, constrict(holder->b));
    *word = Bitpack_newu(*word, UINT, 23, (int)((holder->a) * 511));
 }


/*
 * DCT
 * 
 * Parameters: integers i and j, A2Methods_Uarray2, void* elem and cl
 * Returns: void
 * Purpose: an apply function to go and take each 2 by 2 block of colorspace 
 *          pixels and convert each block into a, b, c, d, which will be held
 *          in a DCT_Space struct. Will calcualte average Pb and Pr
 *          Will then pack into array of words.
 * CRE: i and j are out of bounds, the other parameters are NULL
 */
static void DCT(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    A2Methods_UArray2 array2 = ((Closure *)cl)->array;
    A2Methods_T methods = ((Closure *)cl)->methods;

    /* check to see if it is beginning of block */
    if((i % 2 == 0) && (j % 2 == 0)) {
        uint32_t *word = methods->at(array2, i / BLOCK, j / BLOCK);
        Codeword holder = Pixelspace_to_DCT(elem, methods->at(array, i + 1, j),
                                            methods->at(array, i, j + 1),
                                            methods->at(array, i + 1, j + 1));

        
        // uint32_t *word = methods->at(array2, i / 2, j / 2);
        pack(word, holder);
        // *word = *holder;
        free(holder);
    }


}

/*static void print_words(int i, int j, A2Methods_UArray2 array, void *elem, 
                        void *cl)
{
    assert(elem != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);
    (void) cl;
    putchar(Bitpack_getu(*(uint64_t *)elem, 8, 0));
    putchar(Bitpack_getu(*(uint64_t *)elem, 8, 8));
    putchar(Bitpack_getu(*(uint64_t *)elem, 8, 16));
    putchar(Bitpack_getu(*(uint64_t *)elem, 8, 24));
}*/


/*
 * compress_image()
 * 
 * Parameters: Pnm_ppm image
 * Returns: void
 * Purpose: will call the rest of the implementaion for the compression
 *          functionality
 * CRE: if Pnm_ppm image and methods non-existent
 */
void compress_image(Pnm_ppm image, A2Methods_T methods)
{
    assert(image != NULL);
    assert(methods != NULL);
    int width = image->width;
    int height = image->height;
    Colorspace_pixel temp;
    int size = sizeof(*temp);

    // A2Methods_T methods2 = uarray2_methods_blocked;
    
    /* 2D array with colorspace pixels */
    A2Methods_UArray2 array = methods->new(width, height, size);

    
    Closure *cl = malloc(sizeof(*cl));
    cl->array = array;
    cl->methods = methods;
    cl->denom = (float)(image->denominator);

    methods->map_default(image->pixels, get_colorspace, cl);

    uint32_t temp2;

    // size = sizeof(temp2);
    // Codeword temp2;
    size = sizeof(temp2);

    /* 2D array of words */
    A2Methods_UArray2 words = methods->new(width / BLOCK, height / BLOCK, 
                                            size);
    
    cl->array = words;
    
    methods->map_default(array, DCT, cl);

    // printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
    // methods->map_default(words, print_words, NULL);
    
    decompress_image(words, methods);

    // decompress_image(array, methods);

    free(cl);
    methods->free(&words);
    methods->free(&array);

    // return array;
}

