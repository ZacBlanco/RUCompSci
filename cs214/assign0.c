#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the functions to use
int getLength(char str[]);
int isAlpha(char c);
void printChars(char str[]);
char* createString(int start, int end, char argv[]);

typedef struct ListNode {
  struct ListNode* prev;
  struct ListNode* next;
  char* data;
} ListNode;


int main(int argc, char **argv){
  //Print argc, the number of arguments
  printf("Number of arguments: %i\n", argc);
  
  //Let's look over the 1st (not 0th!) argument - we assume this is the only argument we will need
  if (argc < 2) {
    printf("Not enough arguments suppled. Please try again with a new set of arguments.\n");
    return 1;
  }
  
  
  ListNode* stack = NULL;
  
  int iter = 0; // The iterator variable
  int start, end = 0; // index of a word's last character
  int lastAlpha = 0; // True if last character was alphabetic
  
  //Main loop to iterate over the string
  while (argv[1][iter] != '\0') {

    if (!isAlpha(argv[1][iter])){
    
      //We've approached the end of a word
      //Set the end index to the current iterator
      //printf("Ending Char: \"%c\"\n", argv[1][iter]); //Current Ending character
      
      end = iter;
      
      //Ensure end is greater than start - if so then attempt to create the string pointer
      if (end > start) {

        char* str = createString(start, end, argv[1]);

        //printf("String: %s\n", str); //print the str
        if (stack == NULL) {
          stack = malloc(sizeof(ListNode));
          stack->prev = NULL;
          stack->next = NULL;
          stack->data = str;
        } else {
          //Add the string onto the stack
          ListNode* new = malloc(sizeof(ListNode));
          new->prev = stack;
          new->next = NULL;
          new->data = str;
          stack->next = new;
          stack = new;
        }
        printf("New Stack Ptr: %p\n", stack);

        
      } else {
        //printf("Bad Bounds. Start: %i , End: %i \n", start, end); // Bounds are not correct    
      }
      lastAlpha = 0;
      start = iter + 1;
    
    
    } else {
      //printf("%c", argv[1][iter]);
      lastAlpha = 1;
    }
    iter++;
  }

  //Repeat the process for the final word of the string and add it to the list
  end = iter;
  if (end > start) {
    char* str = createString(start, end, argv[1]);
    //printf("String: %s\n", str);
    //Add the string onto the stack
    ListNode* new = malloc(sizeof(ListNode));
    new->prev = stack;
    new->next = NULL;
    new->data = str;
    stack->next = new;
    stack = new;
  }
  
  ListNode* cur = stack;
  printf("Stack Pointer: %p\n", stack);
  ListNode node = *cur;
  int i = 0;
  while ( &cur != NULL && i < 15) {
    printf("Word: \"%s\"; Pointer: %p\n", (*cur).data, &(*cur).data);
    printf("Next Ptr: %p; Prev Ptr: %p\n", (*cur).next, (*cur).prev);
    cur = node.prev;
    i++;
  }
  
  
  printf("\n");
  
  

  return 0;
}

//ListNode* addToStack(ListNode* stackPtr, 


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
// RETURNS:
//    char*: A pointer to a character array for the word we just created
char* createString(int start, int end, char argv[]){  
  int length = end - start;
  length += 1; //+1 For null terminator
  char* szPtr = malloc(sizeof(char)*length);
  int i; 
  for(i = start; i <= end; i++) {
    szPtr[i - start] = argv[i];
  }
  szPtr[length-1] = '\0';
  //printf("CREATE STRING: \"%s\"; Pointer: %p \n", szPtr, szPtr); //Print string for testing
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


//Compares two strings
// ARGUMENTS:
//    first: The first character array
//    second: The second character array
// RETURNS:
//    -1: When first is less than (comes before) second in alphabetic ordering
//    0:  When both appear at the same location in alphabetic ordering
//    1:  When first appears after (comes after) second in alphabetic ordering
// SPECIAL NOTE:
//    In cases like the two words "bring" and "bringing", because bring is shorter but the first 5 characters are the same
//    we will put "bring" before "bringing" in the ordering, thus the result would be "-1" if we called compareStr("bring", "bringing")
int compareStr(char* first, char* second) {
}


