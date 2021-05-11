/*
 *  colorspace.c
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 *  edow01 and echo02
 * 
 *  This file contains our implementation for all of our functions dealing with
 *  conversions between our Colorspace_pixel struct and Pnm_rgb structs. 
 * 
 */


#include "structs.h"
#include "colorspace.h"
#include "range.h"

#define BLOCK 2
#define P_WIDTH 4
#define INT 5
#define UINT 9
#define DENOM 255

/************************* FUNCTIONS FOR COMPRESSING *************************/

/*
 * RGB_to_Colorspace()
 * 
 * Parameters: Pnm_rgb, which is a pointer to an rgb pixel struct that stores 
 *             the red, blue and green pixel values.
 *             A float containing the denominator value of the image.
 * Returns: Colorspace_pixel, which is a pointer to a struct that contains the
 *          necessary values for video component transformation.
 * Purpose: Takes the rgb elements from the initial pixel and uses equations to
 *          create a Colorspace_pixel that stores the necessary information.
 *          Returns the colorspace_pixel.
 * CRE: Pnm_rgb pixel cannot be NULL, denom cannot be negative. 
 */
static Colorspace_pixel RGB_to_Colorspace(Pnm_rgb pixel, float denom)
{
    assert(pixel != NULL);
    assert(denom >= 0);

    /* allocating space for a Colorspace_pixel, which will be deallocated */
    Colorspace_pixel cp = malloc(sizeof(*cp));

    float r = (float)(pixel->red) / denom;
    float g = (float)(pixel->green) / denom;
    float b = (float)(pixel->blue) / denom;

    cp->Y = 0.299 * r + 0.587 * g + 0.114 * b;
    cp->Pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
    cp->Pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

    /* clamping the calculated Y, Pb, Pr values */
    zero_to_one(&(cp->Y));

    restrict_p(&(cp->Pb));
    restrict_p(&(cp->Pr));

    return cp;
}

/*
 * get_colorspace()
 * 
 * Parameters: column of index, row of index, array of Pnm_ppm elements,
 *             Pnm_ppm element, cl contains Closure struct
 * Returns: Void
 * Purpose: Apply function that will iterate through each cell in an array of
 *          Pnm_ppm elements and stores their corresponding Colorspace_pixel 
 *          types in a new array.    
 * CRE: When the indexing is out of bounds, any of the other parameters are NULL
 */
void get_colorspace(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    A2Methods_UArray2 array2 = ((Closure *)cl)->array;
    A2Methods_T methods = ((Closure *)cl)->methods;
    float denom = ((Closure *)cl)->denom;
    
    /* gives us the uarray2 with the colorspace pixels */
    Colorspace_pixel pixel = methods->at(array2, i, j);
    
    Colorspace_pixel holder = RGB_to_Colorspace(elem, denom);
    *pixel = *holder;

    free(holder);
}

/*****************************************************************************/

/************************ FUNCTIONS FOR DECOMPRESSING ************************/

/*
 * Colorspace_to_RGB
 * 
 * Parameters: Colorspace_pixel, which is a pointer to a struct that contains 
 *          the necessary values for video component transformation.
 * Returns: Pnm_rgb, which is a pointer to an rgb pixel struct that stores 
 *          the red, blue and green pixel values.
 * Purpose: Takes in a Colorspace_pixel struct and uses equations outlined in
 *          the spec to return the rgb elements of each pixel.
 * CRE: Colorspace_pixel pixel cannot be NULL
 */
static Pnm_rgb Colorspace_to_RGB(Colorspace_pixel cp)
{
    assert (cp != NULL);
    Pnm_rgb pixel = malloc(sizeof(*pixel));


    float r = 1.0 * cp->Y + 0.0 * cp->Pb + 1.402 * cp->Pr;
    float g = 1.0 * cp->Y - 0.344136 * cp->Pb - 0.714136 * cp->Pr;
    float b = 1.0 * cp->Y + 1.772 * cp->Pb + 0.0 * cp->Pr;

    /* clamping calculated r g b values */
    zero_to_one(&r);
    zero_to_one(&g);
    zero_to_one(&b);
    pixel->red = (unsigned)(r * DENOM);
    pixel->green = (unsigned)(g * DENOM);
    pixel->blue = (unsigned)(b * DENOM);

    return pixel;
}




/*
 * get_rgb()
 * 
 * Parameters: column of index, row of index, array of Colorspace_pixel structs,
 *             Colorspace_pixel element, cl contains Closer struct
 * Returns:  void
 * Purpose:  Apply function that iterates through array of Colorspace_pixel
 *           types and stores their corresponding Pnm_ppm types
 * CRE: When the indexing is out of bounds, any of the other parameters are NULL
 */
void get_rgb(int i, int j, A2Methods_UArray2 array, void *elem, void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
    assert(i >= 0 && j >= 0);
    assert(array != NULL);

    A2Methods_UArray2 array2 = ((Closure *)cl)->array;
    A2Methods_T methods = ((Closure *)cl)->methods;

    /* gives us a uarray2 with the rgb pixels */

    Pnm_rgb pixel = methods->at(array2, i, j);
    Pnm_rgb holder = Colorspace_to_RGB(elem);
    *pixel = *holder;
    free(holder);

    
}

/*****************************************************************************/
