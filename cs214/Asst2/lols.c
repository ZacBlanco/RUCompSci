#include "lols.h"

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
    char buffer[50];
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
    } else { success = 0;
        fprintf(stderr, "Could not open a pointer to the file %s\n", filename);
    }
    
    return success;
}

// Returns a char*  (string) with the entire file contents.
// Returns empty string (single null terminator) if can't read file
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

// Returns a CompressionBounds struct (see header file);
// YOU MUST FREE THE MEMBERS AND THE STRUCT WHEN YOU ARE FINISHED
CompressionBounds* get_indexes(const char* file_str, const int num_parts) {

    long length = strlen(file_str);
    int base_len = length / num_parts; // Will truncate the decimal values
    int extra = length - (base_len) * num_parts; // Find the difference between length after truncations

    CompressionBounds* cb = malloc(sizeof(CompressionBounds));
    cb->indexes = malloc(sizeof(int)*num_parts); // Indexes where each thread will begin to read.
    cb->lengths = malloc(sizeof(int)*num_parts); // Length of string each thread must compress.
    // printf("Extra: %i and Base Len: %i\n", extra, base_len);
    
    cb->indexes[0] = 0;
    cb->lengths[0] = base_len + extra;

    // printf("String Length: %li\n", length);
    int i;
    for (i = 1; i < num_parts; i++) {
        cb->indexes[i] = cb->indexes[i-1] + cb->lengths[i-1];
        cb->lengths[i] = base_len;
    }

    return cb;
}

/*
 * Returns a string with the filename to write to.
 * NEEDS TO BE FREE'D WHEN FINISHED.
 */
char * get_filename(const char * file_url, int num) {

    int len = strlen(file_url);
    //find the last occurrence of '/' - if it doesn't exist then we start at index 0
    int i;
    int start_index = 0;
    for(i = len; i >= 0; i--) {
        if (file_url[i] == '/') {
            start_index = i + 1;
            break;
        }
    }
    int digits = 0;
    if (num >  0 && num/10 >= 1) {
        digits += 2;
    } else if (num >= 0) {
        digits++;
    }

    int filename_len = len - start_index + digits + 5; // +5 for "_LOLS", +1 for '\0'
    char* t_lols = "_LOLS";
    char* filename = malloc(sizeof(char)*(filename_len + 1)); // add one for null terminator
    for(i = 0; i < filename_len; i++) {
        if (i + start_index > len - 1) {
            int ind = i + start_index - len;
            filename[i] = t_lols[i + start_index - len];
            // printf("Setting LOLS char: %c with index: %i\n", t_lols[ind], ind);
            // printf("LOLS Filename: %s\n", filename);
        } else if(file_url[i + start_index] == '.') {
            filename[i] = '_';
        } else {
            filename[i] = file_url[i + start_index];
            // printf("Setting normal char: %c\n", file_url[i+start_index]);
        }
    }
    filename[i] = '\0';
    if (digits > 0) {
        char* dig = malloc(sizeof(char)*10);
        sprintf(dig, "%i", num);
        strcat(filename, dig);
        free(dig);
    }
    return filename;
}

/*
 * Checks to see if the last 4 characters are .txt
 */
int is_valid_filename(const char * input_file_name) {

    int txt_location = strlen(input_file_name) - 4;
    char * file_extension = ".txt";

    char * txt = strstr(input_file_name, file_extension);
    if (txt == NULL) {
        return 0;
    }

    int position = txt - input_file_name;
    if (position != txt_location) {
        return 0;
    } else {
        return 1;
    }
}


//Returns the number of digits in a number
int get_digits(const int a) {
    int d = 1;
    int b = a;
    while(b >= 10) {
        b /= 10;
        d++;
    }
    return d;
}

// converts a string to an integer
// only works for numbers >= 0
int c2i(const char* str) {
    int i = 1;
    int len = strlen(str);
    int iter = len - 1;
    int num = 0;
    while(iter >= 0) {
        num += i*((int)str[iter] - '0');
        i *= 10;
        iter--;
    }
    return num;
}