#include <gtest/gtest.h> // googletest header file

#include <string>
using std::string;


extern "C" {
    #include "ll.h"
}

TEST(agent_lib, echo) {
  
}
TEST(agent_lib, echo_wrong) {
  
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}