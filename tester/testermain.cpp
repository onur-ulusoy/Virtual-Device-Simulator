/**
 * @file testermain.cpp
 * @brief The main file of the tester program.
 *
 * @author Onur Ulusoy
 * @date 03.02.2023
 */
#include "testercommapi.hpp"

using namespace DriverTester;
/**
 * @brief The main function of the tester program.
 *
 * This function creates the required device directories, prepares the communication-register file to be committed, outputs introductory messages,
 * and takes user commands through the `get_and_transmit_command` function and transmits it to the device simulator.
 *
 * @return Returns 0 if the program terminates successfully.
 * @param log Stream object to manage the log file that is log of communication between driver and tester
 * @param directories Directories that devices take place when they are being simulated
 * @param com Stream object to manage the communicator register file that contains the communication between tester and driver
*  @param _command String to store the received command
 */
int main() {
    fstream log;
    string directories[] = {"dev/gpio", "dev/spi", "dev/i2c", "dev/ethernet", "dev/usart", "dev/uart", "dev/can"};
    create_directories(directories, 7);

    ofstream com ("communication-register");
    com.close();

    cout << "tester started working" << endl;
    string _command;

    cout << endl;

    cout << "Commands: " << endl;
    cout << "show-gpiochipx: shows gpiox set" << endl;
    cout << "read-gpiochipx-offset-property: reads gpiox spesific preperty from offset" << endl;
    cout << "write-gpiochipx-offset-property-newValue: writes new value to gpiox spesific property from offset" << endl;
    cout << "config-gpiochipx: configs gpiox set from default" << endl;
    cout << ".commandSet-textfile: Executes commands from file" << endl;
    cout << endl;

    cout << "Enter command (-1 to terminate): " << endl;

    get_and_transmit_command(_command, com, log);

    return 0;
}


