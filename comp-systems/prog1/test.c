#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include "util.h"
#include "minsmax.h"

void testReadFile();
void testminsmax_main();         // Test Part A
void testminsmax_recurse();      // Test Part B
void testminsmax_iter();         // Test Part C
void testminsmax_iter_recurse(); // Test Part C
void createTestFile(); //Utility
void deleteFile(); //Utility
void timing_test(); //Utility

int main() {
  char* file_name = "test_nums.txt";
  testReadFile();
  testminsmax_main();
  testminsmax_recurse();
  testminsmax_iter();
  testminsmax_iter_recurse();
  createTestFile(file_name, -RAND_MAX/2, RAND_MAX/2, 10000);
  deleteFile(file_name); // At the end
  printf("Timing test:\n");
  int i;
  for(i = 1; i <= 21; i++) {
    printf("===== TIMING TEST FOR %i processes =====\n", i);
    timing_test(i);
    printf("=======================================\n");
  }
}

long get_time() {
  struct timeval tv = {0, 0};
  gettimeofday(&tv, NULL);
  return tv.tv_sec*(1000000) + tv.tv_usec;
}

void timing_test(const int num_proc) {
  int tens = 5;
  char* file_name = "timing_test.txt";
  int i;
  int tn = 1;
  for (i = 1; i <= tens; i++) {
    tn *= 10;
    createTestFile(file_name, -RAND_MAX/2, RAND_MAX/2, tn);
    long start, diff;
    // Test single version
    start = get_time();
    main_minsmax(file_name);
    long single_time = get_time() - start;

    // Test recursive version
    start = get_time();
    main_recurse_minsmax(file_name, num_proc);
    long recursive_time = get_time() - start;

    // Test iterative version
    start = get_time();
    main_iter_minsmax(file_name, num_proc);
    long iterative_time = get_time() - start;

    // Test optimized version
    start = get_time();
    // Put function here
    main_iter_recur_minsmax(file_name, num_proc, num_proc);
    long optimized_time = get_time() - start;

    // Print out stats
    printf("Tests for size %d\n", tn);
    printf("\tSingle version time: \t\t%ld microseconds\n", single_time);
    printf("\tRecursive version time: \t%ld microseconds\n", recursive_time);
    printf("\tIterative version time: \t%ld microseconds\n", iterative_time);
    printf("\tOptmized version time: \t\t%ld microseconds\n\n", optimized_time);
  }
  deleteFile(file_name);
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
  for (i = 1; i <= max_proc; i++) {
    stat = main_recurse_minsmax("./test/t1.txt", i);
    assert(stat.min == t1.min);
    assert(stat.max == t1.max);
    assert(stat.sum == t1.sum);
  }
  // ====================== t2.txt ===================== //
  const struct stats t2 = {-814, 40, 0};
  for (i = 1; i <= max_proc; i++) {
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
  for (i = 1; i <= max_proc_t1; i++) {
    stat = main_iter_minsmax("./test/t1.txt", i);
    assert(stat.min == t1.min);
    assert(stat.max == t1.max);
    assert(stat.sum == t1.sum);
  }
  // ====================== t2.txt ===================== //
  const struct stats t2 = {-814, 40, 0};
  int max_proc_t2 = 10;
  for (i = 1; i <= max_proc_t2; i++) {
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
  for (i = 1; i <= max_proc; i++) {
    for(j = 1; i <= max_proc; i++) {
      stat = main_iter_recur_minsmax("./test/t1.txt", i, j);
      assert(stat.min == t1.min);
      assert(stat.max == t1.max);
      assert(stat.sum == t1.sum);
    }
  }
  // ====================== t2.txt ===================== //
  const struct stats t2 = {-814, 40, 0};
  for (i = 1; i <= max_proc; i++) {
    for(j = 1; i <= max_proc; i++) {
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

void createTestFile(char* file_name, const int min_bound, const int max_bound, const int list_size) {
  time_t t; 
  srand((unsigned) time(&t));
  int i;
  int min = (rand() % (max_bound-min_bound+1)) + min_bound;
  int max = (rand() % (max_bound-min_bound+1)) + min_bound;
  if (min > max) {
    int t = max;
    max = min;
    min = t;
  }

  int* nums = NULL;
  nums = gen_rand_array(min, max, list_size);
  int stats[3];
  minsmax(nums, list_size, stats);
  assert(stats[0] == min);
  assert(stats[1] == max);
  writeFile(file_name, nums, list_size);
}

void deleteFile(char* file_name) {
  remove(file_name);
}