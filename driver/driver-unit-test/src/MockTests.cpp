#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(MockTest, LibDriverUtilityMockTest){
    LibUtilityMock lu;
    char expectedDevName[] = "test_device";
    EXPECT_CALL(lu, _generateDevName("test_device")).Times(1).WillOnce(Return(expectedDevName));
    EXPECT_EQ(lu.mockGenerateDevName("test_device"), expectedDevName);
}

TEST(MockTest, DeviceOpenMockTest){
    GPIODeviceMock devmock;
    GPIODeviceMock* gpioDevHandler = new GPIODeviceMock();
    
    EXPECT_CALL(devmock, device_open(DEFAULT, gpioDevHandler))
        .Times(1)
        .WillOnce(testing::InvokeWithoutArgs([&]() {     
            gpioDevHandler->fd.open("test_device", ios::out);
        }));
    devmock.device_open(DEFAULT, gpioDevHandler);
    EXPECT_EQ(1, gpioDevHandler->fd.is_open());
}


