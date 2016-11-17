//This file goes througha somewhat exhaustive review of the features C including dynamic memory allocation.

//Include basically takes the contents of file and adds it to the location where #include is.
//This gives us access to more functions.
// '<' and '>' tell us we're referencing a standard library.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Use "" for any of your own headers.
//#include "my_header.h"

//Function declarations - See functions pointer and declaration section.
double square(double x);
void string_copy(char* dest, char* src);

//Here we include a file which isn't part of the standard c libraries.
// Double quotes '"' are used to denote libraries which it should search
// for in the local directory when compiling
//#include "mylib.h"

//Main here is the entry point for the program
// It is similar to the "public static void main(String[] args)"
// That is found in the java programming language
// ARGUMENTS:
//     int argc: The number of arguments supplied by the command line
//  char** argv: The  2D array of char arguments (We'll get into types soon)
int main(int argc, char **argv)
{

  //Seed the random number generator.
  srand(time(NULL)); // This line helps us generate random numbers.

  //C is a very robust language. It doesn't have a ton of pretty features
  //but what C does do is give the programmer (user) lots and lots of
  //control over what happens throughout the life of the program.

  //One of the main advantages of C over many other is that it gives the
  //user much more responsibility for managing the memory of a program
  // than almost any other language. To create dynamically changing structures
  // like a binary tree or linked list, we need to tell the system that
  // we need to allocate more memory. Similar to Java, it is like using the
  // "new" keyword in order to create an object

  // And on that note we should say that the basic C implementation does NOT
  // support typical object-oriented programming constructs. In C, there is
  // no notion of an "object". (Although we have something close which will be
  // discussed later as well). All we have are the following basic datatypes

  //char
  //unsigned char
  //signed char
  //int
  //unisgned int
  //short
  //unsigned short
  //long
  //unsigned long
  //float
  //double
  //long double
  //void <-- We'll get to this later.

  //Notice that C does not have the "boolean" or "String" type!

  // sizes of basic datatypes
  printf("Line %i: sizeof char = %zu\n", __LINE__, sizeof(char));
  printf("Line %i: sizeof short = %zu\n", __LINE__, sizeof(short));
  printf("Line %i: sizeof int = %zu\n", __LINE__, sizeof(int));
  printf("Line %i: sizeof long = %zu\n", __LINE__, sizeof(long));
  printf("Line %i: sizeof float = %zu\n", __LINE__, sizeof(float));
  printf("Line %i: sizeof double = %zu\n", __LINE__, sizeof(double));
  printf("Line %i: sizeof void = %zu\n", __LINE__, sizeof(void));

  //You can declare variables just like in Java
  // many of the same rules apply to identifiers
  int my_first_int = 1234;
  char my_first_char = 'a';

  //But C has still got some tricks up its sleeves
  int *int_ptr = &my_first_int;
  //WOAH WOAH HOLD YOUR HORSES BUD. What just happened??

  //What's up with that '*' and the '&'??!?

  //This is where C begins to get really powerful. It allows us to manually
  //access the memory where our variables are store. Remember that the
  //information in a variable must be stored somewhere in memory. Each place
  // in memory has a unique address assigned to it. Those special addresses
  // are called __pointers__. Pointers are incredibly important to  memory management
  // They tell us the exact location where memory is stored. Every variable in C
  // will have a pointer as well.

  //In C, every single variable must have a pointer. You can think of a
  //variable in C as having **two** separate values, sometimes called the left and
  // right values. One value is the actual variable value - the _real_ value. Then
  //the other value is the memory pointer. or the location in memory where the
  //variable is stored.

  //So let's dissect line 57 up there.
  //The first half "int* int_ptr" declares a new variable named "int_ptr". This
  //pointer is typically 8 or so bytes on modern systems. But by adding in the "*"
  //it tells the compiler that "int_ptr" points to the memory address of an integer.
  //NOT an actual integer. We can consider pointers the "left value" of a variable.

  // The second half "= &my_first_int" is assigning "int_ptr" to have the pointer (left)
  // value of "my_first_int". The '&' characters is one that dereferences the integer value
  // and represents now the location of that variable's pointer instead.

  //So let's do something useful and...print out the value of "my_fist_int".
  // Note: __LINE__ stands for the current line number of the statement.

  //Print using the my_first_int variable
  // use %i to denote integers.
  printf("Line %i: Look here's my_first_int the first way: %i!\n", __LINE__, my_first_int);

  //By putting a "*" in front of int_ptr we are doing what is called "dereferencing"
  // This basically replaces *(pointer_name) with the value of the variable in memory
  // the "right" value.
  printf("Line %i: Look here's my_first_int the second way: %i!\n", __LINE__, *int_ptr);

  //Let's see what happens when we try to print the pointer without dereferencing
  printf("Line %i: Trying to print pointer as int %i \n", __LINE__, int_ptr);
  //If you run the program you'll probably get a crazy value which is NOT 1234.
  //Let's actually check out the pointer value.
  // use %p to print pointers
  printf("Line %i: The real pointer location: %p\n", __LINE__, int_ptr);

  //Woah! Cool! We can print variables and memory addresses now.
  //Let's start doing some more advanced memory manipulation.

  /////////////////////////
  ///// Basic Arrays //////
  /////////////////////////

  //We can declare arrays like this:
  int int_arr[10]; //Creates an int array for 10 ints.

  // Let's initialize some values.
  int_arr[0] = -5;
  int_arr[1] = -2;
  int_arr[2] = 0;
  //Okay so let's try to play with these using some of those memory techniques.

  printf("Line %i: Memory location of int_arr: %p \n", __LINE__, &int_arr);
  printf("Line %i: Value of int_arr[0] = %i\n", __LINE__, int_arr[0]);

  int *p1 = int_arr;
  // Hold up there partner, what the heck? I thought int_arr was an array of int
  // types! Not a pointer? How are we doing this assignment?
  // Well as it turns out, arrays don't *actually* exist in C. Really  the line
  // "int int_arr[10];" Just tells the compiler it needs to allocate space
  // for 10 integers, and the value of int_arr is really just a pointer.
  // The brackets are actually doing arithmetic on the pointer as well as
  // dereferencing the pointer at the same time.
  printf("Line %i: Value of int_arr[2] = %i\n", __LINE__, p1[2]);

  // A question that you might ask is: Why exactly do pointers need types
  // even if they're all 8 bytes?
  // The reason is because this tells the program exactly how much to increase
  // the pointer by when doing arithmetic. Example:
  p1 += 1;
  printf("Line %i: Value of p1 + 1 ==> %i\n", __LINE__, *p1);

  //From the above line of code we see that by increasing the value of p1 by
  // 1 that we get the value in int_arr[1].

  //Okay now let's start doing some cooler things.
  //Let's square all the number from 1 to 100 using two pointers.
  int sq_len = 100;
  int squares[sq_len];
  int *sq_ptr = squares;
  int sq_iter;
  for (sq_iter = 0; sq_iter < sq_len; sq_iter++)
  {
    *sq_ptr = (sq_iter + 1) * (sq_iter + 1);
    sq_ptr++;
  }

  // We could actually implement this in a similar way, but I wanted
  // to show that it was possible to use a different pointer to
  // manipulate the

  printf("Line %i: Square of 9: %i\n", __LINE__, squares[9]);
  printf("Line %i: Square of 87: %i\n", __LINE__, squares[86]); //squares[0] == 1^2

  //See if you can understand yourself how this code works.

  ////////////////////////////////
  ///// 2 Dimensional Arrays /////
  ////////////////////////////////

  // Okay so we know how 1D arrays work now, how can we declare 2D arrays?
  int l1, l2;
  l1 = 3;
  l2 = 3;
  int myarr[l1][l2];
  myarr[0][0] = 0;
  myarr[0][1] = 1;
  myarr[1][0] = 2;
  myarr[1][1] = 3;
  myarr[0][2] = 1234;
  // myarr[2][2] = 10;

  //Let's print the memory addresses in a nice table.
  int i, j;
  for (i = 0; i < l1; i++)
  {
    for (j = 0; j < l2; j++)
    {
      printf("[%i][%i]: Rel. Loc.: %2li, %p | ", i, j, (&myarr[i][j] - &myarr[0][0]), &myarr[i][j]);
    }
    printf("\n");
  }

  //Now to go over some important concepts regarding dereferencing 2D+ arrays
  // Look at the difference in the following lines
  printf("Line %i: **(myarr + 1) Ptr %p, Value: %i\n", __LINE__, &(**(myarr + 1)), **(myarr + 1)); //myarr[1][0]
  printf("Line %i: *(*myarr + 1) Ptr %p, Value: %i\n", __LINE__, &(*(*myarr + 1)), *(*myarr + 1)); //myarr[0][1]

  //Note from this that we need to _dereference_ the 2D array TWICE in order to access the value.
  //Also note that dereferencing the pointer only once will return a pointer value, NOT the
  // a number. We can access the same value by using brackets as well. This acts as a an addition
  // and a dereference at the same time.

  // Let's try some more.
  printf("Line %i: Ptr %p, myarr[0][0]: %i\n", __LINE__, &myarr[0][0], myarr[0][0]);
  printf("Line %i: Ptr %p, *(myarr[1] + 1): %i\n", __LINE__, myarr[1] + 1, *(myarr[1] + 1));
  printf("Line %i: Ptr %p, **myarr: %i\n", __LINE__, myarr, **myarr);
  printf("Line %i: Ptr %p, (*myarr + 1)[1]: %i\n", __LINE__, &(*myarr + 1)[1], (*myarr + 1)[1]);
  printf("Line %i: Ptr %p, *(myarr + 1)[1]: %i\n", __LINE__, &*(myarr + 1)[1], *(myarr + 1)[1]);
  printf("Line %i: Ptr %p, myarr[2][2]: %i\n", __LINE__, &myarr[2][2], myarr[2][2]);

  //You should not that by declaring an array in C, like above, we don't actually
  //initialize any values by using the statement int x[a][b]. All this does is
  // allocate memory for us. The values in the location are not guaranteed to be
  // any value unless the array is declared as static.

  //A way to initialize values:
  int x[5] = {5, 4, 3, 2, 1};
  for (i = 0; i < 5; i++)
  { // We defined 'i' at an earlier point.
    printf("Line %i: x[%i] = %i\n", __LINE__, i, x[i]);
  }

  //Okay so now we've got some idea of how arrays work and we can work with pointers
  // a bit. But now the question arises...how can we create dynamic structures?
  // The idea is that by compiling code, we have an exact number of  set, defined
  // variables. However typical programs won't have that. How can we read a file whose
  //length we don't know?

  //Or how can we allocate an array of a random size that we don't know until
  // runtime? This is where we get to use a fancy command called "malloc"  and "free"
  // You may have heard about garbage collection in Java. Well, C doesn't really have
  //any of that. It allocates the memory for the "set" variables it needs at runtime.
  //The memory addresses of those variables are stored on the stack. They don't
  //change when you call the methods. But what malloc let's us do is request the ope-
  //-rating system for more memory **dynamically**. Remember with great power
  // come great responsibility. We can allocate as much memory for our program as we
  // need, but that also takes away resources for the rest of the system. Whenever
  // we write programs we should write them in such a way they only take the resources
  // that they need, and relinquish them when no longer needed.

  // To allocate dynamic memory we can use the method called "malloc" which is
  // inside of <stdlib.h>. To be able to use malloc you simply need to include
  // the following line at the top of your source file.
  //
  //  #include <stdlib.h>
  //
  // You can see that in this file we've already included it.

  //Lets allocate an array with a random size between 1 and 10;
  // rand() is also a part of stdlib
  int a_length = (rand() % 10) + 1; //Random int between 1 and 10.
  printf("Line %i: Dynamic array length:  %i\n", __LINE__, a_length);

  //Okay, so now we need to create a new array of size a_length
  int *d = malloc(sizeof(int) * a_length);

  //Notice here that we're not actually declaring an array.
  //Remember that a '*' means we're getting a pointer to a
  //memory location.

  //malloc has a signature like:
  //
  // malloc(size_t size);
  //
  // 'size_t' is just a special type similar to an unsigned int. The
  // number passed as size_t is the number of bytes that will be allocated.
  // The return value is the pointer to the very beginning of that memory
  // which was allocated by malloc. Note that trying to access memory
  // which was not specifically allocated by malloc can result in a segmentation
  // fault and the termination of the program.
  //
  // By passing the sizeof(int) we are able to get a platform-agnostic
  // implementation due to the fact that different machines may have integers
  // which are either 4 bytes or 8 bytes long. Using sizeof(type) will return the
  // correct size (in bytes) of the datatype we're using, then we simply multiply
  // it by the length of the array we want, and using malloc we then get a memory
  // block which is the size of 'length' integers.
  //

  //Now we can treat 'd' like a regular array.
  for (i = 0; i < a_length; i++)
  {
    d[i] = 42 * (i + 1);
    printf("Line %i: Dynamic Array: i=%i; Val=%i\n",__LINE__,  i, d[i]);
  }

  // Another thing might be useful to know is some of the "different" malloc
  // calls. They are
  // 
  // * calloc(size_t number of elements, size_t size of element) - Zeros the memory first before returning a pointer
  // * realloc(void* old ptr, size_t new size) - Will change the memory size of a previously allocated space
  // 
  // Read the man pages with `man malloc` for more information on these functions' behavior

  // Remember how with great power comes great responsibility? Well here's where
  // that comes into play. We've allocated our array in a dynamic manner, and now
  // we're done using it. This means that we should **free** the memory so that it
  // can be allocated to other programs. To do this we simply use 'free'.

  free(d);
  // free(d); <-- This is undefined behavior and result is implementation dependent.

  // d has now been freed and other programs will be able to request access to its
  // The memory is once occupied.Now obviously 8-80 bytes isn't a terribly large
  // amount of memory, but in a large program it can start to accumulate quickly.
  //
  // One thing that is immensely important to look out for is trying to access
  // invalid memory pointers. That may include
  //
  // * Trying to dereference a NULL (0x0) pointer
  // * Trying to access memory not allocated by the programs process.
  // * Attempting to access a non-existant memory address.
  // * Attempting to write to a read-only portion of memory.
  //
  // Examples of some code which cause segmentation faults.
  //
  // int* e = 0; // doesn't cause segfault.
  // e[0] = 10;
  // int i = e[0];
  // free(e); 

  // STRUCTS and UNIONS
  // Structs are a way to store data kind of like an object in java except they don't really
  // have methods (kind of). 
  
  //Lets define a struct for complex numbers
  struct complex
  {
    int r;
    int i;
  };

  // Set complex_number equal to something by using bracket notation to define arguments in order
  struct complex complex_number = {5, 5};
  // We can set the struct fields to values by using bracket notation.

  struct complex complex_2;
  // However, structs can't be set with bracket notation on a line that they aren't defined on.
  // This is an error
  // complex_2 = {1, 2}

  //but we can access the fields in a simliar way to Java
  complex_2.r = 1;
  complex_2.i = 2;

  /*One thing about structs is that a lot of the time they're created dynamically - meaning we don't
    * know how many structs we'll actually need in our program. Thus we'll need to malloc a new block of
    * memory. But remember malloc returns a pointer. This changes how we'll access the data.
    *
    * This means we need to know the size of the struct as well. Fortunately the sizeof function takes
    * care of that for us.
    */

  printf("Line %i: Sizeof our complex struct: %zu\n", __LINE__, sizeof(struct complex));
  printf("Line %i: Sizeof integer: %zu\n", __LINE__, sizeof(int));

  // So we notice that sizeof is simply the sum of the sizes of its integers. But what about this?
  struct big
  {
    char a;
    int b;
    long c;
    char d;
  } big_test; // This line lets us define some integers for big_test

  big_test.a = 5; //Show that we can set values.

  // Let's check the size of the new struct now.
  size_t ts = sizeof(char) + sizeof(int) + sizeof(long) + sizeof(char);
  printf("Line %i: Sizeof the fields a + b + c + d =  %zu + %zu + %zu + %zu = %zu\n", __LINE__, sizeof(char), sizeof(int), sizeof(long), sizeof(char), ts);
  printf("Line %i: Sizeof our big struct: %zu\n", __LINE__, sizeof(struct big));

  // Wait wait hold on!! What gives?? Why is the struct size so much larger than the actual sizes?
  // The reason is because of memory alignment. In this case We want our blocks to be aligned to
  // the words in a memory. So to store the struct properly it needs to be padded. See below

  /*
    *  +----------+----------+----------+----------+----------+----------+----------+----------+
    *  |     a    |    pad   |     pad  |     pad  |                     a                     |
    *  +----------+----------+----------+----------+----------+----------+----------+----------+
    *  |                                           b                                           |
    *  +----------+----------+----------+----------+----------+----------+----------+----------+
    *  |     d    |    pad   |     pad  |     pad  |    pad   |     pad  |     pad  |   pad    |
    *  +----------+----------+----------+----------+----------+----------+----------+----------+
    *    
    * This picture describes how the memory gets aligned. We end up getting 7 extra pad bytes after
    * char d because we need to fill out the rest of the word b takes up (if you're on a 32 bit system, then
    * you struct size may only be 20 and get 3 pad bytes on top of d instead to do word sizes being smaller.)
    *
    * 
    */

  //Another thing we should go over is dynamic memory with structs
  struct complex *c3 = malloc(sizeof(struct complex));

  // Notice now that our struct is a pointer struct. In order to to access we'll need to dererference it.
  (*c3).r = 2;

  // Fortunately C actually makes it a bit easier to access the fields of a struct pointer by using
  // a special access syntax with the '->' sequence of characters instead of (*ptr).
  c3->i = 2; // equivalent to (*c3).i = 2;
  printf("Line %i: c3 : { .r = %i, .i = %i } \n", __LINE__, c3->r, c3->i);

  //Rename struct pointers to use '.'
  //c3.r // Not valid
  struct complex c4 = *c3; //Copies
  printf("Line %i: c3->r = %i, c4.r = %i } \n", __LINE__, c3->r, c4.r); // These are NOT the same.
  c4.r = 4;
  printf("Line %i: c3->r = %i, c4.r = %i } \n", __LINE__, c3->r, c4.r); // These are NOT the same.
  printf("Line %i: c3-r ptr: %p, c4.r ptr: %p\n", __LINE__, &c3->r, &c4.r );
  
  free(c3);
  //Need to free when we're done!
  

  //There's also this really convenient keyword that we can use with structs called 'typedef'. Instead
  // of always using 'struct struct_name identifier' using typdef lets us rename certain types to help
  // make code more readable

  typedef double Velocity;
  Velocity current_speed; // This is just an int but it can make our code more readable.
  Velocity dv = 10;
  current_speed += dv; 

  // Using this we can make it pretty easy to define object-like types using structs and typedef.
  typedef struct vector {
    double x;
    double y;
  } Vector;

  // You can also define it such
  // typedef struct {
  //   double x;
  //   double y;
  // } Vector;
  //
  // Notice we got rid of 'vector'

  //Let's use our vector to do some physics calculations
  Vector displacement = { 0, 0 };
  Vector vel = {2, 7};
  Vector accel = {0, -9.8};
  double t = 10;
  displacement.x = vel.x*t + 0.5*accel.x*t*t;
  displacement.y = vel.y*t + 0.5*accel.y*t*t;
  printf("Line %i: Displacement after t = 10s: { %3.2f x, %3.2f y }\n", __LINE__, displacement.x, displacement.y);

  // UNIONS
  //
  // Unions are kind of an interesting structure with some behavior that isn't mimicked in too
  // many other languages.
  //

  union my_union {
    int x;
    long y;
    char z;
  } u1;

  printf("Line %i: sizeof union: %zu\n", __LINE__, sizeof(union my_union));
  u1.y = 222247000000;
  printf("Line %i: x: %i ; y: %ld ; z: %c \n", __LINE__, u1.x, u1.y, u1.z);
  u1.x = 90;
  printf("Line %i: x: %i ; y: %ld ; z: %c \n", __LINE__, u1.x, u1.y, u1.z);
  u1.z = 'b';
  printf("Line %i: x: %i ; y: %ld ; z: %c \n", __LINE__, u1.x, u1.y, u1.z);

  //From this you can see how the memory addresses in a union work. The memory size of a union must
  // at least match the largest size field. Then you can see that when you set a field it will
  // overwrite the necessary starting from the 0th byte to the nth byte which represents the value
  // and size of the field being set. 

  // ENUMS
  
  // Enums are going to be the last "complex" type that we cover
  // They allow us to reference values by name rather than integer
  // in order to make readable code;

  enum Fruit {
    APPLE,
    PEAR,
    ORANGE,
    TOMATO=-1
  };
  enum Fruit fruits = PEAR;
  printf("Line %i: TOMATO: %i \n", __LINE__, TOMATO);
  // printf("Line %i: PEAR: %i \n", (int)fruits); // This doesn't actually work.


  // FUNCTIONS AND FUNCTION POINTERS
  //
  // First off, I suggest looking below the "main" function here as I've added
  // a couple methods. They are basic and show the syntax for how a method
  // is defined. It is essentially the same as java except for the fact 
  // that we are able to return normal types AND pointers.

  // Note that it is typical practice to define any function in your C file
  // (if including the main method) at the bottom of the file. However, in 
  // order for the compiler not to throw any errors or a fit you'll need to 
  // define the function towards the top of the file. Typically right after
  // all of the #includes for the functions. See the top of the file and
  // You'll probably see what I mean. 
  //
  // Also for any large number of functions you'll typically write a 
  // *.c file for all of your implementations of your functions, but
  // you'll also write a *.h file for all of your function definitions
  // and include that file in your *.c file. The *.h (header) file
  // defines all of the signatures for the functions that are used
  // within the file. The header files however do NOT need to be 
  // compiled.


  // Basic function usage
  //
  // char* ay123 = "Hello there";
  // char* ccc = malloc(sizeof(char)*12);
  // string_copy(ccc, ay123);
  // printf("%s\n", ccc);
  // printf("%s\n", ay123);

  // FUNCTION POINTERS
  
  //One really interesting part of C is that we can actually have variables
  // which represent functions. They are called "Function Pointers because"
  // they point to the location where the function code resides
  //
  // We define them like this:
  // 
  // RETURN_TYPE (*variable_name)(type_1, arg1, type_2 arg2, ...) = &the_reference_function;
  //
  // So we have this "square" function - let's try to get a function pointer from it

  double (*sq)(double) = &square; // Basically just renaming the function.
  printf("Line %i: Using function pointer to square 5. Result : %.1lf \n", __LINE__, sq(5));

  /////////////////
  ///   const   ///
  /////////////////
  

  //const is a special variable modifier in c that stops you from changing a 
  // value of a variable. It can be used on a pointer or a variable and
  // surprisingly can be quite confusing.
  
  
  const int a = 5; // the value of "a" CANNOT be changed
  const int * aPtr = &a; // The VALUE at address &a CANNOT be changed.
  const int* const b = &a; // The VALUE and ADDRESS CANNOT be changed.
  char* const cptr  = "ABC123"; // Only the ADDRESS CANNOT be changed. HOWEVER "ABC123" is Read-Only string to the compiler so we can't edit it.

  // Compiler errors
  // a = 6;
  // aPtr[1] = 2;
  // *b = 10;
  // b++;
  // cptr = "Cool Cool"; // <--- Different string address
  // *cptr = 'a'; //<--- causes a segfault 

  // VALID
  aPtr += 1; //also aPtr++ or aPtr--;
  (*(int*)&a)++; //Pointer indirection to modify const variable. -- no compiler error.
  printf("Line %i: a: %i \n", __LINE__, a);
   

   
  return 0;
}


//Square a number.
double square(double x){
  return x*x;
}

//Very very primitive strcpy function
void string_copy(char* dest, char* src) {

  int i;
  while (src[i] != '\0') { //Loop till we hit the end.
    dest[i] = src[i]; //Dereferences src[i] for value and sets the dereference of dest[i] to that of src[i].
    i++;
  }

  return;

}