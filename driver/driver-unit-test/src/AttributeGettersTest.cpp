#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, AttributeGettersTest){
    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) Class Attribute Getters Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    EXPECT_EQ("dev/default_GPIO_chipInfo.json", gpio.getDefaultDir());
    EXPECT_EQ("offset", gpio.getPack()[0]);
    EXPECT_EQ("FLAG_IS_OUT", gpio.getPack()[3]);
    EXPECT_EQ(8, gpio.getPackSize());
    deleteGarbage(filenames);

    // SPI (Child) Class Attribute Getters Test
    SPI_Device spi(dev_name);

    EXPECT_EQ("dev/default_SPI_chipInfo.json", spi.getDefaultDir());
    EXPECT_EQ("offset", spi.getPack()[0]);
    EXPECT_EQ("loopback", spi.getPack()[8]);
    EXPECT_EQ(9, spi.getPackSize());
    deleteGarbage(filenames);
}