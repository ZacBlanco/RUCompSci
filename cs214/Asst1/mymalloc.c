#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char memblock[5000];

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




