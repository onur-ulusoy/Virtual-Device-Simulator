/**
 * @file libdriverutility.hpp
 * @brief Library of utility functions used in the driver such as string or file manipulation.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Extended 18/05/2023
 */
#ifndef LIB_DRIVER_UTILITY_HPP
#define LIB_DRIVER_UTILITY_HPP

#include <fstream>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <sstream>
#include <nlohmann/json.hpp>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace DeviceSim{
    /**
     * @brief This function moves the file stream object to the specified line number.
     * @param file A reference to the file stream object.
     * @param num The line number to which the file stream object is moved.
     * @return A reference to the modified file stream object.
     */
    fstream& GotoLine(std::fstream& file, unsigned int num);
    /**
     * @brief This function splits a string into multiple strings based on a delimiter.
     * @param s The input string to be split.
     * @param del The delimiter used to split the string.
     * @param buffers An array of strings used to store the split sub-strings.
     */
    void Split(string s, string del, string* buffers);
    /**
     * @brief This function removes the whitespaces from the right end of a string.
     * @param s A reference to the input string.
     * @return A reference to the modified string.
     */
    string &rtrim(std::string &s);
    /**
     * @brief This function returns the current date and time.
     * @return A string representation of the current date and time.
     */
    string now();
    /**
     * @brief Counts the number of lines in a file.
     * This function opens the specified file and reads its contents line by line. 
     * It increments a counter for each line encountered, effectively counting the total number of lines in the file.
     * @param filename The name of the file to count the lines in.
     * @return The number of lines in the file.
     */
    int count_lines(const string& filename);
    /**
     * @brief Creates directories based on the provided directory paths.
     * This function creates directories based on the array of directory paths provided. 
     * It iterates over the array and creates each directory using the `mkdir` function.
     * @param directories An array of directory paths to be created.
     * @param size The size of the directory array.
     */
    void create_directories(string directories[], int size);
    /**
     * @brief Splits a string into substrings based on a delimiter.
     * This function splits the input string into substrings based on the specified delimiter. 
     * It searches for occurrences of the delimiter in the input string and extracts the substrings between them. 
     * The substrings are stored in a vector and returned.
     * @param input The input string to be split.
     * @param delimiter The delimiter used to separate the substrings.
     * @return A vector of substrings resulting from the split operation.
     */
    std::vector<std::string> split_string(const std::string& input, const std::string& delimiter);
    /**
     * @brief Checks if a directory exists at the specified path.
     * This function checks if a directory exists at the provided path by attempting to open it as a file. 
     * If the file opening operation is successful, it indicates that the directory exists.
     * @param path The path of the directory to be checked.
     * @return `true` if the directory exists, `false` otherwise.
     */
    bool does_directory_exist(const std::string& path);
}

#endif //LIB_DRIVER_UTILITY_HPP
