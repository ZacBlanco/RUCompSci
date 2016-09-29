

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Calculates a value (c) based on the input parameters (a, b) and prints
 * out the result.
 *
 * @param a
 *     Input parameter a.
 *
 * @param b
 *     Input parameter b.
 */
void one(const int a, const int b) {
  int c = (a * a) + (b * b);
  printf("%d^2 + %d^2 = %d\n", a, b, c);
}

/**
 * Checks if the input parameter is a passing grade and prints out if
 * the grade is passing.
 *
 * @param grade
 *     The grade to check.
 */
void two(const char *grade) {
  // you may find the atoi function useful
  int gr = atoi(grade);
  if ( gr > 70) {
    printf("%d passed!\n", gr);
  } else {
    printf("%d not passed!\n", gr);
  }
}

/**
 * Assigns a pointer (int *p) the value of a stack variable (int x).
 */
void three() {
  int x = 4;
  int *p = &x;

  printf("The value of p is: %d\n", *p);
}

/**
 * Prints out a specific message based on if the number is
 * between 0 and 1 (exclusive).
 *
 * @param value
 *     Value to test.
 */
void four(const float value) {
  if (0 < value && value < 1) {
    printf("The value is between zero and one.\n");
  }else {
      printf("The value is not between zero and one.\n");
  }
}

/**
 * Prints out a specific message based on if the two input parameters
 * are equal.
 *
 * @param x
 *     First input parameter.
 *
 * @param y
 *     Second input parameter.
 */
void five(const int *x, const int *y) {
  if (*x == *y)
    printf("x and y are equal.\n");
  else
    printf("x and y are different.\n");
}

/**
 * Returns a new pointer (float *p) which contains the value of the
 * input pointer (int *x).
 *
 * @param x
 *     Input parameter, whose value will be returned as a (float *).
 *
 * @returns
 *     A new pointer, allocated on the heap and not freed, that
 *     contains the value of the input parameter.
 */
float *six(const int *x) {
  int x1 = *x;

  float p = (float)x1;
  float* addr = (float*)malloc(sizeof(float));
  *addr = p;
  return addr;
}

/**
 * Takes an input a and checks whether a is an alphabet, it can be
 * both uppercase and lowercase
 *
 * @param a
 *     Input parameter a, which is a char*
 *
 */
void seven(const char *a) {
  if ( (*a >= 'A' && *a <= 'Z') || (*a >= 'a' && *a <= 'z') )
    printf("a is a letter.\n");
  else
    printf("a is not a letter.\n");
}

/**
 * Constructs a C-string, character by character, and prints out the full
 * string "Hello".
 */
void eight() {
  char *s = malloc(5);

  s[0] = 'H';
  s[1] = 'e';
  s[2] = 'l';
  s[3] = 'l';
  s[4] = 'o';
  s[5] = '\0';
  printf("%s\n", s);

  free(s);
}

/**
 * Assigns a pointer (float *p) a numeric value (12.5).
 */
void nine() {
  float *p;
  *p = 12.5f;

  printf("The value of p is: %f\n", *p);
}

/**
 * Reset the value of x to zero.
 *
 * @param x
 *     Pointer to reset to 0.
 */
void ten(int *x) { *x = 0; }

/**
 * Concatenates "Hello " and the parameter str, which is guaranteed to be a
 * valid c string, and
 * prints the concatenated string.
 */
void eleven(const char *str) {
  char *s = "Hello ";
  printf("%s%s\n", s, str);
}

/**
 * Creates an array of values containing the values {0.0, 0.1, ..., 0.9}.
 */
void twelve() {
  float *values;

  int i, n = 10;
  for (i = 0; i < n; i++)
    values[i] = (float)i / n;

  for (i = 0; i < n; i++) 
    printf("%f ", values[i]);
  printf("\n");
}

/**
 * Creates a 2D array of values and prints out the values on the diagonal.
 */
void thirteen(int a) {
  int **values;

  int i, j;
  values = malloc(10 * sizeof(int*));
  for (i = 0; i < 10; i++) {
    values[i] = (int*)malloc(10 * sizeof(int*));
    for (j = 0; j < 10; j++) {
      values[i][j] = i * j * a;
    }
  }
  for (i = 0; i < 10; i++)
    printf("%d ", values[i][i]);
  printf("\n");
  free(values);
}

/**
 * Prints out a specific string based on the input parameter (s).
 *
 * @param s
 *     Input parameter, used to determine which string is printed.
 */
void fourteen(const char *s) {
  char c1 = *s;
  switch (c1) {
  case 'b':
    printf("Orange and BLUE!\n");
    break;

  case 'o':
    printf("ORANGE and blue!\n");
    break;

  default:
    printf("orange and blue!\n");
    break;
  }
}

/**
 * Prints out a specific string based on the input parameter (value).
 *
 * @param value
 *     Input parameter, used to determine which string is printed.
 */
void fifteen(const int value) {
  switch (value) {
  case 1:
    printf("You passed in the value of one!\n");
    break;

  case 2:
    printf("You passed in the value of two!\n");
    break;

  default:
    printf("You passed in some other value!\n");
    break;
  }
}

/**
 * Returns a newly allocated string on the heap with the value of "Hello".
 * This should not be freed.
 *
 * @returns
 *     A newly allocated string, stored on the heap, with the value "Hello".
 */
char *sixteen() {
  char *s = malloc(sizeof(char)*5);
  strcpy(s, "Hello");
}

/**
 * Prints out the radius of a circle, given its diameter.
 *
 * @param d
 *     The diameter of the circle.
 */
void seventeen(const int d) {
  printf("The radius of the circle is: %f.\n", ((float)d / 2));
}

/**
 * Manipulates the input parameter (k) and prints the result.
 *
 * @param k
 *     The input parameter to manipulate.
 */
void eighteen(const int k) {
  int k1 = k * k;
  k1 += k1;
  k1 *= k1;
  k1 -= 1;

  printf("Result: %d\n", k1);
}

/**
 * @brief
 *     Clears the bits in "value" that are set in "flag".
 *
 * This function will apply the following rules to the number stored
 * in the input parameter "value":
 * (1): If a specific bit is set in BOTH "value" and "flag", that
 *      bit is NOT SET in the result.
 * (2): If a specific bit is set ONLY in "value", that bit IS SET
 *      in the result.
 * (3): All other bits are NOT SET in the result.
 *
 * Examples:
 *    clear_bits(value = 0xFF, flag = 0x55): 0xAA
 *    clear_bits(value = 0x00, flag = 0xF0): 0x00
 *    clear_bits(value = 0xAB, flag = 0x00): 0xAB
 *
 * @param value
 *     The numeric value to manipulate.
 *
 * @param flag
 *     The flag (or mask) used in order to clear bits from "value".
 */
long int clear_bits(long int value, long int flag) {
  printf("Value: 0x%05X\n", value);
  printf("Flag: 0x%05X\n", flag);
  int r = 0x00;
  r = ~(value & flag);
  printf(" Val: 0x%05X\n", r);
  printf("~Val: 0x%05X\n", ~r);
  return ~r;
}



