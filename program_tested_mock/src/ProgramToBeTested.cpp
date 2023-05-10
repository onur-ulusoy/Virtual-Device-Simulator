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

//#include <ProgramToBeTested.hpp>

class ProgramToBeTested {
public:
    ProgramToBeTested(const std::string &spi_data_json)
        : spi_data_json_(spi_data_json) {
            createLogsFolder();
            std::ofstream verification_file_check("verification");
        }

    void GenerateRandomSamples() {
        // Read input JSON file and store all spi_write lines in the vector
        nlohmann::json spi_data;
        std::ifstream input_file(spi_data_json_);
        if (!input_file.is_open()) {
            std::cerr << "Failed to open input file: " << spi_data_json_ << std::endl;
            return;
        }

        input_file >> spi_data;
        input_file.close();

        std::vector<std::string> spi_write_lines;
        for (const auto &trunk : spi_data.items()) {
            for (const auto &entry : trunk.value()) {
                spi_write_lines.push_back(entry["spi_write_line"].get<std::string>());
            }
        }

        // Shuffle the spi_write lines
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(spi_write_lines.begin(), spi_write_lines.end(), std::default_random_engine(seed));

        // Choose a random number of lines to write
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(1, spi_write_lines.size());
        size_t num_lines_to_write = dist(gen);

        // Write the randomly selected lines to the output file
        std::ofstream output_file("SPI_A.txt");
        if (!output_file.is_open()) {
            std::cerr << "Failed to open output file: SPI_A.txt" << std::endl;
            return;
        }

        for (size_t i = 0; i < num_lines_to_write; ++i) {
            output_file << spi_write_lines[i] << std::endl;
            output_file << std::endl;
        }
        output_file.close();
    }

    bool WaitForVerification() {
        std::string input;

        // Check if the "verification.txt" file exists, and create it if not
        std::ifstream verification_file_check("verification");
        if (!verification_file_check.is_open()) {
            std::ofstream create_verification_file("verification");
            create_verification_file.close();
        } else {
            verification_file_check.close();
        }

        // Continuously read input from "verification.txt" file until a valid input is found
        while (true) {
            std::ifstream verification_file("verification");
            if (!verification_file.is_open()) {
                std::cerr << "Failed to open verification file: verification" << std::endl;
                return false;
            }
            getline(verification_file, input);
            verification_file.close();

            if (input == "OK") {
                moveLogFile();
                return true;
            }         

            else if (input == "EXIT") 
                return false;

            else   
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
    }

    void moveLogFile()
    {        
        // Wait for 500ms before proceeding with the file operations
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        // Define the name of the log file and its new location
        std::string logFileName = "SPI_Log.txt";
        std::string logsFolder = "Logs/";
        std::string dateStr = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        
        // Check if the log file exists
        if (!std::filesystem::exists(logFileName)) {
            std::cerr << "Failed to find log file: " << logFileName << std::endl;
            exit(-1);
        }
        
        // Rename the log file with the current date
        std::string newLogFileName = logFileName + "_" + dateStr;
        std::cout << "SPI_Log saved successfully." << std::endl;
        std::filesystem::rename(logFileName, newLogFileName);

        // Move the renamed log file to the /Logs folder
        std::filesystem::create_directory(logsFolder);
        std::filesystem::rename(newLogFileName, logsFolder + newLogFileName);

        // Create a verification file to indicate that the operation was successful
        std::ofstream output_file("verification");
    }

    void createLogsFolder() {
        // Define the name of the logs folder
        std::string logsFolder = "Logs/";

        // Check if the folder already exists
        if (std::filesystem::exists(logsFolder)) {
            //std::cout << "Logs folder already exists." << std::endl;
            return;
        }

        // Create the folder
        try {
            std::filesystem::create_directory(logsFolder);
            //std::cout << "Logs folder created successfully." << std::endl;
        }
        catch (std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to create Logs folder: " << e.what() << std::endl;
        }
    }

private:
    std::string spi_data_json_;
};

#include <unistd.h>

int main() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        std::cerr << "Error getting current working directory" << std::endl;
    }
    
    ProgramToBeTested program("spi_data.json");

    while (true) {
        program.GenerateRandomSamples();

        if (!program.WaitForVerification()) {
            break;
        }
    }

    return 0;
}
