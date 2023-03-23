#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, ConstructorTest) {
    vector<std::string> filenames = getFileNames();

    LibUtilityMock lu;
    char expectedDevName[] = "test_device";
    EXPECT_CALL(lu, _generateDevName("test_device")).Times(1).WillOnce(Return(expectedDevName));

    // GPIO (Parent) Class Constructor Test
    // Test constructor by creating a new instance of the class
    char* dev_name = lu.mockGenerateDevName("test_device");
    DeviceSim::GPIO_Device gpio(dev_name);

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