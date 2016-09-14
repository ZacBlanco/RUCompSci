#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A binary tree node (recursive structure)
// PARAMETERS
//    - BNode* prev: the left subtree
//    - BNode* next: the right subtree
//    - char* data: character pointer for string data 
typedef struct BNode {
  struct BNode* prev;
  struct BNode* next;
  char* data;
} BNode;

// Function Declarations - See implementations for documentation
int getLength(char str[]);
int isAlpha(char c);
void printChars(char str[]);
char* createString(int start, int end, char argv[]);
void printNode(BNode* ptr);
void addToTree(BNode* tree, char* data);
void traverseAndFree(BNode* root);



int main(int argc, char **argv){

  //Let's look over the 1st (not 0th!) argument - we assume this is the only argument we will need
  if (argc < 2) {
    printf("No arguments suppled. Please run with at least one argument.\n");
    return 1;
  } else if (argc > 2) {
    printf("WARNING: More than 1 argument supplied. 2nd arguments and beyond ignored.");
  }


  BNode* tree = (BNode*) malloc(sizeof(BNode));
  tree->prev = NULL;
  tree->next = NULL;
  tree->data = NULL;
  
  int iter = 0; // The iterator variable
  int start = 0, end = 0; // index of a word's last character
  
  //Iterate over the string
  while (argv[1][iter] != '\0') {
    if (!isAlpha(argv[1][iter])){
      //We've approached the end of a word
      //Set the end index to the current iterator
      end = iter;

      //Ensure end is greater than start - if so then attempt to create the string pointer
      if (end > start) {
        char* str = createString(start, end, argv[1]);
        addToTree(tree, str);
      } else {
        //Index bounds were <= 0 - can't create string from this
      }
      start = iter + 1; // Set starting character of the next word to be the next char
    }

    iter++;
  }
  
  //Notify the user in case of an empty string or no words
  if (iter == 0 || tree->data == NULL){ 
    printf("Error: The argument supplied had no words.\n");
    free(tree);
    return 1;
  }
  //Repeat the process for the final word of the string and add it to the list
  end = iter;
  if (end > start) {
    char* str = createString(start, end, argv[1]);
    addToTree(tree, str);
  }
  
  traverseAndFree(tree);
  return 0;
  
}

// Prints a node's value and its immediate childrens' values
// ARGUMENTS
//    BNode* nodePtr: A BNode to print the values for
void printNode(BNode* nodePtr){
  if (nodePtr != NULL) {
  	char* leftStr;
  	char* rightStr;
  	if ((*nodePtr).prev != NULL) {
      leftStr = (*(*nodePtr).prev).data;
  	} else { leftStr = NULL; }
  	if ((*nodePtr).next != NULL) {
      rightStr = (*(*nodePtr).next).data;
  	} else { rightStr = NULL; }

    printf("NodePtr: %p; PrevPtr: %s; NextPtr: %s; Data: %s\n", nodePtr, leftStr, rightStr, (*nodePtr).data);
  } else {
    printf("Node pointer was NULL\n");
  }
}

//Perform an in-order traversal and print the node value then free the node's data and pointer
// ARGUMENTS:
//    BNode* root: A BNode pointer to the root of a tree
void traverseAndFree(BNode* root) {
  if (root == NULL){
    return;
  } else if (root->data == NULL) {
    free(root);
    return;
  }

  traverseAndFree(root->prev);
  printf("%s\n", root->data);
  traverseAndFree(root->next);
  free(root->data);
  free(root);
}

// Add a string to the binary tree
// ARGUMENTS:
//    root: the root node of the tree to add to
//    data: data to add to the tree
void addToTree(BNode* root, char* data) {
  if (root == NULL) {
    //Throw error
    printf("[AddToTree] - NPE - Null pointer passed as the root of the tree");
    return;
  } else if (root->data == NULL) {
    //root = (BNode*) malloc(sizeof(BNode));
    root->prev = NULL;
    root->next = NULL;
    root->data = data;
  } else {
    int cmp = strcasecmp(data, root->data);
     //Binary Tree Insert O(n)
    if (cmp < 0) {
      if (root->prev == NULL) {
        BNode* newPtr = (BNode*)malloc(sizeof(BNode));
        newPtr->prev = NULL;
        newPtr->next = NULL;
        newPtr->data = data;
        root->prev = newPtr;
      } else {
      	addToTree(root->prev, data);
      }
    } else {
      if (root->next == NULL) {
        BNode* newPtr = (BNode*)malloc(sizeof(BNode));
        newPtr->prev = NULL;
        newPtr->next = NULL;
        newPtr->data = data;
        root->next = newPtr;
      } else {
      	addToTree(root->next, data);
      }
    }
  }
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
// RETURNS:
//    char*: A pointer to a character array for the word we just created
char* createString(int start, int end, char argv[]){  
  int length = end - start;
  length += 1; //+1 For null terminator
  char* szPtr = (char*)malloc(sizeof(char)*length);
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


