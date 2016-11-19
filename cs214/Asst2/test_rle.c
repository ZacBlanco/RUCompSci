#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // compile with -pthread
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "lols.h"
#include "compressT_LOLS.h"
int failed = 0;
int total_assertions = 0;

void assert(int c1, char* msg);
void assertEqual(int c1, int c2, char* msg);
void example1();
void setup();
void teardown();
int finish();

//Test methods
void test_file_read_write();
void test_compresst();
void test_get_filename();
void test_is_valid_filename();
void test_lib_methods();

int main() {

    setup();

    //Add test methods here
    // example1();
    //example2();
    //example3();
    //etc....
    test_lib_methods();
    test_file_read_write();
    test_get_filename();
    test_compresst();


    teardown();
    return finish();
}

void test_file_read_write() {

    char* a = "ABC1234567890DEFGHIJKLMOPQRSTUVWXYZ";
    char* test_file = "test.txt";

    assert(write_to_file(a, test_file) == 1, "Should be able to successfully write to a file");

    FILE* f1 = fopen(test_file, "r");
    assert(f1 != NULL, "test.txt should exist after writing to file");
    fclose(f1);

    char* a2 = read_file(test_file);
    // printf("%s\n", a2);
    assert(strlen(a2) == strlen(a), "Written and read strings should be the same size.");
    if (strlen(a2) == strlen(a)) {
        int l  = strlen(a);
        int i;
        for(i=0; i < l;i++) {
            assert(a[i] == a2[i], "File read and write characters not equal");
        }
    }

    free(a2);
}

void test_get_filename() {
    char * test_file = "things.txt";
    char * test_file2 = "otherthings.txt";
    char * output = get_filename(test_file, 2);

    assert(strcmp(output, "things_txt_LOLS2") == 0, "Should be the same filename");
    // printf("%s\n", output);
    output = get_filename(test_file2, 4);
    assert(strcmp(output, "otherthings_txt_LOLS4") == 0, "Should be the same filename");
    // printf("%s\n", output);
}

void test_compresst() {

    char* test_file = "test.txt";
    write_to_file("wwwoooppppdddeeeddoooooo", test_file);
    compressT_LOLS(test_file, 5);
    test_file = "bunchastuff.txt";
    write_to_file("bbboooddbbod", test_file);
    compressT_LOLS(test_file, 1);

}

void test_lib_methods() {

    char* test1 = "./path/to/myfile.txt";
    char* output = "myfile_txt_LOLS0";
    char* m_out = get_filename(test1, 0);
    assert(strcmp(m_out, output) == 0, "Filenames should be equal");
    free(m_out);

    char* output2 = "myfile_txt_LOLS2";
    m_out = get_filename(test1, 2);
    assert(strcmp(m_out, output2) == 0, "Filenames should be equal for 2 parts");
    free(m_out);

    output2 = "myfile_txt_LOLS100";
    m_out = get_filename(test1, 100);
    assert(strcmp(m_out, output2) == 0, "Filenames should be equal for 100");
    free(m_out);

    output2 = "myfile_txt_LOLS99909";
    m_out = get_filename(test1, 99909);
    assert(strcmp(m_out, output2) == 0, "Filenames should be equal for 99909");
    free(m_out);
    
}

//Perform any setup before running the tests
void setup() {
    //Add any necessary steps here.
}

// Perform any cleanup after running the tests.
void teardown() {
    //Add any necessary steps here.
}



/* Don't need to worry about this code
 * Just defines some examples and other code for testings
 * All tests should be defined *above* this section.
*/

void example1() {
    assert(1, "Should be okay");
    // assert(0, "Should Fail"); //FAILS
    // assertEqual(2, 3, "This test should fail");
    assertEqual(5, 5, "This test should pass");
}

//Print out some summary information
int finish() {
    if (failed > 0) {
        printf("%i of %i assertion(s) failed to pass.\n", failed, total_assertions);
    } else {
        printf("%i assertion(s) passed successfully\n", total_assertions);
    }
    return failed > 0;
}

void assert(int c1, char* msg) {
    if(!c1) {
        printf("============================================\n");
        printf("Assert condition failed %i != 1.\n", c1);
        printf("%s\n", msg);
        printf("============================================\n");
        failed++;
    }
    total_assertions++;
    
}

void assertEqual(int c1, int c2, char* msg) {
    total_assertions++;
    if(c1 != c2) {
        printf("============================================\n");
        printf("Assert condition failed %i != %i.\n", c1, c2);
        printf("%s\n", msg);
        printf("============================================\n");
        failed++;
    }
}


