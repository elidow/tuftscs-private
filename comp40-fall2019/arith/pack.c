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

#include "structs.h"
#include "arith40.h"
#include "bitpack.h"

#define BLOCK 2
#define P_WIDTH 4
#define INT 5
#define UINT 9
#define DENOM 255


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
    word->a = (float)(four->Y + three->Y + two->Y + one->Y) / 4.0;
    word->b = (float)(four->Y + three->Y - two->Y - one->Y) / 4.0;
    word->c = (float)(four->Y - three->Y + two->Y - one->Y) / 4.0;
    word->d = (float)(four->Y - three->Y - two->Y + one->Y) / 4.0;
  
    word->Pb = (four->Pb + three->Pb + two->Pb + one->Pb) / 4.0;
    word->Pr = (four->Pr + three->Pr + two->Pr + one->Pr) / 4.0;
    /* memory loss */

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
    assert(holder->Pb >= -0.5 && holder->Pb <= 0.5);
    assert(holder->Pr >= -0.5 && holder->Pr <= 0.5);


    unsigned Pb = compress_p(holder->Pb);
    unsigned Pr = compress_p(holder->Pr);

    uint64_t b = constrict(holder->b);
    uint64_t c = constrict(holder->c);
    uint64_t d = constrict(holder->d);


    *word = Bitpack_newu(*word, P_WIDTH, 0, Pr);
    *word = Bitpack_newu(*word, P_WIDTH, P_WIDTH, Pb);
    *word = Bitpack_news(*word, INT, 8, d);
    *word = Bitpack_news(*word, INT, 13, c);
    *word = Bitpack_news(*word, INT, 18, b);
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
void DCT(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
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

        
        pack(word, holder);
        free(holder);
    }


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
static void to_colorspace(int i, int j, A2Methods_UArray2 array, void *elem, 
                    void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    A2Methods_UArray2 colorspace = ((Closure *)cl)->array;
    A2Methods_T methods = ((Closure *)cl)->methods;

    Codeword word = (Codeword) elem;

    Colorspace_pixel one = methods->at(colorspace, i * BLOCK, j * BLOCK);
    Colorspace_pixel two = methods->at(colorspace, (i * BLOCK) + 1, j * BLOCK);
    Colorspace_pixel three = methods->at(colorspace, i * BLOCK, 
                                        (j * BLOCK) + 1);
    Colorspace_pixel four = methods->at(colorspace, (i * BLOCK) + 1,
                                        (j * BLOCK) + 1);
    /* casting to float? */
    one->Y = word->a - word->b - word->c + word->d;
    two->Y  = word->a - word->b + word->c - word->d;
    three->Y = word->a + word->b - word->c - word->d;
    four->Y = word->a + word->b + word->c + word->d;
    
    zero_to_one(&(one->Y));
    zero_to_one(&(two->Y));
    zero_to_one(&(three->Y));
    zero_to_one(&(four->Y));

    one->Pb = two->Pb = three->Pb = four->Pb = word->Pb;
    one->Pr = two->Pr = three->Pr = four->Pr = word->Pr;
}



static Codeword unpack(uint32_t value)
{
    Codeword word = malloc(sizeof(*word));

    int64_t b = Bitpack_gets(value, INT, 18);
    int64_t c = Bitpack_gets(value, INT, 13);
    int64_t d = Bitpack_gets(value, INT, 8);

    word->a = (float)Bitpack_getu(value, UINT, 23) / (float)511;
    word->b = deconstrict(b);
    word->c = deconstrict(c);
    word->d = deconstrict(d);

    unsigned Pb = Bitpack_getu(value, P_WIDTH, P_WIDTH);
    unsigned Pr = Bitpack_getu(value, P_WIDTH, 0);


    word->Pb = decompress_p(Pb);
    word->Pr = decompress_p(Pr);
 
    return word;
    /* unpack bits from value
        check to make sure values are within specified range */
}


/*
 * unpack()
 * 
 * Parameters:
 * Returns:
 * Purpose: 
 * CRE:
 */
static void get_codeword(int i, int j, A2Methods_UArray2 array, void *elem, 
                    void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    A2Methods_UArray2 codewords = ((Closure *)cl)->array;
    A2Methods_T methods = ((Closure *)cl)->methods;

    Codeword word = methods->at(codewords, i, j);
    
    Codeword holder = unpack(*(uint32_t *)elem);
    *word = *holder;
    free(holder);

}


/*
 *
 */
void find_colorspace(A2Methods_UArray2 array, A2Methods_T methods,
                            A2Methods_UArray2 colorspace)
{
    assert(array != NULL && methods != NULL && colorspace != NULL);
    
    int width = methods->width(array);
    int height = methods->height(array);
    Codeword temp;
    A2Methods_UArray2 codewords = methods->new(width, height, sizeof(*temp));
    Closure *cl = malloc(sizeof(*cl));
    cl->array = codewords;
    cl->methods = methods;
    

    methods->map_default(array, get_codeword, cl);

    cl->array = colorspace;
    /* take array of codeword structs and convert to colorspace */
    methods->map_default(codewords, to_colorspace, cl);
    methods->free(&codewords);
    free(cl);
}



