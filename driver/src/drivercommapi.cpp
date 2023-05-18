/**
 * @file drivercommapi.cpp
 * @brief Communication API for driver to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Reworked: 18/05/2023
 */
#include "drivercommapi.hpp"

using namespace std;

namespace DeviceSim {
    
    string receive_command(Subscriber& subscriber, const int timeout){
        // receive a message from the topic
        std::string command = subscriber.receive(timeout);

        // purify the message
        std::string delimiter = ": ";
        size_t pos = command.find(delimiter);
        if (pos != std::string::npos) {
            command = command.substr(pos + delimiter.length());
        }

        return command;
    }

    void transmit_response(Publisher& publisher, const string msg){
        publisher.publish(msg);
    }
}

