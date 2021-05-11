/*
 * decoder.h
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Interface for our instruction decoder in our universal machine
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bitpack.h"
#include <stdint.h>

#ifndef DECODER_INCLUDED
#define DECODER_INCLUDED



/* 
 * decodeLV
 *
 * Parameters: pointer to an unsigned 32 bit integer (word)
 * Returns: an array of uint32_t's
 * Does: unpacks the word in an instuction 13 format and returns it
 * CRE: the word should not be NULL
 */
uint32_t* decodeLV(uint32_t*);

/* 
 * decodeInstuction
 *
 * Parameters: pointer to an unsigned 32 bit integer (word)
 * Returns: an array of uint32_t's
 * Does: unpacks the word to retrieve an integer value representing an
 *       instruction and returns it
 * CRE: the word should not be NULL
 */
uint32_t* decodeInstruction(uint32_t*);


#endif