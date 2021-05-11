/*
 * um.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of the cpu in our universal machine
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "memory.h"
#include "decoder.h"
#include <stdbool.h>
#include "alu.h"

void createRegisters(uint32_t**);
void freeRegisters(uint32_t **);
void computeInstruction(void *, uint32_t *,
                        uint32_t **, bool *);


int main(int argc, char *argv[])
{

    if (argc != 2) {
        fprintf(stderr, "Improper input, please provide a file name.\n");
        return EXIT_FAILURE;
    }

    /* initializes memory */
    void* memory = initializeMemory(argv[1]);
    uint32_t *registers[8];
 
    for (int i = 0; i < 8; i++) {
        uint32_t *tempRegister = calloc(sizeof(*tempRegister), 1);
        assert(tempRegister != NULL);
        *tempRegister = 0;
        registers[i] = tempRegister;
	}

    bool *stopComp = malloc(sizeof(*stopComp));
	assert(stopComp != NULL);
    *stopComp = false;

    //int index = 0;

    while (!(*stopComp)) {

        uint32_t *instruction =
                decodeInstruction(getNextInstruction(memory));
        if (instruction[0] == 13) {
            uint32_t *valInstruct = malloc(sizeof(*valInstruct));
            assert(valInstruct != NULL);

            *valInstruct = instruction[2];

            executeInstruction(memory, instruction[0],
                registers[instruction[1]], valInstruct, NULL, stopComp);
            free(valInstruct);
        } else {
			executeInstruction(memory, instruction[0],
                registers[instruction[1]], registers[instruction[2]],
                registers[instruction[3]], stopComp);
        }
        free(instruction);
       // index++;
    }
    free(stopComp);
    destroyMemory(memory);

    for (int i = 0; i < 8; i++) {
        free(registers[i]);
    }

    return EXIT_SUCCESS;
}

/* 
 * createRegisters
 *
 * Parameters: register array
 * Returns: pointer to array of uint32_t's representing registers
 * Does: creates an array of 8 unint32_t's to be our registers
 * CRE: malloc works
 */
/*void createRegisters(uint32_t ** registers)
{
    for (int i = 0; i < 8; i++) {
        uint32_t *tempRegister = malloc(sizeof(*tempRegister));
        *tempRegister = 0;
        registers[i] = tempRegister;
    }
}*/

/* 
 * computeInstruction
 *
 * Parameters: void ptr to memory, a uint32_t array representing an
 * instruction, the register array, and the bool that terminates program
 * Returns: nothing
 * Does: calls functionts to properly compute the instruction given
 * CRE: inputs are not null
 */
/*void computeInstruction(void *memory, uint32_t *instruction, 
                        uint32_t **registers, bool *stopComp)
{
    assert(memory != NULL);
    assert(instruction != NULL);
    assert(registers != NULL);
    assert(stopComp != NULL);

    if (instruction[0] == 13) {
        uint32_t *valInstruct = malloc(sizeof(*valInstruct));
        assert(valInstruct != NULL);

        *valInstruct = instruction[2];

        executeInstruction(memory, instruction[0],
                           registers[instruction[1]], valInstruct, NULL, stopComp);
        free(valInstruct);
    }
    else {
        executeInstruction(memory, instruction[0],
                           registers[instruction[1]], registers[instruction[2]],
                           registers[instruction[3]], stopComp);
    }
    free(instruction);
}*/

/* 
 * freeRegisters
 *
 * Parameters: the register array
 * Returns: nothing
 * Does: frees all memory in register array
 * CRE: inputs are not null
 */
/*void freeRegisters(uint32_t** registers)
{
    assert(registers != NULL);
    
    for (int i = 0; i < 8; i++) {
        free(registers[i]);
    }
}*/