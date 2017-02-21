#ifndef __MINIMAX_H__
#define __MINIMAX_H__
#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <limits.h>



/**
* Writes the min, max, and sum into (int* results)
* results should be at minimum 3*sizeof(int).
* @param const int* the integer array
* @param int number of items to read
* @param int* results
*/
void minsmax(const int* data, int len, int* results);

/**
* Prints the min, sum, and max of an array of numbers as
* listed within the file
* @param char* the file to read 
*/
void main_minsmax(char* file);

/**
* Prints the min, sum, and max of an array of numbers as
* listed within the file. Creates subprocesses to calculate
* within the recurse_minsmax_helper method.
* @param char* the file to read
* @param int the number of processes ( > 0 )
*/
void main_recurse_minsmax(char* file, int num_proc);

/**
* if num_proc > 0 then the process will 
* listed within the file
* @param int* pointer to integers
* @param num_proc number of processes
* @param int the num of items to process from int pointer
* @param the pipe to write the min/max/sum back to
*/
void recurse_minsmax_helper(int* data, int num_proc, int data_length, int pipe);

#endif