FROM alpine

RUN apk add --no-cache -q -f git cmake make g++ gcov gcovr valgrind curl-dev python3-pip python3-dev libbsd-dev bash sh

RUN git config --global user.email "hortinstein@gmail.com"
RUN git config --global user.name "alex hortin"

# Download the latest version of monocypher
RUN wget https://monocypher.org/download/monocypher-3.0.0.tar.gz
RUN tar -xzf monocypher-3.0.0.tar.gz

RUN cd monocypher-3.0.0 && make 
RUN cd monocypher-3.0.0 && make install
RUN pip install --upgrade pip
RUN pip install pymonocypher

RUN git clone https://github.com/liteserver/binn
RUN cd binn
RUN make
RUN sudo make install

RUN git clone https://github.com/meeron/pybinn.git && cd pybinn && pip install .

# Get the gtest suite
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

