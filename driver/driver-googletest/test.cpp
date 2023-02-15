#include "gtest/gtest.h"
#include "libdriver.hpp"
#include <iostream>
/* // Tests the constructor of the GPIO_Device class.
TEST(GPIO_DeviceTest, ConstructorTest) {
  // Create a GPIO_Device object with a null device name.
  char *dev_name = NULL;
  GPIO_Device gpio_dev(dev_name);

  // Verify that the default directory of the device is correct.
  std::string default_dir = gpio_dev.getDefaultDir();
  EXPECT_EQ(default_dir, "dev/default_GPIO_chipInfo.json");

  // Verify that the pack of the device is correct.
  std::string expected_pack[8] = {"offset", "name", "consumer", "FLAG_IS_OUT", "FLAG_ACTIVE_LOW",
                                  "FLAG_OPEN_DRAIN", "FLAG_OPEN_SOURCE", "FLAG_KERNEL"};
  std::string *pack = gpio_dev.getPack();
  int pack_size = gpio_dev.getPackSize();
  for (int i = 0; i < pack_size; i++) {
    EXPECT_EQ(pack[i], expected_pack[i]);
  }
}

// Tests the device_open() and device_close() methods of the GPIO_Device class.
TEST(GPIO_DeviceTest, OpenCloseTest) {
  // Create a GPIO_Device object with a null device name.
  char *dev_name = NULL;
  GPIO_Device gpio_dev(dev_name);

  // Open the device in READONLY mode and verify that it is open.
  gpio_dev.device_open(READONLY, &gpio_dev);
  EXPECT_TRUE(gpio_dev.fd.is_open());

  // Close the device and verify that it is closed.
  gpio_dev.device_close();
  EXPECT_FALSE(gpio_dev.fd.is_open());

  // Open the device in WRITEONLY mode and verify that it is open.
  gpio_dev.device_open(WRITEONLY, &gpio_dev);
  EXPECT_TRUE(gpio_dev.fd.is_open());

  // Close the device and verify that it is closed.
  gpio_dev.device_close();
  EXPECT_FALSE(gpio_dev.fd.is_open());

  // Open the device in DEFAULT mode and verify that it is open.
  gpio_dev.device_open(DEFAULT, &gpio_dev);
  EXPECT_TRUE(gpio_dev.fd.is_open());

  // Close the device and verify that it is closed.
  gpio_dev.device_close();
  EXPECT_FALSE(gpio_dev.fd.is_open());
}

// Tests the DeviceContent::config() method of the GPIO_Device class.
TEST(GPIO_DeviceTest, ConfigTest) {
  // Create a GPIO_Device object with a null device name.
  char *dev_name = NULL;
  GPIO_Device gpio_dev(dev_name);

  // Call the config() method with the DEFAULT request type and verify that it returns "true".
  std::string result = gpio_dev.DeviceContent::config(DEFAULT, &gpio_dev);
  EXPECT_EQ(result, "true");
}

// Tests the DeviceContent::show() method of the GPIO_Device class.
TEST(GPIO_DeviceTest, ShowTest) {
  // Create a GPIO_Device object with a null device name.
  char *dev_name = NULL;
  GPIO_Device gpio_dev(dev_name);

  // Call the show() method and verify that it returns "true".
  std::string result = gpio_dev.DeviceContent::show(&gpio_dev);
  EXPECT_EQ(result, "true");
}
 */

int main(){
  std::cout << "CMake" << endl;
  getchar();
}