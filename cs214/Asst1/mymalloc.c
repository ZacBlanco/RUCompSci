#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

//The memory block.
static char memblock[MEM_SIZE*2];

// A nice round number to use for bucketing algorithm
// Downfall is that we can't really allocate > MEM_SIZE/buckets bytes
// because we fall into other buckets which have a high
// probability of already being allocated.
static int buckets = 5;
static int c_bucket = 0; // Current bucket

// A Variable which should only be modified by the isAllocated method which tells us how far the last search went.
static int isallocated_search_size = 0;

void *mymalloc(size_t size, char* file, int line) {
  
  //Handle case <= 0 or > MEM_SIZE;
  if (size <= 0 || size > MEM_SIZE){
    return NULL;
  }

  //Reset the counter when we've hit the bucket limit
  if(c_bucket % buckets == 0) {
    c_bucket = 0;
  }
  
  // Do a search for contiguous block of _size
  // start at (c_bucket)*(bucket_size);
  
  int search_start = (c_bucket % buckets)*get_bucket_size(); //Search start
  int added = 0; // The amount of memory space we've traversed searching for a free contiguous block
  while (added < MEM_SIZE) {
    
    int s = search_start % MEM_SIZE; // Mod the search start to make sure we don't go past MEM_SIZE
    // printf("Search Start @ %i || Added == %i\n", s, added); //DEBUG
    
    if(s + size > MEM_SIZE) {
      search_start = 0;
      added += (MEM_SIZE-s); //Add the difference between current s and MEM_SIZE to the added because we know that further searches to the end won't be successful
    } else if (isAllocated(s, size)) {

      //Allocate the memory and return the pointer.
      allocate_mem(s, size);
      c_bucket++;
      return &memblock[s];

    } else {   

      added += isallocated_search_size;
      search_start += isallocated_search_size;
      //If isallocated_search_size is greater than MEM_SIZE, set search start back to the beginning.
      if( search_start / MEM_SIZE >= 1) {
        search_start = 0;
      }

    }
  }
  printf("Failed to allocate %zu byte(s)\n", size); //DEBUG
  print_file(file);
  print_line(line);
  return NULL;
  
}


void myfree(void* x, char* file, int line) {
    int free_count = 0;
    //Let's call memblock[MEM_SIZE] == allocated[0]
    //Allocated data is not located at memblock indexes starting @ 3500
    int index = x - (void *)&memblock;
    //printf("Free @ Index: %d; Allocated: %c \n", index, memblock[index + MEM_SIZE]);

    //Make sure that x lies within (0 - MEM_SIZE)
    if(index > MEM_SIZE || index < 0) {
      //Out of bounds error -can't free memory used that wasn't malloc'd
      printf("Unable to free. Pointer was not issued by malloc\n");
      print_file(file);
      print_line(line);
      return;
    }

    if (memblock[index + MEM_SIZE] == 'o') {
        memblock[index] = '\0';
        memblock[index + MEM_SIZE] = '\0';
        free_count++;
        //printf("Freed one byte\n"); //DEBUG
        malloc_count -= free_count;
    } else if (memblock[index + MEM_SIZE] == 'b') {
      memblock[index + MEM_SIZE] = '\0';
      memblock[index] = '\0';
      index++;
      free_count++;
      while (memblock[index + MEM_SIZE] != 'e') {
          memblock[index + MEM_SIZE] = '\0';
          memblock[index] = '\0';
          free_count++;
          index++;
          if(index + MEM_SIZE > MEM_SIZE*2) {
            printf("Went out of bounds of memblock trying to free. Breaking from free-loop\n");
            printf("Index: %i, Index+MEM_SIZE: %i\n", index, index + MEM_SIZE);
            break;
          }
      }
      memblock[index + MEM_SIZE] = '\0'; // Free the very last byte.
      memblock[index] = '\0';
      free_count++;
      //printf("Free'd %d bytes\n", free_count); //DEBUG
      malloc_count -= free_count;
    } else if (memblock[index + MEM_SIZE] == '\0') {
        printf("Free failed. Memory not was not allocated.\n");
        print_file(file);
        print_line(line);
    } else {
      printf("Free Failed. Pointer does not start memory block.\n");
      print_file(file);
      print_line(line);
    }
    // malloc_count -= free_count;

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

  // An integer that tells us if we break due to hitting the MEM_SIZE bounds.
  int broke = 0;
  
  int i = start;
  //printf("isAllocated @ Index %i: Value: %i; Search Size: %i\n", start, (int)memblock[i+MEM_SIZE], size); //DEBUG
  while(i - start < size) {
    //Break if we go over the bounds
    if(i + MEM_SIZE >= MEM_SIZE*2) {
      broke = 1;
      break;

    } else if (memblock[i + MEM_SIZE] != '\0') {
      // printf("memblock[i + MEM_SIZE] != '\\0' ==> %i\n", (memblock[i + MEM_SIZE] != '\0') );
      // printf("The offending char: \"%i\" \n", (int)memblock[i+MEM_SIZE]);
      break;
    }
    i++;
  }

  if(broke) {
    //Set to i-start because i is equal to 5k so we only want to add the amt we searched.
    isallocated_search_size = i-start;
    return 0;
  } else {
    if (i-start == 0) {
      //End up in an infinite allocation loop if i-start = 0;
      isallocated_search_size = 1;
    } else {
      isallocated_search_size = i-start;
    }

    //Return whether it's possible to allocate
    return ((i - start) >= size);
  }
  
}

void allocate_mem(int start, size_t size) {
  
  if (size == 1) {
    memblock[start + MEM_SIZE] = 'o';
    malloc_count++;
  } else {
    memblock[start + MEM_SIZE] = 'b';
    malloc_count++;
    int i;
    for(i = start + 1; i < size + start; i++) {
      memblock[i + MEM_SIZE] = 'x';
      malloc_count++;
    }
    memblock[i + MEM_SIZE - 1] = 'e';
    //printf("Malloc of size %zu\n", size);
    //print_memblock(start, size);
  }
}

//Prints all the metadata of a memory block section
void print_memblock(int start, int size) {
  int i;
  printf("Printing Metadata from %i to %i\n...", start, start + size);
  for (i = start; i < size + start; i++) {
    if (memblock[i + MEM_SIZE] == '\0') {
      printf("\\0");
    } else {
      printf("%c", memblock[i + MEM_SIZE]);
    }
  }
  printf("...\n");
}

//Print the filename
void print_file(char* filename) {
  printf("Error in file: %s\n", filename);
  return;
}

//print the line number.
void print_line(int line) {
  printf("Occurred on line No.: %d\n", line);
  return;
}

//print all of the metadata
void print_meta() {
  int i;
  for(i = 0; i < MEM_SIZE; i++) {
    if(memblock[i + MEM_SIZE] == '\0') {
      printf("\\0");
    } else {
      printf("%c", memblock[i + MEM_SIZE]);
    }
  }
  printf("\n");
}
//return the number of bytes which have been allocated by our malloc.
int get_bytes_malloc(){
  return malloc_count;  
}

