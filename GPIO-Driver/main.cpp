#include "gpiolib.h"
#include <unistd.h>
#include <regex>
#include <chrono>
#include <ctime>
#include <iomanip>
//#include <boost/format.hpp>

using namespace std;

//#define DEV_NAME "dev/gpiochip0"

fstream receiver;
string _command;
fstream history;

int receive_command(enum command_request req, string devType);
void throw_command();
string now();

int main() {
    //GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);

    //gpioDevHandler->device_open(READONLY, gpioDevHandler);

    //gpioDevHandler->device_close();

    //gpioDevHandler->devContent.fill(DEFAULT, gpioDevHandler);

    //gpioDevHandler->devContent.show(gpioDevHandler);

    //int GPIO_offset = 0;
    //string GPIO_status = gpioDevHandler->devContent.read(GPIO_offset, FLAG_IS_OUT,  gpioDevHandler);
    //gpioDevHandler->devContent.write(1, FLAG_IS_OUT, "[OUTPUT]", gpioDevHandler);
    //feature inputlar için enum ayarlanabilir write için string input vermektense
    //cout << "GPIO Device " << GPIO_offset << " " << "FLAG_IS_OUT is " << GPIO_status << endl;

    //gpioDevHandler->devContent.show(gpioDevHandler);

    float sleep_time = 0.2;
    //cout << "Build finished."<< endl;
    cout << "program started working."<< endl;

    //usleep(2000000);

    receiver.open("command", ios::in);
    string device;

    receiver >> device;

    cout << device << " device is being simulated." << endl;

    receiver.close();

    receiver.open("command", ios::out);
    receiver << "start";

    receiver.close();

    ofstream hFile;
    hFile.open("history");
    hFile.close();

    usleep(1000000 * sleep_time);

    while (receive_command(ONESHOT, device) != -1) {

        usleep(1000000 * sleep_time);


    }

    cout << "Program is terminating .." << endl;
    return 0;
}

int receive_command(enum command_request req, string devType) {

    if (req == ONESHOT){
        receiver.open("command");
        receiver >> _command;
        //cout << _command << endl;

        receiver.close();
        //cout << _command.length();
    }

    else if (req == RECURSIVE)
        ;

    if (_command == "-1")
        return -1;

    else if (_command == "&"){
        return 0;
    }

    else {
        string delimiter = "-";
        vector<string> substrings;
        stringstream ss(_command);
        string item;

        while (getline(ss, item, delimiter[0])) {
            substrings.push_back(item);
        }

        if (substrings[0] == "show") {

            history.open("history", ios::app);

            history << "Date: " << now() << endl << "Command: " << _command << endl;
            history << "Output: " << endl;

            char* dev_name = const_cast<char *>(("dev/" + substrings[1]).c_str());

            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                gpioDevHandler->devContent.show(gpioDevHandler);
            }
            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                spiDevHandler->devContent.show(spiDevHandler);
            }
            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                i2cDevHandler->devContent.show(i2cDevHandler);
            }
            else if (devType == "ethernet"){
                ETHERNET_Device* ethernetDevHandler = new ETHERNET_Device(dev_name);
                ethernetDevHandler->devContent.show(ethernetDevHandler);
            }
            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                usartDevHandler->devContent.show(usartDevHandler);
            }
            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                uartDevHandler->devContent.show(uartDevHandler);
            }
            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                canDevHandler->devContent.show(canDevHandler);
            }



            history << endl << endl;
            history.close();

        }

        else if (substrings[0] == "read") {

            history.open("history", ios::app);

            history << "Date: " << now() << endl << "Command: " << _command << endl;
            history << "Output: " << endl;

            char* dev_name = const_cast<char *>(("dev/" + substrings[1]).c_str());

            int offset = stoi(substrings[2]);

            enum feature property;

            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                string data = gpioDevHandler->devContent.read(offset, substrings[3], gpioDevHandler);
            }

            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                string data = spiDevHandler->devContent.read(offset, substrings[3], spiDevHandler);
            }

            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                string data = i2cDevHandler->devContent.read(offset, substrings[3], i2cDevHandler);
            }

            else if (devType == "ethernet"){
                ETHERNET_Device* ethDevHandler = new ETHERNET_Device(dev_name);
                string data = ethDevHandler->devContent.read(offset, substrings[3], ethDevHandler);
            }

            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                string data = usartDevHandler->devContent.read(offset, substrings[3], usartDevHandler);
            }

            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                string data = uartDevHandler->devContent.read(offset, substrings[3], uartDevHandler);
            }

            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                string data = canDevHandler->devContent.read(offset, substrings[3], canDevHandler);
            }

            history << endl << endl;
            history.close();

        }

        else if (substrings[0] == "write") {

            history.open("history", ios::app);

            history << "Date: " << now() << endl << "Command: " << _command << endl;
            history << "Output: " << endl;

            char* dev_name = const_cast<char *>(("dev/" + substrings[1]).c_str());
            //cout << dev_name << " * " << endl;


            int offset = stoi(substrings[2]);
            /*for (int i = 0; i < 5; i++) {
                cout << i << " " << substrings[i] << endl;
            }*/
            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                gpioDevHandler->devContent.write(offset, substrings[3], substrings[4], gpioDevHandler);
            }

            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                spiDevHandler->devContent.write(offset, substrings[3], substrings[4], spiDevHandler);            }

            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                i2cDevHandler->devContent.write(offset, substrings[3], substrings[4], i2cDevHandler);            }

            else if (devType == "ethernet"){
                ETHERNET_Device* ethDevHandler = new ETHERNET_Device(dev_name);
                ethDevHandler->devContent.write(offset, substrings[3], substrings[4], ethDevHandler);            }

            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                usartDevHandler->devContent.write(offset, substrings[3], substrings[4], usartDevHandler);            }

            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                uartDevHandler->devContent.write(offset, substrings[3], substrings[4], uartDevHandler);            }

            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                canDevHandler->devContent.write(offset, substrings[3], substrings[4], canDevHandler);            }


            history << endl << endl;
            history.close();
        }

        else if (substrings[0] == "fill") {

            history.open("history", ios::app);

            history << "Date: " << now() << endl << "Command: " << _command << endl;
            history << "Output: " << endl;

            char* dev_name = const_cast<char *>(("dev/" + substrings[1]).c_str());

            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                gpioDevHandler->devContent.fill(DEFAULT, gpioDevHandler);
            }

            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                spiDevHandler->devContent.fill(DEFAULT, spiDevHandler);
            }
            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                i2cDevHandler->devContent.fill(DEFAULT, i2cDevHandler);
            }
            else if (devType == "ethernet"){
                ETHERNET_Device* ethernetDevHandler = new ETHERNET_Device(dev_name);
                ethernetDevHandler->devContent.fill(DEFAULT, ethernetDevHandler);
            }
            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                usartDevHandler->devContent.fill(DEFAULT, usartDevHandler);
            }
            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                uartDevHandler->devContent.fill(DEFAULT, uartDevHandler);
            }
            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                canDevHandler->devContent.fill(DEFAULT, canDevHandler);
            }


            history << endl << endl;
            history.close();

        }
        //.commandSet-commandsText
        else if (substrings[0] == ".commandSet") {

            fstream f;
            f.open(substrings[1]);

            while (true) {
                if(f.eof()) break;
                _command.clear();
                f >> _command;
                if (_command.length()  == 0)
                    break;
                cout << "Command is running: " << _command << endl;
                receive_command(RECURSIVE, devType);
            }

        }

        else {
            cout << "Command is not valid." << endl;
        }

    }
    //cout << "3322" << endl;

    //_command = "&";
    //usleep(1000000 * 3);
    throw_command();
    //usleep(10000000);
    //exit(0);
    return 0;
}

void throw_command(){
    ofstream outfile ("command");
    outfile << "&";
    outfile.close();
}

string now(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *tm = std::localtime(&now_c);

    // Create a stringstream object
    std::stringstream ss;

    // Use the stringstream's operator << to format the time
    ss << std::put_time(tm, "%c");

    // Get the string from the stringstream
    return ss.str();
}



