#include "arith40.h"
#include <stdlib.h>
#include "assert.h"
#include <stdint.h>
#include "range.h"

/*
 * decompress_p
 * 
 * Parameters:
 * Returns:
 * Purpose:
 * CRE:
 */
float decompress_p(unsigned P)
{
    return Arith40_chroma_of_index(P);
}


float deconstrict(int64_t val)
{
    return (float)(val / 50.0);
}


void zero_to_one(float *x)
{
    if (*x > 1.0) {
        *x = 1.0;
    }
    else if (*x < 0.0) {
        *x = 0.0;
    }
}

/*
 * constrict
 * 
 * Parameters:
 * Returns:
 * Purpose:
 * CRE:
 */
int64_t constrict(float x)
{
    if (x > 0.3) {
        return 15;
    }
    else if (x < -0.3) {
        return -15;
    }

    return (int)(x * 50); // need to change
}

void restrict_p(float *p)
{
    if (*p > 0.5) {
        *p = 0.5;
    }
    else if (*p < -0.5) {
        *p = -0.5;
    }
    
}


/*
 * compress_p
 * 
 * Parameters:
 * Returns:
 * Purpose:
 * CRE:
 */
unsigned compress_p(float P)
{
    return Arith40_index_of_chroma(P);
}
