#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char memblock[5000];
static char allocated[5000];
static int count = 0;

char *mymalloc(int size, char* file, double line) {

    printf("Printing current memory address from mymalloc: %p\n", &memblock[0]);
    count += size;
    return &memblock[0];
}

void myfree(char* x, char* file, double line) {
  

}

