# CPP Wrapper Usage Documentation

## Introduction

This document outlines the usage of the `SpiProcessorWrapper` C++ class, which is used to interface with a Python script for processing SPI log data. The wrapper provides functionality to run the script in both `i` and `f` modes, as well as to request associated SPI read data for a given SPI write line.

## Class Definition

The `SpiProcessorWrapper` class is defined in the `spi_processor_wrapper.hpp` header file, which must be included in any code using the wrapper.

## Usage

Example usage for the -i mode:
```c++
SpiProcessorWrapper spi_wrapper;
std::string input_file_path = "SPI_Log_Trial.txt";

spi_wrapper.copyFile(input_file_path);
spi_wrapper.runWithIFlag();
```

Example usage for the -f mode:
```c++
SpiProcessorWrapper spi_wrapper;
spi_wrapper.runWithFFlag();
std::string write_line = "spi_write: Bytes written: 5: 0x01 0x04 0x03 0x04 0x00";
std::string read_line = spi_wrapper.requestReadLine(write_line);
```

An example main() function that uses both -i and -f modes:
```c++
#include <random>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    SpiProcessorWrapper spi_wrapper;
    std::string input_file_path = "SPI_Log_Trial.txt";

    spi_wrapper.copyFile(input_file_path);
    spi_wrapper.runWithIFlag();

    spi_wrapper.runWithFFlag();

    for (size_t i = 0; i < 30; i++)
    {
        std::string write_line = "spi_write: Bytes written: 5: 0x01 0x04 0x03 0x04 0x00";
        std::string read_line = spi_wrapper.requestReadLine(write_line);

        std::cout << "Read line: " << read_line << std::endl;
        std::uniform_real_distribution<float> dis(0.1f, 2.0f);
         float random_num = dis(gen);
        float stime = random_num;
       usleep((int) (stime * 1000000));

     }
    
    return 0;
}
```

Another example main() function that uses both -i and -f modes, and includes an input file copy:

```c++
int main() {
    std::string spi_processor_workpath = "../spi-processing/cpp_wrapper/src";
    chdir(spi_processor_workpath.c_str());

    SpiProcessorWrapper spi_wrapper;

    std::string input_file_path = "SPI_Log2.txt";
    std::string target_path = "../../SPI_Log.txt";
    fs::copy_file("../../../runtime-environment/" + input_file_path, target_path, fs::copy_options::overwrite_existing);

    float stime = 0.01;

    spi_wrapper.runWithIFlag();
    usleep((int) (stime * 1000000));

    std::string write_line;
    std::string read_line;

    spi_wrapper.runWithFFlag();

    for (size_t i = 0; i < 30; i++)
    {
        write_line = "spi_write: Bytes written: 5: 0x01 0x04 0x03 0x04 0x00";
        read_line = spi_wrapper.requestReadLine(write_line);

        std::cout << "Read line: " << read_line << std::endl;

        usleep((int) (stime * 1000000));

    }

    usleep((int) (stime * 1000000));

    write_line = "TERMINATE";

    read_line = spi_wrapper.requestReadLine(write_line);

    std::cout << "Read line: " << read_line << std::endl;

    spi_wrapper.runWithIFlag();

    return 0;
}
```

**Note that stime should be minimum of sleep_time of spi_processor specified in its config file**