/*
 * io.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of our io functions our universal machine
 * 
 */

#include "io.h"

/* 
 * outputRegister
 *
 * Parameters: unsigned 32 bit integer that represents a register
 * Returns: Nothing
 * Does: the register value is written to the I/O device
 * CRE: index must be greater than or equal to 0 and less than the length,
 * 	    segment struct cannot be NULL
 */
void outputRegister(uint32_t* ra)
{
    assert(ra != NULL);
    uint32_t val = Bitpack_getu(*ra, 8, 0);
    assert(val < 256);
    //fprintf(stderr, "Output is: %d\n", val);

    //val = (int) val;

    //JUST WANT TO PRINT A VALUE LOWER THAN 256 SO ONLY NEED FIRST BYTE

    //fprintf(stderr, "Register to be outputted: %ld\n", Bitpack_getu(*ra, 8, 0));

    putchar(val);
}

/* 
 * inputRegister
 *
 * Parameters: unsigned 32 bit integer that represents a register
 * Returns: Nothing
 * Does: the register is loades with the input
 * CRE: index must be greater than or equal to 0 and less than the length,
 * 	    segment struct cannot be NULL
 */
void inputRegister(uint32_t* ra)
{
    assert(ra != NULL);
    
    char input =  getchar();

    if (input == EOF) {
        *ra = ~0;
    } else {
        *ra = input;
    }
}