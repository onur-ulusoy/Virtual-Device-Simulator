#include <fstream>
#include <gtest/gtest.h>
#include "libdriver.hpp"
#include "libtesterutility.hpp"

using namespace DeviceSim;

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}