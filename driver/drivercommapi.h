/**
 * @file drivercommapi.h
 * @brief Communication API for driver to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#ifndef DRIVER_DRIVERCOMMAPI_H
#define DRIVER_DRIVERCOMMAPI_H

#include "libdriver.h"
/**
 * @namespace DeviceSim
 * @brief Gathers around all the tools contributing to simulate device under a frame.
 * 
 */
namespace DeviceSim {
    /**
    * @brief Receives commands that are sent by tester with the specified request and performs the necessary operations.
    *
    * @param req      The type of command request. Can be either ONESHOT or RECURSIVE. If commands received from text file, RECURSIVE mode runs. 
    * @param devType  The type of device being processed. Can be one of "gpio", "spi", "i2c", "ethernet", "usart", "uart", "can".
    * @param receiver Stream object for reading the command from command text.
    * @param _command String to store the received command.
    * @param _com     Stream object for writing responses to communication register.
    *
    * @return -1 if the command is to disconnect the device, 0 if the command is to process the device or to execute nothing.
    */
    int receive_command(enum command_request req, string devType, fstream& receiver, string& _command, ofstream& _com);
    /**
    * @brief Transmits a command to tester by writing "&" to the file "command" meaning that driver is ready to receive a new command.
    */
    void throw_command();
    /**
    * @brief Commits the message to the communication register file that is return of the command.
    *
    * @param com      Stream object for writing the message to the communication register file.
    * @param message  String containing the message to be written to the communication register file.
    *
    */
    void slave_writing(ofstream& com, string message);
    void transmit_response(string message);
}


#endif //DRIVER_DRIVERCOMMAPI_H
