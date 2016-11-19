#ifndef __COMPRESSR_LOLS__
#define __COMPRESSR_LOLS__
#include <stdio.h>
#include <stdlib.h>
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

void compressR_LOLS(char * file_url, int num_of_parts);

#endif
