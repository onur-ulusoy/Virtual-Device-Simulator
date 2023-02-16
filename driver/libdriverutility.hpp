/**
 * @file libdriverutility.hpp
 * @brief Library of utility functions used in the driver such as string or file manipulation.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
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
#include <nlohmann/json.hpp>

using namespace std;

namespace DeviceSim{
    /**
    @brief This function moves the file stream object to the specified line number.
    @param file A reference to the file stream object.
    @param num The line number to which the file stream object is moved.
    @return A reference to the modified file stream object.
    */
    fstream& GotoLine(std::fstream& file, unsigned int num);
    /**
    @brief This function splits a string into multiple strings based on a delimiter.
    @param s The input string to be split.
    @param del The delimiter used to split the string.
    @param buffers An array of strings used to store the split sub-strings.
    */
    void Split(string s, string del, string* buffers);
    /**
    @brief This function removes the whitespaces from the right end of a string.
    @param s A reference to the input string.
    @return A reference to the modified string.
    */
    string &rtrim(std::string &s);
    /**
    @brief This function returns the current date and time.
    @return A string representation of the current date and time.
    */
    string now();
}


#endif //LIB_DRIVER_UTILITY_HPP
