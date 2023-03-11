/**
 * @file testercommapi.hpp
 * @brief Communication API for tester to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#ifndef TESTERCOMMAPI_HPP
#define TESTERCOMMAPI_HPP

#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include "libtesterutility.hpp"

using namespace std;
/**
 * @namespace DriverTester
 * @brief Contains set of functions to test the driver simulates a virtual device.
 *
 */
namespace DriverTester{
    /**
    @brief Writes the master command to the communication-register file
    @param com Reference to the ofstream object for communcation-register file
    @param _command A string stores the master command
    */
    void master_writing(ofstream& com, string _command);
    /**
    @brief Evaluates the driver response to a specified command and writes the output to the log file
    @param _command A string stores the command been received from driver
    @param word The return value from driver for the command
    @param log Reference to the fstream object used to open the log file
    */
    void evaluate_slave(string _command, string word, fstream& log);
    /**
    * @brief Gets commands from user and transmits to the driver
    * 
    * This function gets the commands from the user and transmits it to the driver in order to be processed while device is being simulated.
    * It also logs the communication between the two systems and terminates if the input is -1.
    *
    * @param _command A string stores the command to be transmitted
    * @param com Reference to the ofstream object for communcation-register file
    * @param log Reference to the fstream object used to open the log file
    */
    void get_and_transmit_command(string& _command, ofstream& com, fstream& log);
}

#endif //TESTERCOMMAPI_HPP
