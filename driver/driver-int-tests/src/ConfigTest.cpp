#include "lib-drv-itest.hpp"
using namespace DriverIntTestSuite;

TEST(DriverTest, ConfigTest) {
    vector<std::string> filenames = getFileNames();
    
    // GPIO (Parent) Class Config (Method) Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    gpio.devContent.config(DEFAULT, &gpio);

    // Assert that the values are correctly configurated
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

    // SPI (Child) Class Config (Method) Test
    SPI_Device spi(dev_name);

    spi.devContent.config(DEFAULT, &spi);

    // Assert that the values are correctly configurated
    EXPECT_EQ("SPI_1", spi.devContent.read(1, "name", &spi));
    EXPECT_EQ("-", spi.devContent.read(1, "consumer", &spi));
    EXPECT_EQ("0", spi.devContent.read(1, "cpol", &spi));
    EXPECT_EQ("0", spi.devContent.read(1, "cpha", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "lsb_first", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "cs_high", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "3wire", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "loopback", &spi));

    EXPECT_EQ("SPI_2", spi.devContent.read(2, "name", &spi));
    EXPECT_EQ("-", spi.devContent.read(2, "consumer", &spi));
    EXPECT_EQ("0", spi.devContent.read(2, "cpol", &spi));
    EXPECT_EQ("0", spi.devContent.read(2, "cpha", &spi));
    EXPECT_EQ("false", spi.devContent.read(2, "lsb_first", &spi));
    EXPECT_EQ("false", spi.devContent.read(2, "cs_high", &spi));
    EXPECT_EQ("false", spi.devContent.read(2, "3wire", &spi));
    EXPECT_EQ("false", spi.devContent.read(2, "loopback", &spi));
    
    deleteGarbage(filenames);
    
}
