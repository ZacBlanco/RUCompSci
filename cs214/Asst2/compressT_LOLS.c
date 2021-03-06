#include "compressT_LOLS.h"

void compressT_LOLS(char * file_url, int num_parts) {

    //////////// Check to make sure args are valid ////////////
    if (num_parts <= 0) {
        fprintf(stderr, "Error - Must have positive number of parts\n");
        return;
    }
    
    if (!is_valid_filename(file_url)) {
        fprintf(stderr, "Error - filename must end with '.txt'\n");
        return;
    }

    FILE * file = fopen(file_url, "r");

    if (!file) {
        fprintf(stderr, "Error with File(first argument).\n%s\n", strerror(errno));
        return;
    }

    fclose(file);
    ///////////////////////////////////////////////////////////

    // Create n threads 
    char* file_str = read_file(file_url); // Don't forget to free this
    if (strcmp(file_str, "") == 0) {
        free(file_str);
        return;
    } else if( strlen(file_str) < num_parts) {
        fprintf(stderr, "Error - too many parts for string.\n");
        free(file_str);
        return;
    }
    CompressionBounds* c = get_indexes(file_str, num_parts); //Don't forget to free this and its members
    free(file_str);
    int i;
    // for (i = 0; i < num_parts; i++) {
    //     printf("Index: %i, Start index: %i, Length: %i\n", i, c->indexes[i], c->lengths[i]);
    // }

    pthread_t threads[num_parts];

    i = 0;
    void * (*worker)(void *);
    worker = (void *)(&thread_worker);
    for(i = 0; i < num_parts; i++) {
        // It will be up to the worker thread to free this struct since we need one for every thread.
        // we need to free the filename and the struct for the
        compression_args* c_args = malloc(sizeof(compression_args));
        c_args->index = c->indexes[i];
        c_args->length = c->lengths[i];
        c_args->file = file_url;
        if (num_parts == 1) {
            c_args->filename = get_filename(file_url, -1);
        } else {
            c_args->filename = get_filename(file_url, i); //Need to figure out how long the filename is
        }
        pthread_create(&(threads[i]), NULL, worker, c_args);
    }

    // Must wait on all threads
    for (i = 0; i < num_parts; i++) {
        pthread_join(threads[i], NULL);
    }

    free(c->indexes);
    free(c->lengths);
    free(c);
}

// Takes the original (Large) file data, gets a copy of the data and compresses it and writes it to file_name
void* thread_worker(compression_args* ca) {
    
    int index       = ca->index;
    int length      = ca->length;
    char* fstr      = read_file(ca->file);

    if (strcmp(fstr, "") == 0) {
        free(fstr);
        free(ca->filename);
        free(ca);
        return;
    }

    char* filename = ca->filename;
    char* orig = malloc(sizeof(char)*(length + 1)); // Don't forget to free
    strncpy(orig, (fstr + index), length);
    orig[length] = '\0';
    char* lold = lols(orig); //Don't forget to free

    //Now we need to write to a file
    int success = write_to_file(lold, filename);
    if(success) {
        // printf("Wrote compressed file\n");
    } else {
        printf("Failed to write compression file\n");
    }

    free(fstr);
    free(ca->filename);
    free(ca);
    free(orig);
    free(lold);
}

