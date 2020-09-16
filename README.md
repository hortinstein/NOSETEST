[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/hortinstein/NOSETEST) 

# NOSETEST

```
docker build -t nosetest .
sudo docker run -it --mount src=$(pwd),target=/workspace,type=bind nosetest /bin/ash



cp /monocypher-3.0.0/src/monocypher.h /workspaces/NOSETEST/include/
cp /monocypher-3.0.0/src/monocypher.c /workspaces/NOSETEST/src/

```

Course Notes:

##### Disclaimer
- some of this code is intentionally bad, and this is not up to any standards anywhere.  there are a few reasons for that:
-- I am lazy and this is academic...so it suits me to capatilize on laziness if i can derive learning value from showing you why its wrong (so i dont have to refactor)
-- I intentionally coded some things terribly (we will discuss)
-- It's C...find my errors, I will be honest and tell you what category
-- most of this course was written after work...or amidst covid quarentine, wine might have been involved
-- software is hard to write...and we are inherently bad at doing it, this course is to show you how to minimize your risk with some things I have found benefitted me over the years.  

##### high level objs
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
#### curl
- what happens if the data is not NULL delimited



#### encryption
- 

#### building exercise into a bootable OS
- https://iximiuz.com/en/posts/from-docker-container-to-bootable-linux-disk-image/
- https://github.com/iximiuz/docker-to-linux