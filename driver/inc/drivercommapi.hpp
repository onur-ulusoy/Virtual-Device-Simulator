/**
 * @file drivercommapi.hpp
 * @brief Communication API for driver to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Reworked: 18/05/2023
 */
#ifndef DRIVER_DRIVERCOMMAPI_HPP
#define DRIVER_DRIVERCOMMAPI_HPP

#include "libdriver.hpp"
#include "CommInterface.hpp"
#include <unordered_map>

/**
 * @namespace DeviceSim
 * @brief Brings together all the essential tools necessary for simulating a device within a comprehensive framework.
 * 
 */
namespace DeviceSim {
    /**
     * @brief Receives a command from a subscriber.
     *
     * This function receives a command message from the specified subscriber within the given timeout period. The received message is purified
     * by removing any prefix before the actual command. The purified command is then returned as a string.
     *
     * @param subscriber The subscriber object used to receive the command.
     * @param timeout The timeout value for receiving the command, in milliseconds.
     * @return The received command message.
     */
    string receive_command(Subscriber& subscriber, const int timeout);
    /**
     * @brief Transmits a response message using a publisher.
     *
     * This function publishes the specified response message using the provided publisher. The response message is sent to the corresponding topic
     * for further processing or analysis.
     *
     * @param publisher The publisher object used to transmit the response.
     * @param msg The response message to be transmitted.
     */
    void transmit_response(Publisher& publisher, const string msg);
}

#endif //DRIVER_DRIVERCOMMAPI_HPP
