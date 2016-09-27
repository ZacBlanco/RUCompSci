#include <stdlib.h>

//Define the malloc and free directives
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

void* mymalloc(size_t size, char* file, int line);
void myfree(void* ptr, char* file, int line);

int isAllocated(int start, int size);
void allocate_mem(int start, int size);
void free_mem(int start, int size);


void print_file(char* file);
void print_line(int line);
