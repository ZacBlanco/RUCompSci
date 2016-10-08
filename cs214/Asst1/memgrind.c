#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mymalloc.h"

void workloadA();
void workloadB();
void workloadC();
void workloadD(int mem_size);
void workloadE();
void workloadF();
long get_time();

int main() {
  srand(time(NULL));
  float run_times = 100.0f;
  time_t mytime = time(NULL);
  struct timeval tv = {0, 0};
  gettimeofday(&tv, NULL); //Puts time into tv
  //printf("%ld\n", tv.tv_usec);
  long total = 0;
  int i = 0; //Iterator

  //Run Workload A 100 times;
  long init = get_time();
  //printf("Workload A start time: %ld\n", init);
  for (i = 0; i < run_times; i ++) {
    workloadA();    //printf("%ld\n", tv.tv_usec);
  }
  long stop = get_time();
  //printf("Total Time: %ld microseconds\n", stop - init);
  //printf("Workload A Stop Time: %ld\n", stop);
  total = stop - init;
  float mean = ((float)total)/run_times;
  printf("Workload A Runtime: %.6f Microseconds\n", mean);
  
  //Run Workload B 100 times;
  init = get_time();  
  for (i = 0; i < run_times; i ++) {
    workloadB();    //printf("%ld\n", tv.tv_usec);
  }
  stop = get_time();
  //printf("Total Time: %ld microseconds\n", stop - init);
  total = stop - init;
  mean = ((float)total)/run_times;
  printf("Workload B Runtime: %.6f Microseconds\n", mean);
  
  
  
  // //Run Workload C 100 times;=

  init = get_time();
  for (i = 0; i < run_times; i ++) {
    workloadC();    //printf("%ld\n", tv.tv_usec);
  }
  stop = get_time();
  //printf("Total Time: %ld microseconds\n", stop - init);
  total = stop - init;
  mean = ((float)total)/run_times;
  printf("Workload C Runtime: %.6f Microseconds\n", mean);
  
  
  
  // //Run Workload D 100 times;
  init = get_time();
  for (i = 0; i < run_times; i ++) {
    workloadD(MEM_SIZE);    //printf("%ld\n", tv.tv_usec);
  }
  stop = get_time();
  //printf("Total Time: %ld microseconds\n", tv.tv_usec - init);
  total = stop - init;
  mean = ((float)total)/run_times;
  printf("Workload D Runtime: %.6f Microseconds\n", mean);
  
  //Run Workload E 100 times;
  init = get_time();
  for (i = 0; i < run_times; i ++) {
    workloadE();    //printf("%ld\n", tv.tv_usec);
  }
  stop = get_time();
  //printf("Total Time: %ld microseconds\n", tv.tv_usec - init);
  total = stop - init;
  mean = ((float)total)/run_times;
  printf("Workload E Runtime: %.6f Microseconds\n", mean);

  
  //Run Workload F 100 times;
  init = get_time();
  for (i = 0; i < run_times; i ++) {
    workloadF();    //printf("%ld\n", tv.tv_usec);
  }
  stop = get_time();
  //printf("Total Time: %ld microseconds\n", tv.tv_usec - init);
  total = stop - init;
  mean = ((float)total)/run_times;
  printf("Workload F Runtime: %.6f Microseconds\n", mean);
  


  return 0;
}

// Returns a long value representing the time in microseconds since epoch
// Uses gettimeofday to return.
// Multiplies second by 10^6 and adds the microseconds value.
long get_time() {
  struct timeval tv = {0, 0};
  gettimeofday(&tv, NULL);
  return tv.tv_sec*(1000000) + tv.tv_usec;
}

// Case A:
// Malloc 1 byte * 3000
// Free 1 byte * 3000
// Put code below
void workloadA() {

  char *a[3000];
  int i;
  for (i = 0; i < 3000; i++) {
    a[i] = (char*)malloc(sizeof(char)); //sizeof(char) == 1
  }

  for (i = 0; i < 3000; i++) {
    free(a[i]);
  }


}

// Case B:
// for i in 3000:
//   malloc 1 byte
//   free 1 byte
// endfor;
void workloadB() {
  char *b[3000];
  int i;
  for (i = 0; i < 3000; i++) {
    char *b = (char*)malloc(sizeof(char)); //sizeof(char) == 1
    free(b);
  }

}

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
void workloadC() {
  int bm = 0; // Number of bytes malloc'd
  int freed = 0; // Number of bytes freed
  int r;
  char *alloc[3000];
  //Lets make a stack;
  while (freed < 3000 && bm < 3000) {
    r = rand() % 10;
    if (r < 5 || freed >=bm ) {
      alloc[bm] = (char*)malloc(sizeof(char));//Allocates a byte on the next index
      bm++;
    } else {
      free(alloc[freed]); //Frees the index of the last mallocd byte
      freed++;
    }
  }

  int f; //Free the rest.
  for(f = freed; f < 3000; f++){
    free(alloc[f]);
    freed++;
  }
  
}

// Case D:
// Choose between either:
//    A randomly sized malloc
//    A free
// Ensure all pointers are freed
void workloadD(int mem_size) {
  int max_size = 100; //Set a max memory size;
  int bm = 0;
  int freed = 0;
  int totalMalloc = 0;
  char* ptrs[6000];
  int ptrSizes[6000];
  int r = 0; //random int for memory block size
  int r2; //random integer from 0-9
  int s = -1; //counter in case we get stuck
  int i; //normal iterator
  for (i = 0; i < 6000; i++) {
    r2 = rand() % 10;

    //attempt random malloc only if:
    //freed < bm AND totalMalloc < 5000;
    //else free the last pointer in the next iteration.
    int cond =(((r2 < 5 && totalMalloc <= mem_size) || freed >= bm) && bm < MEM_SIZE);

    if (cond) { // Randomly allocate a new block;
      r = (rand() % max_size) + 1; // Produces an int in the range [1, max_size]
      char *p = (char*)malloc(sizeof(char)*r);
      if(p != NULL) {
        ptrs[bm] = p;
        totalMalloc += r;
        ptrSizes[bm] = r;
        bm++;
      } else {
        i--; //Don't count this loop if it didn't work.
      }

    } else if (freed <= bm){ // Free the last block mallocd
      if (freed > bm) {
        printf("Freed: %i; Bytes Mallocd: %i\n", freed, bm);
      }
      free(ptrs[freed]);
      totalMalloc -= ptrSizes[freed];
      freed++;
    } else {
      //Stalemate.....?
      printf("Uh oh we've hit a stalemate between mallocs and freed in case D.\n");
      if (s == -1) {
        s = i;
      }
      if( i - s > 10) {
        break;
      }
    }
    //printf("bm: %6i | freed: %6i | totalMalloc: %6i | i: %5i | s: %5i\n", bm, freed, totalMalloc, i, s);
  }
  //printf("bm: %6i | freed: %6i | totalMalloc: %6i | i: %5i | s: %5i\n", bm, freed, totalMalloc, i, s);
}

// Case E:
// For Exactly 10,000 iterations do the following:
// Randomly Pick one of the 7 following choices:
//  1. Attempt to malloc a large (random) amount of memory (200-1000 bytes)
//  2. Attempt to malloc a small (random) amount of memory (1-100 bytes)
//  3. Malloc a size of 0
//  4. Malloc a number greater than MEMSIZE
//  5. Malloc a number equal to MEMSIZE
//  6. Free a Null pointer
//  7. Free the last malloc'd pointer
//  8. Free a pointer not in the memblock
//
// After the 10k iterations
// Free the last of the pointers 
void workloadE() {
  int i;
  int m_count = 0;
  int f_count = 0;
  char *ptrs[10000];
  char* a;
  for(i = 0; i < 10000;i++) {
    int r = rand() % 8;
    switch (r) {
      case 0: //malloc large
        r = (rand() % 1000) + 1;   
        a = (char*)malloc(sizeof(char)*r);
        ptrs[m_count] = a;
        m_count++;
        break;
      case 1: //malloc small
        r = (rand() % 200) + 1;
        a = (char*)malloc(sizeof(char)*r);
        ptrs[m_count] = a;
        m_count++;
        break;
      case 2: //malloc 0
        a = (char*)malloc(0);
        ptrs[m_count] = a;
        m_count++;
        break;
      case 3: //malloc > MEM_SIZE
        a = (char*)malloc(sizeof(char)*MEM_SIZE+500);
        ptrs[m_count] = a;
        m_count++;
        break;
      case 4: //malloc = MEM_SIZE
        a = (char*)malloc(sizeof(char)*MEM_SIZE);
        ptrs[m_count] = a;
        m_count++;
        break;
      case 5: // Free null
        free(NULL);
        break;
      case 6: // free malloc'd pointer
        free(ptrs[f_count]);
        f_count++;
        break;
      case 7: // Free a malloc outside the memblock
        {
          char *b = "Break me";
          free(b);
          break;
        }
    }
  }

  while(f_count <= m_count) {
    free(ptrs[f_count]);
    f_count++;
  }
}

// Case F:
// Custom test case (TBD)
// for 20000 times:
//   malloc the size of our buckets n times
//   if we get a pointer that isn't null after the 5th iteration (only 5 buckets)
//   we need to break because that memory isn't ours
//   
//   free the mallocs that we made
//
void workloadF() {

    char * ptrs[10000];
    char *current_pointer;
    int iteration = 20;
    int i, j, malloc_count;
    for (j = 0; j < 20000; j++) {
        malloc_count = 0;
        for (i = 0; i < iteration; i++) {
            current_pointer = (char *)malloc(MEM_SIZE / 5);
            if (i > 5 && current_pointer != NULL) {
                printf("Malloc'd memory that wasn't ours");
                break;
            }
            ptrs[malloc_count] = current_pointer;
            malloc_count++;
        }

        for (i = 0; i < malloc_count; i++) {
            free(ptrs[i]);
        }
    }
}

