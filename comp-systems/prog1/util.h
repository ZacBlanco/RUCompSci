#ifndef __PUTIL__
#define __PUTIL__
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

/**
 * Reads an input file line by line. Assumes input is a single integer on every line. 
 * @param char* The name of the file
 * @param int** The pointer which we are going to store out int array at. If passed as null it must be freed later.
 * @return The length of ints
 */
int readFile(char* filename, int** ints);

/**
 * Opens/creates a new file and writes a list of values into it line by line.
 * @param char* The name of the file to open or be created
 * @param int* The pointer to the list of numbers
 * @param int The length of the list of numbers
 * @return Boolean representing write success or failure
 */
bool writeFile(char* filename, int* ints, int ints_len);

#endif