#ifndef SPI_PROCESSOR_WRAPPER_HPP
#define SPI_PROCESSOR_WRAPPER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

class SpiProcessorWrapper {
public:
    SpiProcessorWrapper() = default;

    void copy_file(const std::string& input_file_path);

    void run_with_i_flag();

    void kill_i_flag_process();

    void run_with_f_flag();

    std::string request_read_line(const std::string& write_line);

private:
    std::ifstream input_file;
};

#endif // SPI_PROCESSOR_WRAPPER_HPP
