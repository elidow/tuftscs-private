/*
 * MemInstructions.h
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Interface of the memory instructions for our universal machine
 *          All of the functions call our memory file to execute their 
*          instructions.
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "memory.h"

#ifndef MEMINSTRUCTIONS_INCLUDED
#define MEMINSTRUCTIONS_INCLUDED

/* 
 * segmentedLoad
 *
 * Parameters: pointer to our memory struct, pointer to three registers which
 *             store words (unsigned 32 bit integers words)
 * Returns: Nothing
 * Does: Loads the instuction from the segment at index rb and instruction at
 *       index rc into regeister ra
 * CRE: the words should not be NULL, the struct should not be NULL
 */
void segmentedLoad(void *, uint32_t *, uint32_t *, uint32_t *);

/* 
 * segmentedStore
 *
 * Parameters: pointer to our memory struct, pointer to three registers which
 *             store words (unsigned 32 bit integers words)
 * Returns: Nothing
 * Does: Sets register rc to the instruction at index rb within the segment
 *       at index ra
 * CRE: the words should not be NULL, the struct should not be NULL
 */
void segmentedStore(void *, uint32_t *, uint32_t *, uint32_t *);

/* 
 * mapSeg
 *
 * Parameters: pointer to our memory struct, pointer to two registers which
 *             store words (unsigned 32 bit integers words)
 * Returns: Nothing
 * Does: Maps a segment of a specific size (register rc) and places it in
 *       the segment specified in register rb
 * CRE: the words should not be NULL, the struct should not be NULL
 */
void mapSeg(void *, uint32_t *, uint32_t *);

/* 
 * unMapSeg
 *
 * Parameters: pointer to our memory struct, pointer to a register which stores
 *             a word (unsigned 32 bit integers words)
 * Returns: Nothing
 * Does: Unmaps the segment in the register
 * CRE: the word should not be NULL, the struct should not be NULL
 */
void unMapSeg(void *, uint32_t *);

/* 
 * segmentedLoad
 *
 * Parameters: pointer to our memory struct, pointer to two registers which
 *             store words (unsigned 32 bit integers words)
 * Returns: Nothing
 * Does:  segment specified at rb is duplicated, and the
 *        duplicate replaces segment 0 (unless it is already at segment 0). Then
 *        the program counter is set to point to the instuction at index rc
 * CRE: the words should not be NULL, the struct should not be NULL
 */
void loadProgram(void *, uint32_t *, uint32_t *);

#endif