/**
 * @file SpiProcessorWrapper.hpp
 * @brief C++ wrapper containing the SpiProcessorWrapper class, which provides a high-level interface for processing the SPI communication output.
 * 
 * The SpiProcessorWrapper class provides a set of methods to copy an input file, run the spi_processor.py script with different flags,
 * request a read operation from SPI communication, and kill the SPI communication process.
 * 
 * @author Onur Ulusoy
 * @date 18.04.2023
 * @license MIT
 */

#include "SpiProcessorWrapper.hpp"

void SpiProcessorWrapper::copyFile(const std::string& input_file_path) {
    const fs::path target_path = "../../SPI_Log.txt";
        
    try {
        fs::copy_file(input_file_path, target_path, fs::copy_options::overwrite_existing);
        std::cout << "File copied successfully." << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
    }
}

void SpiProcessorWrapper::runWithIFlag() {
    std::string command = "cd ../.. && python3 spi_processor.py -i";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Data saved to json successfully." << std::endl;
    } else {
        std::cerr << "Failed to run spi_processor.py" << std::endl;
    }
}

void SpiProcessorWrapper::killIFlagProcess() {
    std::string command = "pkill -f 'python3 spi_processor.py -i'";
    std::system(command.c_str());
    std::cout << "i flag process terminated." << std::endl;
}

void SpiProcessorWrapper::runWithFFlag() {
    // Create "in" and "out" files
    std::ofstream in_file("../../in");
    std::ofstream out_file("../../out");

    in_file.close();
    out_file.close();

    // Run spi_processor.py with -f flag in the background without console output
    std::string command = "cd ../.. && python3 spi_processor.py -f > /dev/null 2>&1 &";
    FILE* proc = popen(command.c_str(), "r");
    if (proc == nullptr) {
        std::cerr << "Failed to run spi_processor.py" << std::endl;
    } else {
        pclose(proc);
    }
}

std::string SpiProcessorWrapper::requestReadLine(const std::string& write_line) {
    // Save write_line to "in" file
    std::ofstream in_file("../../in");
    in_file << write_line;
    in_file.close();

    std::string read_line;
    std::string identifier;

    // Loop until a valid spi_read line is found
    while (true) {
        std::ifstream out_file("../../out");

        if (out_file.is_open()) {
            std::getline(out_file, read_line);
            out_file.close();
            identifier = read_line.substr(0, 8);
            if ( identifier == "spi_read" || identifier == "TERMINAT") {
                break;
            }
        }
        usleep(10000);
    }

    // Recreate the "in" file
    in_file.open("../../in", std::ofstream::out | std::ofstream::trunc);
    in_file.close();

    return read_line;
}
