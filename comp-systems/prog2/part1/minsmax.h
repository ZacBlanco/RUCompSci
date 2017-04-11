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
#include <signal.h>

#ifndef PROC_OUT
#define PROC_OUT 0
#endif

#ifndef PRINTLN
#define PRINTLN printf("Line: %i\n", __LINE__);
#endif


struct stats {
  int min;
  int max;
  int sum;
};


extern int main_stats[3];

// Project 2 Part 1
void minsmax_signal_handler(int signal, siginfo_t* data, void* context);

/**
* Print the min/max/sum from struct
* @param struct stats the statistics
*/
void print_stats(struct stats stat);

/**
* Writes the min, max, and sum into (int* results)
* results should be at minimum 3*sizeof(int).
* @param const int* the integer array
* @param int number of items to read
* @param int* results
*/
void minsmax(const int* data, int len, int* results);

// PART A ---------------------------------------------
/**
* Prints the min, sum, and max of an array of numbers as
* listed within the file
* @param char* the file to read 
*/
struct stats main_minsmax(char* file);

// PART B ---------------------------------------------
/**
* Prints the min, sum, and max of an array of numbers as
* listed within the file. Creates subprocesses to calculate
* within the recurse_minsmax_helper method.
* @param char* the file to read
* @param int the number of processes ( > 0 )
*/
struct stats main_recurse_minsmax(char* file, int num_proc);

// PART B ---------------------------------------------
/**
* if num_proc > 0 then the process will create a new processs
* and also calculate the stats for its specific subarray
* @param int* pointer to integers
* @param num_proc number of processes
* @param int the num of items to process from int pointer
* @param the pipe to write the min/max/sum back to
*/
void recurse_minsmax_helper(int* data, int num_proc, int data_length);


// PART C ---------------------------------------------
/**
* Iteration version of IPC where the main process
* creates many child processes.
* @param char* filename
* @param num_proc number of processes
*/
struct stats main_iter_minsmax(char* file, int num_proc);

// PART D ---------------------------------------------
/**
* Iteration+Recursive version of minsmax
* creates many child processes
* @param char* filename
* @param int proc_breadth - number of processes for main process to create
* @param int proc_depth - number of processes for each immediate child to create
*/
struct stats main_iter_recur_minsmax(char* file, int proc_breadth, int proc_depth);

#endif