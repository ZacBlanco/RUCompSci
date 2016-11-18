#ifndef __LOLS__H__
#define __LOLS__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // compile with -pthread
#include <string.h>
#include <errno.h>
#include <ctype.h>


typedef struct {
    int* indexes;
    int* lengths;
} CompressionBounds;

char * lols(char * original_word);
char * itoa(char * output, int num);
char * append_string(char * output, int letter_count, char letter);
int is_number(char * num_string);
int is_args_valid(int args, char** argv);
int write_to_file(const char* str, char* filename);
char* read_file(char* filename);
CompressionBounds* get_indexes(const char* file_str, const int num_parts);
char * get_filename(const char * input_file_name, int num_of_worker);
int is_valid_filename(const char * input_file_name);


#endif
