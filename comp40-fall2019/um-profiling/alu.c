/*
 * alu.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of the ALU in our universal machine
 * 
 */

#include "alu.h"


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
void executeInstruction(void* mem, uint32_t instruction, uint32_t *registerA,
uint32_t *registerB, uint32_t *registerC, bool* halt)
{
    //fprintf(stderr, "Instruction ID: %d\n", instruction);
    
    /*if (registerA != NULL) {
        fprintf(stderr, "Register ra: %d\n", *registerA);
    }
    if (registerB != NULL) {
        fprintf(stderr, "Register rb: %d\n", *registerB);
    }
    if (registerC != NULL)
    {
        fprintf(stderr, "Register rc: %d\n", *registerC);
    }*/
    //fprintf(stderr, "Instruction #: %d\n", (int)instruction);
    switch((int)instruction) {
        case 0:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            //fprintf(stderr, "RegisterB: %d\n", *registerB);
            //fprintf(stdout, "RegisterC: %d\n\n", *registerC);
            conditionalMove(registerA, registerB, registerC);
            break;
        case 1:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            //fprintf(stderr, "rb in segmentedLoad: %d\n", *registerB);
            segmentedLoad(mem, registerA, registerB, registerC);
            break;
        case 2:
            //fprintf(stderr, "ALU - RegisterA: %d\n", *registerA);
            //segmentedStore(mem, registerA, registerB, registerC);
            //segmentedStore(mem, registerC, registerA, registerB);
            //segmentedStore(mem, registerA, registerC, registerB);
            //segmentedStore(mem, registerB, registerC, registerA);
            //segmentedStore(mem, registerB, registerA, registerC);
            //segmentedStore(mem, registerC, registerB, registerA);
            segmentedStore(mem, registerA, registerB, registerC);
            break;
        case 3:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            //fprintf(stderr, "ALU: registerA: %d\n", *registerA);
            //fprintf(stderr, "ALU: registerB: %d\n", *registerB);
            //fprintf(stderr, "ALU: registerC: %d\n", *registerC);
            add(registerA, registerB, registerC);
            break;
        case 4:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            mult(registerA, registerB, registerC);
            break;
        case 5:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            divide(registerA, registerB, registerC);
            break;
        case 6:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            nand(registerA, registerB, registerC);
            break;
        case 7:
            //fprintf(stderr, "Yer boi is halting\n");
            *halt = true;
            break;
        case 8:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            mapSeg(mem, registerB, registerC);
            //mapSeg(mem, registerA, registerB);
            break;
        case 9:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            unMapSeg(mem, registerC);
            break;
        case 10:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            //fprintf(stderr, "RegisterC: %d\n", *registerC);
            //fprintf(stderr, "RegisterB: %d\n", *registerB);
            outputRegister(registerC);
            break;
        case 11:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            inputRegister(registerC);
            break;
        case 12:
            loadProgram(mem, registerB, registerC);
            break;
        case 13:
            //fprintf(stderr, "RegisterA: %d\n", *registerA);
            //fprintf(stderr, "RegisterB: %d\n", *registerB);
            loadValue(registerA, *registerB);
            break;
        default:
            /* opcode is out of bounds */
            assert(0);
            break;
        
    }
}

