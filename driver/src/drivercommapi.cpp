/**
 * @file drivercommapi.cpp
 * @brief Communication API for driver to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#include "drivercommapi.hpp"

using namespace std;

namespace DeviceSim {
    
    string receive_command(Subscriber& subscriber){
        // receive a message from the topic
        std::string command = subscriber.receive();

        // purify the message
        std::string delimiter = ": ";
        size_t pos = command.find(delimiter);
        if (pos != std::string::npos) {
            command = command.substr(pos + delimiter.length());
        }

        return command;
    }

    string execute_command(const enum command_request request_type, const string dev_type, const string command, ofstream& register_file) {

        string driver_response = "false";
        
        std::vector<std::string> substrings = split_string(command, "-");

        static string operation, device_name, property_name, property_new_value;
        static int offset;

        device_name = substrings[1];
        string device_path = "dev/" + dev_type + "/" + device_name;
        
        Device& dev = create_device(dev_type, device_path);

        operation = substrings[0];

        if (request_type == ONESHOT){
            if (operation == "write"){
                
                if (!does_directory_exist(device_path))
                    dev.devContent.config(DEFAULT);

                offset = stoi(substrings[2]);
                property_name = substrings[3];
                property_new_value = substrings[4];
                cout << property_name << endl;
                dev.devContent.write(offset, property_name, property_new_value);
            }

            // else if ...

        }



        /*if (request == ONESHOT) {
            
            std::vector<std::string> substrings = split_string(command, "-");



            if (substrings[0] == "show") {
                string path = "dev/" + devType + "/" + substrings[1];
                char* dev_name = new char[path.length() + 1];
                strcpy(dev_name, path.c_str());

                if (devType == "gpio"){
                    GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                    returnVal = gpioDevHandler->devContent.show(gpioDevHandler);
                }
                else if (devType == "spi"){
                    SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                    returnVal = spiDevHandler->devContent.show(spiDevHandler);
                }
                else if (devType == "i2c"){
                    I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                    returnVal = i2cDevHandler->devContent.show(i2cDevHandler);
                }
                else if (devType == "ethernet"){
                    ETHERNET_Device* ethernetDevHandler = new ETHERNET_Device(dev_name);
                    returnVal = ethernetDevHandler->devContent.show(ethernetDevHandler);
                }
                else if (devType == "usart"){
                    USART_Device* usartDevHandler = new USART_Device(dev_name);
                    returnVal = usartDevHandler->devContent.show(usartDevHandler);
                }
                else if (devType == "uart"){
                    UART_Device* uartDevHandler = new UART_Device(dev_name);
                    returnVal = uartDevHandler->devContent.show(uartDevHandler);
                }
                else if (devType == "can"){
                    CAN_Device* canDevHandler = new CAN_Device(dev_name);
                    returnVal = canDevHandler->devContent.show(canDevHandler);
                }

                slave_writing(_com, returnVal);

            }

            else if (substrings[0] == "read") {
                string path = "dev/" + devType + "/" + substrings[1];
                char* dev_name = new char[path.length() + 1];
                strcpy(dev_name, path.c_str());

                int offset = stoi(substrings[2]);
                string data;
                if (devType == "gpio"){
                    GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                    data = gpioDevHandler->devContent.read(offset, substrings[3], gpioDevHandler);
                }

                else if (devType == "spi"){
                    SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                    data = spiDevHandler->devContent.read(offset, substrings[3], spiDevHandler);
                }

                else if (devType == "i2c"){
                    I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                    data = i2cDevHandler->devContent.read(offset, substrings[3], i2cDevHandler);
                }

                else if (devType == "ethernet"){
                    ETHERNET_Device* ethDevHandler = new ETHERNET_Device(dev_name);
                    data = ethDevHandler->devContent.read(offset, substrings[3], ethDevHandler);
                }

                else if (devType == "usart"){
                    USART_Device* usartDevHandler = new USART_Device(dev_name);
                    data = usartDevHandler->devContent.read(offset, substrings[3], usartDevHandler);
                }

                else if (devType == "uart"){
                    UART_Device* uartDevHandler = new UART_Device(dev_name);
                    data = uartDevHandler->devContent.read(offset, substrings[3], uartDevHandler);
                }

                else if (devType == "can"){
                    CAN_Device* canDevHandler = new CAN_Device(dev_name);
                    data = canDevHandler->devContent.read(offset, substrings[3], canDevHandler);
                }

                slave_writing(_com, data);

            }

            else if (substrings[0] == "write") {
                string path = "dev/" + devType + "/" + substrings[1];
                char* dev_name = new char[path.length() + 1];
                strcpy(dev_name, path.c_str());

                int offset = stoi(substrings[2]);

                if (devType == "gpio"){
                    GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                    returnVal = gpioDevHandler->devContent.write(offset, substrings[3], substrings[4], gpioDevHandler);
                }

                else if (devType == "spi"){
                    SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                    returnVal = spiDevHandler->devContent.write(offset, substrings[3], substrings[4], spiDevHandler);            }

                else if (devType == "i2c"){
                    I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                    returnVal = i2cDevHandler->devContent.write(offset, substrings[3], substrings[4], i2cDevHandler);            }

                else if (devType == "ethernet"){
                    ETHERNET_Device* ethDevHandler = new ETHERNET_Device(dev_name);
                    returnVal = ethDevHandler->devContent.write(offset, substrings[3], substrings[4], ethDevHandler);            }

                else if (devType == "usart"){
                    USART_Device* usartDevHandler = new USART_Device(dev_name);
                    returnVal = usartDevHandler->devContent.write(offset, substrings[3], substrings[4], usartDevHandler);            }

                else if (devType == "uart"){
                    UART_Device* uartDevHandler = new UART_Device(dev_name);
                    returnVal = uartDevHandler->devContent.write(offset, substrings[3], substrings[4], uartDevHandler);            }

                else if (devType == "can"){
                    CAN_Device* canDevHandler = new CAN_Device(dev_name);
                    returnVal = canDevHandler->devContent.write(offset, substrings[3], substrings[4], canDevHandler);            }

                slave_writing(_com, returnVal);
            }

            else if (substrings[0] == "config") {
                string path = "dev/" + devType + "/" + substrings[1];
                char* dev_name = new char[path.length() + 1];
                strcpy(dev_name, path.c_str());

                if (devType == "gpio"){
                    GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                    returnVal = gpioDevHandler->devContent.config(DEFAULT, gpioDevHandler);
                }
                else if (devType == "spi"){
                    SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                    returnVal = spiDevHandler->devContent.config(DEFAULT, spiDevHandler);
                }
                else if (devType == "i2c"){
                    I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                    returnVal = i2cDevHandler->devContent.config(DEFAULT, i2cDevHandler);
                }
                else if (devType == "ethernet"){
                    ETHERNET_Device* ethernetDevHandler = new ETHERNET_Device(dev_name);
                    returnVal = ethernetDevHandler->devContent.config(DEFAULT, ethernetDevHandler);
                }
                else if (devType == "usart"){
                    USART_Device* usartDevHandler = new USART_Device(dev_name);
                    returnVal = usartDevHandler->devContent.config(DEFAULT, usartDevHandler);
                }
                else if (devType == "uart"){
                    UART_Device* uartDevHandler = new UART_Device(dev_name);
                    returnVal = uartDevHandler->devContent.config(DEFAULT, uartDevHandler);
                }
                else if (devType == "can"){
                    CAN_Device* canDevHandler = new CAN_Device(dev_name);
                    returnVal = canDevHandler->devContent.config(DEFAULT, canDevHandler);
                }

                slave_writing(_com, returnVal);
            }

            else if (substrings[0] == ".commandSet") {
                int line_count = count_lines(substrings[1]);
                cout << "Command count:" << line_count << endl;
                float sleep_time = 0.0006;
                fstream f;
                f.open(substrings[1]);
                int counter = 0;
                while (counter++ < line_count) {
                    if(f.eof()) break;
                    _command.clear();
                    f >> _command;
                    
                    if (_command.length()  == 0)
                        break;
                    receive_command(RECURSIVE, devType, receiver, _command, _com);
                    transmit_response("&&" + _command);

                    string master_response;

                    while (master_response != "-2"){
                            
                        usleep(1000000 * sleep_time);
                        master_response.clear();
                        receiver.open("command");
                        receiver >> master_response;
                        receiver.close();
                    }
                }

                f.close();
                slave_writing(_com, "disconnecting");
                cout << "Driver is terminating .." << endl;    
                
                transmit_response("&1");
                ofstream outfile ("command");
                outfile << "-1";
                outfile.close();

                exit(0);

            }*/

            // else {
            //     slave_writing(_com, "Invalid command");
            // }

        
        
        //throw_command();
        return driver_response;
    }

    Device& create_device(const std::string& dev_type, const std::string& dev_name) {
        // create and return the appropriate device object
        if (dev_type == "gpio") {
            return GPIO_Device::getInstance(dev_name);
        } else if (dev_type == "spi") {
            return SPI_Device::getInstance(dev_name);
        } else if (dev_type == "i2c") {
            return I2C_Device::getInstance(dev_name);
        } else if (dev_type == "uart") {
            return UART_Device::getInstance(dev_name);
        } else if (dev_type == "usart") {
            return USART_Device::getInstance(dev_name);
        } else if (dev_type == "can") {
            return CAN_Device::getInstance(dev_name);
        } else if (dev_type == "ethernet") {
            return Ethernet_Device::getInstance(dev_name);
        } else {
            // handle the case where the device type is unknown
            std::cerr << "Error: unknown device type" << std::endl;
            exit(1);
        }
    }

    void throw_command(){
        ofstream outfile ("command");
        outfile << "&";
        outfile.close();
    }

    void transmit_response(string message){
        ofstream outfile ("command0");
        outfile << message;
        outfile.close();
    }

    void slave_writing(ofstream& com, string message){
        com.open("communication-register", ios::app);
        com << now() << "\t      slave writing: " << message << endl;
        com.close();
    }
}

