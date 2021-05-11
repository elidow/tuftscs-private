/*
 * decoder.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of the instruction decoder in our universal machine
 * 
 */

#include "decoder.h"

#define LV 13
#define SIZE_LV_ARRAY 3
#define SIZE_OP_ARRAY 4
#define REG_WIDTH 3

#define RA_LSB_LV 25
#define VAL_LSB_LV 0
#define VAL_WIDTH 25

#define RA_LSB 6
#define RB_LSB 3
#define RC_LSB 0

#define OP_LSB 28
#define OP_WIDTH 4


/* 
 * decodeLV
 *
 * Parameters: pointer to an unsigned 32 bit integer (word)
 * Returns: an array of uint32_t's
 * Does: unpacks the word in an instuction 13 format and returns it
 * CRE: the word should not be NULL
 */
uint32_t* decodeLV(uint32_t * instrPointer)
{
    assert(instrPointer != NULL);

    uint32_t ra = Bitpack_getu(*instrPointer, REG_WIDTH, RA_LSB_LV);
    uint32_t value = Bitpack_getu(*instrPointer, VAL_WIDTH, VAL_LSB_LV);

    uint32_t *result = malloc(sizeof(*result) * SIZE_LV_ARRAY);
    assert(result != NULL);

    result[0] = (uint32_t)LV;
    result[1] = ra;
    //fprintf(stderr, "Register a decodeLV: %d\n", result[1]);
    result[2] = value;
    //fprintf(stderr, "value decodeLV: %d\n", result[2]);
    
    free(instrPointer);

    return result;
}

/* 
 * decodeInstuction
 *
 * Parameters: pointer to an unsigned 32 bit integer (word)
 * Returns: an array of uint32_t's
 * Does: unpacks the word to retrieve an integer value representing an
 *       instruction and returns it
 * CRE: the word should not be NULL
 */
uint32_t* decodeInstruction(uint32_t * instrPointer)
{
    //uint32_t *instrPointer = malloc(sizeof(*instrPointer));
    assert(instrPointer != NULL);

    //*instrPointer = instr;

    uint32_t opcode = Bitpack_getu(*instrPointer, OP_WIDTH, OP_LSB);

    //fprintf(stderr, "opcode: %d\n", opcode);
    if ((int)opcode == LV) {
        //free(instrPointer);
        return decodeLV(instrPointer);
    }

    uint32_t ra = Bitpack_getu(*instrPointer, REG_WIDTH, RA_LSB);
    uint32_t rb = Bitpack_getu(*instrPointer, REG_WIDTH, RB_LSB);
    uint32_t rc = Bitpack_getu(*instrPointer, REG_WIDTH, RC_LSB);

    uint32_t *result = malloc(sizeof(*result) * SIZE_OP_ARRAY);
    assert(result != NULL);
    
    result[0] = opcode;
    result[1] = ra;
    result[2] = rb;
    result[3] = rc;

    free(instrPointer);

    return result;
}


#undef LV
#undef SIZE_LV_ARRAY
#undef SIZE_OP_ARRAY
#undef REG_WIDTH

#undef RA_LSB_LV
#undef VAL_LSB_LV
#undef VAL_WIDTH

#undef RA_LSB
#undef RB_LSB
#undef RC_LSB

#undef OP_LSB
#undef OP_WIDTH