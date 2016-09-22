#include <stdio.h>
#include "mymalloc.h"

int main() {
  printf("Hello, World!\n");
	
	//Test Cases
	
	// Case A:
	// Malloc 1 byte * 3000
	// Free 1 byte * 3000
  // Put code below
	
	
	// Case B:
	// for i in 3000:
	//   malloc 1 byte
  //   free 1 byte
	// endfor;
  // Put code below
  
  // Case C:
  // bytes malloc'd = 0;
  // bytes freed = 0;
  // while freed < 3000 and malloc'd < 3000:
  //   r <- rand();  
  //   if r < .5 && mallocd > freed: //ensures we always malloc more than free.
  //     malloc something
  //     malloc <- malloc+1
  //   else
  //     free the last malloc space
  //     free <- free+1
  
  // for f in 3000-freed: //rest of the non-free pointers
  //   free f;
  // Put code below
  
  
  // Case D:
  // Choose between either:
  //    A randomly sized malloc
  //    A free
  // Ensure all pointers are freed
  // Put code below
  
 

  // Case E:
  // Custom test case (TBD)
  
  
  // Case F:
  // Custom test case (TBD)
  

  return 0;
}

