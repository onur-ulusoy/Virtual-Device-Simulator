/**
* @file drivermain.cpp
* @brief Main function for the driver program.
*
* This file contains the main function for the driver program. The program simulates a device and listens for commands
* sent to it. The commands are read, processed and appropriate responses are sent back to tester through the communication interface.
* @author Onur Ulusoy
* @date 03/02/2023, Reworked: 18/05/2023
*
* @section LICENSE
* Licensed under the MIT License.
*/

#include "drivercommapi.hpp"
#include "SpiProcessorWrapper.hpp"
#include "SpiProcessorUtil.hpp"

using namespace DeviceSim;
/**

* @brief Main entry point for the driver program.
*
* This function is the entry point for the driver program. It initializes the necessary components, creates the required directories,
* sets up the communication channels with the tester, and listens for commands. Upon receiving a command, it executes the corresponding
* action and sends the appropriate response back to the tester.
* @return 0 indicating successful execution of the program.
*/
int main() {
    std::ofstream pidFile("driver.pid");
    pidFile << getpid();
    pidFile.close();

    string dev_root[] = {"dev"};
    create_directories(dev_root, 1);
    string directories[] = {"dev/gpio", "dev/spi", "dev/i2c", "dev/ethernet", "dev/usart", "dev/uart", "dev/can"};
    create_directories(directories, 7);

    // Pipeline of commands from tester to driver
    string commands_topic = "tcp://localhost:6000";

    // Pipeline of responses to a command from driver to tester
    string responses_topic = "tcp://localhost:6002";

    Subscriber tester_listener(commands_topic);
    Publisher tester_speaker(responses_topic, "driver");

    const int tester_listener_timeout = 4000;
    ofstream register_file;
    string command;
    string response;

    while (true) { // keep listening for messages indefinitely
        
        command = receive_command(tester_listener, tester_listener_timeout);
        if (command == "")
            break;

        cout << command << endl;
        response = execute_command(ONESHOT, "spi", command, register_file);
        transmit_response(tester_speaker, response);
        sleep(1);

    }
}




