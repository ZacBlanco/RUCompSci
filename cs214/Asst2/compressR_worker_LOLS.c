#include <stdlib.h>
#include <stdio.h>
#include "compressT_LOLS.h"
#include "lols.h"

int main(int argc, char** argv) {

    //Arg 1 is file to read
    //Arg 2 index to read
    //Arg 3 length to read
    //Arg 4 filename to write to
    int i;
    if(argc != 4) {
        printf("Should only have 4 arguments\n");
        return;
    }

    char* fstr = read_file(argv[0]);
    // printf("Worker Input File: %s\n", argv[0]);
    if (strcmp(fstr, "") == 0) {
        free(fstr);
        printf("Error. File %s was empty. Please use an existing file.", argv[0]);
        _Exit(EXIT_FAILURE);
        return;
    }
    int ind = atoi(argv[1]);
    int len = atoi(argv[2]);
    char* filename = argv[3];

    char* lold = malloc(sizeof(char)*(len + 1));
    strncpy(lold, (fstr + ind), len);
    lold[len] = '\0';
    char* ll = lols(lold);
    write_to_file(ll, filename);

    free(ll);
    free(lold);
    free(fstr);
    return;

}
