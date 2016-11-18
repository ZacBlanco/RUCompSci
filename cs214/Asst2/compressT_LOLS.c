#include "compressT_LOLS.h"

void compressT_LOLS(char * file_url, int num_of_parts) {

    //////////// Check to make sure args are valid ////////////
    if (num_of_parts <= 0) {
        fprintf(stderr, "Error - Must be split into a positive number");
        return;
    }
    FILE * file = fopen(file_url, "r");

    if (!file) {
        fprintf(stderr, "Error with File(first argument).\n%s\n", strerror(errno));
    }

    fclose(file);
    ///////////////////////////////////////////////////////////

    // Create n threads 


}

