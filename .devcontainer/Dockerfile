FROM alpine


RUN apk add --no-cache -q -f git cmake make g++ gcov gcovr valgrind curl-dev

RUN git config --global user.email "hortinstein@gmail.com"
RUN git config --global user.name "alex hortin"


# Get the gtest suite
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

# Get fuzzer-test-suite
RUN git clone https://github.com/google/fuzzing.git fuzzing

RUN git clone https://github.com/google/fuzzer-test-suite.git FTS

#RUN ./fuzzing/tutorial/libFuzzer/install-deps.sh  # Get deps
#RUN ./fuzzing/tutorial/libFuzzer/install-clang.sh # Get fresh clang binaries