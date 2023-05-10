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

class ProgramToBeTested {
public:
    ProgramToBeTested(const std::string &spi_data_json)
        : spi_data_json_(spi_data_json) {
            std::ofstream verification_file_check("verification.txt");
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
            std::ofstream create_verification_file("verification.txt");
            create_verification_file.close();
        } else {
            verification_file_check.close();
        }

        // Continuously read input from "verification.txt" file until a valid input is found
        while (true) {
            std::ifstream verification_file("verification");
            if (!verification_file.is_open()) {
                std::cerr << "Failed to open verification file: verification.txt" << std::endl;
                return false;
            }
            getline(verification_file, input);
            verification_file.close();

            if (input == "OK") {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::ofstream output_file("verification.txt");
                return true;
            }
                

            else if (input == "EXIT") 
                return false;

            else   
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
    }

private:
    std::string spi_data_json_;
};


int main() {
    ProgramToBeTested program("spi_data.json");

    while (true) {
        program.GenerateRandomSamples();

        if (!program.WaitForVerification()) {
            break;
        }
    }

    return 0;
}
