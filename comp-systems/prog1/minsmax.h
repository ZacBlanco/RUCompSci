#ifndef __MINIMAX_H__
#define __MINIMAX_H__
#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#ifndef PROC_OUT
#define PROC_OUT 0
#endif


struct stats {
  int min;
  int max;
  int sum;
};

/**
* Print the min/max/sum from struct
* @param struct stats the statistics
*/
void print_stats(struct stats stat);

/**
* Generates a random array adhering to the following parameters
* @param const int The minimum value
* @param const int The maximum value
* @param const int The size of the array
* @return The pointer to the array
*/
int* gen_rand_array(const int min, const int max, const int n);

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
struct stats main_minsmax(char* file);

/**
* Prints the min, sum, and max of an array of numbers as
* listed within the file. Creates subprocesses to calculate
* within the recurse_minsmax_helper method.
* @param char* the file to read
* @param int the number of processes ( > 0 )
*/
struct stats main_recurse_minsmax(char* file, int num_proc);

/**
* if num_proc > 0 then the process will create a new processs
* and also calculate the stats for its specific subarray
* @param int* pointer to integers
* @param num_proc number of processes
* @param int the num of items to process from int pointer
* @param the pipe to write the min/max/sum back to
*/
void recurse_minsmax_helper(int* data, int num_proc, int data_length, int pipe);


/**
* Iteration version of IPC where the main process
* creates many child processes.
* @param char* filename
* @param num_proc number of processes
*/
struct stats main_iter_minsmax(char* file, int num_proc);

#endif