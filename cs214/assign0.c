#include <stdio.h>
#include <string.h>

// Define the functions to use
int getLength(char str[]);
int isAlpha(char c);
void printChars(char str[]);

int main(int argc, char **argv){
  //Print argc, the number of arguments
  printf("Number of arguments: %i\n", argc);
  
  //This section is for testing/learning purposes
  //Iterate over the arguments array and print argument, the length, and the individual chars of the argument.
//  for(int i = 0; i < argc; i++) {
//    printf("Argument %i: \"%s\"\n", i+1, argv[i]);
//    int l = getLength(argv[i]);
//    printf("Number of Characters: %i\n", l);
//   for(int j = 0; j < l;j++){
//    printf("Char: \"%c\"; Is alphabetic: %i\n", argv[i][j], isAlpha(argv[i][j]));    
//    }
//  }
  
  
  //Let's look over the first (not 0th!) argument - we assume this is the only argument we will need
  if (argc < 2) {
    printf("You need to supply an argument\n");
    return 1;
  }
  
  char* arg = &argv[1]; // The array of characters which we'll be reading from
  int start = 0; // The index of a word's first character
  int end = 0; // index of a word's last character
  
  printf("String arg: \"%s\"\n", arg);
  
  //Main loop to iterate over the string
  while (arg[start] != '\0') {
    start++;
  }
  
  
  

  return 0;
}

// Takes a char array and prints each char to separate line
// ARGUMENTS:
//   char str[]: A character array to print
void printChars(char str[]){
  int iter = 0;
  while(str[iter] != '\0'){
    printf("%c\n",str[iter]);
    iter++;
  }
}

// Takes a char array and returns the length of the string (Not including the null terminator)
// ARGUMENTS:
//    char str[]: The character array to get the length of
// RETURNS:
//    int: A number representing the number of characters in the string 
int getLength(char str[]) {
  int iter = 0;
  while (str[iter]!= '\0') {
    iter++;
  }
  return iter;
}

//Determine if a character is in the Alphabet or not
// ARGUMENTS:
//    char c: A character to analyze
// RETURNS:
//    int: 1 if the character is alphabetic or 0 otherwise.
int isAlpha(char c){
  if (c >= 'A' && c <= 'Z'){
    return 1;
  } else if (c >= 'a' && c <= 'z') {
    return 1;
  } else {
    return 0;
  }
}


