#include <stdio.h>
#include <string.h>

// Define the functions to use
int getLength(char str[]);
void printChars(char str[]);

int main(int argc, char **argv){
        //Print argc, the number of arguments
        printf("Number of arguments: %i\n", argc);
        
        //Iterate over the arguments array and print argument, the length, and the individual chars of the argument.
        for(int i = 0; i < argc; i++) {
                printf("Argument %i: \"%s\"\n", i+1, argv[i]);
                printf("Number of Characters: %i\n", getLength(argv[i]));
                printChars(argv[i]);
        }

        return 0;
}
// Takes a char array and prints each char to separate line
// ARGUMENTS:
//     char str[]: A character array to print
void printChars(char str[]){
        int iter = 0;
        while(str[iter] != '\0'){
                printf("%c\n",str[iter]);
                iter++;
        }
}

// Takes a char array and returns the length of the string (Not including the null terminator)
// ARGUMENTS:
//     char str[]: The character array to get the length of
int getLength(char str[]) {
        int iter = 0;
        while (str[iter]!= '\0') {
                iter++;
        }
        return iter;
}
