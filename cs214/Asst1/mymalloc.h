#include <stdlib.h>

//Define the malloc and free directives
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

char* mymalloc(int size_t, char* file, int line);
void myfree(char* ptr, char* file, int line);


void print_file(char* file);
void print_line(int line);