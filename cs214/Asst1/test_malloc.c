#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"



int main() {
	printf("Running Tests\n");


	printf("Test Print\n");

    char *test = (char *)malloc(5);
    printf("Current memory address after using mymalloc: %p\n", test);
    test = "help";
    printf("Test: %s\n", test);
    printf("Test ptr: %p\n", test);
    
  
  return 0;
  
  
}
