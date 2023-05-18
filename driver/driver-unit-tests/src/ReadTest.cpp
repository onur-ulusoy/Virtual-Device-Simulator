#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, ReadTest) {
    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) Class Read (Method) Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    //Check if the read function can return successfully
    gpio.devContent.read(10, "name", &gpio);
    deleteGarbage(filenames);

    // SPI (Child) Class Read (Method) Test
    SPI_Device spi(dev_name);

    //Check if the read function can return successfully
    spi.devContent.read(1, "name", &spi);
    deleteGarbage(filenames);
    
}