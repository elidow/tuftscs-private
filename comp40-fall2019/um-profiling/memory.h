/*
 * memory.h
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Memory interface in our universal machine
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "seq.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "bitpack.h"
#include "except.h"

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

typedef struct MemArrayStruct MemArray;

/*struct MemArrayStruct
{
	Seq_T *memory;
	Seq_T *mapped;
	int instructIndex;
};*/

/*struct MemArrayStruct
{
	Seq_T *memory;
};*/


/* 
 * readFile
 *
 * Parameters: Nothing
 * Returns: Nothing
 * Does: takes in a function that reads a file of our um program
 * loads it into segment 0 
 * CRE: can open file
 */
void* readFile(char *);

/* initializeMemory
 *
 * Parameters: char* file name
 * Returns: void* containing memeory
 * Does: Initializes our main memory which is a sequence of uint32_t arrays,
 * as well as a parallel array of bools indicating if segment is mapped
 * and a sequence of indicies indicating unmapped segments, and puts 
 * instructions from files in it
 *
 * CRE: file name is not null
 */
void* initializeMemory(char *);

/* 
 * getNextInstruction
 *
 * Parameters: void* containing our memory
 * Returns: pointer to the next instruction
 * Does: Takes in our memory and fetches the next instruction 
 * from segment 0, and returns a pointer to it
 * CRE: memory is not null
 */
uint32_t* getNextInstruction(void *);

/* destroyMemory
 *
 * Parameters: pointer to memory
 * Returns: Nothing
 * Does: frees all memory in memory ptr
 * CRE: checks memory is not null
 */
void destroyMemory(void *);

/* loadSegmentProg
 *
 * Parameters: pointer to memory, two registers 
 * Returns: Nothing
 * Does: duplicates specified segment at index rb and sets program
 * counter to rc
 * CRE: index must be less than the length, segment struct 
 * 	 cannot be NULL, registers cannot be null
 */
void loadSegmentProg(void *, uint32_t *, uint32_t*);

/* getWord
 *
 * Parameters: pointer to memory, two registers
 * Returns: unsigned 32 bit integer representing a word
 * Does: Accesses word at specified indexes[rb][rc] and returns it
 * CRE: inputs are not null, we are not trying to access an out of 
 * bounds index
 */
uint32_t getWord(void *, uint32_t *, uint32_t *);

/* setWord
 *
 * Parameters: pointer to memory, and three registers
 * Returns: Nothing
 * Does: Mutator function that changes word at specified index [ra][rb]
 * and sets it to rc
 * CRE: inputs must not be null, and indicies must not be out of bounds
 */
void setWord(void*, uint32_t* , uint32_t*, uint32_t*);

/* mapSegment
	 *
	 * Parameters: two pointers to unsigned 32 bit integers that are registers
     * and a void pointer to our memory
	 * Returns: Nothing
	 * Does: maps a segment of size rc at first unmapped segment, and sets
     * rc to indicate where it was mapped
	 * CRE: inputs are not null, 
 */
void mapSegment(void*, uint32_t*, uint32_t*);

/* unmapSegment
	 *
	 * Parameters: pointer to memory, ptr to uint32_t rc
	 * Returns: Nothing
	 * Does: unmapps uint32_t array at index rc, and adds that index
     * to the queue of unmapped indicies
	 * CRE: index must be less than the length, must not
     * unmap an unmapped segment
 */
void unmapSegment(void*, uint32_t*);


//void loadSegment(void*, uint32_t*, uint32_t*, uint32_t*);

#endif
