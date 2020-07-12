/*
 * Erin Yilmaz and Gustavo Curioso
 * eyilma02 and gcurio01
 * HW 6 (UM)
 * main.c
 */

#include <stdio.h>
#include <sys/stat.h>
#include "um.h"

#define WORDSZ 4
#define REGISTERS 8


FILE *openFile(char *input);
int getNumWords(char *filename);
void readInstructs(FILE *fp, uint32_t instructions[], int numWords);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./um [filename]\n");
        exit(EXIT_FAILURE);
    }

    int numWords = getNumWords(argv[1]);

    FILE *fp = openFile(argv[1]);
    uint32_t instructions[numWords];

    readInstructs(fp, instructions, numWords);

    Seq_T mainMem = initMainMem(instructions, numWords);
    Stack_T reuseMem = initReuseMem();
    uint32_t *registers = initRegisters();
    uint32_t *progCount = getWord(mainMem, 0, 0);

    runProgram(mainMem, reuseMem, registers, progCount, numWords);

    freeAllMem(mainMem, reuseMem, registers);

    fclose(fp);
}


/*
 * Function: openFile
 *
 * Parameters: char *input - name of file to be opened
 *
 * Returns: FILE
 *
 * Does: opens the specified file and returns it opened
 *
 * Error Cases: This function will raise a CRE if the file specified
 *              can't be opened
 */
FILE *openFile(char *input)
{
    FILE *fp;
    fp = fopen(input, "rb");
    assert(fp != NULL);
    return fp;
}


/*
 * Function: getNumWords
 *
 * Parameters: char *filename
 *
 * Returns: int
 *
 * Does: Returns the number of 32-bit words in the file provided
 *
 * Error Cases: There are no CREs in this function
 */
int getNumWords(char *filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size / WORDSZ;
}


/*
 * Function: readInstructs
 *
 * Parameters: File *fp - file to read instructions from
 *             uint32_t instructions[] - array to be populated
 *             int numWords - number of instructions to be read
 *
 * Returns: nothing
 *
 * Does: Reads instructions from file specified and stores them in
 *       array passed in as parameter
 *
 * Error Cases: It is a CRE to pass a NULL file pointer as a parameter
 */
void readInstructs(FILE *fp, uint32_t instructions[], int numWords)
{
    assert(fp != NULL);
    uint32_t byte;
    uint32_t word;
    for (int i = 0; i < numWords; i++) {
        word = 0;
        for (int numBytes = 3; numBytes >= 0; numBytes--) {
            byte = fgetc(fp);
            assert((int)byte != EOF);
            byte = byte << (numBytes * 8);
            word = word | byte;
        }
        instructions[i] = word;
    }
}
