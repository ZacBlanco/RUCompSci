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
void test_compressr();

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
    test_compressr();

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
    free(output);
    output = get_filename(test_file2, 4);
    assert(strcmp(output, "otherthings_txt_LOLS4") == 0, "Should be the same filename");
    // printf("%s\n", output);

    free(output);
}

void test_compresst() {

    char* test_file = "test.txt";
    write_to_file("wwwoooppppdddeeeddoooooo", test_file);
    compressT_LOLS(test_file, 5);
    
    FILE* one = fopen("test_txt_LOLS0", "r");
    assert( one != NULL, "File pointer to LOLS0 should exist");
    fclose(one);

    one = fopen("test_txt_LOLS1", "r");
    assert( one != NULL, "File pointer to LOLS1 should exist");
    fclose(one);
    one = fopen("test_txt_LOLS2", "r");
    assert( one != NULL, "File pointer to LOLS2 should exist");
    fclose(one);
    one = fopen("test_txt_LOLS3", "r");
    assert( one != NULL, "File pointer to LOLS3 should exist");
    fclose(one);
    one = fopen("test_txt_LOLS4", "r");
    assert( one != NULL, "File pointer to LOLS4 should exist");
    fclose(one);
    
    test_file = "bunchastuff.txt";
    write_to_file("bb   boooddbbod", test_file);
    compressT_LOLS(test_file, 1);
    FILE* two = fopen("bunchastuff_txt_LOLS", "r");
    assert( two != NULL, "File pointer to compressed file should exist");
    fclose(two);

    //printf("-------------------------\nTesting unexisting file.\n-------------------------\n");
    test_file = "thisdoesnotexist.txt";
    compressT_LOLS(test_file, 2);

    //printf("-------------------------\nTesting bad file.\n-------------------------\n");
    test_file = "badfile.jpg";
    compressT_LOLS(test_file, 3);

    //printf("-------------------------\nTesting empty file.\n-------------------------\n");
    test_file = "emptyfile.txt";
    compressT_LOLS(test_file, 2);

    //printf("-------------------------\nTesting large split.\n-------------------------\n");
    test_file = "bunchastuff.txt";
    compressT_LOLS(test_file, 300);

    //printf("-------------------------\nTesting exact split.\n-------------------------\n");
    test_file = "10a.txt";
    compressT_LOLS(test_file, 10);

    // If you make a dir named 'newdir' this will work.
    // test_file = "./newdir/test.txt";
    // write_to_file("Rrrrrrruuuuuuttggggggeeeeerrrrrsssss coooommmmpppuuuuuuuuuuuuuuuuuuuttttteerrrrrrrrssscccciiiiieeeeennnnnceeeee", test_file);
    // compressT_LOLS(test_file, 2);

}

void test_compressr() {

    /* Basic Test of Process Compression */
    char* test1 = "zzzzzzaaaaaccccc aaaa nnnnnnnn dddddd mmiiiiikkeeee";
    char* fname = "compr_t.txt";
    write_to_file(test1, fname);
    compressR_LOLS(fname, 3);
    char* inp = read_file("compr_t_txt_LOLS0");
    assert(strcmp("6z5a5c", inp) == 0, "Strings should be equal.");
    free(inp);
    inp = read_file("compr_t_txt_LOLS1");
    assert(strcmp("4a8n3d", inp) == 0, "Strings should be equal.");
    free(inp);
    char * test_file = "aaaaaaaaaa";
    fname = "10a.txt";
    write_to_file(test_file, fname);
    compressR_LOLS(fname, 1);

    //printf("-------------------------\nTesting unexisting file.\n-------------------------\n");
    test_file = "thisdoesnotexist.txt";
    compressR_LOLS(test_file, 2);

    //printf("-------------------------\nTesting bad file.\n-------------------------\n");
    test_file = "badfile.jpg";
    compressR_LOLS(test_file, 3);

    //printf("-------------------------\nTesting empty file.\n-------------------------\n");
    test_file = "emptyfile.txt";
    compressR_LOLS(test_file, 2);

    //printf("-------------------------\nTesting large split.\n-------------------------\n");
    test_file = "bunchastuff.txt";
    compressR_LOLS(test_file, 300);

    //printf("-------------------------\nTesting exact split.\n-------------------------\n");
    test_file = "10a.txt";
    compressR_LOLS(test_file, 10);


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

    output2 = "myfile_txt_LOLS";
    m_out = get_filename(test1, -1);
    assert(strcmp(m_out, output2) == 0, "Filenames should have no number for < 0");
    free(m_out);

    char* file2 = "no_slashes_here.txt";
    output2 = "no_slashes_here_txt_LOLS";
    m_out = get_filename(file2, -1);
    assert(strcmp(m_out, output2) == 0, "No slash in filename should still pass");
    free(m_out);

    file2 = "compr_t.txt";
    output2 = "compr_t_txt_LOLS";
    m_out = get_filename(file2, -1);
    assert(strcmp(m_out, output2) == 0, "Interesting case");
    free(m_out);

    assert(c2i("1234") == 1234, "Nums should be equal");
    assert(c2i("0") == 0, "Nums should equal zero");
    assert(c2i("10") == 10, "Nums should equal ten");
    assert(c2i("104") == 104, "Nums should equal 104");
    
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


