#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(DriverTest, ParseGPIOTest) {
    vector<std::string> filenames = getFileNames();

    // Friend function ParseGPIO Test
    // This function is a copy of ParseSPI, ParseCAN etc. in terms of behavior

    GPIODeviceMock devmock;
    GPIODeviceMock* gpioDevHandler = new GPIODeviceMock();
    

    EXPECT_CALL(devmock, device_open(WRITEONLY, gpioDevHandler))
            .Times(1)
            .WillOnce(testing::InvokeWithoutArgs([&]() {     
                gpioDevHandler->fd.open("test_device", ios::out);
                }));

    // Open device
    devmock.device_open(WRITEONLY, gpioDevHandler);

    string json_file_dir = "dev/default_GPIO_chipInfo.json";
    // Call the function to be tested
    //DeviceSim::GPIO_Device gpio(dev_con)
    //parse_GPIO(json_file_dir, &gpio);
    //gpio.device_close();

    // to be continued
    // Assert that the values are correctly parsed

    /*
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
    */
    deleteGarbage(filenames);
}