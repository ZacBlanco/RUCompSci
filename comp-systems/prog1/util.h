#ifndef __PUTIL__
#define __PUTIL__
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <string.h>


/**
 * Reads an input file line by line. Assumes input is a single integer on every line. 
 * @param char* The name of the file
 * @param int** The pointer which we are going to store out int array at. If passed as null it must be freed later.
 * @return The length of ints
 */
int readFile(char* filename, int** ints);


#endif
