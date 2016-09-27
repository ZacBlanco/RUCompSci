#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char memblock[5000];
static char allocated[5000];
static int count = 0;

// A nice round number to use for bucketing algorithm
// Downfall is that we can't really allocate > 625 bytes
static int buckets = 8;
static int c_bucket = 0; // Current bucket



char *mymalloc(int size, char* file, int line) {
  
  return &memblock[0];
}

void myfree(char* x, char* file, int line) {
  
}

void print_file(char* filename) {
  printf("Error in file: %s\n", filename);
  return;
}

void print_line(int line) {
  printf("Occurred on line No.: %d\n", line);
  return;
}




