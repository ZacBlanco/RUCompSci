#include <string.h>
#include <stdio.h>
#include "mymalloc.h"

void fail(char* msg);
void assert(int condition, char* msg);
void println(char* msg);
void pass_msg(char* msg);

void TestBasicMalloc();
void TestOverMalloc();
void TestBasicFree();
void TestNullAndZero();

static int FAILED = 0;




int main() {
	printf("Running Tests\n");

  println("=============================================");
  FAILED = 0;
  TestBasicMalloc();
  pass_msg("TestBasicMalloc");
  println("=============================================");
  
  
  println("=============================================");
  FAILED = 0;
  TestBasicFree();
  pass_msg("TestBasicFree");
  println("=============================================");
  
  
  println("=============================================");
  FAILED = 0;
  TestOverMalloc();
  pass_msg("TestOverMalloc");
  println("=============================================");


  println("=============================================");
  FAILED = 0;
  TestNullAndZero();
  pass_msg("TestNullAndZero");
  println("=============================================");

  return 0;
  
  
}

void TestNullAndZero() {
  
  //Malloc 0
  assert(get_bytes_malloc() == 0, "Starting bytes malloc should be zero");
  char* a = malloc(0);
  assert(a == NULL, "Char must be NULL after 0 Malloc");

  //Malloc NULL (should be the same)
  assert(get_bytes_malloc() == 0, "bytes after failed malloc should still be 0");
  a = malloc(-1);
  assert(a == NULL, "Char must be NULL after 0 Malloc");

  //Free a null pointer
  char* b = NULL;
  free(b);
  assert(b == NULL, "b should not change");
  assert(get_bytes_malloc() == 0, "Bytes mallocd should still be 0");

  //Free a pointer twice
  b = malloc(sizeof(char)*10);
  assert(get_bytes_malloc() == 10, "Bytes mallocd should be 10");
  free(b);
  assert(b != NULL, "b should not change");
  assert(get_bytes_malloc() == 0, "Bytes mallocd should still be 0");
  free(b);
  assert(get_bytes_malloc() == 0, "Bytes mallocd should still be 0");
  assert(b != NULL, "b should not change");

  //Free pointer outside of memory range
  int p = 2342;
  free(&p);

  //Free non-beginning pointer
  char* x = malloc(sizeof(char)*20);
  assert(get_bytes_malloc() == 20, "Bytes malloc'd should be 20");
  char* x2 = x+1;
  // printf("x2: %p, x: %p, Diff %i\n", x2, x, x-x2);
  assert((int)(x2-x) == 1, "Difference in pointers should be 1");
  free(x2);
  assert(get_bytes_malloc() == 20, "Shouldn't be able to free a pointer that isn't the start of a memory block");
  free(x);
  assert(get_bytes_malloc() == 0, "All memory should be freed now");



}

//Test a single malloc call
// Make sure 2 consecutive bytes have pointers of distance 625
void TestBasicMalloc() {
  
  char* a = malloc(sizeof(char));
  char* b = malloc(sizeof(char));

  int space = (b) - (a);
  assert(a != NULL, "a should not be null");
  assert(b != NULL, "b should not be null");
  assert(b - a == 500, "Pointers not spaced by 500 bytes");

  char c = 'a';
  char d = 'b';

  *a = c;
  *b = d;
  assert(*a == 'a', "Char does not hold the correct value");

  // printf("Bytes Malloc'd: %i\n", get_bytes_malloc());
  free(a);
  assert(get_bytes_malloc() == 1, "Only 1 byte should be malloc'd");

  free(b);
  assert(get_bytes_malloc() == 0, "No bytes should be malloc'd");

  char* e = malloc(sizeof(char)*10);
  assert(get_bytes_malloc() == 10, "10 bytes should be malloc'd");
  char* f = malloc(sizeof(char)*10);
  assert(get_bytes_malloc() == 20, "20 bytes should be malloc'd");
  char* g = malloc(sizeof(char)*10);
  assert(get_bytes_malloc() == 30, "30 bytes should be malloc'd");

  char* h = malloc(sizeof(char)*40);
  assert(get_bytes_malloc() == 70, "70 bytes should be malloc'd");

  char* i = malloc(sizeof(char)*700);
  assert(get_bytes_malloc() == 770, "Should still be able to malloc 700 bytes");


  free(e);
  free(f);
  free(g);
  free(h);
  free(i);
  assert(get_bytes_malloc() == 0, "No bytes should be malloc'd");
}

void TestBasicFree(){
  char* a = malloc(sizeof(char));
  *a = 'b';
  free(a);
  assert(*a == '\0', "Memory not nulled on free");
  assert(get_bytes_malloc() == 0, "No bytes should be malloc'd");
}

void TestOverMalloc(){
  
  char* a = malloc(sizeof(char)*2501);
  assert(a == NULL, "Malloc of 2501 bytes should be null");
  assert(get_bytes_malloc() == 0, "No bytes should be malloc'd");
  
  a = malloc(sizeof(char)*2500);
  assert(a != NULL, "Malloc of 2500 bytes should pass");

  char* b = malloc(sizeof(char));
  assert(b == NULL, "Should not allocate a byte over 2500");

  free(a);
  free(b);
  // printf("BM: %i\n", get_bytes_malloc());
  assert(get_bytes_malloc() == 0, "No bytes should be malloc'd");

}


void fail(char* msg) {
  printf("FAILED: %s\n", msg);
}

void println(char* msg) {
  printf("%s\n", msg);
}

void assert(int condition, char* msg) {
  if(!condition) {
    fail(msg);
    FAILED = 1;
  } 
}

void pass_msg(char* test_name){
  if(!FAILED) {
    printf("%s passed \u221A \n", test_name);
  } else {
    printf("%s FAILED \u00D7\n", test_name);
  }
}
