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
  struct stats stat = main_minsmax("./test/t1.txt");
  assert(stat.min == 5);
  assert(stat.max == 55);
  assert(stat.sum == 123);
}

void testminsmax_recurse() {
  struct stats stat = main_recurse_minsmax("./test/t1.txt", 1);
  assert(stat.min == 5);
  assert(stat.max == 55);
  assert(stat.sum == 123);
  stat = main_recurse_minsmax("./test/t1.txt", 2);
  assert(stat.min == 5);
  assert(stat.max == 55);
  assert(stat.sum == 123);
  // ====================== t2.txt ===================== //
  stat = main_recurse_minsmax("./test/t2.txt", 1);
  assert(stat.min == -814);
  assert(stat.max == 40);
  assert(stat.sum == 0);
  stat = main_recurse_minsmax("./test/t2.txt", 2);
  assert(stat.min == -814);
  assert(stat.max == 40);
  assert(stat.sum == 0);
  stat = main_recurse_minsmax("./test/t2.txt", 3);
  assert(stat.min == -814);
  assert(stat.max == 40);
  assert(stat.sum == 0);
  stat = main_recurse_minsmax("./test/t2.txt", 4);
  assert(stat.min == -814);
  assert(stat.max == 40);
  assert(stat.sum == 0);
  stat = main_recurse_minsmax("./test/t2.txt", 5);
  assert(stat.min == -814);
  assert(stat.max == 40);
  assert(stat.sum == 0);
  stat = main_recurse_minsmax("./test/t2.txt", 6);
  assert(stat.min == -814);
  assert(stat.max == 40);
  assert(stat.sum == 0);
  print_stats(stat);
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

void createTestFile() {
  
}