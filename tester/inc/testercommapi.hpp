/**
 * @file testercommapi.hpp
 * @brief Communication API for tester to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Reworked: 18/05/2023
 *
 * @section LICENSE
 * Licensed under the MIT License.
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
#include "CommInterface.hpp"

using namespace std;
/**
 * @namespace Tester
 * @brief Encapsulates the necessary components for testing system operations in a structured framework.
 *
 */
namespace Tester{
    /**
     * @brief Logs a master command message to the specified communication register file.
     * 
     * This function records the time of action, specifies the master line command, and the actual command that is 
     * being written by the master. This function is used to keep track of the master commands that are executed.
     *
     * @param com Reference to the output file stream used to open the communication register file.
     * @param command The command string to be logged as a master command.
     */
    void write_master_command(std::ofstream& com, const std::string& command);
    /**
     * @brief Logs a slave response message to the specified communication register file.
     * 
     * This function records the time of action and the actual response message that is being written by the slave.
     * This function is used to keep track of the slave responses that are received.
     *
     * @param com Reference to the output file stream used to open the communication register file.
     * @param message The response message string to be logged as a slave response.
     */
    void write_slave_response(std::ofstream& com, const std::string& message);
    /**
     * @brief Evaluates the driver's response to a specific command and logs the output to the log file.
     *
     * This function logs the command received from the driver, the date of reception, and the driver's response.
     * It differentiates between different types of commands (read-, config-, show-, write-) and logs the output 
     * accordingly, providing detailed feedback on each operation. This function is used to maintain a record of 
     * all interactions with the driver for debugging and review.
     *
     * @param command The command string received from the driver.
     * @param response The driver's response to the command.
     * @param log Reference to the output file stream used to open the log file.
     */
    void write_driver_log(const std::string& command, const std::string& response, std::ofstream& log);
    /**
     * @brief Sends a command through a specified publisher.
     * 
     * This function uses the provided publisher to transmit a given command.
     * 
     * @param publisher Reference to the publisher object that transmits the command.
     * @param command The command to be transmitted.
     */
    void send_command(Publisher& publisher, const std::string& command);
    /**
     * @brief Receives and processes a response from a specified subscriber.
     * 
     * This function receives a raw response from the provided subscriber, then it
     * processes the response by extracting the part after a specific delimiter.
     * 
     * @param subscriber Reference to the subscriber object that receives the response.
     * @return string Returns the processed response.
     */
    string receive_response(Subscriber& subscriber);
}

#endif //TESTERCOMMAPI_HPP
