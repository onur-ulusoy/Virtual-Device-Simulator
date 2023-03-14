#include "lib-drv-itest.hpp"
using namespace DriverIntTestSuite;

TEST(DriverTest, ConstructorTest) {
    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) Class Constructor Test
    // Test constructor by creating a new instance of the class
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Check that the instance was created successfully
    ASSERT_STREQ(gpio.getDevName(), dev_name);

    ifstream logFile("log");
    EXPECT_TRUE(logFile.good());

    deleteGarbage(filenames);

    // SPI (Child) Class Constructor Test
    SPI_Device spi(dev_name);

    // Check that the instance was created successfully
    ASSERT_STREQ(spi.getDevName(), dev_name);
    EXPECT_EQ("dev/default_SPI_chipInfo.json", spi.getDefaultDir());
    EXPECT_TRUE(logFile.good());

    deleteGarbage(filenames);
}