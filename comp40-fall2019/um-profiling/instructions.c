/*
 * instructions.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of the instructions for our universal machine
 * 
 */

#include "instructions.h"

#define SEG_ZERO 0

/* 
 * conditionalMove
 *
 * Parameters: pointers to three registers which store words (unsigned 32 bit
 *             integers words)
 * Returns: Nothing
 * Does: If the third register is not 0, then it moves the second register 
 *       into the first register
 * CRE: the words should not be NULL
 */
void conditionalMove(uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);

    if (*rc != 0) {
        *ra = *rb;
    }
}

/* 
 * add
 *
 * Parameters: pointers to three registers which store words (unsigned 32 bit
 *             integers words)
 * Returns: Nothing
 * Does: Puts the sum of the second register and third register into the first 
 *       register
 * CRE: the words should not be NULL
 */
void add(uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);

    //fprintf(stderr, "Instructions: registerB: %d\n", *rb);
    //fprintf(stderr, "Instructions: registerC: %d\n", *rc);
    *ra = (uint32_t)(*rb + *rc);
    //fprintf(stderr, "After adding: ra is %d\n", *ra);
}

/* 
 * mult
 *
 * Parameters: pointers to three registers which store words (unsigned 32 bit
 *             integers words)
 * Returns: Nothing
 * Does: Puts the product of the second register and third register into the
 *       first register
 * CRE: the words should not be NULL
 */
void mult(uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);

    *ra = (uint32_t)(*rb * *rc);
}

/* 
 * divide
 *
 * Parameters: pointers to three registers which store words (unsigned 32 bit
 *             integers words)
 * Returns: Nothing
 * Does: Divides the second register by the third register and puts the
 *       quotient into the first register
 * CRE: the words should not be NULL, the third register should not equal 0
 */
void divide(uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);

    if (*rc != 0) {
        *ra = (uint32_t)(*rb / *rc);
    } else {
        /* Cannot divide by 0 */
        assert(0);
    }
}

/* 
 * nand
 *
 * Parameters: pointers to three registers which store words (unsigned 32 bit
 *             integers words)
 * Returns: Nothing
 * Does: Computes a bitwise ~(b & c) where b = second register and c = third
 *       register, then stores that value in the first register
 * CRE: the words should not be NULL
 */
void nand(uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);

    *ra = ~(*rb & *rc);
}

/* 
 * loadValue
 *
 * Parameters: pointer to a register which stores a word (unsigned 32 bit
 *             integers words), pointer to a binary value
 * Returns: Nothing
 * Does: Loads the value into the register
 * CRE: the word should not be NULL, value should not be NULL
 */
void loadValue(uint32_t *ra, uint32_t value)
{
    assert(ra != NULL);
    //assert(value != NULL);

    *ra = value;
}

#undef SEG_ZERO