#include <stdio.h>
#include <string.h>

// Define the functions to use
int getLength(char str[]);
int isAlpha(char c);
void printChars(char str[]);

int main(int argc, char **argv){
  //Print argc, the number of arguments
  printf("Number of arguments: %i\n", argc);
  
  //Iterate over the arguments array and print argument, the length, and the individual chars of the argument.
  for(int i = 0; i < argc; i++) {
    printf("Argument %i: \"%s\"\n", i+1, argv[i]);
    int l = getLength(argv[i]);
    printf("Number of Characters: %i\n", l);
    for(int j = 0; j < l;j++){
      printf("Char: \"%c\"; Is alphabetic: %i\n", argv[i][j], isAlpha(argv[i][j]));    
    }
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
//   char str[]: The character array to get the length of
int getLength(char str[]) {
  int iter = 0;
  while (str[iter]!= '\0') {
    iter++;
  }
  return iter;
}

int isAlpha(char c){
  if (c >= 'A' && c <= 'Z'){
    return 1;
  } else if (c >= 'a' && c <= 'z') {
    return 1;
  } else {
    return 0;
  }
}
