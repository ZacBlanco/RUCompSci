#include <stdlib.h>

//Define the malloc and free directives
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

// MEM_SIZE is the size of the memory we'll be able to store. size(memblock) == MEM_SIZE * 2
#define MEM_SIZE (2500)

// mymalloc and myfree functions
void* mymalloc(size_t size, char* file, int line);
void myfree(void* ptr, char* file, int line);

// Helper function to check if mem is allocated
int isAllocated(int start, int size);

// Function to actually allocate memory (in metadata)
void allocate_mem(int start, size_t size);

// Function which actually frees memory.
void free_mem(int start, int size);


// Helper functions to print errors. 
void print_file(char* file);
void print_line(int line);
void print_memblock(int start, int size);
int get_bytes_malloc();

// Gets the size of a single bucket based on MEM_SIZE
int get_bucket_size();
