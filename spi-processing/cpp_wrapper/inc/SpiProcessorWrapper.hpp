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

#ifndef SPI_PROCESSOR_WRAPPER_HPP
#define SPI_PROCESSOR_WRAPPER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @class SpiProcessorWrapper
 * @brief A class that provides a high-level interface for controlling SPI communication output.
 */
class SpiProcessorWrapper {
public:
    /**
     * @brief Default constructor for the SpiProcessorWrapper class.
     */
    SpiProcessorWrapper() = default;
    /**
     * @brief Copies the input file to a target path.
     * 
     * This method takes an input file path and copies the file to a target path. If the file already exists at the target path,
     * it will be overwritten. Purpose of that is put the input file into the spi processor runtime environment.
     * 
     * @param inputFilePath The path to the input file to be copied.
     */
    void copyFile(const std::string& inputFilePath);
    /**
     * @brief Runs the spi_processor.py script with the -i flag.
     * 
     * This method runs the spi_processor.py script with the -i flag to process the input file copied to the target path.
     */
    void runWithIFlag();
    /**
     * @brief Kills the spi_processor.py script running with the -i flag.
     * 
     * This method kills the spi_processor.py script that is running with the -i flag.
     */
    void killIFlagProcess();
    /**
     * @brief Runs the spi_processor.py script with the -f flag.
     * 
     * This method runs the spi_processor.py script with the -f flag.
     */
    void runWithFFlag();
    /**
     * @brief Requests a read_line from the spi processor.
     * 
     * This method takes a string representing the write_line to be sent to spi processor, writes the command to the "in" file,
     * and waits for a response in the "out" file. If the response is a valid spi_read line or the string "TERMINAT", the method returns the response.
     * 
     * @param writeLine The write command to be sent to spi processor.
     * @return A string representing the read response from spi processor.
     */
    std::string requestReadLine(const std::string& writeLine);

private:
    /// An input file stream used for reading input files.
    std::ifstream inputFile;
};

#endif // SPI_PROCESSOR_WRAPPER_HPP
