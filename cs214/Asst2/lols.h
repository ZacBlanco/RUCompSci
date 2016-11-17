#ifndef __LOLS__H__
#define __LOLS__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * lols(char * original_word);
char * itoa(char * output, int num);
char * append_string(char * output, int letter_count, char letter);

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

#endif
