#include <assert.h>
#include "util.h"
#include "minsmax.h"

void testReadFile();
void testminsmax_main();
void testminsmax_recurse();

int main() {
  testReadFile();
  testminsmax_main();
  testminsmax_recurse();
}

void testminsmax_main() {
  main_minsmax("./test/t1.txt");
}

void testminsmax_recurse() {
  main_recurse_minsmax("./test/t1.txt", 2);
  main_recurse_minsmax("./test/t2.txt", 3);
  main_recurse_minsmax("./test/t2.txt", 4);
  main_recurse_minsmax("./test/t2.txt", 5);
  main_recurse_minsmax("./test/t2.txt", 6);
}

void testReadFile() {
  int* nums = NULL;
  int len = readFile("./test/t1.txt", &nums);
  int i = 0;
  for(i = 0; i < len; i++) {
    printf("%i ", nums[i]);
  }
  printf("\n");
}