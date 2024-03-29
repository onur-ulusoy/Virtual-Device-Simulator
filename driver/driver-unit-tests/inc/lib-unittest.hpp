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

    class GPIO_Device{
    public:
        virtual ~GPIO_Device() {}
        fstream fd;

        void mockGpioContructor(char* dev_name) { gpioConstructor(dev_name); }
        fstream& mockGetFd() { return getFd(); }
        fstream& mockGetLog() { return getLog(); }
        char* mockGetDevName(string path) { return getDevName(); }
        string mockGetDefaultDir() { return getDefaultDir(); }
        string* mockGetPack() { return getPack(); }
        int mockGetPackSize() { return getPackSize(); }
        void mockDeviceOpen(command request, GPIO_Device *gpioDevHandler) { device_open(request, gpioDevHandler); }
        void mockDeviceClose() { device_close(); }
        void mockParse(string dir, GPIO_Device *gpioDevHandler) { parse(dir, gpioDevHandler); }


    private:
        virtual void gpioConstructor(char* dev_name){};
        virtual fstream& getFd() { fstream* fd = new fstream(); return *fd; }
        virtual fstream& getLog() { fstream* log = new fstream(); return *log; }
        virtual char* getDevName() { return(nullptr); }
        virtual string getDefaultDir() { return nullptr; }
        virtual string* getPack() { return nullptr; }
        virtual int getPackSize() { return 0; }
        virtual void device_open(command request, GPIO_Device *gpioDevHandler){}
        virtual void device_close() {}
        virtual void parse(string dir, GPIO_Device *gpioDevHandler) {}

    };

    class GPIODeviceMock : public GPIO_Device{
    public:
        MOCK_METHOD(void, gpioConstructor, (char* dev_name), (override));
        MOCK_METHOD(fstream&, getFd, (), (override));
        MOCK_METHOD(fstream&, getLog, (), (override));
        MOCK_METHOD(char*, getDevName, (), (override));
        MOCK_METHOD(string, getDefaultDir, (), (override));
        MOCK_METHOD(string*, getPack, (), (override));
        MOCK_METHOD(int, getPackSize, (), (override));
        MOCK_METHOD(void, device_open, (command request, GPIO_Device *gpioDevHandler), (override));
        MOCK_METHOD(void, device_close, (), (override));
        MOCK_METHOD(void, parse, (string dir, GPIO_Device *gpioDevHandler), (override));

    };  

    class GPIO_DevContent{
    public:
        virtual ~GPIO_DevContent() {}

        void mockConfig(command request, GPIO_Device *gpioDevHandler) { config(request, gpioDevHandler); }
        string mockRead(int offset, string property, GPIO_Device *gpioDevHandler) { return read(offset, property, gpioDevHandler); }
        string mockWrite(int offset, string property, string new_value, GPIO_Device *gpioDevHandler) { return write(offset, property, new_value, gpioDevHandler); }
        string mockShow(GPIO_Device *gpioDevHandler) {return show(gpioDevHandler); }
        

    private:
        virtual void config(command request, GPIO_Device *gpioDevHandler) {}
        virtual string read(int offset, string property, GPIO_Device *gpioDevHandler) { return nullptr; }
        virtual string write(int offset, string property, string new_value, GPIO_Device *gpioDevHandler) { return nullptr; }
        virtual string show(GPIO_Device *gpioDevHandler) { return nullptr; }
    };

    class GPIODevContentMock : public GPIO_DevContent{
    public:
        MOCK_METHOD(void, config, (command request, GPIO_Device *gpioDevHandler), (override));
        MOCK_METHOD(string, read, (int offset, string property, GPIO_Device *gpioDevHandler), (override));
        MOCK_METHOD(string, write, (int offset, string property, string new_value, GPIO_Device *gpioDevHandler), (override));
        MOCK_METHOD(string, show, (GPIO_Device *gpioDevHandler), (override));

    };  


    // Returns the files created in run time
    vector<std::string> getFileNames();
    void deleteGarbage(vector<string> filenames);
    bool areFilesEqual(const std::string& filePath1, const std::string& filePath2);
}

#endif // DRIVERTEST_LIB_UNITTEST_HPP