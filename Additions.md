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
  - Reproducible Builds
  - Dependency Management
  - Code Coverage
  - Valgrind
  - GUNIT 
  