/**
 * @file libtesterutility.hpp
 * @brief Library of utility functions used in the tester such as string or file manipulation, directory creation.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Extended: 18/05/2023
 *
 * @section LICENSE
 * Licensed under the MIT License.
 */
#ifndef LIBTESTERUTILITY_HPP
#define LIBTESTERUTILITY_HPP

#include "testercommapi.hpp"

using namespace std;

namespace Tester {
    /**
     * @brief This function returns the current date and time.
     * @return A string representation of the current date and time.
     */
    string now();
    /**
     * @brief Gets the last word of a file.
     * 
     * @param file_name The name of the file to be processed.
     * @return The last word of the file as a string.
     */
    string get_last_word(const string& file_name);
    /**
     * @brief Removes all empty lines from a file.
     *
     * This function reads a file line by line and writes all non-empty lines to a temporary buffer. 
     * Then it overwrites the original file with the content of the buffer, effectively removing 
     * all empty lines from the file. If an error occurs during file opening, the function will output 
     * an error message to the standard error stream and return immediately.
     *
     * @param file_path The path to the file from which empty lines should be removed.
     */
    void remove_empty_lines(const std::string& file_path);
}

#endif //LIBTESTERUTILITY_HPP
