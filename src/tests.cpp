#include <gtest/gtest.h> // googletest header file

#include <string>
using std::string;


extern "C" {
    #include "ll.h"
    #include "curlwrapper.h"
}

void num_teardownss(void *n) {
    *(int *)n *= -1; // just so we can visually inspect removals afterwards
}

void num_printerss(void *n) {
    printf(" %d", *(int *)n);
}

int num_equals_3ss(void *n) {
    return *(int *)n == 3;
}


//This is an example of semi working, but absolute garbage testing
TEST(ll_lib, bad_tests) {
   int *_n; // for storing returned ones
    int test_count = 1;
    int fail_count = 0;
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int e = 4;
    int f = 5;
    int g = 6;
    int h = 3;
    int i = 3;

    ll_t *list = ll_new(num_teardownss);
    list->val_printer = num_printerss;

    ll_insert_first(list, &c); // 2 in front

    _n = (int *)ll_get_first(list);
    if (!(*_n == c)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, c, *_n);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (list->len != 1) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 1, list->len);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_first(list, &b); // 1 in front
    ll_insert_first(list, &a); // 0 in front -> 0, 1, 2

    _n = (int *)ll_get_first(list);
    if (!(*_n == a)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, a, *_n);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (!(list->len == 3)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 3, list->len);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_last(list, &d); // 3 in back
    ll_insert_last(list, &e); // 4 in back
    ll_insert_last(list, &f); // 5 in back

    _n = (int *)ll_get_n(list, 5);
    if (!(*_n == f)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, f, *_n);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (!(list->len == 6)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 6, list->len);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_n(list, &g, 6); // 6 at index 6 -> 0, 1, 2, 3, 4, 5, 6

    int _i;
    for (_i = 0; _i < list->len; _i++) { // O(n^2) test lol
        _n = (int *)ll_get_n(list, _i);
        if (!(*_n == _i)) {
            fail_count++;
            fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", 1, _i, *_n);
        } else
            fprintf(stderr, "PASS Test %d!\n", test_count);
        test_count++;
    }

    // (ll: 0 1 2 3 4 5 6), length: 7

    ll_remove_first(list);                // (ll: 1 2 3 4 5 6), length: 6
    ll_remove_n(list, 1);                 // (ll: 1 3 4 5 6),   length: 5
    ll_remove_n(list, 2);                 // (ll: 1 3 5 6),     length: 4
    ll_remove_n(list, 5);                 // (ll: 1 3 5 6),     length: 4; does nothing
    ll_remove_search(list, num_equals_3ss); // (ll: 1 5 6),       length: 3
    ll_insert_first(list, &h);            // (ll: 3 1 5 6),     length: 5
    ll_insert_last(list, &i);             // (ll: 3 1 5 6 3),   length: 5
    ll_remove_search(list, num_equals_3ss); // (ll: 1 5 6 3),     length: 4
    ll_remove_search(list, num_equals_3ss); // (ll: 1 5 6),       length: 3

    ll_print(*list);

    ll_delete(list);

    if (fail_count) {
        fprintf(stderr, "FAILED %d tests of %d.\n", fail_count, test_count);
        return;
    }

    fprintf(stderr, "PASSED all %d tests!\n", test_count);
    
}


///!_SKELETON
//!_ // add a test here to cause a segmentation fault and fix that fault
//!_ // HINT - look at linked list initiliazation, insert functions and argument checking
//!_ TEST(agent_lib, segfault) {
//!_     
//!_ }
///!_SKELETON

///!_SOLUTION

//checks to see all the functions work with null values
TEST(ll_lib, segfault) {     
    //ll_insert_first(NULL, (void*) "asd");
}
///!_SOLUTION

////////////////////////////////
////////// CURL TESTS ////////// 
////////////////////////////////

///!_SKELETON

//!_ TEST(curl_lib, get) {
//!_     
//!_ }

//!_ TEST(curl_lib, post) {
//!_     
//!_ }
///!_SKELETON

///!_SOLUTION

TEST(curl_lib, get) {
    get_request((char *)"http://localhost:8000");     
}

TEST(curl_lib, get_invalid) {
    get_request((char *)"http://localhost:808make0");     
}

TEST(curl_lib, post) {
     post_request((char *)"http://localhost:8000",(char*)"helloworld");
}
///!_SOLUTION


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}