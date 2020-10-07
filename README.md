[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/hortinstein/NOSETEST) 

# NOSETEST

```
docker build -t nosetest .
sudo docker run -it --mount src=$(pwd),target=/workspace,type=bind nosetest /bin/ash
```

##### create student files

```
#get one level back on this directory
cd .. 
make --directory NOSETEST clean && rm -rf student_NOSETEST && python3 NOSETEST/SWANKILLER.py NOSETEST && cp student_NOSETEST/SOLUTION.md NOSETEST/
```

##### Disclaimer
- some of this code is intentionally bad, and this is not up to any standards anywhere.  there are a few reasons for that:
  - I am lazy and this is academic...so it suits me to capatilize on laziness if i can derive learning value from showing you why its wrong (so i dont have to refactor)
  - I intentionally coded some things terribly (we will discuss)
  - It's C...find my errors, I will be honest and tell you what category
  - most of this course was written after work...or amidst covid quarentine, wine might have been involved
  - software is hard to write...and we are inherently bad at doing it, this course is to show you how to minimize your risk with some things I have found benefitted me over the years.  

