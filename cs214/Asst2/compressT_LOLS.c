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

    char* file_str = read_file(file_url);
    long length = strlen(file_str);
    int base_len = length / num_of_parts; // Will truncate the decimal values
    int extra = length - (base_len) * num_of_parts; // Find the difference between length after truncations
    printf("Extra: %i and Base Len: %i\n", extra, base_len);
    int start[num_of_parts]; // Indexes where each thread will begin to read.
    int lengths[num_of_parts]; // Length of string each thread must compress.
    
    start[0] = 0;
    lengths[0] = base_len + extra;

    printf("String Length: %i\n", length);
    int i;
    for (i = 1; i < num_of_parts; i++) {
        start[i] = start[i-1] + lengths[i-1];
        lengths[i] = base_len;
    }


    for (i = 0; i < num_of_parts; i++) {
        printf("Index: %i, Start index: %i, Length: %i\n", i, start[i], lengths[i]);
    }

    printf("Read string: %s\n", file_str);


}

void thread_worker(const char* str, int index, int length) {

}

