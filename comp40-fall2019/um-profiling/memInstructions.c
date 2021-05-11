/*
 * MemInstructions.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of the memory instructions for our universal machine
 * 
 */

#include "memInstructions.h"

#define SEG_ZERO 0

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
void segmentedLoad(void *mem, uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);
    assert(mem != NULL);

    //fprintf(stderr, "Get word from SEG LOAD\n");
    uint32_t *temp = malloc(sizeof(*temp));
    assert(temp != NULL);

    *temp = *rc;
    *temp = *temp + 1;

    //fprintf(stderr, "Mem Instrcutions: Rc is %d\n", *rc);
    //fprintf(stderr, "Mem Instructions: tem is %d\n", *temp);

    //fprintf(stderr, "SEG LOAD: rc is %d\n", *rc);
    uint32_t *newWord = malloc(sizeof(*newWord));
    assert(newWord != NULL);
    //fprintf(stderr, "rb in segmentedLoad: %d\n", *rb);
    *newWord = getWord(mem, rb, temp);

    *ra = *newWord;

    free(newWord);
    free(temp);
}

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
void segmentedStore(void *mem, uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);
    assert(mem != NULL);

    uint32_t *temp = malloc(sizeof(*temp));
    assert(temp != NULL);

    //fprintf(stderr, "Mem Instrcutions: Rb is before %d\n", *rb);
    *temp = *rb;
    *temp = *temp + 1;
    //fprintf(stderr, "Mem Instrcutions: Rb is %d\n", *rb);
    //fprintf(stderr, "Mem Instructions: tem is %d\n", *temp);

    //fprintf(stderr, "Ra in seg store: %d\n", *ra);
    setWord(mem, ra, temp, rc);

    free(temp);
}

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
void mapSeg(void *mem, uint32_t *rb, uint32_t *rc)
{
    assert(rb != NULL);
    assert(rc != NULL);
    assert(mem != NULL);

    mapSegment(mem, rb, rc);
}

/* 
 * unMapSeg
 *
 * Parameters: pointer to our memory struct, pointer to a register which stores
 *             a word (unsigned 32 bit integers words)
 * Returns: Nothing
 * Does: Unmaps the segment in the register
 * CRE: the word should not be NULL, the struct should not be NULL
 */
void unMapSeg(void *mem, uint32_t *rc)
{
    assert(rc != NULL);
    assert(mem != NULL);

    unmapSegment(mem, rc);
}

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
void loadProgram(void *mem, uint32_t *rb, uint32_t *rc)
{
    assert(rb != NULL);
    assert(rc != NULL);
    assert(mem != NULL);

   // uint32_t *zero = malloc(sizeof(*zero));
   // assert(zero != NULL);
   // *zero = 0;

    //*rc = *rc + 1;

    loadSegmentProg(mem, rb, rc);

    
    //loadSegment(mem, rb, zero, rc);

   // free(zero);
}

#undef SEG_ZERO
