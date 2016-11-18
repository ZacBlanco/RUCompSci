#ifndef __LOLS__H__
#define __LOLS__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // compile with -pthread
#include <string.h>
#include <errno.h>
#include <ctype.h>

char * lols(char * original_word);
char * itoa(char * output, int num);
char * append_string(char * output, int letter_count, char letter);
int is_number(char * num_string);
int is_args_valid(int args, char** argv);
int write_to_file(const const char* str, char* filename);
char* read_file(char* filename);

/*
 * Takes in a string and compresses it using a modified RLE
 * algorithm. 
 */
char * lols(char * original_word) {

    int string_length = strlen(original_word);
    
    if (string_length == 0 || 
            string_length == 1 || 
            string_length == 2) {
        return original_word;
    }
    
    char current_letter, previous_letter;
    char * compressed_string = (char *)malloc(sizeof(char) * string_length);
    int i = 1, letter_count = 1;
    
    previous_letter = original_word[0];

    for (i = 1; i < string_length; i++) {
        current_letter = original_word[i];
		
		if (!isalpha(current_letter)) {
			continue;
		}

        if (current_letter == previous_letter) {
            letter_count++;
            continue;
        } else {
            compressed_string = append_string(compressed_string, letter_count, previous_letter);
            previous_letter = current_letter;
            letter_count = 1;
        }
    }

    compressed_string = append_string(compressed_string, letter_count, previous_letter);
    
    return compressed_string;
}

/*
 * Converts an integer to a string.
 */
char * itoa(char * output, int num) {
    int mod = 10;
    char buffer[1000];
    buffer[0] = '\0';
    
    while (num != 0) {
        char c = (num % mod) + '0';
        buffer[0] = c;
        buffer[1] = '\0';
        strcat(output, buffer);
        num /= mod;
    }

    return output;

}

/*
 * Appends a number and a letter to the end of a string.
 */
char * append_string(char * output, int letter_count, char letter) {

    char buffer[1000]; 
    buffer[0] = '\0';

    if (letter_count == 1 || letter_count == 2) {
        int i = 0;
        for (i = 0; i < letter_count; i++) {
            buffer[i] = letter;
        }
        buffer[letter_count] = '\0';
        strcat(output, buffer);
        return output;
    }

    strcpy(buffer, itoa(buffer, letter_count));
    strcat(output, buffer);
    buffer[0] = letter;
    buffer[1] = '\0';
    strcat(output, buffer);
        
    return output;
}

// Writes a string to a file
// returns 0 on failure, 1 on success
int write_to_file(const char* str, char* filename) {
    int success = 1; //Assume it to first always be successfull
    FILE* f = fopen(filename, "w");
    if (f) { // No error when attempting to open file.
        if (fputs(str, f) == EOF) { //EOF when error
            // An Error occurred when writing to file
            fprintf(stderr, "Error when writing to file %s", filename);
            success = 0;
        }
        fclose(f);
        // printf("Wrote to file\n");
    } else {
        success = 0;
        fprintf(stderr, "Could not open a pointer to the file %s\n", filename);
    }
    
    return success;
}

// Returns a char*  (string) with the entire file contents.
// Returns empty string (single null terminator)
// Don't forget to free the pointer returned from this function. 
char* read_file(char* filename) {

    if(filename == NULL) {
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    char* buf;
    if (f) { // If true opened successfully.
        fseek(f, 0, SEEK_END);
        long file_length = ftell(f);
        fseek(f, 0, SEEK_SET); 

        // printf("File size is %li\n", fsize);
        buf = malloc(sizeof(char)*(file_length + 1)); // Add 1 for null terminator
        int chars_read  = fread(buf, sizeof(char), file_length, f);
        buf[file_length] = '\0'; //Set null terminator as last char
    } else {
        printf("Could not open file to read\n");
        buf = malloc(sizeof(char));
        *buf = '\0';
    }
    return buf;
}

int is_args_valid(int args, char** argv) {
    if (args < 3) {
        fprintf(stderr, "Error - Not enough arguments.\n");
        return 0;
    } else if(args > 3) {
        printf("Warning - too many arguments.\nExtraneous arguments rest will be discarded.\n");
    }
    
    char * file_url = argv[1];
    int num_of_workers;

    if (is_number(argv[2])) {
        num_of_workers = atoi(argv[2]);
    } else {
        fprintf(stderr, "Error - second argument must be a valid number.\n");
        return 0;
    }
}


int is_number(char * num_string) {
    char character;
    int i = 0;
    
    for (i = 0; i < strlen(num_string); i++) {
        if (!isdigit(num_string[i])) {
            return 0;
        }
    }
    return 1;
}


#endif