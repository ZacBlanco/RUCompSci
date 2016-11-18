#ifndef __LOLS__H__
#define __LOLS__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // compile with -pthread
#include <string.h>
#include <errno.h>
#include <ctype.h>

char * lols(char * original_word);
char * itoa(char * output, int num);
char * append_string(char * output, int letter_count, char letter);
int is_number(char * num_string);
int is_args_valid(int args, char** argv);
int write_to_file(const char* str, char* filename);
char* read_file(char* filename);


#endif
