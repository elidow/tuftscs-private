/*
 * alu.h
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Interface for the ALU in our universal machine
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "instructions.h"
#include "memInstructions.h"
#include "io.h"

#ifndef ALU_INCLUDED
#define ALU_INCLUDED

/* 
 * executeInstruction
 *
 * Parameters: pointer to our memory struct, pointer to our instruction, 
 *             three pointers to three registers (which holds words),
 *             pointer to a bool to keep track of when the program halts
 * Returns: Nothing
 * Does: Executes the instruction by calling our instructions file functions
 * CRE:  If the instuction is not 0-13
 *       Note: Each instruction has its own specific CRE
 */
void executeInstruction(void*, uint32_t, uint32_t*, uint32_t*, 
                        uint32_t*, bool*);

#endif