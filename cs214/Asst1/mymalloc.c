#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MEM_SIZE (3500)

static char memblock[MEM_SIZE*2];
static int count = 0;

// A nice round number to use for bucketing algorithm
// Downfall is that we can't really allocate > MEM_SIZE/buckets bytes
// because we fall into other buckets which have a high
// probability of already being allocated.
static int buckets = 5;
static int c_bucket = 0; // Current bucket

void *mymalloc(size_t size, char* file, int line) {
  
  //Reset the counter when we've hit the bucket limit
  if(c_bucket % buckets == 0) {
    c_bucket = 0;
  }

  //Handle case <= 0 or > MEM_SIZE;
  if (size <= 0 || size > MEM_SIZE){
    return NULL;
  }
  
  // Do a search for contiguous block of _size
  // start at (c_bucket)*(bucket_size);
  
  int search_start = (c_bucket % buckets)*get_bucket_size(); //Search start
  // printf("Search Start @ %i\n", search_start);
  int added = 0; // The amount of memory space we've traversed searching for a free contiguous block
  while (added < MEM_SIZE) {
    int s = search_start % MEM_SIZE; // Mod the search start to make sure we don't go past MEM_SIZE
    // printf("Search Start @ %i\n", s); //DEBUG
    if (isAllocated(s, size)) {
      //printf("Able to allocate %zu blocks at %i in mem\n", size, s); //DEBUG
      allocate_mem(s, size);
      //printf("Allocated memory\n"); //DEBUG
      c_bucket++;
      //printf("Return Pointer: %p\n", &memblock[s]); //DEBUG
      //printf("Allocated memspace: %s\n", &allocated[s]); //DEBUG
      return &memblock[s];
    } else {   
      //printf("isAllocated @ location %i returned False\n", s); //DEBUG
      added += size;
      search_start += size;
    }
  }
  printf("Failed to allocate %zu byte(s)\n", size); //DEBUG
  print_file(file);
  print_line(line);
  return NULL;
  
}


void myfree(void* x, char* file, int line) {
    int free_count = 0;
    //Let's call memblock[3500] == allocated[0]
    //Allocated data is not located at memblock indexes starting @ 3500
    int index = x - (void *)&memblock;
    //printf("Free @ Index: %d; Allocated: %c \n", index, memblock[index + MEM_SIZE]);

    if (memblock[index + MEM_SIZE] == 'o') {
        memblock[index] == '\0';
        count--;
        //printf("Freed one byte\n"); //DEBUG
    } else if (memblock[index + MEM_SIZE] == 'b') {
        while(memblock[index + MEM_SIZE] != 'e') {
            memblock[index + MEM_SIZE] = '\0';
            free_count++;
            index++;
        }
        memblock[index + MEM_SIZE] = '\0'; // Free the very last byte.
        free_count++;
        //printf("Free'd %d bytes\n", free_count); //DEBUG
        count -= free_count;
    } else if (memblock[index + MEM_SIZE] == '\0') {
        printf("Error. Trying to free unallocated memory.\n");
        print_file(file);
        print_line(line);
    } else {
      printf("Error. Trying to free incorrect pointer.\n");
      print_file(file);
      print_line(line);
    }
  
}

int get_bucket_size() {
  return MEM_SIZE/buckets;
}

//Return whether or not we can allocate a block @
// index of _start_ and of size _size
int isAllocated(int start, int size) {
  if (start < 0 || start > MEM_SIZE) {
    //printf("Out of bounds for allocation. Return 0;\n"); //DEBUG
    return 0;
  }
  
  int i = start;
  //printf("isAllocated @ Index %i: Value: %i; Search Size: %i\n", start, (int)memblock[i+MEM_SIZE], size); //DEBUG
  while(i - start <= size) {
    //Break if we go over the bounds
    if ( (int)memblock[i + MEM_SIZE] != 0 || i >= MEM_SIZE) {
      // printf("memblock[i + MEM_SIZE] != '\\0' ==> %i\n", (memblock[i + MEM_SIZE] != '\0') );
      // printf("The offending char: \"%i\" \n", (int)memblock[i+MEM_SIZE]);
      // printf("i + start >= MEM_SIZE ==> %i, i+start = %i\n",  (i + start) >= MEM_SIZE, i + start);
      // printf("i = %i; start = %i, size = %i\n", i, start, size);
      //printf("Breaking isAllocated: i: %i; Start: %i\n", i, start);
      break;
    }
    i++;
  }
  //printf("Went until mem location %i \n", i); //DEBUG
  return ((i - start) >= size);
  
}

void allocate_mem(int start, size_t size) {
  
  if (size == 1) {
    memblock[start + MEM_SIZE] = 'o';
  } else {
    memblock[start + MEM_SIZE] = 'b';
    int i;
    for(i = start + 1; i < size + start; i++) {
      memblock[i + MEM_SIZE] = 'x';
    }
    memblock[i + MEM_SIZE] = 'e';
  }
  //print_memblock(start, size);
}

void print_memblock(int start, int size) {
  int i;
  printf("Printing Allocated from %i to %i\n", start, start + size);
  for (i = start; i < size + start; i++) {
    if (memblock[i + MEM_SIZE] == '\0') {
      printf("\\0");
    } else {
      printf("%c", memblock[i + MEM_SIZE]);
    }
  }
  printf("\n");
}

void free_mem(int start, int size) {
  
  if (size == 1) {
    memblock[start + MEM_SIZE] = '\0';
  } else {
    memblock[start + MEM_SIZE] = '\0';
    int i;
    for(i = start+1; i < size + start; i++) {
      memblock[i + MEM_SIZE] = '\0';
    }
    memblock[start + size + MEM_SIZE] = '\0';
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

