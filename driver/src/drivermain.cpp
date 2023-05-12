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
#include "SpiProcessorWrapper.hpp"
#include "SpiProcessorUtil.hpp"

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
    std::ofstream pidFile("driver.pid");
    pidFile << getpid();
    pidFile.close();

    /* fstream receiver;
    ofstream _com;
    string _command;

    ofstream outf ("command0");

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

    cout << "Device simulator is terminating .." << endl; */

    // char* dev_name = generateDevName("device");
    // I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
    // i2cDevHandler->device_open(WRITEONLY, i2cDevHandler);

    // cout << i2cDevHandler->getDevName() << endl;
    // cout << i2cDevHandler->getDefaultDir() << endl;
    // cout << i2cDevHandler->getPackSize() << endl;z
    // cout << i2cDevHandler->getPack()[5] << endl;
    // i2cDevHandler->parse();

    // Device& gpio_dev = GPIO_Device::getInstance("device"); 
    // gpio_dev.device_open(WRITEONLY);
    // gpio_dev.devContent.config(DEFAULT);
    // cout << gpio_dev.devContent.read(2, "name") << endl;
    // gpio_dev.device_close();

    // Publisher publisher("tcp://*:5555", "driver");

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




