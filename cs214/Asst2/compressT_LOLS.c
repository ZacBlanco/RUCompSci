#include <stdio.h>
#include <pthread.h> // compile with -pthread
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "lols.h"
void compressT_LOLS(char * file_url, int num_of_parts);
int isNumber(char * num_string);

int main(int args, char ** argv){

    if (args < 3) {
        fprintf(stderr, "Error - Not enough arguments.\n");
        return 1;
    } else if(args > 3) {
        printf("Warning - too many arguments.\nThe rest will be discarded.\n");
    }
    
    char * file_url = argv[1];
    int num_of_workers;

    if (isNumber(argv[2])) {
        num_of_workers = atoi(argv[2]);
    } else {
        fprintf(stderr, "Error - second argument must be a valid number.\n");
        return 1;
    }

    /* 
     * This function is the function that will be graded.
     * Main can just be the way we test.
     */
    compressT_LOLS(file_url, num_of_workers);

    
    return 0;
}

void compressT_LOLS(char * file_url, int num_of_parts) {

    if (num_of_parts <= 0) {
        fprintf(stderr, "Error - Must be split into a positive number");
    }

    FILE * file = fopen(file_url, "r");

    if (!file) {
        fprintf(stderr, "Error with File(first argument).\n%s\n", strerror(errno));
        return 1;
    }
}

int isNumber(char * num_string) {
    char character;
    int i = 0;
    
    for (i = 0; i < strlen(num_string); i++) {
        if (!isdigit(num_string[i])) {
            return 0;
        }
    }
    return 1;
}
