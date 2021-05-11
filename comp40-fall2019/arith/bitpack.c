/*
 *  bitpack.c
 * 
 *  COMP40
 *  Fall 2019
 *  arith
 * 
 *  Eli Dow and Zetty Cho
 * 
 *  This file contains our implementatons for the bitpack functions listed
 *  in the bitpack.h header file
 * 
 */


#include "bitpack.h"
#include "except.h"
#include "assert.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

#define INT_SIZE 64

static uint64_t fill(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value);

/*
 * Bitpack_fitsu
 * 
 * Parameters: unsigned 64 bit integer, unsigned integer for width
 * Returns: bool value
 * Purpose: Passes in an unsigned 64 bit integer and unsigned integer for width
 *          and tells whether the argument n can be represented in width
 *          bits 
 * CRE: width cannot be greater than 64
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= INT_SIZE && width > 0);

    uint64_t mask = ~0;

    uint64_t offset = INT_SIZE - width;

    mask = mask >> offset;

    if (n > mask) {
        return false;
    }
    return true;
    
}

/*
 * Bitpack_fitss
 * 
 * Parameters: signed 64 bit integer, unsigned integer for width
 * Returns: bool value
 * Purpose: Passes in a signed 64 bit integer and unsigned integer for width
 *          and tells whether the argument n can be represented in width
 *          bits 
 * CRE: n, width cannot be 0
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= INT_SIZE && width > 0);

    int64_t lower = ~0;
    lower = lower << (width - 1); 

    int64_t upper = ~lower;

    if (n < lower || n > upper){
        return false;
    }
    return true;

}

/*
 * Bitpack_getu
 * 
 * Parameters: unsigned 64 bit integer that represents a word, unsigned integer
 *             for width, unsigned integer for the least significant bit
 * Returns: unsigned 64 bit integer
 * Purpose: Extracts a field from an unsigned word given the width of the field
 *          and the location of the field’s least significant bit
 * CRE: word, width cannot be 0
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width > 0 && width <= INT_SIZE);
    assert((width + lsb) <= INT_SIZE);
    // assert(lsb >= 0);

    uint64_t mask = ~0;
    mask = mask >> (INT_SIZE - width);
    mask = mask << lsb;

    uint64_t final = word & mask;

    final = final >> lsb;

    return final;
}

/*
 * Bitpack_gets
 * 
 * Parameters: unsigned 64 bit integer that represents a word, unsigned integer
 *             for width, unsigned integer for the least significant bit
 * Returns: signed 64 bit integer
 * Purpose: Extracts a field from an unsigned word given the width of the field
 *          and the location of the field’s least significant bit
 * CRE: word, width cannot be 0
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width > 0 && width <= INT_SIZE);
    assert((width + lsb) <= INT_SIZE);
    // assert(lsb >= 0);

    uint64_t mask = ~0;
    mask = mask >> (INT_SIZE - width);
    mask = mask << lsb;

    uint64_t step = word & mask;

    step = step >> lsb;

    if ((step >> (width - 1))) {
        int64_t final = ~0;
        final = final << (width - 1);
        return (final | step);
    }
    else {
        return step;
    }
}

/*
 * Bitpack_newu
 * 
 * Parameters: unsigned 64 bit integer that represents a word, unsigned integer
 *             for width, unsigned integer for the least significant bit,
 *             unsigned 64 bit integer that represents the value that will be 
 *             inputted.
 * Returns: unsigned 64 bit integer
 * Purpose: Returns a new word which is identical to the original word, except
 *          that the field of width with least significant bit at lsb will
 *          have been replaced by a width-bit representation of value.
 * CRE: word, width cannot be 0
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
    assert(width > 0 && width <= INT_SIZE);
    assert((width + lsb) <= INT_SIZE);
    // assert(lsb >= 0);

    if (Bitpack_fitsu(value, width) == false) {
        RAISE(Bitpack_Overflow);
        assert(0);
    }

    return fill(word, width, lsb, value);
}
/*
 * Bitpack_news
 * 
 * Parameters: unsigned 64 bit integer that represents a word, unsigned integer
 *             for width, unsigned integer for the least significant bit,
 *             signed 64 bit integer that represents the value that will be
 *             inputted.
 * Returns: unsigned 64 bit integer
 * Purpose: Returns a new word which is identical to the original word, except
 *          that the field of width with least significant bit at lsb will
 *          have been replaced by a width-bit representation of value.
 * CRE: word, width cannot be 0
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value)
{
    assert(width > 0 && width <= INT_SIZE);
    assert((width + lsb) <= INT_SIZE);
    // assert(lsb >= 0);

    if (Bitpack_fitss(value, width) == false) {
        RAISE(Bitpack_Overflow);
        assert(0);
    }
    
    uint64_t value2 = value << (INT_SIZE - width);
    value2 = value2 >> (INT_SIZE - width);

    /* once we know that the value will fit and is proper */
    return fill(word, width, lsb, value2);
}

/* 
 * fill
 * 
 * Parameters: unsigned 64 bit integer that represents a word, unsigned integer
 *             for width, unsigned integer for the least significant bit,
 *             signed 64 bit integer that represents value that will be input
 * Returns: unsigned 64 bit integer
 * Purpose: Returns a new word which is identical to the original word, except
 *          that the field of width with least significant bit at lsb will
 *          have been replaced by a width-bit representation of value.
 * CRE: none, the functions that calls this would account for things
 */
static uint64_t fill(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
    uint64_t mask = ~0;
    uint64_t mask2 = ~0;

    mask = mask >> (INT_SIZE - lsb);
    mask2 = mask2 << (width + lsb);

    uint64_t maskf = mask | mask2;

    word = word & maskf; /* clears out space */

    value = value << lsb;

    return (word | value);
}