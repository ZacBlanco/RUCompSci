#include <stdio.h>
#include <string.h>

// Define the functions to use
int getLength(char str[]);
int isAlpha(char c);
void printChars(char str[]);
char* createString(int start, int end, char argv[]);

int main(int argc, char **argv){
  //Print argc, the number of arguments
  printf("Number of arguments: %i\n", argc);
  
  
  //Let's look over the 1st (not 0th!) argument - we assume this is the only argument we will need
  if (argc < 2) {
    printf("You need to supply an argument\n");
    return 1;
  }
  
  int iter = 0; // The iterator variable
  int start, end = 0; // index of a word's last character
  int lastAlpha = 0; // True if last character was alphabetic
  
  //Main loop to iterate over the string
  while (argv[1][iter] != '\0') {

    if (!isAlpha(argv[1][iter])){
      end = iter;
      if (end > start) {
        char* str = createString(start, end, argv[1]);
        //printf("String: %s\n", str);
      } else {
        printf("Bad Bounds. Start: %i , End: %i \n", start, end);      
      }
      lastAlpha = 0;
      start = iter + 1;
    } else {
      //printf("%c", argv[1][iter]);
      lastAlpha = 1;
    }
    iter++;
  }
  end = iter;
  if (end > start) {
    char* str = createString(start, end, argv[1]);
    //printf("String: %s\n", str);
  }
  
  printf("\n");
  
  

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

//Return a pointer to a character array of a string
// ARGUMENTS:
//    start: the start position of the array for the string
//    end: the end position of the array for the string
//    argv: The array to copy characters from
char* createString(int start, int end, char argv[]){  
  int length = end - start;
  length += 1;
  char str[length];
  char* szPtr = str;
  int i; 
  for(i = start; i <= end; i++) {
    str[i - start] = argv[i];
    //printf("Character: %c\n", str[i]);
  }
  str[length-1] = '\0';
  printf("STRING: %s\n", str);
  return szPtr;

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


