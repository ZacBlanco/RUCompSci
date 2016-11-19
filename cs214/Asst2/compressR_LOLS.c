#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lols.h"
#include "compressR_worker_LOLS.h"

void compressR_LOLS(char * file_url, int num_parts) {

    //////////// Check to make sure args are valid ////////////
    if (num_parts <= 0) {
        fprintf(stderr, "Error - Must have positive number of parts");
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

    char* file_str = read_file(file_url); // Don't forget to free this
    CompressionBounds* c = get_indexes(file_str, num_parts); //Don't forget to free this and its members
    free(file_str);
    pid_t pids[num_parts];
    int is_parent = 1;
    int i;
    for(i = 0; i < num_parts; i++) {
        int f; // Fork value;
        int ind = c->indexes[i];
        int len = c->lengths[i];
        char* filename;
        if (is_parent) {
            f = fork();
            pids[i] = f;
        } else {
            f == 0;
        }
        
        if (f == 0) {
            is_parent = 0;
            if (num_parts == 1) {
                filename = get_filename(file_url, -1);
            } else {
                filename = get_filename(file_url, i); //Need to figure out how long the filename is
            }
            //Must convert args to str.
            char* index = malloc(sizeof(char)*get_digits(ind) + 1); // Freed on exec
            char* length = malloc(sizeof(char)*get_digits(len) + 1); // Freed on exec
            sprintf(index, "%i", ind);
            sprintf(length, "%i", len);
            // printf("I'm about to Exec\n");
            execl("./comp_proc", file_url, index, length, filename, NULL);
            exit(1);
        } else if (f == -1) {
            printf("Child processe(s) could not be created. Aborting...\n");
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        }
    }

    if(is_parent) {
        for( i = 0; i < num_parts; i++) {
            int a = 0;
            // printf("waiting on %i\n", pids[i]);
            waitpid(pids[i], &a, 0);
        }
    }
    
    
}



