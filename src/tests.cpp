#include <gtest/gtest.h> // googletest header file

#include <string>
using std::string;

extern "C"
{
    #include "ll.h"
    #include "curlwrapper.h"
    #include "config.h"
    #include "encryption.h"
    #include "listwrapper.h"
    #include "binn.h"
}

void num_teardownss(void *n)
{
    *(int *)n *= -1; // just so we can visually inspect removals afterwards
}

void num_printerss(void *n)
{
    printf(" %d", *(int *)n);
}

int num_equals_3ss(void *n)
{
    return *(int *)n == 3;
}

//!COMMENT So lets talk a little bit about these bad tests
//!COMMENT why are they good?
//!COMMENT - they are testing!
//!COMMENT - pretty meh code coverage
//!COMMENT - allows for refactoring key portions
//!COMMENT why are they bad? 
//!COMMENT - they are manually keeping track of things that we have library
//!COMMENT - it would be hard to see if they introduced a new test to cover some edge case
//!COMMENT - they are not great for regression testing after refactors
//!COMMENT - feedback is very manual
//!COMMENT - hard for people to add new tests in
//!COMMENT - semi stateful, needs to be descrete
//!CUT_START
// Why are tests like TEST(ll_lib, bad_tests) good...why are they bad?
//!CUT_END

//This is an example of semi working, but absolute garbage testing
TEST(ll_lib, bad_tests)
{
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
    if (!(*_n == c))
    {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, c, *_n);
        fail_count++;
    }
    else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (list->len != 1)
    {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 1, list->len);
        fail_count++;
    }
    else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_first(list, &b); // 1 in front
    ll_insert_first(list, &a); // 0 in front -> 0, 1, 2

    _n = (int *)ll_get_first(list);
    if (!(*_n == a))
    {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, a, *_n);
        fail_count++;
    }
    else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (!(list->len == 3))
    {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 3, list->len);
        fail_count++;
    }
    else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_last(list, &d); // 3 in back
    ll_insert_last(list, &e); // 4 in back
    ll_insert_last(list, &f); // 5 in back

    _n = (int *)ll_get_n(list, 5);
    if (!(*_n == f))
    {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, f, *_n);
        fail_count++;
    }
    else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (!(list->len == 6))
    {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 6, list->len);
        fail_count++;
    }
    else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_n(list, &g, 6); // 6 at index 6 -> 0, 1, 2, 3, 4, 5, 6

    int _i;
    for (_i = 0; _i < list->len; _i++)
    { // O(n^2) test lol
        _n = (int *)ll_get_n(list, _i);
        if (!(*_n == _i))
        {
            fail_count++;
            fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", 1, _i, *_n);
        }
        else
            fprintf(stderr, "PASS Test %d!\n", test_count);
        test_count++;
    }

    // (ll: 0 1 2 3 4 5 6), length: 7

    ll_remove_first(list);                  // (ll: 1 2 3 4 5 6), length: 6
    ll_remove_n(list, 1);                   // (ll: 1 3 4 5 6),   length: 5
    ll_remove_n(list, 2);                   // (ll: 1 3 5 6),     length: 4
    ll_remove_n(list, 5);                   // (ll: 1 3 5 6),     length: 4; does nothing
    ll_remove_search(list, num_equals_3ss); // (ll: 1 5 6),       length: 3
    ll_insert_first(list, &h);              // (ll: 3 1 5 6),     length: 5
    ll_insert_last(list, &i);               // (ll: 3 1 5 6 3),   length: 5
    ll_remove_search(list, num_equals_3ss); // (ll: 1 5 6 3),     length: 4
    ll_remove_search(list, num_equals_3ss); // (ll: 1 5 6),       length: 3

    ll_print(*list);

    ll_delete(list);

    if (fail_count)
    {
        fprintf(stderr, "FAILED %d tests of %d.\n", fail_count, test_count);
        return;
    }

    fprintf(stderr, "PASSED all %d tests!\n", test_count);
}

TEST(datatypes, init)
{
    //!CUT_START
    ll_insert_first(NULL, (void *)"asd");
    //!CUT_END
}

// //!COMMENT here is a warmup, for this we need to check and see that we are correctly 
// //!COMMENT checking for null variables 
// //!COMMENT go check where I added some code to make this pass, uncomment it and see a 
// //!COMMENT fail
// //!COMMENT here i will talk about why I like to write wrappers for code that I use
// TEST(ll_lib, segfault)
// {
//     //!CUT_START
//     ll_insert_first(NULL, (void *)"asd");
//     //!CUT_END
// }

// ////////////////////////////////
// ////////// CURL TESTS //////////
// ////////////////////////////////

// //!COMMENT so CURL is a big library I...like all the open stuff I use I 
// //!COMMENT write wrappers around their APIS 
// //!COMMENT I think it's super important to have maintainable code base 
// //!COMMENT with consistent APIs this allows: 
// //!COMMENT - structure to look largely the same
// //!COMMENT - standardization of error codes
// //!COMMENT - ease of refactoring
// //!COMMENT - common data structure usage
// //!COMMENT now lets try to implement some unit tests for the curl wrapper
// //!COMMENT there are a few examples below on some tests I would reccomend
// //!COMMENT but there are a ton of others that you should use when fleshing 
// //!COMMENT out this library
// //!COMMENT this is a simple get test, make sure you also startup PRETTYGOOD
// //!COMMENT python3 PRETTYGOOD.py
// //!COMMENT but it is also invalid
// TEST(curl_lib, get_invalid)
// {
//     //!CUT_START
//     MemoryStruct ms;
//     wrapper_curl_init();
//     ASSERT_EQ(FAILURE,get_request(&ms,(char *)""));
//     wrapper_curl_free();
//     //!CUT_END
// }

// //!COMMENT this is a simple get test, make sure you also startup PRETTYGOOD
// //!COMMENT python3 PRETTYGOOD.py
// //!COMMENT check the #defines in the C code for the URLS
// //!COMMENT if you want experiment on how you can make valgrind complain
// TEST(curl_lib, get)
// {
//     //!CUT_START
//     MemoryStruct ms;
  
//     wrapper_curl_init();
//     ASSERT_EQ(SUCCESS,get_request(&ms,(char *)PP_URL));
//     wrapper_curl_free();
//     //!CUT_END
// }

// //!COMMENT now test post in a similar way
// //!COMMENT again experiment in the tests to see if you can irritate valgrind
// //!COMMENT also watch the code coverage
// TEST(curl_lib, post)
// {
//     //!CUT_START
//     wrapper_curl_init();
    
//     MemoryStruct ms;
//     uint8_t plain_text[12] = "Lorem ipsum"; /* Secret message */

//     ms.memory = plain_text;
//     ms.size = sizeof(plain_text);
    

//     ASSERT_EQ(SUCCESS,post_request((char *)PP_URL, &ms));
//     wrapper_curl_free();
//     //!CUT_END
// }

// //!COMMENT now write another failing test
// TEST(curl_lib, post_invalid)
// {
//     //!CUT_START
//     wrapper_curl_init();

//     MemoryStruct ms;
//     uint8_t plain_text[12] = "Lorem ipsum"; /* Secret message */

//     ms.memory = plain_text;
//     ms.size = sizeof(plain_text);
//     ASSERT_EQ(FAILURE,post_request((char *)"", &ms));
//     wrapper_curl_free();
//     //!CUT_END
// }

// //!COMMENT ENCRYPTION TIME!
// //!COMMENT Encryption is usually one of the first things I write
// //!COMMENT its hard to test and always deserces a clean API
// //!COMMENT sidebar me with encryption stories
// //!COMMENT I reccomend libsodium/NACL
// //!COMMENT alright simple test to see if the keys gen correctly
// //!COMMENT my test is bad in the solution...but how could it be improved?
// //checks to see if the key generates correctly
// TEST(enc_lib, gen_key)
// {
//     //!CUT_START
//     LocalKeys me;
//     gen_keys(&me);
//     //!CUT_END
// }

// void exchange_helper(KeyMat *km, LocalKeys *me, LocalKeys *them)
// {
//     gen_keys(me);
//     gen_keys(them);

//     //copy into the structure
//     memcpy(km->sender_pub_key, them->my_pub_key, KEY_LEN);
//     memcpy(km->my_priv_key, me->my_priv_key, KEY_LEN);
// }


// //!COMMENT still building on it, make your session key!
// //checks to see if the key generates correctly
// TEST(enc_lib, gen_session_key)
// {
//     //!CUT_START
//     LocalKeys me;
//     LocalKeys them;
//     KeyMat km;
//     exchange_helper(&km, &me, &them);
//     derive_session_key(&km);
//     //!CUT_END
// }

// //!COMMENT moment of truth...test my wrapper!
// //!COMMENT big problem with the previous tests in these functions is that they 
// //!COMMENT did not do any real validation.  So it that case what were they good for?
// //checks to see if the key generates correctly
// TEST(enc_lib, enc_dec)
// {
//     //!CUT_START
//     LocalKeys me;
//     LocalKeys them;
//     KeyMat km;
//     KeyMat km2;
//     EncryptedBytes eb;
//     DecryptedBytes db;
//     DecryptedBytes db2;
//     eb_init(&eb);
//     db_init(&db);
//     db_init(&db2);
//     uint8_t plain_text[12] = "Lorem ipsum"; /* Secret message */

//     db.plain_text = plain_text;
//     db.len = sizeof(plain_text);

//     exchange_helper(&km, &me, &them);
//     exchange_helper(&km2, &them, &me);

//     derive_session_key(&km);
//     derive_session_key(&km2);

//     enc(&eb, &km, &db);
//     ASSERT_STRNE((const char *)eb.cypher_text, (const char *)db2.plain_text);
//     dec(&db2, &km2, &eb);

//     ASSERT_EQ(eb.len, db.len) << "eb and dp have different lenths";
//     ASSERT_STREQ((const char *)plain_text, (const char *)db2.plain_text);

//     eb_free(&eb);
//     db_free(&db2);
//     //!CUT_END
// }

// //!COMMENT now test out the b64 library 
// //!COMMENT ASSERT_STREQ is an assert you can use to check
// //checks to see if the base64 works correctly
// TEST(encryption, base64encodedecode)
// {
//     //!CUT_START
//     const char * array = "thistest";
//     MemoryStruct ms;
//     MemoryStruct ms2;
//     MemoryStruct ms3;
//     ms2.memory = NULL;
//     ms.memory = (uint8_t * ) array;
//     ms.size = sizeof("thistest");

//     ms2.memory = base64_encode(ms.memory, ms.size,(size_t*)&ms2.size);
//     ms3.memory = base64_decode(ms2.memory, ms2.size,(size_t*)&ms.size);
//     //ASSERT_STREQ((const char *)ms.memory, (const char *)ms2.memory);
//     ASSERT_STREQ((const char *)ms.memory, (const char *)ms3.memory);
//     free(ms.memory);
//     free(ms2.memory);
//     free(ms3.memory);
//     //!CUT_END
// }

// //!COMMENT now, get familiar with my list wrapper, 
// //!COMMENT test a push and pop and use ASSERT_STREQ to check
// //checks to see if the list wrapper works correctly generates correctly
// TEST(ll_wrap, push_pop)
// {
//     //!CUT_START
//     const char * array = "thistest";
//     SerializableList sl;
//     MemoryStruct ms;
//     MemoryStruct ms2;
//     ms2.memory = NULL;
//     ms.memory = (uint8_t * ) array;
//     ms.size = sizeof("thistest");

//     ll_init(&sl);
//     ll_push(&sl,&ms);

//     ll_pop(&ms2,&sl);
//     ASSERT_STREQ((const char *)ms.memory, (const char *)ms2.memory);
   
//     free(ms2.memory);
//     ll_free(&sl);
//     //!CUT_END
// }

// //!COMMENT alright now test serialization and deserialzation 
// //!COMMENT does the solution code even test serialization and deserialzation?  
// //checks to see if the list wrapper works correctly generates correctly?
// //also using a wrapper lets you combine more than one library function into a clean API
// TEST(ll_wrap, serialize_deserialize)
// {
//     //!CUT_START
//     const char * array = "thistest";
//     SerializableList sl;
//     SerializableList sl2;
//     SerializableList sl3;
//     SerializableList sl4;
    
//     MemoryStruct ms;
//     MemoryStruct ms2;
//     MemoryStruct ms3;
//     MemoryStruct ms4;
    
//     ms2.memory = NULL;
//     ms3.memory = NULL;
//     ms4.memory = NULL;
    
//     ms.memory = (uint8_t * ) array;
//     ms.size = sizeof("thistest");

//     ll_init(&sl);
//     ll_push(&sl,&ms);
//     ll_push(&sl,&ms);
//     ll_push(&sl,&ms);
//     ll_push(&sl,&ms);
//     ll_push(&sl,&ms);

    

//     ll_serialize(&ms2,&sl);
//     ll_deserialize(&sl2,&ms2);
//     DEBUG_PRINT("sl2 size: %d",sl2.list->len);
//     ll_serialize(&ms3,&sl2);
//     ll_deserialize(&sl3,&ms3);
//     DEBUG_PRINT("sl3 size: %d",sl2.list->len);
//     ll_serialize(&ms4,&sl3);
//     ll_deserialize(&sl4,&ms4);
//     DEBUG_PRINT("sl4 size: %d",sl2.list->len);
    
//     free(ms2.memory);
//     free(ms3.memory);
//     free(ms4.memory);

//     ll_free(&sl);
//     ll_free(&sl2);
//     ll_free(&sl3);
//     ll_free(&sl4);
//     //!CUT_END
// }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}