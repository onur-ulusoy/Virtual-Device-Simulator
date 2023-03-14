#include "lib-drv-itest.hpp"
using namespace DriverIntTestSuite;

TEST(DriverTest, CloseTest) {
    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) Class Close (Method) Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(DEFAULT, &gpio);
    gpio.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Child) Class Close (Method) Test
    SPI_Device spi(dev_name);

    // Open the device for reading
    spi.device_open(DEFAULT, &spi);
    spi.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(spi.getFd().is_open());
    deleteGarbage(filenames);
}
