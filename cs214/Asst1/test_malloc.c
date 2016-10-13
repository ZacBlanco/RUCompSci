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

  return 0;
  
  
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
