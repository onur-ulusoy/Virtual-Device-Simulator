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

    class GPIODevice{
    public:
        virtual ~GPIODevice() {}

        void mockGpioContructor(char* dev_name) {gpioConstructor(dev_name);}
        fstream& mockGetFd() { return getFd(); }
        fstream& mockGetLog() { return getLog(); }
        char* mockGetDevName(string path) { return getDevName();};


        //char* mockDeviceOpen(string path) {return deviceOpen(path);};

    private:
        virtual void gpioConstructor(char* dev_name){};
        virtual fstream& getFd() { fstream fd; return fd; }
        virtual fstream& getLog() { fstream log; return log; }
        virtual char* getDevName() { return(nullptr); }

        //virtual void deviceOpen(string path) {return(nullptr);}
    };

    class GPIODeviceMock : public GPIODevice{
    public:
        MOCK_METHOD(void, gpioConstructor, (char* dev_name), (override));
        MOCK_METHOD(fstream&, getFd, (), (override));
        MOCK_METHOD(fstream&, getLog, (), (override));
        MOCK_METHOD(char*, getDevName, (), (override));
    };  

    // Returns the files created in run time
    vector<std::string> getFileNames();
    void deleteGarbage(vector<string> filenames);
    bool areFilesEqual(const std::string& filePath1, const std::string& filePath2);
}

#endif // DRIVERTEST_LIB_UNITTEST_HPP