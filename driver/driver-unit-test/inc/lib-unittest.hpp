#ifndef DRIVERTEST_LIB_UNITTEST_HPP
#define DRIVERTEST_LIB_UNITTEST_HPP

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "libdriver.hpp"
#include <iostream>
#include <vector>
#include <cstdio>

using ::testing::Return;
using namespace DeviceSim;

namespace UnitTestSuite{
    
    class LibUtility{
    public:
        virtual ~LibUtility() {}
        char* mockGenerateDevName(string path) {return _generateDevName(path);};

    private:
        virtual char* _generateDevName(string path) {return(nullptr);}
    };

    class LibUtilityMock : public LibUtility{
    public:
        MOCK_METHOD(char*, _generateDevName, (string path), (override));
    }; 

    // Returns the files created in run time
    vector<std::string> getFileNames();
    void deleteGarbage(vector<string> filenames);
    bool areFilesEqual(const std::string& filePath1, const std::string& filePath2);
}

#endif // DRIVERTEST_LIB_UNITTEST_HPP