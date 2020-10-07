# Steps 
- write code
- lint code 
- (Unit/integration/System/Benchmark/) test code locally
- Push to a repo
- static analysis (security?) 
- build server kicks off
- tests run against variety of environments/versions/platforms ie Travis
- code review before merging to master (LOL)

# Ideas to add to NOSETEST
- Test out the list serialization and deserialization libraries
- Utilize the Serializable linked list and write a parser for it in Python and test it...hint it might be broken
- Refactor the TLV type structs to variable length structs where the variable length data is stored at the end
- Change encryption wrappers to work with above
- Add return codes to the response values pased back 
- Add in error code checking for all the places it's not checked (alot)
- Implement new tasks
- Write a cleaner implementaion of how python correlates the task and the response...maybe task ids to correlate responses with outstanding tasks?
- Rewrite how the keying works...lets discuss
- Create some malformed data on either the python system tests or the C unit tests to find and fix the errors
- 
# Class Outline
- Environment Setup
- Introduction
  - What is software testing
  - What are the high level different types of testing
  - Why should I care here
  - What does a good testing ecosystem look like
- Basic Python Testing
- NOSETEST
  - Environment Setup
  - Scenario Introduction
  - Enc Wrapper Test exercise (python)
  - Reproducible Builds
  - Dependency Management
  - Code Coverage intro
  - Valgrind intro
  - Linters?
  - Static code analysis
  - Makefile instrumentation
  - GUNIT 
  - LL bad testing example
  - How do we test it better exercise
  - Hard to reach and hard to test functions
  - Monocypher test example (c)
  - Curl Tests (c)
  - Serializable list tests (c)
  - Encryptionwrapper tests (c) 
  - Base64 tests (c)
  - Integration/System testing 
  - test runner overview
  - discussions on alternate approaches
  - 

##### high level objs
- talk about how testing builds confidence
- instrumenting a build environment that is cohesive to testing
- unit
- integration
- code coverage
- end to end (system)
- acceptance
- opsec
- platform (maybe)
- regression (maybe)
- stylistic - writing testable code keep things on the stack as much as you can
- standardize arg order
##### general 
- poor tests (LL example)
- writing testable functions
- CURL - does not fail without initing or freeing, but might cross platform?  This is specific to winsock...i think
- cover hard to reach coverage tests: https://stackoverflow.com/questions/2592107/how-do-i-mock-memory-allocation-failures
- cleanup errors, how do i structure clean code where I need to clean up the same things
- debug print 
- debug error
- binary data substitutes
- memory structs, and obj or data type wrappers for testing
- compiler flags
#### curl
- what happens if the data is not NULL delimited

#### encryption
- ```python3 -m unittest PRETTYGOOD/test/encryption_test.py```

#### building exercise into a bootable OS
- https://iximiuz.com/en/posts/from-docker-container-to-bootable-linux-disk-image/
- https://github.com/iximiuz/docker-to-linux