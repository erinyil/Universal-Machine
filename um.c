/*
 * Erin Yilmaz and Gustavo Curioso
 * eyilma02 and gcurio01
 * HW 6 (UM)
 * um.c
 */

#include "um.h"

#define REGISTERS 8
#define OPWID 4
#define OPLSB 28
#define ALSB 6
#define BLSB 3
#define CLSB 0
#define REGWID 3
#define LOADVALSB 25
#define VALWID 25

#define CMOVE 0
#define SEGLOAD 1
#define SEGSTORE 2
#define ADD 3
#define MULT 4
#define DIV 5
#define NAND 6
#define HALT 7
#define MAP 8
#define UNMAP 9
#define OUTPUT 10
#define INPUT 11
#define LOADPROG 12
#define LOADVAL 13

#define SEQSIZE 25


uint32_t insertWord(Seq_T main, uint32_t ID, uint32_t offset,
                    uint32_t val);
uint32_t extractReg(uint32_t word, char reg);


/*
 * Function: initMainMem
 *
 * Parameters: uint32_t instructions[] - uint32_t array with instructions
 *             int num_words - number of instructions to be inserted
 *
 * Returns: Seq_T
 *
 * Does: Initializes main memory, inserts the instructions array and
 *       Returns the initialized memory (Hanson Sequence)
 *
 * Error Cases: It is a CRE to pass a null pointer as the instructions
 *              array
 */
Seq_T initMainMem(uint32_t instructions[], int numWords)
{
    assert(instructions != NULL);
    Seq_T memory = Seq_new((uint32_t)1 << SEQSIZE);
    uint32_t ID = map(memory, NULL, numWords);
    for (int i = 0; i < numWords; i++) {
        insertWord(memory, ID, i, instructions[i]);
    }
    return memory;
}


/*
 * Function: initReuseMem
 *
 * Parameters: none
 *
 * Returns: Stack_T
 *
 * Does: Initialiizes and returns the reusable (unmapped memory segments)
 *       memory for the  UM (Hanson Stack)
 *
 * Error Cases: There are no CREs for this function
 */
Stack_T initReuseMem()
{
    return Stack_new();
}


/*
 * Function: initRegisters
 *
 * Parameters: none
 *
 * Returns: uint32_t *
 *
 * Does: initializes and returns an array of size 8 to be used as the
 *       registers for the UM. Initializes all array cells to 0 and
 *       returns pointer to array
 *
 * Error Cases: The function will raise a CRE if malloc can not allocate
 *              the necessary space for the registers
 */
uint32_t *initRegisters()
{
    uint32_t *array = malloc(REGISTERS * sizeof(uint32_t));
    assert(array != NULL);

    for (int i = 0; i < REGISTERS; i++) {
        array[i] = 0;
    }
    return array;
}


/*
 * Function: freeAllMem
 *
 * Parameters: Seq_T main - main memory Hanson Sequence
 *             Stack_T reuse - reusable memory Hanson Stack
 *             uint32_t *registers - registers array pointer
 *
 * Returns: nothing
 *
 * Does: unmaps all remaining segment in main memory, then frees the main
 *       memory itself. Frees anything still in the reusable memory, then
 *       frees the reusable memory itself. Frees the registers array.
 *
 * Error Cases: It is a CRE to pass a NULL pointer as any of the
 *              parameters fo this function
 */
void freeAllMem(Seq_T main, Stack_T reuse, uint32_t *registers)
{
    assert(main != NULL && reuse != NULL && registers != NULL);
    int seqLen = Seq_length(main);
    for (int i = 0; i < seqLen; i++) {
        unmap(main, reuse, i);
    }
    Seq_free(&main);
    uint32_t *toFree;
    while (!Stack_empty(reuse)) {
        toFree = Stack_pop(reuse);
        free(toFree);
    }
    Stack_free(&reuse);
    free(registers);
}


/*
 * Function: insertWord
 *
 * Parameters: Seq_T main - main memory
 *             uint32_t ID - identifier of segment to insert word
 *             uint32_t offset - offset of word to insert
 *             uint32_t val - value to be inserted
 *
 * Returns: uint32_t
 *
 * Does: Inserts the given word into the given location, and returns the
 *       old value in that location
 *
 * Error Cases: It is a CRE to pass a NULL pointer for main memory
 *              parameter and to give an ID bigger than the maximum size
 *              for main memory.
 */
uint32_t insertWord(Seq_T main, uint32_t ID, uint32_t offset, uint32_t val)
{
    assert(main != NULL && ID < (unsigned)Seq_length(main));
    Segment segInfo = Seq_get(main, ID);
    uint32_t oldVal = segInfo->seg[offset];
    segInfo->seg[offset] = val;
    return oldVal;
}

/*
 * Function: getWord
 *
 * Parameters: Seq_T main - main memory
 *             uint32_t ID - identifier of segment of word to retrieve
 *             uint32_t offset - offset of word to retrieve inside
 *             segment
 *
 * Returns: uint32_t *
 *
 * Does: returns a pointer to the word at the location specified
 *
 * Error Cases: It is a CRE to pass a NULL pointer for main memory
 *              parameter and to give an ID bigger than the maximum size
 *              for main memory.
 */
uint32_t *getWord(Seq_T main, uint32_t ID, uint32_t offset)
{
    assert(main != NULL && ID < (unsigned)Seq_length(main));
    Segment segInfo = Seq_get(main, ID);
    return &(segInfo->seg[offset]);
}


/*
 * Function: runProgram
 *
 * Parameters:  Seq_T main - main memory
 *              Stack_T reuse - reusable memory
 *              uint32_t *registers - registers array
 *              uint32_t progCount - program counter
 *              uint32_t numInstructs - number of instructions to run
 *
 * Returns: nothing
 *
 * Does: Executes the instructions stored in segment 0
 *
 * Error Cases: It is a CRE to pass in a NULL pointer to any of the first
 *              four parameters of this function
 */
void runProgram(Seq_T main, Stack_T reuse, uint32_t *registers,
                uint32_t *progCount, uint32_t numInstructs)
{
    assert(main != NULL && reuse != NULL && registers != NULL &&
           progCount != NULL);
    uint32_t OP, curr, regA, regB, regC;
    unsigned i = 0;
    while (i < numInstructs) {
        curr = *progCount;
        OP = Bitpack_getu(curr, OPWID, OPLSB);
        if (OP == CMOVE) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            cmove(registers, regA, regB, regC);
        }
        else if (OP == SEGLOAD) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            loadSeg(main, registers, regA, regB, regC);
        }
        else if (OP == SEGSTORE) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            storeSeg(main, registers, regA, regB, regC);
        }
        else if (OP == ADD) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            add(registers, regA, regB, regC);
        }
        else if (OP == MULT) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            mult(registers, regA, regB, regC);
        }
        else if (OP == DIV) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            division(registers, regA, regB, regC);
        }
        else if (OP == NAND) {
            regA = extractReg(curr, 'a');
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            nand(registers, regA, regB, regC);
        }
        else if (OP == HALT) {
            return;
        }
        else if (OP == MAP) {
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            registers[regB] = map(main, reuse, registers[regC]);
        }
        else if (OP == UNMAP) {
            regC = extractReg(curr, 'c');
            unmap(main, reuse, registers[regC]);
        }
        else if (OP == OUTPUT) {
            regC = extractReg(curr, 'c');
            output(registers, regC);
        }
        else if (OP == INPUT) {
            regC = extractReg(curr, 'c');
            input(registers, regC);
        }
        else if (OP == LOADPROG) {
            regB = extractReg(curr, 'b');
            regC = extractReg(curr, 'c');
            numInstructs = loadProg(main, reuse, registers, regB);
            i = registers[regC];
            Segment segInfo = Seq_get(main, 0);
            progCount = &(segInfo->seg[registers[regC]]);
            continue;
        }
        else if (OP == LOADVAL) {
            regA = Bitpack_getu(curr, REGWID, LOADVALSB);
            uint32_t value = Bitpack_getu(curr, VALWID, 0);
            loadVal(registers, regA, value);
        }
        progCount++;
        i++;
    }
}

/*
 * Function: extractReg
 *
 * Parameters: uint32_t word - word to extract register from
 *             char reg - character determining which register, a, b, or
 *             c is to be extracted from the word
 *
 * Returns: uint32_t
 *
 * Does: Extracts specified register from word received and returns it
 *
 * Error Cases: There are no CREs in this function
 */
uint32_t extractReg(uint32_t word, char reg)
{
    if (reg == 'a') {
        return Bitpack_getu(word, REGWID, ALSB);
    }
    else if (reg == 'b') {
        return Bitpack_getu(word, REGWID, BLSB);
    }
    else {
        return Bitpack_getu(word, REGWID, CLSB);
    }
}
