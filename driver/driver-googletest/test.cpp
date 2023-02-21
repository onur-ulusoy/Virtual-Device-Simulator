#include "gtest/gtest.h"
#include "libdriver.hpp"
#include <iostream>
#include <vector>
#include <cstdio>

using namespace DeviceSim;

int counter = 0;
vector<std::string> filenames = {"test_device", "log", "command", "communication-register"};
void deleteGarbage(vector<string> filenames);

TEST(GPIOTest, ConstructorTest) {
    // Test constructor by creating a new instance of the class
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Check that the instance was created successfully
    ASSERT_STREQ(gpio.getDevName(), dev_name);
    deleteGarbage(filenames);

}

TEST(GPIOTest, OpenReadOnlyTest) {
    // Test device open and close
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(READONLY, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);
}

TEST(GPIOTest, OpenWriteOnlyTest) {
    // Test device open and close
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(WRITEONLY, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);
}

TEST(GPIOTest, OpenDefaultTest) {
    // Test device open and close
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(DEFAULT, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);
}

TEST(GPIOTest, CloseTest) {
    // Test device open and close
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(DEFAULT, &gpio);
    gpio.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(gpio.getFd().is_open());
    deleteGarbage(filenames);
}

TEST(GPIOTest, ReadTest) {
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    //Check if the read function can return successfully
    gpio.devContent.read(10, "name", &gpio);
    
    deleteGarbage(filenames);
    
}

TEST(GPIOTest, ParseGPIOTest) {
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open device
    gpio.device_open(WRITEONLY, &gpio);

    string json_file_dir = "dev/default_GPIO_chipInfo.json";
    // Call the function to be tested
    parse_GPIO(json_file_dir, &gpio);
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
    
}

TEST(GPIOTest, ConfigTest) {
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open device
    //gpio.device_open(WRITEONLY, &gpio);

    //string json_file_dir = "dev/default_GPIO_chipInfo.json";
    // Call the function to be tested
    
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
    
}

TEST(GPIOTest, WriteTest) {
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
    
}


void deleteGarbage(vector<string> filenames){
    // iterate over the filenames and delete each file
    cout << ++counter << endl;
    for (const auto& filename : filenames) {
      remove(filename.c_str());
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}