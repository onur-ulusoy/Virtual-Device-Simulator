#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, OpenReadOnlyTest) {

    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) class open (method) test to open device with argument readonly
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);
    
    // Open the device for reading
    gpio.device_open(READONLY, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Child) class open (method) test to open device with argument readonly
    SPI_Device spi(dev_name);

    // Open the device for reading
    spi.device_open(READONLY, &spi);
    // Check that the device file stream is open
    ASSERT_TRUE(spi.getFd().is_open());
    deleteGarbage(filenames);
    
}