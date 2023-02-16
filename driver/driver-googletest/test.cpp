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