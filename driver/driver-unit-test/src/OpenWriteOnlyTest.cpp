#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, OpenWriteOnlyTest) {
    vector<std::string> filenames = getFileNames();

    LibUtilityMock lu;
    char expectedDevName[] = "test_device";
    EXPECT_CALL(lu, _generateDevName("test_device")).Times(1).WillOnce(Return(expectedDevName));

    // GPIO (Parent) class open (method) test to open device with argument writeonly
    char* dev_name = lu.mockGenerateDevName("test_device");
    DeviceSim::GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(WRITEONLY, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Child) class open (method) test to open device with argument writeonly
    SPI_Device spi(dev_name);

    // Open the device for writing
    spi.device_open(WRITEONLY, &spi);
    // Check that the device file stream is open
    ASSERT_TRUE(spi.getFd().is_open());
    deleteGarbage(filenames);
}