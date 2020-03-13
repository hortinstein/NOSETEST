FROM alpine

RUN apk add --no-cache -q -f git cmake make g++ gcov gcovr valgrind curl-dev python3-pip python3-dev libbsd-dev bash sh

RUN git config --global user.email "hortinstein@gmail.com"
RUN git config --global user.name "alex hortin"

# Download the latest version of monocypher
RUN wget https://monocypher.org/download/monocypher-3.0.0.tar.gz
RUN tar -xzf monocypher-3.0.0.tar.gz

RUN cd monocypher-3.0.0 && make 
RUN cd monocypher-3.0.0 && make install
RUN pip3 install --upgrade pip
RUN pip3 install pymonocypher

# Get the gtest suite
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest


RUN cp /monocypher-3.0.0/src/monocypher.h /workspace/NOSETEST/include/
RUN cp /monocypher-3.0.0/src/monocypher.c /workspace/NOSETEST/src/