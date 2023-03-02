#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, WriteTest) {
    vector<std::string> filenames = getFileNames();

    // GPIO (Parent) Class Write (Method) Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    gpio.devContent.config(DEFAULT, &gpio);

    gpio.devContent.write(1, "name", "test_name", &gpio);
    string test_data = gpio.devContent.read(1, "name", &gpio);
    // Assert that the values are correctly written to the file  
    EXPECT_EQ("test_name", test_data);

    gpio.devContent.write(7, "consumer", "test_consumer", &gpio);
    test_data = gpio.devContent.read(7, "consumer", &gpio);
    // Assert that the values are correctly written to the file  
    EXPECT_EQ("test_consumer", test_data);

    gpio.devContent.write(10, "FLAG_ACTIVE_LOW", "[ACTIVE_LOW]", &gpio);
    test_data = gpio.devContent.read(10, "FLAG_ACTIVE_LOW", &gpio);
    // Assert that the values are correctly written to the file  
    EXPECT_EQ("[ACTIVE_LOW]", test_data);

    deleteGarbage(filenames);

    // SPI (Child) Class Write (Method) Test
    SPI_Device spi(dev_name);

    spi.devContent.config(DEFAULT, &spi);

    spi.devContent.write(1, "name", "test_name", &spi);
    test_data = spi.devContent.read(1, "name", &spi);
    // Assert that the values are correctly written to the file  
    EXPECT_EQ("test_name", test_data);

    spi.devContent.write(0, "consumer", "test_consumer", &spi);
    test_data = spi.devContent.read(0, "consumer", &spi);
    // Assert that the values are correctly written to the file  
    EXPECT_EQ("test_consumer", test_data);

    spi.devContent.write(1, "lsb_first", "true", &spi);
    test_data = spi.devContent.read(1, "lsb_first", &spi);
    // Assert that the values are correctly written to the file  
    EXPECT_EQ("true", test_data);

    deleteGarbage(filenames);
    
}
