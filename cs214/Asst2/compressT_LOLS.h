#ifndef __COMPRESST_LOLS__
#define __COMPRESST_LOLS__
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // compile with -pthread
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "lols.h"


typedef struct {
    char* str;
    int index;
    int length;
    char* filename;
} compression_args;

void compressT_LOLS(char * file_url, int num_of_parts);

#endif