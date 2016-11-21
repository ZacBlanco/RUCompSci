#include <stdlib.h>
#include <stdio.h>
#include "compressR_worker_LOLS.h"
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
    if (strcmp(fstr, "") == 0) {
        free(fstr);
        _Exit(EXIT_FAILURE);
    }
    int ind = atoi(argv[1]);
    int len = atoi(argv[2]);
    char* filename = argv[3];
    // char* sect = malloc()


    char* lold = malloc(sizeof(char)*len + 1);
    strncpy(lold, (fstr + ind), len);
    char* ll = lols(lold);
    write_to_file(ll, filename);

    free(ll);
    free(lold);
    free(fstr);

}
