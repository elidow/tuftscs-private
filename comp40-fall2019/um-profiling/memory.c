/*
 * memory.c
 * 
 * By: Emil Polakiewicz (epolak01) and Eli Dow (edow01)
 * Fall 2019
 * 
 * Purpose: Implementation of the memory in our universal machine
 * 
 */

#include "memory.h"

#define MINREGISTER 0
#define MAXREGISTER 7


struct MemArrayStruct {Seq_T* memory; Seq_T *mapped; uint32_t* instructIndex;
                       Seq_T* unmapped;};

/*void printbytes(void *p, unsigned int len)
{
    unsigned int i;
    unsigned char *cp = (unsigned char *)p;
    for (i = 0; i < len; i++)
    {
        printf("%02X", *cp++);
    }
    printf("\n");
}

void binprintf(int v)
{
    unsigned int mask = 1 << (sizeof(int) * CHAR_BIT - 1);
    while (mask)
    {
        printf("%d", (v & mask ? 1 : 0));
        mask >>= 1;
    }
    printf("\n");
}*/

/* 
 * readFile
 *
 * Parameters: Nothing
 * Returns: Nothing
 * Does: takes in a function that reads a file of our um program
 * loads it into segment 0 
 * CRE: can open file
 */
void* readFile(char *fn)
{
    assert(fn != NULL);
    
    FILE *file;
    file = fopen(fn, "r");

    Seq_T* instructions = malloc(sizeof(*instructions));
    assert(instructions != NULL);

	extern const Except_T Mem_Failed;
	TRY
		*instructions = Seq_new(5);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to create sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;


    int index = 0;
    
    uint32_t *packedInput = NULL;

    while (!feof(file)) {
        packedInput = malloc(sizeof(*packedInput));
        assert(packedInput != NULL);

        //WANT TO CATCH FILE THAT MAY NOT BE OF RIGHT SIZE
        for (int i = 24; i >= 0; i -= 8) {
            int input = fgetc(file);
            //fprintf(stderr, "i is %d and input is %d\n", i, input);
            if (input == EOF) {

                break;
            }

            *packedInput = Bitpack_newu(*packedInput, 8, i, input);
            //if ()
        }

        if (feof(file)) {
          //  fprintf(stderr, "At end of file\n");

           // *packedInput = ~0;

            //CHECK SPEC FOR ERROR
            //assert(0);
            break;
        }
        //}
        //fprintf(stderr, "Instruction # %d\n", index);
        //binprintf((*packedInput));
		
		TRY
			Seq_addhi(*instructions, packedInput);
		EXCEPT(Mem_Failed)
		fprintf(stderr, "Failed to add to sequence\n");
		exit(EXIT_FAILURE);
		END_TRY;
        
        
        index++;
    }

    fclose(file);
    free(packedInput);
    return instructions;
}




/* Makes sequence of segments, and creates segment 0 for program to be
loaded into */

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
void* initializeMemory(char *fn)
{
    assert(fn != NULL);

    MemArray *memoryArray = malloc(sizeof(*memoryArray));
    assert(memoryArray != NULL);

    Seq_T *segArray = malloc(sizeof(*segArray));
    assert(segArray != NULL);

    Seq_T *boolArray = malloc(sizeof(*boolArray));
    assert(boolArray);

    Seq_T *unmappedSeg = malloc(sizeof(*unmappedSeg));
    assert(boolArray);

	extern const Except_T Mem_Failed;
	TRY
		*boolArray = Seq_new(5);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to create sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;
	
	extern const Except_T Mem_Failed;
	TRY
		*segArray = Seq_new(5);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to create sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;

	extern const Except_T Mem_Failed;
	TRY
		*unmappedSeg = Seq_new(5);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to create sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;

    memoryArray->memory = segArray;
    memoryArray->mapped = boolArray;
    memoryArray->instructIndex = malloc(sizeof(*(memoryArray->instructIndex)));
    assert(memoryArray->instructIndex != NULL);
    *(memoryArray->instructIndex) = 0;
    memoryArray->unmapped = unmappedSeg;
    
    Seq_T *instructions = readFile(fn);
    //fprintf(stderr, "File Read\n");

    /* Makes segment 0 */
    uint32_t *c = malloc(sizeof(*c));
    assert(c != NULL);

    //*b = 0;
    *c = Seq_length(*instructions) + 1;

    //mapSegment(memoryArray, b, c);
    uint32_t *segZero = malloc(sizeof(*segZero) * (*c));
    assert(segZero != NULL);

    segZero[0] = *c;

    //fprintf(stderr, "Instructions Length: %d\n", Seq_length(*instructions));
    //fprintf(stderr, "Zero Length: %d\n", *c + 1);

    for (int i = 0; i < (int)*c - 1; i++) {
        uint32_t *newInstruction = Seq_get(*instructions, i);
        //binprintf((*newInstruction));
        //segZero[i+i] = malloc(sizeof(segZero[i+1]))

        (segZero[i + 1]) = *newInstruction;
    }

	TRY
		Seq_addhi(*(memoryArray->memory), segZero);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to add to sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;
	

    bool *zeroMapped = malloc(sizeof(*zeroMapped));
    assert(zeroMapped != NULL);
    *zeroMapped = true;

	TRY
		Seq_addhi(*(memoryArray->mapped), zeroMapped);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to add to sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;
	

    //free(b);
    free(c);

    for (int i = 0; i < Seq_length(*instructions); i++) {
        void *instruct = Seq_get(*instructions, i);
        free(instruct);
    }

    Seq_free(instructions);
    free(instructions);

    return memoryArray;
}

/* 
 * getNextInstruction
 *
 * Parameters: void* containing our memory
 * Returns: pointer to the next instruction
 * Does: Takes in our memory and fetches the next instruction 
 * from segment 0, and returns a pointer to it
 * CRE: memory is not null
 */
uint32_t* getNextInstruction(void * memory)
{
    assert(memory != NULL);
    
    MemArray *memoryArray = memory;

    //fprintf(stderr, "Instruction Index: %d\n", memoryArray->instructIndex + 1);

    uint32_t *zeroSeg = malloc(sizeof(*zeroSeg));
    assert(zeroSeg != NULL);
    *zeroSeg = 0;
    
    //uint32_t *indexSeg = malloc(sizeof(*indexSeg));
    //assert(indexSeg != NULL);
    //*indexSeg = memoryArray->instructIndex;

    *(memoryArray->instructIndex) = *(memoryArray->instructIndex) + 1;

    //fprintf(stderr, "Get word from NEXT INSTRUCTION\n");
    uint32_t nextInstruct = getWord(memoryArray, zeroSeg, memoryArray->instructIndex);
    
    /* Updates instruction pointer to next instruction */
    //memoryArray->instructPtr = getWord(memory, zeroSeg, indexSeg);

    uint32_t *instrPtr = malloc(sizeof(*instrPtr));
    assert(instrPtr != NULL);

    *instrPtr = nextInstruct;

    //fprintf(stderr, "Instruction number: %d\n", *(memoryArray->instructIndex));
    //*(memoryArray->instructIndex) = *(memoryArray->instructIndex) + 1;

    free(zeroSeg);
    //free(indexSeg);

    return instrPtr;
}

/* 
 * addSegment
 *
 * Parameters: void* containing our memory, uint32_t pointer containing
 * size of segment to be made, and int indicating position in which to
 * add segment
 * Returns: Nothing
 * Does: Makes a uint32_t array (segment) of size rc and puts it into
 * position pos in our memory array 
 * CRE: memory is not null, rc is not null
 */
void addSegment(void* array, uint32_t* rc, int pos)
{
    assert(array != NULL);
    assert(rc != NULL);

    MemArray *mem = array;
    Seq_T *segArray = mem->memory;
    Seq_T *mapArray = mem->mapped;

    assert(pos < Seq_length(*segArray));

    uint32_t *seg = calloc(sizeof(*seg), (*rc + 1));
    assert(seg != NULL);

    seg[0] = *rc + 1;

    //fprintf(stderr, "Segment Length in addSeg: %d\n", seg[0]);

    //CAN RAISE MEMFAILED, CATCH
    if (pos == -1) {
        bool *newMap = malloc(sizeof(*newMap));
		assert(newMap);
		
        *newMap = true;

		extern const Except_T Mem_Failed;
		TRY
			seg = Seq_addhi(*segArray, seg);
		EXCEPT(Mem_Failed)
		fprintf(stderr, "Failed to add to sequence\n");
		exit(EXIT_FAILURE);
		END_TRY;
		TRY
			newMap = Seq_addhi(*mapArray, newMap);
		EXCEPT(Mem_Failed)
		fprintf(stderr, "Failed to add to sequence\n");
		exit(EXIT_FAILURE);
		END_TRY;
		
        
    } else {
        free(Seq_get(*segArray, pos));
        seg = Seq_put(*segArray, pos, seg);
        bool *newMap = Seq_get(*mapArray, pos);
        *newMap = true;
    }

}

/* mapSegment
	 *
	 * Parameters: two pointers to unsigned 32 bit integers that are registers
     * and a void pointer to our memory
	 * Returns: Nothing
	 * Does: maps a segment of size rc at first unmapped segment, and sets
     * rc to indicate where it was mapped
	 * CRE: inputs are not null, 
 */
void mapSegment(void* array, uint32_t* rb, uint32_t* rc)
{
    assert(rb != NULL);
    assert(rc != NULL);
    assert(array != NULL);

    MemArray *temp = array;
    Seq_T *segArray = temp->memory;
    Seq_T *boolMap = temp->mapped;
    Seq_T *unmappedSeg = temp->unmapped;

    if (Seq_length(*unmappedSeg) != 0) {
        //fprintf(stderr, "found unmapped segment\n");
        uint32_t *index = Seq_remhi(*unmappedSeg);
        bool *mapped = Seq_get(*boolMap, *index);
        *mapped = true;
        addSegment(temp, rc, *index);
        *rb = *index;
        //fprintf(stderr, "rb in map segment\n", *rb);
        free(index);
        return;
    }

    addSegment(temp, rc, -1);

    *rb = Seq_length(*segArray) - 1;
        //fprintf(stderr, "New Segment in position: %d\n", *rb);
    //}
}

/* unmapSegment
	 *
	 * Parameters: pointer to memory, ptr to uint32_t rc
	 * Returns: Nothing
	 * Does: unmapps uint32_t array at index rc, and adds that index
     * to the queue of unmapped indicies
	 * CRE: index must be less than the length, must not
     * unmap an unmapped segment
 */
void unmapSegment(void* array, uint32_t* rc)
{
    assert(rc != NULL);
    assert(array != NULL);
    assert(*rc != 0);

    MemArray *temp = array;
    Seq_T *segArray = temp->memory;
    Seq_T *boolMap = temp->mapped;
    Seq_T *unmappedSeg = temp->unmapped;


    //fprintf(stderr, "rc is %d\n", *rc);
    //fprintf(stderr, "Segarray length is %d\n", Seq_length(*segArray));
    assert((int)*rc < Seq_length(*segArray));
    //uint32_t *curr = Seq_get(*segArray, *rc);

    //Segment *curr = Seq_get(*segArray, *rc);

    //if (curr != NULL) {
        //free(curr);
    bool *oldVal = Seq_get(*boolMap, *rc);
    
    /* Cannot unmap an unmapped segment */
    if (!*oldVal) {
        assert(0);
    }

    /* Adds to unmapped queue */
    *oldVal = false;
    uint32_t *index = malloc(sizeof(*index));
	assert(index != NULL);
    *index = *rc;

	extern const Except_T Mem_Failed;
	TRY
		Seq_addhi(*unmappedSeg, index);
	EXCEPT(Mem_Failed)
	fprintf(stderr, "Failed to add to sequence\n");
	exit(EXIT_FAILURE);
	END_TRY;
	
    //free(index);

}

/* loadSegmentProg
 *
 * Parameters: pointer to memory, two registers 
 * Returns: Nothing
 * Does: duplicates specified segment at index rb and sets program
 * counter to rc
 * CRE: index must be less than the length, segment struct 
 * 	 cannot be NULL, registers cannot be null
 */
void loadSegmentProg(void *array, uint32_t *rb, uint32_t *rc)
{
    assert(rb != NULL);
    assert(rc != NULL);
    assert(array != NULL);

    MemArray *temp = array;
    Seq_T *segArray = temp->memory;
    Seq_T *boolMap = temp->mapped;

    bool *currBool = Seq_get(*boolMap, *rb);

    /* Error if segment to be duplicated is unmapped */
    if (!*currBool) {
        assert(0);
    }
    //fprintf(stderr, "Instruct Index b4: %d\n", temp->instructIndex);

    /* setting instuction to rc because it is already offset by 1 */
    *(temp->instructIndex) = *rc;
    
    //fprintf(stderr, "Instruct Index after: %d\n", temp->instructIndex);

    //fprintf(stderr, "Duplicating: %d\n", *rb);

    /* If loading segment 0 into segment 0 we don't need to continue */
    if (*rb == 0) {
        return;
    }

    //fprintf(stderr, "NOT loading seg 0\n");
    /* duplicate segment rb, put it into segment 0 */
    uint32_t *curr = Seq_get(*segArray, *rb);
    //fprintf(stderr, "Rb in load program: %d\n", *rb);

    uint32_t *newSeg = malloc(sizeof(*newSeg) * (curr[0]));
    assert(newSeg != NULL);

    /* copies desired segment into new segment */
    for (int i = 0; i < (int)curr[0]; i++) {
        newSeg[i] = curr[i];
        //fprintf(stderr, "Segment index %d: %d\n", i, newSeg[i]);
    }

    /* Put duplicate into segment 0 */
    uint32_t *oldValue = Seq_put(*segArray, 0, newSeg);

    //bool *currBool = Seq_get(*boolMap, *rb);

    bool *newBool = malloc(sizeof(*newBool));
	assert(newBool != NULL);
    *newBool = *currBool;

    currBool = Seq_put(*boolMap, 0, newBool);

    //*(temp->instructIndex) = *rc;

    free(currBool);
    free(oldValue);
}

/* destroyMemory
 *
 * Parameters: pointer to memory
 * Returns: Nothing
 * Does: frees all memory in memory ptr
 * CRE: checks memory is not null
 */
void destroyMemory(void* array)
{
    assert(array != NULL);

    MemArray *temp = array;
    Seq_T *segArray = temp->memory;
    Seq_T *boolMap = temp->mapped;
    Seq_T *unmappedSeg = temp->unmapped;

    /* Free segment sequence and bool sequence */
    for (int i = 0; i < Seq_length(*segArray); i++) {

        uint32_t *curr = Seq_get(*segArray, i);
        if (curr != NULL) {
            free(curr);   
        }

        bool *currBool = Seq_get(*boolMap, i);
        if (currBool != NULL) {
            free(currBool);
        }
        
    }

    /* Free unmapped queue */
    for (int i = 0; i < Seq_length(*unmappedSeg); i++) {
        uint32_t *currUnmapped = Seq_get(*unmappedSeg, i);
        if (currUnmapped != NULL) {
            free(currUnmapped);
        }
    }
    
    Seq_free(unmappedSeg);
    free(temp->unmapped);

    Seq_free(segArray);
    free(temp->memory);

    Seq_free(boolMap);
    free(temp->mapped);
    free(temp->instructIndex);

    free(temp);
}

/* getWord
 *
 * Parameters: pointer to memory, two registers
 * Returns: unsigned 32 bit integer representing a word
 * Does: Accesses word at specified indexes[rb][rc] and returns it
 * CRE: inputs are not null, we are not trying to access an out of 
 * bounds index
 */
uint32_t getWord(void* array, uint32_t* rb, uint32_t* rc)
{
    assert(rb != NULL);
    assert(rc != NULL);
    assert(array != NULL);
    
    MemArray *temp = array;
    Seq_T *segArray = temp->memory;
    Seq_T *boolMap = temp->mapped;

    //WE ARE ACCESSING THE WROMG SEGMEMNT

	//fprintf(stderr, "rb in getWord: %d\n", *rb);

    /* Checking to see rb is in range of segArray */
    assert((int)*rb < Seq_length(*segArray));
    uint32_t *curr = Seq_get(*segArray, *rb);
    assert(curr != NULL);


    /* Checking to see that our current segment is mapped */
    bool *curr_bool = Seq_get(*boolMap, *rb);
    assert(*curr_bool);
    //fprintf(stderr, "rc is %d in getword\n", (*rc));
    //fprintf(stderr, "rb is %d in getword\n", (*rb));
    //fprintf(stderr, "curr length is %d\n", (int)curr[0]);


    /* ACESSING WRONG SEGMENT, RB IS WRONG VALUE */
    //assert(((int)*rc) < (int)curr[0]);

    //fprintf(stderr, "its yo boy, %d at index %d\n", curr[*rb + 1], (*rb+1));
    return curr[*rc];

}

/* setWord
 *
 * Parameters: pointer to memory, and three registers
 * Returns: Nothing
 * Does: Mutator function that changes word at specified index [ra][rb]
 * and sets it to rc
 * CRE: inputs must not be null, and indicies must not be out of bounds
 */
void setWord(void* array, uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
    assert(ra != NULL);
    assert(rb != NULL);
    assert(rc != NULL);
    assert(array != NULL);

    MemArray *temp = array;
    Seq_T *segArray = temp->memory;

    //fprintf(stderr, "Looking in segment number %d\n", *ra);
    //fprintf(stderr, "Segment index %d\n", *rb);
    //fprintf(stderr, "set word: se is %d\n", Seq_length(*segArray));
    
    assert((int)*ra < Seq_length(*segArray));
    uint32_t *curr = Seq_get(*segArray, *ra);
    assert(curr != NULL);


    //fprintf(stderr, "Segment Length: %d\n", (int)curr[0]);
    //fprintf(stderr, "rb in Setword: %d\n", (int)*rb);
    
    assert((int)*rb < (int)curr[0]);
    curr[*rb] = *rc;
}

#undef MINREGISTER
#undef MAXREGISTER 