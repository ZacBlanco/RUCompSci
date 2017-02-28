#include "util.h"

//See header
int readFile(char* filename, int** ints) {

  FILE* fp = fopen(filename, "r");
  if(fp == NULL) {
    perror("Could not open input file");
    return -1;    
  }

  int ints_len = 10;
  *ints = malloc(sizeof(int)*ints_len);

  char* line = NULL;
  ssize_t read = 0;
  size_t line_len = 0;
  volatile int a = 0;
  volatile int cnt = -1;
  
  while ( (read =  getline(&line, &line_len, fp)) != -1) {
    if(++cnt >= ints_len) {
      int* tmp = (int*)malloc(sizeof(int)*2*ints_len);
      memcpy(tmp,*ints, sizeof(int)*ints_len);
      free(*ints);
      *ints = tmp;
      ints_len *= 2;
    }
    a =  atoi(line);
    (*ints)[cnt] = a;
  }
  if (line != NULL) {
    free(line);
    cnt++; // Account for last line
  }

  fclose(fp);
  return cnt;

}

bool writeFile(char* filename, int* ints, int ints_len) {
  FILE* fp = fopen(filename, "w");

  if (fp == NULL) {
    perror("Could not open/create file");
    return false;
  }

  int i;
  for (i = 0; i < ints_len; i++) {
    fprintf(fp, "%d\n", ints[i]);
    fflush(fp);
  }

  fclose(fp);
  return true;
}
