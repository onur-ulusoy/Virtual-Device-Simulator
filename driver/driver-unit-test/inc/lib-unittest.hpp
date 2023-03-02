#ifndef DRIVERTEST_LIB_UNITTEST_HPP
#define DRIVERTEST_LIB_UNITTEST_HPP

#include "gtest/gtest.h"
#include "libdriver.hpp"
#include <iostream>
#include <vector>
#include <cstdio>

using namespace DeviceSim;

namespace UnitTestSuite{
    // Returns the files created in run time
    vector<std::string> getFileNames();
    int* initCounter();
    void deleteGarbage(vector<string> filenames);
    bool areFilesEqual(const std::string& filePath1, const std::string& filePath2);
}

#endif // DRIVERTEST_LIB_UNITTEST_HPP