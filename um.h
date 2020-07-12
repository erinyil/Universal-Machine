/*
 * Erin Yilmaz and Gustavo Curioso
 * eyilma02 and gcurio01
 * HW 6 (UM)
 * um.h
 */

#include <stdlib.h>
#include <seq.h>
#include <stack.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <bitpack.h>

#include "instructions.h"
#include "segment.h"

#ifndef _UM_H
#define _UM_H


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
extern Seq_T initMainMem(uint32_t instructions[], int numWords);


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
extern Stack_T initReuseMem();


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
extern uint32_t *initRegisters();


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
extern uint32_t *getWord(Seq_T main, uint32_t ID, uint32_t offset);


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
extern void freeAllMem(Seq_T main, Stack_T reuse, uint32_t *registers);


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
extern void runProgram(Seq_T main, Stack_T reuse, uint32_t *registers,
                uint32_t *progCount, uint32_t numInstructs);

#endif
