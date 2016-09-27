#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mymalloc.h"



int main() {
	printf("Running Tests\n");
  char* a = (char*)malloc(sizeof(char)*5);
  printf("Ptr1: %p\n", a);
  int i;
  for(i = 0; i < 5; i++) {
    a[i] = 'x';
  }
  char b[]  = "help\0";
  a[0] = b[0];
  a[1] = b[1];
  //a[2] = b[5];
  printf("A: %s\n", a);
  printf("Ptr2: %p\n", a);
  char *c = (char *)malloc(sizeof(char) * 10);
  free(a);
  free(c);

  return 0;
  
  
}
