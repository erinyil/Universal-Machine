/*
 * Erin Yilmaz and Gustavo Curioso
 * eyilma02 and gcurio01
 * HW 6 (UM)
 * instructions.h
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <seq.h>
#include <stack.h>

#include "segment.h"

#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H


/* Function: map
 *
 * Parameters: the references to the memory sequence and stack, and the number
 *             of words in the segment to be mapped
 *
 * Returns: the ID of the segment of size numWords that was mapped
 *
 * Does: maps a segment of memory of size numWords, initializes its elements
 *       to 0, and inserts the new segment into the main memory sequence
 *
 * Error Cases: it is a CRE to pass in a NULL main memory sequence
 */
extern uint32_t map(Seq_T main, Stack_T reuse, int numWords);


/* Function: unmap
 *
 * Parameters: the references to the memory sequence and stack and the memory
 *             ID to be unmapped
 *
 * Returns: void
 *
 * Does: unmaps the segment with the specified ID and pushes its ID on the
 *       reuse stack
 *
 * Error Cases: It is a CRE to pass in NULL arguments for the memory structures
 *              or an out-of-bounds ID value
 */
extern void unmap(Seq_T main, Stack_T reuse, uint32_t ID);


/* Function: cmove
 *
 * Parameters: the pointer to the registers array and the values of registers
 *             A, B, and C
 *
 * Returns: void
 *
 * Does: if the value in register is nonzero, then the value in register B is
 *       put in register A
 *
 * Error Cases: none
 */
extern void cmove(uint32_t *registers, uint32_t regA, uint32_t regB,
                  uint32_t regC);


/* Function: nand
 *
 * Parameters: the registers and the values of registers A, B, and C
 *
 * Returns: void
 *
 * Does: performs the binary operations & and ~ on the values in registers
 *       B and C and stores the result in register A
 *
 * Error Cases: none
 */
extern void nand(uint32_t *registers, uint32_t regA, uint32_t regB,
                 uint32_t regC);

/* Function: output
 *
 * Parameters: the pointer to the registers and the value of register C
 *
 * Returns: void
 *
 * Does: sends the value in register C to stdout
 *
 * Error Cases: the function can only display values within the ASCII range of
 *              0-255
 */
extern void output(uint32_t *registers, uint32_t regC);


/* Function: input
 *
 * Parameters: the registers and the value of register C
 *
 * Returns: void
 *
 * Does: takes a byte of user input and stores it in register C, otherwise
 *       stores ~0 in register C
 *
 * Error Cases: none
 */
extern void input(uint32_t *registers, uint32_t regC);


/* Function: loadVal
 *
 * Parameters: the reference to the registers, the value of register A, and the
 *             value to be loaded into registers[A]
 *
 * Returns: void
 *
 * Does: loads the specified value into register A
 *
 * Error Cases: none
 */
extern void loadVal(uint32_t *registers, uint32_t regA, uint32_t value);


/* Function: loadProg
 *
 * Parameters: the references to the memory sequence and stack, the pointer
 *             to the registers and the value of regB
 *
 * Returns: the number of new instructions in the loaded program
 *
 * Does: Duplicates the segment referred to by register B and replace the 0
 *       segment's instructions
 *
 * Error Cases: It is a CRE to pass NULL arguments for the memory stack or
 *              sequence or the registers
 */
extern uint32_t loadProg(Seq_T main, Stack_T reuse, uint32_t *registers,
                  uint32_t regB);


/* Function: add
 *
 * Parameters: the reference to the registers and the values of registers A, B
 *             and C
 *
 * Returns: void
 *
 * Does: adds the values in registers B and C and stores the result in reg A
 *
 * Error Cases: It is a CRE to pass a NULL reference to registers
 */
extern void add(uint32_t *registers, uint32_t regA, uint32_t regB,
                uint32_t regC);


/* Function: mult
 *
 * Parameters: the reference to the registers and the values of registers
 *             A, B, and C
 *
 * Returns: void
 *
 * Does: multiplies the values in registers B and C and puts their product in
 *        register A
 *
 * Error Cases: It is a CRE for registers to be NULL
 */
extern void mult(uint32_t *registers, uint32_t regA, uint32_t regB,
                 uint32_t regC);


/* Function: division
 *
 * Parameters: the reference to the registers and the values of registers
 *             A, B, and C
 *
 * Returns: void
 *
 * Does: puts the integer result of dividing the value in register B by the
 *       value in register C into register A
 *
 * Error Cases: It is a CRE to pass in a NULL reference to registers
 */
extern void division(uint32_t *registers, uint32_t regA, uint32_t regB,
                     uint32_t regC);


/* Function: loadSeg
 *
 * Parameters: the reference to the memory sequence, the reference to the
 *             registers, and the values of registers A, B, and C
 *
 * Returns: void
 *
 * Does: puts the word from segment denoted by register B and offset denoted
 *       by register C in register A
 *
 * Error Cases: It is a CRE to pass a NULL main memory sequence or a NULL
 *              reference to registers
 */
extern void loadSeg(Seq_T main, uint32_t *registers, uint32_t regA,
                    uint32_t regB, uint32_t regC);


/* Function: storeSeg
 *
 * Parameters: the reference to the memory sequence, the reference to the
 *             registers, and the value of registers A, B, and C
 *
 * Returns: void
 *
 * Does: puts the word from the segment denoted by register A at offset B into
 *       register C
 *
 * Error Cases: It is a CRE to pass NULL arguments as the main memory reference
 *              or the reference to the registers
 */
extern void storeSeg(Seq_T main, uint32_t *registers, uint32_t regA,
                     uint32_t regB, uint32_t regC);

#endif
