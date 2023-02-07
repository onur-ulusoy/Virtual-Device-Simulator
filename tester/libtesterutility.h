/**
 * @file libtesterutility.h
 * @brief Library of utility functions used in the tester such as string or file manipulation, directory creation.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#ifndef LIBTESTERUTILITY_H
#define LIBTESTERUTILITY_H

#include "testercommapi.h"

using namespace std;

namespace DriverTester {
    /**
    @brief This function returns the current date and time.
    @return A string representation of the current date and time.
    */
    string now();
    /**
    * @brief Get the last word of a file.
    * 
    * @param file_name The name of the file to be processed.
    * @return The last word of the file as a string.
    */
    string get_last_word(const string& file_name);
    /**
    * @brief Creates directories specified in the input array
    * 
    * Given an array of strings and its size, this function creates
    * directories with the names specified in the input array.
    *
    * This function is used to create directories for virtual devices to place. 
    * @param directories Array of strings representing the names of the directories to be created.
    * @param size Size of the input array.
    */
    void create_directories(string directories[], int size);


}

#endif //LIBTESTERUTILITY_H
