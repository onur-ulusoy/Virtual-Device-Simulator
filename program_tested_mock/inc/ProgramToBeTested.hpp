/**
 * @file ProgramToBeTested.hpp
 * @brief Contains the definition of the ProgramToBeTested class.
 *
 * The ProgramToBeTested class serves as a mock structure of a program to be tested.
 * It simulates the interactions with SPI devices by generating random SPI commands and storing logs of the communication for analysis.
 * 
 * @date 18.05.2023
 * @section LICENSE
 * Licensed under the MIT License.
 */

#ifndef PROGRAM_TO_BE_TESTED_MOCK
#define PROGRAM_TO_BE_TESTED_MOCK

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <filesystem>

/**
 * @class ProgramToBeTested
 * @brief Simulates a program that uses and requires SPI devices for testing purposes.
 *
 * This class is designed to generate random SPI commands and log their sequence for testing and debugging.
 * It also handles the storage and retrieval of these logs, allowing for detailed analysis of the software's behavior.
 */
class ProgramToBeTested {
public:
    /**
     * @brief Construct a new ProgramToBeTested object.
     * @param spi_data_json Path to the SPI data json file.
     */
    ProgramToBeTested(const std::string& spi_data_json);
    /**
     * @brief Generates random samples of SPI commands from a provided JSON file.
     *
     * The function reads the JSON file, stores all SPI write lines in a vector,
     * shuffles them, and then writes a random number of these lines to an output file.
     */
    void GenerateRandomSamples();
    /**
     * @brief Waits for verification of SPI commands by continuously reading from a "verification.txt" file.
     *
     * The function continuously reads the "verification.txt" file until a valid input is found.
     * If the input is "OK", it moves the log file and returns true.
     * If the input is "EXIT", it moves the log file and returns false.
     *
     * @return True if verification is successful, false otherwise.
     */
    bool WaitForVerification();
    /**
     * @brief Moves the log file to a dedicated logs directory after verification.
     *
     * The function checks if the log file exists, renames it with the current date, and then moves it to the logs folder.
     * It also creates a verification file to indicate that the operation was successful.
     */
    void moveLogFile();

private:
    /**
     * @brief Creates a Logs folder to store all logs generated during testing.
     *
     * The function checks if the logs folder exists and creates it if it doesn't.
     * If there's an issue during the creation of the folder, an error message is logged.
     */
    void createLogsFolder();
    /**
     * @brief Path to the SPI data json file.
     */
    std::string spi_data_json_;
};

#endif //PROGRAM_TO_BE_TESTED_MOCK