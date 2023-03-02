#include "lib-unittest.hpp"
//using namespace UnitTestSuite;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}