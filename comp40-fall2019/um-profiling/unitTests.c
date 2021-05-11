/*
 * unitTests.c
 * 
 * Fall 2019
 * By Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * 
 * 
 * This file contains the unit testing for functions in our UM
 * 
 * */

#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "seq.h"
#include "umStructs.h"
#include "instuct_fetcher.h"
#include "instruct_decoder.h"
#include "umlab.c"



int main() {

    return 0;
}

/*
 * Tests the setWord and getWord functions from our memory implementation
 * It starts by simulating our memory architecture, and then just calls
 * the functions and outputs what values they return so that we can check
 * to make sure that they are correct
 * 
 */
void test_Get_And_Set_Word()
{
    Seq_T segmentArray; 
    
    extern const Except_T Mem_Failed;
    TRY
        segmentArray = Seq_new(5);
    EXCEPT(Mem_Failed)
        fprintf(stderr, "Failed to create Seqence\n");
        exit(EXIT_FAILURE);
    END_TRY;

    for (int i = 0; i < 2; i++) {
        Seq_T segMem;
        extern const Except_T Mem_Failed;
        TRY
            segMem = Seq_new(5);
        EXCEPT(Mem_Failed)
            fprintf(stderr, "Failed to create Seqence\n");
            exit(EXIT_FAILURE);
        END_TRY;

        Segment *temp = malloc(sizeof(Struct Segment));
        temp->data = segMem;
        temp->mapped = true;
        void *useless = Seq_addHi(temp->data, i);
    }
    
    uint32_t* a = 2;
    uint32_t* b = 1;
    uint32_t* c = 0;
    uint32_t* d = 4;
    
    /* Expect to put 1 into the 3rd place in memory in the 2nd segment */
    setWord(b, a, b);
    Segment *tempseg;
    tempseg = Seq_get(segmentArray, 1);
    uint32_t* tempval = Seq_get(tempseg->data, 2);
    printf("SetWord: %d\n", *tempval);

    printf("GetWord: %d\n", *(getWord(a, b)));
    

    /* Expect to put 4 into the 3rd place in memory in the 3nd segment */
    setWord(a, a, d);
    Segment *tempseg;
    tempseg = Seq_get(segmentArray, 2);
    uint32_t* tempval = Seq_get(tempseg->data, 2);
    printf("SetWord: %d\n", *tempval);

    printf("GetWord: %d\n", *(getWord(a, a)));

    /* Expect out of bounds error */
    setWord(d, a, b);

    /* Expect out of bounds error */
    getWord(d, b);
    getWord(b, d);
    
}

/*
 * Tests the map and unmap functions just to make sure that we are handling 
 * the mapping correctly. It outputs the sequence lengths to make sure that
 * we are mapping sequences of the correct length.
 */
void test_Map_UnMap()
{
    Seq_T segmentArray;
    
    
    extern const Except_T Mem_Failed;
    TRY
        segmentArray = Seq_new(5);
    EXCEPT(Mem_Failed)
        fprintf(stderr, "Failed to create Seqence\n");
        exit(EXIT_FAILURE);
    END_TRY;

    for (int i = 0; i < 2; i++) {
        Seq_T segMem;
        extern const Except_T Mem_Failed;
        TRY
            segMem = Seq_new(5);
        EXCEPT(Mem_Failed)
            fprintf(stderr, "Failed to create Seqence\n");
            exit(EXIT_FAILURE);
        END_TRY;

        Segment *temp = malloc(sizeof(Struct Segment));
        temp->data = segMem;
        temp->mapped = true;
        void *useless = Seq_addHi(temp->data, i);
    }
    
    uint32_t* a = 8;
    uint32_t* b = 3;
    uint32_t* c = 0;
    uint32_t* d = 4;

    /*
     * This maps to the first unmapped segment: 3rd place in memory, 1st segment
     * We want this to print 4
     */
    mapSegment(b, 4);

    Segment *tempseg;
    tempseg = Seq_get(segmentArray, 3);
    printf("Size of the sequence: %d\n", Seq_length(tempseg->data));

    /*
     * This maps to the same segment as above, it should fail
     */
    mapSegment(b, 2);

    Segment *tempseg;
    tempseg = Seq_get(segmentArray, 3);
    printf("Size of the sequence: %d\n", Seq_length(tempseg->data));

    /*
     * This maps to a segment that is currrently out of bounds of the seg array
     */
    mapSegment(a, 2);


    /*
     * This unmaps a segment that is currently mapped
     */
    unmapSegment(b, 2);

    Segment *tempseg;
    tempseg = Seq_get(segmentArray, 3);
    if(!tempseg->mapped)
        printf("The segment is successfully unmapped!");


    /*
     * This unmaps a segment that is already unmapped (should do the same thing)
     */
    unmapSegment(b, 2);

    Segment *tempseg;
    tempseg = Seq_get(segmentArray, 3);
    if(!tempseg->mapped)
        printf("The segment is successfully unmapped\n");

    
    /*
     * This unmaps a segment out of bounds
     */
    unmapSegment(a, 2);   


}

/*
 * Tests our loadSegment function with edgecases, and outputs the result
 * so that we are able to compare what we expect the result to be and
 * what it actually is
 */
void test_Seg_Load()
{
    Seq_T segmentArray;
    
    extern const Except_T Mem_Failed;
    TRY
        segmentArray = Seq_new(5);
    EXCEPT(Mem_Failed)
        fprintf(stderr, "Failed to create Seqence\n");
        exit(EXIT_FAILURE);
    END_TRY;

    for (int i = 0; i < 3; i++) {
        Seq_T segMem;
        extern const Except_T Mem_Failed;
        TRY
            segMem = Seq_new(5);
        EXCEPT(Mem_Failed)
            fprintf(stderr, "Failed to create Seqence\n");
            exit(EXIT_FAILURE);
        END_TRY;

        Segment *temp = malloc(sizeof(Struct Segment));
        temp->data = segMem;
        temp->mapped = true;
        void *useless = Seq_addHi(temp->data, i);
    }
    
    uint32_t* a = 1;
    uint32_t* b = 3;
    uint32_t* c = 0;
    uint32_t* d = 4;

    setWord(b, c, d);
    setWord(b, a, b);

    /* Expect segment 0 to be identical to segment 3 */
    loadSegment(b, c);

    printf("Segment b: %d\n", getWord(b,c));
    printf("Segment c: %d\n", getWord(c,c));

    printf("Segment b: %d\n", getWord(b,a));
    printf("Segment c: %d\n", getWord(c,a));

    /* Load an unmapped segment, expect error */

    loadSegment(d, a);

    /* Load into an unmapped segment, expect segment to be mapped now */

    loadSegment(b, d);

    printf("Segment b: %d\n", getWord(b,c));
    printf("Segment d: %d\n", getWord(d,c));

    printf("Segment b: %d\n", getWord(b,a));
    printf("Segment d: %d\n", getWord(d,a));


}

/*
 * Tests our decodeInstruction function in the Instruction Decoder.
 * Uses threeRegisters to pack an instruction, then sets the result of
 * decodeInstruction to an array to print and check each element.
 *
 */
void test_decode_instruction()
{
    /* Make some instructions, decode them and print results*/

    uint32_t instuct1 = threeRegisters(CMOV, r0,r0,r0);
    
    int* array1 = decodeInstruction(instruct1);
n
    printf("Instruct1 opCode (CMOV): %d\n", array1[0]);
    printf("Instruct1 r0: %d\n", array1[1]);
    printf("Instruct1 r0: %d\n", array1[2]);
    printf("Instruct1 r0: %d\n", array1[3]);
    
    
    
    uint32_t instuct2 = threeRegisters(CMOV, r1,r0,r0);
    
    int* array2 = decodeInstruction(instruct2);
    
    printf("Instruct2 opCode (CMOV): %d\n", array2[0]);
    printf("Instruct2 r0: %d\n", array2[1]);
    printf("Instruct2 r1: %d\n", array2[2]);
    printf("Instruct2 r0: %d\n", array2[3]);

    uint32_t instuct3 = threeRegisters(CMOV, r0,r2,r0);

        
    int* array3 = decodeInstruction(instruct3);

    printf("Instruct3 opCode (CMOV): %d\n", array3[0]);
    printf("Instruct3 r0: %d\n", array3[1]);
    printf("Instruct3 r2: %d\n", array3[2]);
    printf("Instruct3 r0: %d\n", array3[3]);


    uint32_t instuct4 = threeRegisters(CMOV, r0,r0,r3);

        
    int* array4 = decodeInstruction(instruct1);

    printf("Instruct4 opCode (CMOV): %d\n", array4[0]);
    printf("Instruct1 r0: %d\n", array4[1]);
    printf("Instruct1 r0: %d\n", array4[2]);
    printf("Instruct1 r3: %d\n", array4[3]);

    
    uint32_t instuct5 = threeRegisters(ADD, r0,r0,r0);

        
    int* array5 = decodeInstruction(instruct5);

    printf("Instruct5 opCode (ADD): %d\n", array5[0]);
    printf("Instruct5 r0: %d\n", array5[1]);
    printf("Instruct5 r0: %d\n", array5[2]);
    printf("Instruct5 r0: %d\n", array5[3]);

    uintt32_t instruct6 = loadval(r4, 30987);

    int* array6 = decodeInstruction(instruct6);

    printf("Instruct6 opcode (LV): %d\n", array6[0]);
    printf("Instruct6 r4: %d\n", array6[1]);
    printf("Instruct6 value (30987): %d\n", array6[2]);
}