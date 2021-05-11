/*
 * io.h
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Interface for the input and output in our universal machine
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include "seq.h"
#include "assert.h"

#ifndef IO_INCLUDED
#define IO_INCLUDED


/* 
 * outputRegister
 *
 * Parameters: unsigned 32 bit integer that represents a register
 * Returns: Nothing
 * Does: the register value is written to the I/O device
 * CRE: index must be greater than or equal to 0 and less than the length, segment struct 
 * 	 cannot be NULL
 */
void outputRegister(uint32_t*);

/* inputRe
 *
 * Parameters: unsigned 32 bit integer that represents a register
 * Returns: Nothing
 * Does: the register is loades with the input
 * CRE: index must be greater than or equal to 0 and less than the length, segment struct 
 * 	 cannot be NULL
 */
void inputRegister(uint32_t*);




#endif