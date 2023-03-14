#include "lib-drv-itest.hpp"
using namespace DriverIntTestSuite;

TEST(DriverTest, ParseTest) {
    vector<std::string> filenames = getFileNames();

    //GPIO Parse (Method) Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open device
    gpio.device_open(WRITEONLY, &gpio);

    string json_file_dir = "dev/default_GPIO_chipInfo.json";
    // Call the function to be tested
    gpio.parse(json_file_dir, &gpio);
    gpio.device_close();

    // Assert that the values are correctly parsed
    EXPECT_EQ("-", gpio.devContent.read(10, "name", &gpio));
    EXPECT_EQ("-", gpio.devContent.read(10, "consumer", &gpio));
    EXPECT_EQ("[INPUT]", gpio.devContent.read(10, "FLAG_IS_OUT", &gpio));
    EXPECT_EQ("[ACTIVE_HIGH]", gpio.devContent.read(10, "FLAG_ACTIVE_LOW", &gpio));
    EXPECT_EQ("[...]", gpio.devContent.read(10, "FLAG_OPEN_DRAIN", &gpio));
    EXPECT_EQ("[...]", gpio.devContent.read(10, "FLAG_OPEN_SOURCE", &gpio));
    EXPECT_EQ("[]", gpio.devContent.read(10, "FLAG_KERNEL", &gpio));

    EXPECT_EQ("-", gpio.devContent.read(20, "name", &gpio));
    EXPECT_EQ("-", gpio.devContent.read(20, "consumer", &gpio));
    EXPECT_EQ("[INPUT]", gpio.devContent.read(20, "FLAG_IS_OUT", &gpio));
    EXPECT_EQ("[ACTIVE_HIGH]", gpio.devContent.read(20, "FLAG_ACTIVE_LOW", &gpio));
    EXPECT_EQ("[...]", gpio.devContent.read(20, "FLAG_OPEN_DRAIN", &gpio));
    EXPECT_EQ("[...]", gpio.devContent.read(20, "FLAG_OPEN_SOURCE", &gpio));
    EXPECT_EQ("[]", gpio.devContent.read(20, "FLAG_KERNEL", &gpio));
    
    deleteGarbage(filenames);

    // SPI Parse (Method) Test
    SPI_Device spi(dev_name);

    // Open device
    spi.device_open(WRITEONLY, &spi);

    json_file_dir = "dev/default_SPI_chipInfo.json";
    // Call the function to be tested
    spi.parse(json_file_dir, &spi);
    spi.device_close();

    // Assert that the values are correctly parsed
    EXPECT_EQ("SPI_0", spi.devContent.read(0, "name", &spi));
    EXPECT_EQ("-", spi.devContent.read(0, "consumer", &spi));
    EXPECT_EQ("0", spi.devContent.read(0, "cpol", &spi));
    EXPECT_EQ("0", spi.devContent.read(0, "cpha", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "lsb_first", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "cs_high", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "3wire", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "loopback", &spi));


    EXPECT_EQ("SPI_1", spi.devContent.read(1, "name", &spi));
    EXPECT_EQ("-", spi.devContent.read(1, "consumer", &spi));
    EXPECT_EQ("0", spi.devContent.read(1, "cpol", &spi));
    EXPECT_EQ("0", spi.devContent.read(1, "cpha", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "lsb_first", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "cs_high", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "3wire", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "loopback", &spi));

    deleteGarbage(filenames);
}
