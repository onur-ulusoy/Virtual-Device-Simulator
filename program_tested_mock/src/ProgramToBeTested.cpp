/**
 * @file ProgramToBeTested.cpp
 * @brief Contains the definition of the ProgramToBeTested class and main() function.
 *
 * The ProgramToBeTested class serves as a mock structure of a program to be tested.
 * It simulates the interactions with SPI devices by generating random SPI commands and storing logs of the communication for analysis.
 * 
 * @date 18.05.2023
 * @section LICENSE
 * Licensed under the MIT License.
 */

#include <ProgramToBeTested.hpp>
#include <unistd.h>

ProgramToBeTested::ProgramToBeTested(const std::string& spi_data_json)
    : spi_data_json_(spi_data_json) {
    createLogsFolder();
    std::ofstream verification_file_check("verification");
}

void ProgramToBeTested::GenerateRandomSamples() {
    nlohmann::json spi_data;
    std::ifstream input_file(spi_data_json_);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file: " << spi_data_json_ << std::endl;
        return;
    }

    input_file >> spi_data;
    input_file.close();

    std::vector<std::string> spi_write_lines;
    for (const auto& trunk : spi_data.items()) {
        for (const auto& entry : trunk.value()) {
            spi_write_lines.push_back(entry["spi_write_line"].get<std::string>());
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(spi_write_lines.begin(), spi_write_lines.end(), std::default_random_engine(seed));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(1, spi_write_lines.size());
    size_t num_lines_to_write = dist(gen);

    std::ofstream output_file("SPI_A.txt");
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file: SPI_A.txt" << std::endl;
        return;
    }

    for (size_t i = 0; i < num_lines_to_write; ++i) {
        output_file << spi_write_lines[i] << '\n';
        output_file << '\n';
    }

    output_file.close();
}

bool ProgramToBeTested::WaitForVerification() {
    std::string input;

    std::ifstream verification_file_check("verification");
    if (!verification_file_check.is_open()) {
        std::ofstream create_verification_file("verification");
        create_verification_file.close();
    } else {
        verification_file_check.close();
    }

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
        } else if (input == "EXIT") {
            moveLogFile();
            return false;
        } else
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ProgramToBeTested::moveLogFile() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::string logFileName = "SPI_Log.txt";
    std::string logsFolder = "Logs/";
    std::string dateStr = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    if (!std::filesystem::exists(logFileName)) {
        std::cerr << "Failed to find log file: " << logFileName << std::endl;
        exit(-1);
    }

    std::string newLogFileName = logFileName + "_" + dateStr;
    std::cout << "SPI_Log saved successfully." << std::endl;
    std::filesystem::rename(logFileName, newLogFileName);

    std::filesystem::create_directory(logsFolder);
    std::filesystem::rename(newLogFileName, logsFolder + newLogFileName);

    std::ofstream output_file("verification");
}

void ProgramToBeTested::createLogsFolder() {
    std::string logsFolder = "Logs/";

    if (std::filesystem::exists(logsFolder)) {
        return;
    }

    try {
        std::filesystem::create_directory(logsFolder);
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create Logs folder: " << e.what() << std::endl;
    }
}

 /**
  * @brief The main entry point for the testing program.
  *
  * This function constructs the ProgramToBeTested object and initiates the generation of random samples.
  * It then waits for verification. If the verification returns false, it breaks the loop and ends the program.
  *
  * @return 0 upon successful execution.
  */
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
