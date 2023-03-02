/**
* @file drivermain.cpp
* @brief Main function for the device simulator program.
*
* This file contains the main function for the driver simulator program. The program simulates a device and listens for commands
* sent to it. The commands are read from a file, processed and appropriate responses are sent back to the communication interface.
* @author Onur Ulusoy
* @date 03/02/2023
*/
#include "drivercommapi.hpp"

using namespace DeviceSim;
/**

@brief Main function for the driver simulator program.

This function starts the driver simulator program. The program reads commands for the device to be simulated from a file and
listens for commands sent to it. The commands are processed and appropriate responses are sent back to the communication interface.

@return 0 if the program terminates successfully.

* @param receiver Stream object for reading the command from command text.
* @param _command String to store the received command.
* @param _com     Stream object for writing responses to communication register.
*/
int main() {
    fstream receiver;
    ofstream _com;
    string _command;

    float sleep_time = 0.2;

    cout << "device simulator started working."<< endl;

    receiver.open("command", ios::in);
    string device;

    receiver >> device;

    cout << device << " device is being simulated." << endl;

    receiver.close();

    receiver.open("command", ios::out);
    receiver << "start";

    receiver.close();

    ofstream logFile;
    logFile.open("log");
    logFile.close();

    usleep(1000000 * sleep_time);

    while (receive_command(ONESHOT, device, receiver, _command, _com) != -1)
        usleep(1000000 * sleep_time);

    cout << "Device simulator is terminating .." << endl;
    return 0;
}




