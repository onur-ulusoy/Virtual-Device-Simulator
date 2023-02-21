#include "gtest/gtest.h"
#include "libdriver.hpp"
#include <iostream>
#include <vector>
#include <cstdio>

using namespace DeviceSim;

int counter = 0;
vector<std::string> filenames = {"test_device", "log", "command", "communication-register", "temp"};
void deleteGarbage(vector<string> filenames);
bool areFilesEqual(const std::string& filePath1, const std::string& filePath2);

TEST(DriverTest, AttributeGettersTest){
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

}

TEST(DriverTest, ConstructorTest) {
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

TEST(DriverTest, OpenReadOnlyTest) {
    // GPIO (Parent) Class OpenReadOnlyTest
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(READONLY, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Main) Class OpenReadOnlyTest
    SPI_Device spi(dev_name);

    // Open the device for reading
    spi.device_open(READONLY, &spi);
    // Check that the device file stream is open
    ASSERT_TRUE(spi.getFd().is_open());
    deleteGarbage(filenames);
    
}

TEST(DriverTest, OpenWriteOnlyTest) {
    // GPIO (Parent) Class OpenWriteOnlyTest
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(WRITEONLY, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Child) Class OpenWriteOnlyTest
    SPI_Device spi(dev_name);

    // Open the device for writing
    spi.device_open(WRITEONLY, &spi);
    // Check that the device file stream is open
    ASSERT_TRUE(spi.getFd().is_open());
    deleteGarbage(filenames);
}

TEST(DriverTest, OpenDefaultTest) {
    // GPIO (Parent) Class OpenWriteOnlyTest
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading and writing
    gpio.device_open(DEFAULT, &gpio);
    // Check that the device file stream is open
    ASSERT_TRUE(gpio.getFd().is_open());
    deleteGarbage(filenames);    

    // SPI (Child) Class OpenWriteOnlyTest

    SPI_Device spi(dev_name);

    // Open the device for reading and writing
    spi.device_open(DEFAULT, &spi);
    // Check that the device file stream is open
    ASSERT_TRUE(spi.getFd().is_open());
    deleteGarbage(filenames);   

}

TEST(DriverTest, CloseTest) {
    // GPIO (Parent) Class Close Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open the device for reading
    gpio.device_open(DEFAULT, &gpio);
    gpio.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(gpio.getFd().is_open());
    deleteGarbage(filenames);

    // SPI (Child) Class Close Test
    SPI_Device spi(dev_name);

    // Open the device for reading
    spi.device_open(DEFAULT, &spi);
    spi.device_close();
    // Check that the device file stream is open
    ASSERT_FALSE(spi.getFd().is_open());
    deleteGarbage(filenames);

}

TEST(DriverTest, ReadTest) {
    // GPIO (Parent) Class Read Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    //Check if the read function can return successfully
    gpio.devContent.read(10, "name", &gpio);
    deleteGarbage(filenames);

    // SPI (Child) Class Read Test
    SPI_Device spi(dev_name);

    //Check if the read function can return successfully
    spi.devContent.read(1, "name", &spi);
    deleteGarbage(filenames);
    
}

TEST(DriverTest, ParseGPIOTest) {
    // Friend method ParseGPIO Test
    // This function is copy of ParseSPI, ParseCAN in terms of behavior
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

TEST(DriverTest, ConfigTest) {
    // GPIO (Parent) Class Config Test
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

    // SPI (Child) Class Config Test
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

TEST(DriverTest, WriteTest) {
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

TEST(DriverTest, ShowTest) {
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    gpio.devContent.config(DEFAULT, &gpio);

    gpio.devContent.show(&gpio);
    //It's gonna generate temp file

    ASSERT_TRUE(areFilesEqual("temp", "defaultConfigShow"));
    
    deleteGarbage(filenames);
}

TEST(DriverTest, ParseTest) {
    //GPIO Parse Method Test
    char* dev_name = generateDevName("test_device");
    GPIO_Device gpio(dev_name);

    // Open device
    gpio.device_open(WRITEONLY, &gpio);

    string json_file_dir = "dev/default_GPIO_chipInfo.json";
    // Call the function to be tested
    gpio.parse(json_file_dir, &gpio);
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
    // SPI Parse Method Test
    SPI_Device spi(dev_name);

    // Open device
    spi.device_open(WRITEONLY, &spi);

    json_file_dir = "dev/default_SPI_chipInfo.json";
    // Call the function to be tested
    spi.parse(json_file_dir, &spi);
    spi.device_close();

    // Assert that the values are correctly parsed
    EXPECT_EQ("SPI_0", spi.devContent.read(0, "name", &spi));
    EXPECT_EQ("-", spi.devContent.read(0, "consumer", &spi));
    EXPECT_EQ("0", spi.devContent.read(0, "cpol", &spi));
    EXPECT_EQ("0", spi.devContent.read(0, "cpha", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "lsb_first", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "cs_high", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "3wire", &spi));
    EXPECT_EQ("false", spi.devContent.read(0, "loopback", &spi));


    EXPECT_EQ("SPI_1", spi.devContent.read(1, "name", &spi));
    EXPECT_EQ("-", spi.devContent.read(1, "consumer", &spi));
    EXPECT_EQ("0", spi.devContent.read(1, "cpol", &spi));
    EXPECT_EQ("0", spi.devContent.read(1, "cpha", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "lsb_first", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "cs_high", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "3wire", &spi));
    EXPECT_EQ("false", spi.devContent.read(1, "loopback", &spi));

    deleteGarbage(filenames);
    
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

void deleteGarbage(vector<string> filenames){
    // iterate over the filenames and delete each file
    cout << ++counter << " behaviors tested." << endl;
    for (const auto& filename : filenames) {
      remove(filename.c_str());
    }
}

bool areFilesEqual(const std::string& filePath1, const std::string& filePath2) {
    std::ifstream file1(filePath1, std::ios::binary);
    std::ifstream file2(filePath2, std::ios::binary);

    if (!file1 || !file2) {
        // Failed to open one or both files
        return false;
    }

    // Read the files into two strings
    std::stringstream buffer1, buffer2;
    buffer1 << file1.rdbuf();
    buffer2 << file2.rdbuf();
    std::string contents1 = buffer1.str();
    std::string contents2 = buffer2.str();

    // Replace end-of-line characters with a common format
    for (char& c : contents1) {
        if (c == '\r') {
            c = '\n';
        }
    }
    for (char& c : contents2) {
        if (c == '\r') {
            c = '\n';
        }
    }

    // Compare the strings
    return contents1 == contents2;
}