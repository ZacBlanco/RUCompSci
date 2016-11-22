#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // compile with -pthread
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "lols.h"
#include "compressT_LOLS.h"

#define assert(x, y) lassert(x, y, __LINE__)
#define assertEqual(x, y, z) lassertEqual(x, y, z, __LINE__)

int failed = 0;
int total_assertions = 0;

void lassert(int c1, char* msg, int line);
void lassertEqual(int c1, int c2, char* msg, int line);
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
void test_lols();
void final_test_suite();

int main() {

    setup();

    //Add test methods here
    // example1();
    //example2();
    //example3();
    //etc....
    test_lols();
    test_lib_methods();
    test_file_read_write();
    test_get_filename();
    test_compresst();
    test_compressr();
    printf("====================\n");
    printf("~ Final Test Suite ~\n");
    printf("====================\n");
    final_test_suite();

    teardown();
    return finish();
}

void final_test_suite() {
    char* bad_fname = "final.nottxt";
    compressT_LOLS(bad_fname, 5);
    FILE* f = fopen("final_nottxt_LOLS0", "r");
    assert(f == NULL, "Should not get a valid pointer final.nottxt");
    compressR_LOLS(bad_fname, 5);
    f = fopen("final_nottxt_LOLS0", "r");
    assert(f == NULL, "Should not get a valid pointer from final.nottxt");
    // Should print 2 error statements
    char* fname = "things.txt";
    char* content = "rrrrruuuuutttttgggggeeeeerrrrrsssss"; //5 of each 35 total
    write_to_file(content, fname);


    //compress with N = 35 (number of characters)
    compressT_LOLS(fname, 35);
    int sl = strlen("things_txt_LOLS");
    char* nm = malloc(sizeof(char)*(sl + 3));
    strcpy(nm, "things_txt_LOLS");
    int i;
    for(i = 1; i < 35; i++) {
        nm[sl]= '\0';
        itoa(nm + sl, i);
        FILE* f2 = fopen(nm, "r");
        assert(f2 != NULL, "File should not be null");
        fclose(f2);
    }

    compressR_LOLS(fname, 35);
    for(i = 1; i < 35; i++) {
        nm[sl]= '\0';
        itoa(nm + sl, i);
        FILE* f2 = fopen(nm, "r");
        assert(f2 != NULL, "File should not be null");
        fclose(f2);
    }
    free(nm);


    // Test where workers is greater
    // Should print 2 errors
    compressT_LOLS(fname, 36);
    compressR_LOLS(fname, 36);

    //Test normal
    compressT_LOLS(fname, 7);
    char* cntnt = read_file("things_txt_LOLS0");
    assert(strcmp(cntnt, "5r") == 0, "First file should have 5r");
    free(cntnt);

    compressR_LOLS(fname, 7);
    cntnt = read_file("things_txt_LOLS0");
    assert(strcmp(cntnt, "5r") == 0, "First file should have 5r");
    free(cntnt); 

    // Nonexistent file
    char* test_file = "thisdoesnotexist.txt";
    compressT_LOLS(test_file, 2);
    compressR_LOLS(test_file, 2);

    // Empty File
    // Should print 2 errors
    test_file = "emptyfile.txt";
    compressT_LOLS(test_file, 2);
    compressR_LOLS(test_file, 2);


    // Workers <= 0
    compressT_LOLS(test_file, 0);
    compressR_LOLS(test_file, 0);
    compressT_LOLS(test_file, -1);
    compressR_LOLS(test_file, -1);











    // char*
}

void test_lols() {

    char* str1 = "aaabbbccc";
    char* a1 = lols(str1);
    assert(strcmp(a1, "3a3b3c") == 0, "aaabbbccc should lols to 3a3b3c");
    free(a1);
    str1 = "12322093";
    a1 = lols(str1);
    assert(strcmp(a1, "") == 0, "12322093 should lols to nothing");
    free(a1);
    str1 = "a 2345243523452";
    a1 = lols(str1);
    assert(strcmp(a1, "a") == 0, "a 2345243523452 should lols to a");
    free(a1);
    str1 = "234982734092742347 a";
    a1 = lols(str1);
    assert(strcmp(a1, "a") == 0, "234982734092742347 a should lols to a");
    free(a1);

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
    assert(strcmp(output, "things_txt_LOLS2") == 0, "Should be the same filename for LOLS2");
    free(output);
    // printf("%s\n", output);
    output = get_filename(test_file2, 4);
    assert(strcmp(output, "otherthings_txt_LOLS4") == 0, "Should be the same filename for LOLS4");
    free(output);
    // printf("%s\n", output);

}

void test_compresst() {

    char* test_file = "test.txt";
    write_to_file("wwwoooppppdddeeeddoooooo", test_file);
    compressT_LOLS(test_file, 5);
    
    FILE* one = fopen("test_txt_LOLS0", "r");
    assert( one != NULL, "File pointer to LOLS0 should exist");
    fclose(one);
    char* f = read_file("test_txt_LOLS0");
    assert(strcmp(f, "3w3opp") == 0, "First section should be '3w3opp'");
    // printf("Got: %s\n", f);
    free(f);

    one = fopen("test_txt_LOLS1", "r");
    assert( one != NULL, "File pointer to LOLS1 should exist");
    fclose(one);
    f = read_file("test_txt_LOLS1");
    assert(strcmp(f, "ppdd") == 0, "First section should be 'ppee'");
    // printf("Got: %s\n", f);
    free(f);

    one = fopen("test_txt_LOLS2", "r");
    assert( one != NULL, "File pointer to LOLS2 should exist");
    fclose(one);
    f = read_file("test_txt_LOLS2");
    assert(strcmp(f, "d3e") == 0, "First section should be 'd3e'");
    free(f);

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

    // printf("-------------------------\nTesting unexisting file.\n-------------------------\n");
    test_file = "thisdoesnotexist.txt";
    compressT_LOLS(test_file, 2);

    //printf("-------------------------\nTesting bad file.\n-------------------------\n");
    test_file = "badfile.jpg";
    compressT_LOLS(test_file, 3);
    test_file = "lol.docx";
    compressT_LOLS(test_file, 10);

    //printf("-------------------------\nTesting empty file.\n-------------------------\n");
    test_file = "emptyfile.txt";
    write_to_file("", test_file);
    compressT_LOLS(test_file, 2);

    //printf("-------------------------\nTesting large split.\n-------------------------\n");
    test_file = "bunchastuff.txt";
    compressT_LOLS(test_file, 300);

    //printf("-------------------------\nTesting bad content.\n-------------------------\n");
    test_file = "badcontent.txt";
    write_to_file("123423  !!3433939", test_file);
    compressT_LOLS(test_file, 4);

    //printf("-------------------------\nTesting some bad content.\n-------------------------\n");
    test_file = "somebadcontent.txt";
    write_to_file("a                      3434343       aa", test_file);
    compressT_LOLS(test_file, 3);

    // //printf("-------------------------\nTesting exact split.\n-------------------------\n");
    test_file = "10a.txt";
    compressT_LOLS(test_file, 10);

    // //printf("-------------------------\nTesting large characters and split.\n-------------------------\n");
    test_file = "giant.txt";
    write_to_file("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb              cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc;;;;;;;;dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeefffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggglolhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh", test_file);
    compressT_LOLS(test_file, 1);

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

    char* output = "./path/to/myfile_txt_LOLS0";
    char* m_out = get_filename(test1, 0);
    assert(strcmp(m_out, output) == 0, "Filenames should be equal");
    printf("Filename: %s\n", m_out);
    free(m_out);

    char* output2 = "./path/to/myfile_txt_LOLS2";
    m_out = get_filename(test1, 2);
    assert(strcmp(m_out, output2) == 0, "Filenames should be equal for 2 parts");
    free(m_out);

    output2 = "./path/to/myfile_txt_LOLS100";
    m_out = get_filename(test1, 100);
    assert(strcmp(m_out, output2) == 0, "Filenames should be equal for 100");
    free(m_out);

    output2 = "./path/to/myfile_txt_LOLS99909";
    m_out = get_filename(test1, 99909);
    assert(strcmp(m_out, output2) == 0, "Filenames should be equal for 99909");
    free(m_out);

    output2 = "./path/to/myfile_txt_LOLS";
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

void lassert(int c1, char* msg, int line) {
    if(!c1) {
        printf("============================================\n");
        printf("Assert condition failed %i != 1. on line %i of test_rle\n", c1, line);
        printf("%s\n", msg);
        printf("============================================\n");
        failed++;
    }
    total_assertions++;
    
}

void lassertEqual(int c1, int c2, char* msg, int line) {
    total_assertions++;
    if(c1 != c2) {
        printf("============================================\n");
        printf("Assert condition failed %i != %i on line %i of test_rle\n", c1, c2, line);
        printf("%s\n", msg);
        printf("============================================\n");
        failed++;
    }
}


