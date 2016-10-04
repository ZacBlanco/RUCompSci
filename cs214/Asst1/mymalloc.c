#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char memblock[5000];
static char allocated[5000];
static int count = 0;

// A nice round number to use for bucketing algorithm
// Downfall is that we can't really allocate > 625 bytes
// because we fall into other buckets which have a high
// probability of already being allocated.
static int buckets = 8;
static int c_bucket = 0; // Current bucket
static int bucket_size = 625;

void *mymalloc(size_t size, char* file, int line) {
  
  //Handle case <= 0 or > 5000;
  if (size <= 0 || size > 5000){
    return NULL;
  }
  
  // Do a search for contiguous block of _size
  // start at (c_bucket)*(bucket_size);
  
  int search_start = c_bucket*bucket_size; //Search start
  int added = 0;
  while (added < 5000) {
    int s = search_start % 5000;
    //printf("Start search at %i\n", s); //DEBUG
    if (isAllocated(s, size)) {
      //printf("Able to allocated %i blocks at %i in mem\n", size, s); //DEBUG
      allocate_mem(s, size);
      //printf("Allocated memory\n"); //DEBUG
      c_bucket++;
      //printf("Return Pointer: %p\n", &memblock[s]); //DEBUG
      //printf("Allocated memspace: %s\n", &allocated[s]); //DEBUG
      return &memblock[s];
    } else {   
      printf("Failed to allocate memory\n"); //DEBUG
      added += size;
      search_start += size;
    }
  }
  
  return NULL;
  
}


void myfree(void* x, char* file, int line) {
    int free_count = 0;
    int index = x - (void *)&memblock;
    printf("Index: %d and Allocated Index: %c \n", index, allocated[index]);

    if (allocated[index] == 'o') {
        //printf("Freeing one byte\n"); //DEBUG
        memblock[index] == '\0';
        count--;
        x = NULL;
    } else if (allocated[index] == 'b') {
        while(allocated[index] != 'e') {
            allocated[index] = '\0';
            free_count++;
            index++;
        }
        allocated[index] = '\0'; // Free the very last byte.
        free_count++;
        //printf("Free'd %d bytes\n", free_count); //DEBUG
        count -= free_count;
        x = NULL;
    } else if (allocated[index] == '\0') {
        printf("Error. Trying to free unallocated memory.\n");
        print_file(file);
        print_line(line);
    } else {
      printf("Error. Trying to free incorrect pointer.\n");
      print_file(file);
      print_line(line);
    }
  
}

//Return whether or not we can allocate a block @
// index of _start_ and of size _size
int isAllocated(int start, int size) {
  if (start < 0 || start > 5000) {
    //printf("Out of bounds for allocation. Return 0;\n"); //DEBUG
    return 0;
  }
  
  int i = start;
  //printf("Allocated[i]: %i\n", allocated[i] == '\0'); //DEBUG
  while(allocated[i] != '\0' || (i-start <= size)) {
    i++;
  }
  //printf("Went until mem location %i \n", i); //DEBUG
  return ((i - start) >= size);
  
}

void allocate_mem(int start, int size) {
  
  if (size == 1) {
    allocated[start] = 'o';
  } else {
    allocated[start] = 'b';
    int i;
    for(i = start + 1; i < size + start - 1; i++) {
      allocated[i] = 'x';
    } 
    allocated[start + size - 1] = 'e';
  }

  print_memblock(start, size);
}

void print_memblock(int start, int size) {
  int i;
  printf("Printing Allocated from %i to %i", start - 5, start + size - 5);
  for (i = start - 5; i < size + 5; i++) {
    if (allocated[i] == '\0') {
      printf("\\0");
    } else {
      printf("%c", allocated[i]);
    }
  }
}

void free_mem(int start, int size) {
  
  if (size == 1) {
    allocated[start] = '\0';
  } else {
    allocated[start] = '\0';
    int i;
    for(i = start+1; i < size + start; i++) {
      allocated[i] = '\0';
    }
    allocated[start + size] = '\0';
  }
}

void print_file(char* filename) {
  printf("Error in file: %s\n", filename);
  return;
}

void print_line(int line) {
  printf("Occurred on line No.: %d\n", line);
  return;
}

