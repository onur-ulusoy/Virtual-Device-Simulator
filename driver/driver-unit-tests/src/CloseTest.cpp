#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, CloseTest) {
    vector<std::string> filenames = getFileNames();
   
    LibUtilityMock lu;
    char expectedDevName[] = "test_device";
    EXPECT_CALL(lu, _generateDevName("test_device")).Times(1).WillOnce(Return(expectedDevName));

    GPIODeviceMock devmock;
    GPIODeviceMock* gpioDevHandler = new GPIODeviceMock();
    
    // GPIO (Parent) Class Close (Method) Test
    char* dev_name = lu.mockGenerateDevName("test_device");
    DeviceSim::GPIO_Device gpio(dev_name);

    EXPECT_CALL(devmock, device_open(DEFAULT, gpioDevHandler))
            .Times(1)
            .WillOnce(testing::InvokeWithoutArgs([&]() {     
                gpioDevHandler->fd.open("test_device", ios::out);
                gpio.setFd(&gpioDevHandler->fd);

                }));

    // Open the device in def mode
    devmock.device_open(DEFAULT, gpioDevHandler);

    //cout << gpio.getFd().is_open() << "**" << endl;

    gpio.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Child) Class Close (Method) Test
    SPI_Device spi(dev_name);
    GPIODeviceMock* spiDevHandler = new GPIODeviceMock();

    EXPECT_CALL(devmock, device_open(DEFAULT, spiDevHandler))
            .Times(1)
            .WillOnce(testing::InvokeWithoutArgs([&]() {     
                spiDevHandler->fd.open("test_device", ios::out);
                spi.setFd(&spiDevHandler->fd);
                }));

    // Open the device in def mode
    devmock.device_open(DEFAULT, spiDevHandler);

    //cout << spi.getFd().is_open() << "**" << endl;

    spi.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(spi.getFd().is_open());
    deleteGarbage(filenames);
}
