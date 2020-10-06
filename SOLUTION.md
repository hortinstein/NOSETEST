

- [student_NOSETEST/STOPANDCHAT/1_basic_testing.py](#student_NOSETESTSTOPANDCHAT1_basic_testing.py)
- [student_NOSETEST/STOPANDCHAT/2_automated_testing.py](#student_NOSETESTSTOPANDCHAT2_automated_testing.py)
- [student_NOSETEST/STOPANDCHAT/3_writing_first_test.py](#student_NOSETESTSTOPANDCHAT3_writing_first_test.py)
- [student_NOSETEST/STOPANDCHAT/4_understanding_testoutput.py](#student_NOSETESTSTOPANDCHAT4_understanding_testoutput.py)
- [student_NOSETEST/PRETTYGOOD/test/encryption_test.py](#student_NOSETESTPRETTYGOODtestencryption_test.py)
- [student_NOSETEST/PRETTYGOOD/data_utils.py](#student_NOSETESTPRETTYGOODdata_utils.py)
- [student_NOSETEST/src/tests.cpp](#student_NOSETESTsrctests.cpp)
- [student_NOSETEST/src/monocypher.c](#student_NOSETESTsrcmonocypher.c)
- [student_NOSETEST/src/SPITESTORE.c](#student_NOSETESTsrcSPITESTORE.c)
- [student_NOSETEST/src/ll.c](#student_NOSETESTsrcll.c)
- [student_NOSETEST/src/encryption.c](#student_NOSETESTsrcencryption.c)
- [student_NOSETEST/PRETTYGOOD/PRETTYGOOD.py](#student_NOSETESTPRETTYGOODPRETTYGOOD.py)
## student_NOSETEST/STOPANDCHAT/1_basic_testing.py
1  Warmup Exercise #1

## student_NOSETEST/STOPANDCHAT/2_automated_testing.py
1  Warmup Exercise #2

## student_NOSETEST/STOPANDCHAT/3_writing_first_test.py
1  Warmup Exercise #3

## student_NOSETEST/STOPANDCHAT/4_understanding_testoutput.py
1  Warmup Exercise #4

## student_NOSETEST/PRETTYGOOD/test/encryption_test.py
14  Take a look how the monocypher library is tested

15  This should help in understanding the crypto wrappers that I wrote

16  and how to test them

## student_NOSETEST/PRETTYGOOD/data_utils.py
105          write a test for the above encrypt and decrypt library

``` python
        #START CODE HERE
107         a_private_secret, a_public_secret = monocypher.generate_key_exchange_key_pair()
108         b_private_secret, b_public_secret = monocypher.generate_key_exchange_key_pair()
109         b_shared_secret = monocypher.key_exchange(b_private_secret, a_public_secret)
110         a_shared_secret = monocypher.key_exchange(a_private_secret, b_public_secret)
111         self.assertEqual(a_shared_secret, b_shared_secret)
112         dumb_message = bytes("this is my message",encoding='utf8')
113 
114         #CODE CAVING
115         self.assertEqual(dumb_message,
116                          decrypt_wrapper(a_shared_secret,
117                                          encrypyt_wrapper(b_shared_secret, a_public_secret,
118                                                           dumb_message))
119                         )
        #END CODE HERE
```
## student_NOSETEST/src/tests.cpp
31  So lets talk a little bit about these bad tests

32  why are they good?

33  - they are testing!

34  - pretty meh code coverage

35  - allows for refactoring key portions

36  why are they bad? 

37  - they are manually keeping track of things that we have library

38  - it would be hard to see if they introduced a new test to cover some edge case

39  - they are not great for regression testing after refactors

40  - feedback is very manual

41  - hard for people to add new tests in

42  - semi stateful, needs to be descrete

``` c
//START CODE HERE
44 // Why are tests like TEST(ll_lib, bad_tests) good...why are they bad?
//END CODE HERE
```
173  here is a warmup, for this we need to check and see that we are correctly 

174  checking for null variables 

175  go check where I added some code to make this pass, uncomment it and see a 

176  fail

177  here i will talk about why I like to write wrappers for code that I use

``` c
    //START CODE HERE
181     ll_insert_first(NULL, (void *)"asd");
    //END CODE HERE
```
189  so CURL is a big library I...like all the open stuff I use I 

190  write wrappers around their APIS 

191  I think it's super important to have maintainable code base 

192  with consistent APIs this allows: 

193  - structure to look largely the same

194  - standardization of error codes

195  - ease of refactoring

196  - common data structure usage

197  now lets try to implement some unit tests for the curl wrapper

198  there are a few examples below on some tests I would reccomend

199  but there are a ton of others that you should use when fleshing 

200  out this library

201  this is a simple get test, make sure you also startup PRETTYGOOD

202  python3 PRETTYGOOD.py

203  but it is also invalid

``` c
    //START CODE HERE
207     MemoryStruct ms;
208     wrapper_curl_init();
209     ASSERT_EQ(FAILURE,get_request(&ms,(char *)""));
210     wrapper_curl_free();
    //END CODE HERE
```
214  this is a simple get test, make sure you also startup PRETTYGOOD

215  python3 PRETTYGOOD.py

216  check the #defines in the C code for the URLS

217  if you want experiment on how you can make valgrind complain

``` c
    //START CODE HERE
221     MemoryStruct ms;
222   
223     wrapper_curl_init();
224     ASSERT_EQ(SUCCESS,get_request(&ms,(char *)PP_URL));
225     wrapper_curl_free();
    //END CODE HERE
```
229  now test post in a similar way

230  again experiment in the tests to see if you can irritate valgrind

231  also watch the code coverage

``` c
    //START CODE HERE
235     wrapper_curl_init();
236     
237     MemoryStruct ms;
238     uint8_t plain_text[12] = "Lorem ipsum"; /* Secret message */
239 
240     ms.memory = plain_text;
241     ms.size = sizeof(plain_text);
242     
243 
244     ASSERT_EQ(SUCCESS,post_request((char *)PP_URL, &ms));
245     wrapper_curl_free();
    //END CODE HERE
```
249  now write another failing test

``` c
    //START CODE HERE
253     wrapper_curl_init();
254 
255     MemoryStruct ms;
256     uint8_t plain_text[12] = "Lorem ipsum"; /* Secret message */
257 
258     ms.memory = plain_text;
259     ms.size = sizeof(plain_text);
260     ASSERT_EQ(FAILURE,post_request((char *)"", &ms));
261     wrapper_curl_free();
    //END CODE HERE
```
265  ENCRYPTION TIME!

266  Encryption is usually one of the first things I write

267  its hard to test and always deserces a clean API

268  sidebar me with encryption stories

269  I reccomend libsodium/NACL

270  alright simple test to see if the keys gen correctly

271  my test is bad in the solution...but how could it be improved?

``` c
    //START CODE HERE
276     LocalKeys me;
277     gen_keys(&me);
    //END CODE HERE
```
292  still building on it, make your session key!

``` c
    //START CODE HERE
297     LocalKeys me;
298     LocalKeys them;
299     KeyMat km;
300     exchange_helper(&km, &me, &them);
301     derive_session_key(&km);
    //END CODE HERE
```
305  moment of truth...test my wrapper!

306  big problem with the previous tests in these functions is that they 

307  did not do any real validation.  So it that case what were they good for?

``` c
    //START CODE HERE
312     LocalKeys me;
313     LocalKeys them;
314     KeyMat km;
315     KeyMat km2;
316     EncryptedBytes eb;
317     DecryptedBytes db;
318     DecryptedBytes db2;
319     eb_init(&eb);
320     db_init(&db);
321     db_init(&db2);
322     uint8_t plain_text[12] = "Lorem ipsum"; /* Secret message */
323 
324     db.plain_text = plain_text;
325     db.len = sizeof(plain_text);
326 
327     exchange_helper(&km, &me, &them);
328     exchange_helper(&km2, &them, &me);
329 
330     derive_session_key(&km);
331     derive_session_key(&km2);
332 
333     enc(&eb, &km, &db);
334     ASSERT_STRNE((const char *)eb.cypher_text, (const char *)db2.plain_text);
335     dec(&db2, &km2, &eb);
336 
337     ASSERT_EQ(eb.len, db.len) << "eb and dp have different lenths";
338     ASSERT_STREQ((const char *)plain_text, (const char *)db2.plain_text);
339 
340     eb_free(&eb);
341     db_free(&db2);
    //END CODE HERE
```
345  now test out the b64 library 

346  ASSERT_STREQ is an assert you can use to check

``` c
    //START CODE HERE
351     const char * array = "thistest";
352     MemoryStruct ms;
353     MemoryStruct ms2;
354     MemoryStruct ms3;
355     ms2.memory = NULL;
356     ms.memory = (uint8_t * ) array;
357     ms.size = sizeof("thistest");
358 
359     ms2.memory = base64_encode(ms.memory, ms.size,(size_t*)&ms2.size);
360     ms3.memory = base64_decode(ms2.memory, ms2.size,(size_t*)&ms.size);
361     //ASSERT_STREQ((const char *)ms.memory, (const char *)ms2.memory);
362     ASSERT_STREQ((const char *)ms.memory, (const char *)ms3.memory);
363     free(ms.memory);
364     free(ms2.memory);
365     free(ms3.memory);
    //END CODE HERE
```
369  now, get familiar with my list wrapper, 

370  test a push and pop and use ASSERT_STREQ to check

``` c
    //START CODE HERE
375     const char * array = "thistest";
376     SerializableList sl;
377     MemoryStruct ms;
378     MemoryStruct ms2;
379     ms2.memory = NULL;
380     ms.memory = (uint8_t * ) array;
381     ms.size = sizeof("thistest");
382 
383     ll_init(&sl);
384     ll_push(&sl,&ms);
385 
386     ll_pop(&ms2,&sl);
387     ASSERT_STREQ((const char *)ms.memory, (const char *)ms2.memory);
388    
389     free(ms2.memory);
390     ll_free(&sl);
    //END CODE HERE
```
394  alright now test serialization and deserialzation 

395  does the solution code even test serialization and deserialzation?  

``` c
    //START CODE HERE
401     const char * array = "thistest";
402     SerializableList sl;
403     SerializableList sl2;
404     SerializableList sl3;
405     SerializableList sl4;
406     
407     MemoryStruct ms;
408     MemoryStruct ms2;
409     MemoryStruct ms3;
410     MemoryStruct ms4;
411     
412     ms2.memory = NULL;
413     ms3.memory = NULL;
414     ms4.memory = NULL;
415     
416     ms.memory = (uint8_t * ) array;
417     ms.size = sizeof("thistest");
418 
419     ll_init(&sl);
420     ll_push(&sl,&ms);
421     ll_push(&sl,&ms);
422     ll_push(&sl,&ms);
423     ll_push(&sl,&ms);
424     ll_push(&sl,&ms);
425 
426     
427 
428     ll_serialize(&ms2,&sl);
429     ll_deserialize(&sl2,&ms2);
430     DEBUG_PRINT("sl2 size: %d",sl2.list->len);
431     ll_serialize(&ms3,&sl2);
432     ll_deserialize(&sl3,&ms3);
433     DEBUG_PRINT("sl3 size: %d",sl2.list->len);
434     ll_serialize(&ms4,&sl3);
435     ll_deserialize(&sl4,&ms4);
436     DEBUG_PRINT("sl4 size: %d",sl2.list->len);
437     
438     free(ms2.memory);
439     free(ms3.memory);
440     free(ms4.memory);
441 
442     ll_free(&sl);
443     ll_free(&sl2);
444     ll_free(&sl3);
445     ll_free(&sl4);
    //END CODE HERE
```
## student_NOSETEST/src/monocypher.c
1  What do you like about this code

2  what do you hate about it

## student_NOSETEST/src/SPITESTORE.c
1  try running it with out the task loop

35      take this out and see what happens

55      copy their keymat into the km structure

56      what should i be checking for here

150      this is embarrassing dont judge me

## student_NOSETEST/src/ll.c
245      take this out per instructions

317      add this here to see valgrind does 

318      int * lol = NULL;

319      lol = (int*) malloc(100);

320      lol[1] = '0';

## student_NOSETEST/src/encryption.c
294 	 if we take this out...what happens

295 	 how could you write an additional test for that?

## student_NOSETEST/PRETTYGOOD/PRETTYGOOD.py
170          just use the solution on this one, but lets talk better ways

171          i got really lazy...

``` python
        #START CODE HERE
173         print("starting echo test")
174         my_echo_string = "testing my echo string" 
175         q_task.put(TaskEcho(my_echo_string))
176         print("awaiting echo test")
177         task_resp = q_resp.get()
178         q_resp.task_done()
179         num, res = task_resp.return_res()
180         self.assertEqual(ECHO,num)
181         self.assertEqual(my_echo_string,res)
        #END CODE HERE
```
