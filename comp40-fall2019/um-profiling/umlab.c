/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>
#include "bitpack.h"

#include <assert.h>
#include <seq.h>

#define OP_CODE_WIDTH 4
#define OP_POS 28
#define OP_RANGE 13
#define REGISTER_WIDTH 3
#define REGISTER_A_POS 6
#define REGISTER_B_POS 3
#define REGISTER_C_POS 0
#define REGISTER_RANGE 7
#define REGISTER_LV_POS 25
#define VALUE_WIDTH 25
#define VALUE_POS 0


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction mult(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map_segment(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_segment(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction load_program(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

static inline Um_instruction segmented_load(Um_register a, 
                                                Um_register b, Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction segmented_store(Um_register a,
                                                Um_register b, Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction conditional_move(Um_register a,
                                             Um_register b, Um_register c)
{
        return three_register(CMOV, a, b, c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

extern void Um_write_sequence(FILE *output, Seq_T stream) 
{
       assert(stream != NULL);
       assert(output != NULL);

       //make sure file is open
       
        for (int i = 0; i < Seq_length(stream); i++) {
                Um_instruction instruct = 
                        (Um_instruction)(uintptr_t)Seq_get(stream, i);
                for (int j = 0; j < 32; j+= 8) {
                        fputc(Bitpack_getu(instruct, 8, 24-j), output);
                }
        }
}


/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        /* Expect it to halt */
        emit(stream, halt());
}


void emit_verbose_halt_test(Seq_T stream)
{

        /* Expect it to halt before any other instructions are called */
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '$'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '@'));
        emit(stream, output(r1));
}

void emit_load_value_test(Seq_T stream)
{
        /* Expect r1 = 1, r2 = 2 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, output(r1));
        emit(stream, output(r2));

        /* Expect r3 is 6 */
        emit(stream, loadval(r3, 3));
        emit(stream, loadval(r3, 6));
        emit(stream, output(r3));

        emit(stream, halt());
}

void emit_add_test(Seq_T stream)
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        
        /* Expect r3 = 3 */
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));

        /* Expect r2 = 4 */
        emit(stream, add(r2, r3, r1));
        emit(stream, output(r2));
        

        emit(stream, loadval(r4, 60));
        emit(stream, loadval(r5, 8));
        
        /* Expect r6 = 68 */
        emit(stream, add(r5, r4, r5));
        emit(stream, output(r5));

        emit(stream, loadval(r1, 'a'));
        emit(stream, loadval(r2, 1));

        /* Expect r3 = b */
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));

        emit(stream, halt());
}

void emit_multiply_test(Seq_T stream)
{
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));

        /* Expect r3 = 2 */
        emit(stream, mult(r3, r1, r2));
        emit(stream, output(r3));

        /* Expect r1 = 4 */
        emit(stream, mult(r1, r3, r2));
        emit(stream, output(r1));

        emit(stream, loadval(r4, 20));
        emit(stream, loadval(r5, 8));

        /* Expect r5 = 160 */
        emit(stream, mult(r5, r4, r5));
        emit(stream, output(r5));

        emit(stream, loadval(r6, 0));

        /* Expect r6 = 0 */

        emit(stream, mult(r6, r6, r5));
        emit(stream, output(r6));

        emit(stream, loadval(r1, 'a'));
        emit(stream, loadval(r2, 1));

        /* Expect r3 = a */
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));

        emit(stream, halt());
}

void emit_divide_test(Seq_T stream)
{
        emit(stream, loadval(r1, 6));
        emit(stream, loadval(r2, 3));

        /* Expect r3 = 2 */
        emit(stream, div(r3, r1, r2));
        emit(stream, output(r3));

        /* Expect r1 = 1 */
        emit(stream, div(r1, r2, r3));
        emit(stream, output(r1));

        emit(stream, loadval(r4, 20));
        emit(stream, loadval(r5, 8));

        /* Expect r5 = 2 */
        emit(stream, div(r5, r4, r5));
        emit(stream, output(r5));

        emit(stream, loadval(r6, 0));

        emit(stream, loadval(r1, 21));
        emit(stream, loadval(r2, 22));

        /* Expect r2 = 0 */
        emit(stream, div(r2, r1, r2));
        emit(stream, output(r2));

        emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
        // uint32_t newInt = 2147483647;
        //emit(stream, loadval(r1, 2147483647);
        //emit(stream, loadval(r2, 4294967295));

        /* Expect r3 = 4294967295 */
        emit(stream, nand(r1, r1, r1));
        emit(stream, output(r1));

        /* Expect r3 = 0 */
        emit(stream, nand(r3, r1, r2));
        emit(stream, output(r3));

        emit(stream, loadval(r4, 20));
        emit(stream, loadval(r5, 8));

        /* Expect r5 = BIG */
        emit(stream, nand(r5, r4, r5));
       // emit(stream, output(r5));

        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 0));

        /* Expect r2 = 4294967295 */
        emit(stream, nand(r2, r1, r2));
       // emit(stream, output(r2));

        emit(stream, halt());
}

void emit_input_test(Seq_T stream)
{
        /* Expect 81 inputs from a file, outputs them */
        for (int i = 0; i < 81; i++) {
                emit(stream, input(r1));
                emit(stream, output(r1));
        }
        emit(stream, halt());
}

void emit_output_test(Seq_T stream)
{
        /* Expect 2 */
        emit(stream, loadval(r1, '2'));
        emit(stream, output(r1));

        /* Expect e */
        emit(stream, loadval(r2, 'e'));
        emit(stream, output(r2));

        /*Expect  n */
        emit(stream, loadval(r2, 'n'));
        emit(stream, output(r2));

        /* Expect 5 */
        emit(stream, loadval(r3, '5'));
        emit(stream, output(r3));

        /* Expect 1 */
        emit(stream, loadval(r1, '1'));
        emit(stream, output(r1));

        emit(stream, halt());
}

void emit_output_adv_test(Seq_T stream)
{
        /* Expect 3 */
        emit(stream, loadval(r1, '3'));
        emit(stream, output(r1));

        /* Expect e */
        emit(stream, loadval(r2, 'e'));
        emit(stream, output(r2));

        /*Expect  n*/
        emit(stream, loadval(r2, 'n'));
        emit(stream, output(r2));

        /* Expect 5 */
        emit(stream, loadval(r3, '5'));
        emit(stream, output(r3));

        /* Expect 1 */
        emit(stream, loadval(r1, '1'));
        emit(stream, output(r1));

        /* r1 = 'A' */
        emit(stream, input(r1));
        emit(stream, output(r1));

        /* r1 = '~' */
        emit(stream, input(r1));
        emit(stream, output(r1));

        /* r1 = 'd' */
        emit(stream, input(r1));
        emit(stream, output(r1));

        /* r1 = '‡' */
        emit(stream, input(r1));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_map_segment_test(Seq_T stream)
{
        /* r2 = '1' */
        emit(stream, loadval(r1, 5));
        emit(stream, loadval(r2, 1));
        emit(stream, map_segment(r2, r1));
        emit(stream, output(r2));

        /* r2 = '2' */
        emit(stream, loadval(r1, 15));
        emit(stream, loadval(r2, 1));
        emit(stream, map_segment(r2, r1));
        emit(stream, output(r2));

        /* r2 = '3' */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 200));
        emit(stream, map_segment(r2, r1));
        emit(stream, output(r2));

        /* r2 = '4' */
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 1));
        emit(stream, map_segment(r2, r1));
        emit(stream, output(r2));

        /* r2 = '6' */
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 1));
        emit(stream, loadval(r3, 2));
        emit(stream, map_segment(r3, r1));
        emit(stream, map_segment(r2, r1));
        emit(stream, output(r2));

        emit(stream, halt());
}

void emit_segmented_store_test(Seq_T stream)
{
        /* r3 = 3 */
        emit(stream, loadval(r1, 6));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 'a'));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r1, r1, r3));

        /* r3 = 11 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 3));
        emit(stream, loadval(r3, 11));
        emit(stream, loadval(r4, 2));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r4, r4, r3));

        /* r3 = Q */
        emit(stream, loadval(r1, 240));
        emit(stream, loadval(r2, 310));
        emit(stream, loadval(r3, 'Q'));
        emit(stream, loadval(r4, 3));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r4, r1, r3));

        /* r3 = y */
        emit(stream, loadval(r1, 6));
        emit(stream, loadval(r2, 10));
        emit(stream, loadval(r3, 'y'));
        emit(stream, loadval(r4, 4));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r4, r1, r3));

        /* r3 = 35 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 3));
        emit(stream, loadval(r3, 35));
        emit(stream, loadval(r4, 5));
        emit(stream, map_segment(r1, r2));
        emit(stream, loadval(r1, 1));
        emit(stream, segmented_store(r4, r1, r3));

        emit(stream, halt());
}

void emit_segmented_load_test(Seq_T stream)
{
        /* r1 = d */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 'd'));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r1, r1, r3));
        emit(stream, segmented_load(r4, r1, r1));
        emit(stream, output(r4));

        /* r1 = f */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 3));
        emit(stream, loadval(r3, 'f'));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r1, r1, r3));
        emit(stream, segmented_load(r4, r1, r1));
        emit(stream, output(r4));

        /* r1 = Q */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 78));
        emit(stream, loadval(r6, 30));
        emit(stream, loadval(r3, 'Q'));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r1, r6, r3));
        emit(stream, segmented_load(r4, r1, r6));
        emit(stream, output(r4));

        /* r1 = p */
        emit(stream, loadval(r1, 6));
        emit(stream, loadval(r2, 10));
        emit(stream, loadval(r3, 'p'));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r1, r1, r3));
        emit(stream, segmented_load(r4, r1, r1));
        emit(stream, output(r4));

        /* r1 = 7 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 3));
        emit(stream, loadval(r3, '7'));
        emit(stream, loadval(r5, 1));
        emit(stream, map_segment(r1, r2));
        emit(stream, segmented_store(r1, r5, r3));
        emit(stream, segmented_load(r1, r1, r5));
        emit(stream, output(r1));

        emit(stream, halt());
}

void emit_unmap_segment_test(Seq_T stream)
{
        /* expect output 1 */
        emit(stream, loadval(r1, 4));
        emit(stream, loadval(r4, 69));
        emit(stream, map_segment(r1, r4));
        emit(stream, unmap_segment(r1));
        emit(stream, map_segment(r4, r1));
        emit(stream, output(r4));

        /* Expect output 2 */
        emit(stream, loadval(r1, 12));
        emit(stream, loadval(r4, 80));
        emit(stream, map_segment(r1, r4));
        emit(stream, map_segment(r2, r4));
        emit(stream, unmap_segment(r1));
        emit(stream, map_segment(r4, r1));
        emit(stream, output(r4));

        /* Expect output 5 */
        emit(stream, loadval(r1, 12));
        emit(stream, loadval(r4, 80));
        emit(stream, map_segment(r1, r4));
        emit(stream, map_segment(r2, r4));
        emit(stream, unmap_segment(r1));
        emit(stream, unmap_segment(r2));
        emit(stream, map_segment(r4, r1));
        emit(stream, output(r4));

        emit(stream, halt());
}

void emit_load_program_test(Seq_T stream)
{
        //r3 represents our i value
        //r2 is the value that we loop back on
        //r4 is the value that we use to exit the loop
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r4, 7));
        emit(stream, loadval(r2, 4));
        emit(stream, loadval(r3, 0));

        emit(stream, loadval(r3, 10));

        emit(stream, conditional_move(r2, r4, r3));
        emit(stream, load_program(r1,r2));

        /* Just Halt program */
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 40));
        emit(stream, loadval(r3, 2));
        emit(stream, loadval(r4, 12));

        
        emit(stream, load_program(r1, r4));
        
        emit(stream, halt());
}

void emit_conditional_move_test(Seq_T stream)
{
        /* r1 = 1 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 3));
        emit(stream, loadval(r3, 0));
        emit(stream, conditional_move(r1, r2, r3));
        emit(stream, output(r1));

        /* r1 = 3 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 3));
        emit(stream, loadval(r3, 1));
        emit(stream, conditional_move(r1, r2, r3));
        emit(stream, output(r1));

        /* r1 = 20 */
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 20));
        emit(stream, loadval(r3, 54));
        emit(stream, conditional_move(r1, r2, r3));
        emit(stream, output(r1));

        /* r1 = 0 */
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 0));
        emit(stream, loadval(r3, 0));
        emit(stream, conditional_move(r1, r2, r3));
        emit(stream, output(r1));

        /* r1 = 1 */
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 1));
        emit(stream, loadval(r3, 1));
        emit(stream, conditional_move(r1, r2, r3));
        emit(stream, output(r1));

        /* r1 = 93 */
        emit(stream, loadval(r1, 'b'));
        emit(stream, loadval(r2, 93));
        emit(stream, loadval(r3, 'a'));
        emit(stream, conditional_move(r1, r2, r3));
        emit(stream, output(r1));

        emit(stream, halt());
}




void emit_print_six_test(Seq_T stream)
{
        emit(stream, loadval(r2, 48));
        emit(stream, loadval(r3, 6));

        emit(stream, add(r1, r2, r3));

        emit(stream, output(r1));

        emit(stream, halt());
}



Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        assert(op >= 0 && op <= OP_RANGE);
        assert(ra >= 0 && ra <= REGISTER_RANGE);
        assert(rb >= 0 && rb <= REGISTER_RANGE);
        assert(rc >= 0 && rc <= REGISTER_RANGE);
        
        uint64_t instruct = 0;
        
        instruct = Bitpack_newu(instruct, OP_CODE_WIDTH, OP_POS, (unsigned)op);
        instruct = Bitpack_newu(instruct, REGISTER_WIDTH, REGISTER_A_POS,
                                (unsigned)ra);
        instruct = Bitpack_newu(instruct, REGISTER_WIDTH, REGISTER_B_POS,
                                (unsigned)rb);
        instruct = Bitpack_newu(instruct, REGISTER_WIDTH, REGISTER_C_POS,
                                (unsigned)rc);

        return (uint32_t)instruct;       
}



Um_instruction loadval(unsigned ra, unsigned val)
{       
        uint64_t instruct = 0;

        instruct = Bitpack_newu(instruct, OP_CODE_WIDTH, OP_POS, LV);

        instruct = Bitpack_newu(instruct, REGISTER_WIDTH, REGISTER_LV_POS,
                                ra);
        instruct = Bitpack_newu(instruct, VALUE_WIDTH, VALUE_POS,
                                val);
                                
        return (uint32_t) instruct;
}

