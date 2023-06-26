#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, ShowTest) {
    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) Class Show (Method) Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    gpio.devContent.config(DEFAULT, &gpio);

    gpio.devContent.show(&gpio);
    //The temp file is generated 

    ASSERT_TRUE(areFilesEqual("temp", "defaultConfigShowGPIO"));
    deleteGarbage(filenames);

    SPI_Device spi(dev_name);

    spi.devContent.config(DEFAULT, &spi);

    spi.devContent.show(&spi);
    //The temp file is generated 

    ASSERT_TRUE(areFilesEqual("temp", "defaultConfigShowSPI"));
    
    deleteGarbage(filenames);
}
