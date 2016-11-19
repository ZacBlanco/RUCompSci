#include "compressT_LOLS.h"

void compressT_LOLS(char * file_url, int num_parts) {

    //////////// Check to make sure args are valid ////////////
    if (num_parts <= 0) {
        fprintf(stderr, "Error - Must be split into a positive number");
        return;
    }
    
    if (!is_valid_filename(file_url)) {
        fprintf(stderr, "Error - filename must end with '.txt'\n");
        return;
    }

    FILE * file = fopen(file_url, "r");

    if (!file) {
        fprintf(stderr, "Error with File(first argument).\n%s\n", strerror(errno));
        fclose(file);
        return;
    }

    fclose(file);
    ///////////////////////////////////////////////////////////

    // Create n threads 
    char* file_str = read_file(file_url); // Don't forget to free this
    CompressionBounds* c = get_indexes(file_str, num_parts); //Don't forget to free this and its members
    int i;

    for (i = 0; i < num_parts; i++) {
        printf("Index: %i, Start index: %i, Length: %i\n", i, c->indexes[i], c->lengths[i]);
    }

    pthread_t threads[num_parts];

    i = 0;
    void * (*worker)(void *);
    worker = thread_worker;
    for(i = 0; i < num_parts; i++) {
        // It will be up to the worker thread to free this struct since we need one for every thread.
        // we need to free the filename and the struct for the
        compression_args* c_args = malloc(sizeof(compression_args));
        c_args->index = c->indexes[i];
        c_args->length = c->lengths[i];
        c_args->str = file_str;
        if (num_parts == 1) {
            c_args->filename = get_filename(file_url, -1);
        } else {
            c_args->filename = get_filename(file_url, i); //Need to figure out how long the filename is
        }
        // Write a function in lols.c to get the filename from the file_url 
        // pthread_create(&(threads[i]), NULL, (void *)(&thread_worker)(void *), c_args);
        // printf("Created thread\n");
        pthread_create(&(threads[i]), NULL, worker, c_args);
    }

    // Must wait on all threads
    for (i = 0; i < num_parts; i++) {
        pthread_join(threads[i], NULL);
    }

    free(c->indexes);
    free(c->lengths);
    free(c);
    free(file_str);
}

// Takes the original (Large) file data, gets a copy of the data and compresses it and writes it to file_name
void* thread_worker(compression_args* ca) {
    
    int index      = ca->index;
    int length     = ca->length;
    char* str      = ca->str;
    char* filename = ca->filename;
    
    char* orig = malloc(sizeof(char)*length); // Don't forget to free
    
    int i;
    for(i=0; i < length; i++) {
        orig[i] = str[index + i];
    }
    char* lold = lols(orig); //Don't forget to free

    //Now we need to write to a file
    int success = write_to_file(lold, filename);
    
    if(success) {
        // printf("Wrote compressed file\n");
    } else {
        printf("Failed to write compression file\n");
    }


    free(ca->filename);
    free(ca);
    free(orig);
    free(lold);
}

