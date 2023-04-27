/**
 * @file libtesterutility.cpp
 * @brief Library of utility functions used in the tester such as string or file manipulation, directory creation.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#include "libtesterutility.hpp"

namespace DriverTester {
    string now(){
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm *tm = std::localtime(&now_c);

        // Create a stringstream object
        std::stringstream ss;

        // Use the stringstream's operator << to format the time
        ss << std::put_time(tm, "%c");

        // Get the string from the stringstream
        return ss.str();
    }

    string get_last_word(const string& file_name) {
        std::ifstream file(file_name);
        std::string buffer;

        // Check if the file is open
        if (!file.is_open()) {
            return buffer;
        }

        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        if (size == 0) {
            file.close();
            return buffer;
        }

        // Move the position to the end of the file
        file.seekg(-1, std::ios_base::end);

        // Find the beginning of the last word
        while (file.peek() != ' ' && file.tellg() > 0) {
            file.seekg(-1, std::ios_base::cur);
        }
        if (file.tellg() == 0) {
            std::getline(file, buffer);
        } else {
            file.seekg(1, std::ios_base::cur);
            std::string line;
            std::getline(file, line);
            buffer = line.substr(line.find_last_of(" ") + 1);
        }

        // Close the file
        file.close();

        return buffer;
    }

    void create_directories(string directories[], int size) {
        for (int i = 0; i < size; i++) {
            mkdir(directories[i].c_str(), 0777);
        }
    }

    void remove_empty_lines(const std::string& file_path) {
        std::ifstream inputFile(file_path);
        std::ostringstream output;
        std::string line;

        if (!inputFile) {
            std::cerr << "Error opening file: " << file_path << std::endl;
            return;
        }

        while (std::getline(inputFile, line)) {
            if (!line.empty()) {
                output << line << '\n';
            }
        }

        inputFile.close();

        std::ofstream outputFile(file_path);
        if (!outputFile) {
            std::cerr << "Error opening file for writing: " << file_path << std::endl;
            return;
        }

        outputFile << output.str();
        outputFile.close();
    }
}