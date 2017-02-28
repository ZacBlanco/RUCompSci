#include <assert.h>
#include "util.h"
#include "minsmax.h"

void testminsmax_iter_recurse();
void testReadFile();
void testminsmax_main();
void testminsmax_recurse();
void testminsmax_iter();

int main() {
  testReadFile();
  testminsmax_main();
  testminsmax_recurse();
  testminsmax_iter();
  testminsmax_iter_recurse();
}

void testminsmax_main() {
  struct stats stat = main_minsmax("./test/t1.txt");
  assert(stat.min == 5);
  assert(stat.max == 55);
  assert(stat.sum == 123);
}

void testminsmax_recurse() {
  // ====================== t1.txt ===================== //
  struct stats stat;
  const struct stats t1 = {5, 55, 123};
  int max_proc = 10;
  int i;
  for (i = 1; i < max_proc; i++) {
    stat = main_recurse_minsmax("./test/t1.txt", i);
    assert(stat.min == t1.min);
    assert(stat.max == t1.max);
    assert(stat.sum == t1.sum);
  }
  // ====================== t2.txt ===================== //
  const struct stats t2 = {-814, 40, 0};
  for (i = 1; i < max_proc; i++) {
    stat = main_recurse_minsmax("./test/t2.txt", i);
    assert(stat.min == t2.min);
    assert(stat.max == t2.max);
    assert(stat.sum == t2.sum);
  }
}

void testminsmax_iter() {
  // ====================== t1.txt ===================== //
  struct stats stat;
  const struct stats t1 = {5, 55, 123};
  int max_proc_t1 = 10;
  int i;
  for (i = 1; i < max_proc_t1; i++) {
    stat = main_iter_minsmax("./test/t1.txt", i);
    assert(stat.min == t1.min);
    assert(stat.max == t1.max);
    assert(stat.sum == t1.sum);
  }
  // ====================== t2.txt ===================== //
  const struct stats t2 = {-814, 40, 0};
  int max_proc_t2 = 10;
  for (i = 1; i < max_proc_t2; i++) {
    stat = main_iter_minsmax("./test/t2.txt", i);
    assert(stat.min == t2.min);
    assert(stat.max == t2.max);
    assert(stat.sum == t2.sum);
  }
}

void testminsmax_iter_recurse() {
  // ====================== t1.txt ===================== //
  struct stats stat;
  const struct stats t1 = {5, 55, 123};
  int max_proc = 10;
  int i, j;
  for (i = 1; i < max_proc; i++) {
    for(j = 1; i < max_proc; i++) {
      stat = main_iter_recur_minsmax("./test/t1.txt", i, j);
      assert(stat.min == t1.min);
      assert(stat.max == t1.max);
      assert(stat.sum == t1.sum);
    }
  }
  // ====================== t2.txt ===================== //
  const struct stats t2 = {-814, 40, 0};
  for (i = 1; i < max_proc; i++) {
    for(j = 1; i < max_proc; i++) {
      stat = main_iter_recur_minsmax("./test/t2.txt", i, j);
      assert(stat.min == t2.min);
      assert(stat.max == t2.max);
      assert(stat.sum == t2.sum);
    }
  }
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