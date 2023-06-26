/**
 * @file libdriver.cpp
 * @brief Driver library contains command operation functions and device classes simulating their attributes and behaviors to create virtual devices.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Reworked: 28/03/2023, 18/05/2023
 * 
 * @section LICENSE
 * Licensed under the MIT License.
 */
#include "libdriver.hpp"

namespace DeviceSim {

    void Device::setDevName(const string devName){
        this->devName = devName;
    }

    void Device::openDevice(command request) {
        if (devIsOpen){
            closeDevice();
        }

        switch (request) {

            case READONLY:
                cout << "function 'Device::openDevice' worked as READONLY" << endl;

                fd.open(devName, ios::in);

                if (!fd.is_open()) {
                    Device::openDevice(WRITEONLY);

                    devContent.config(DEFAULT);

                    Device::closeDevice();
                    Device::openDevice(READONLY);
                    break;

                }

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << devName << ": " << std::strerror(errno) << std::endl;
                    break;
                } 
                
                else
                    cout << devName << " is opened successfully as READONLY" << endl;

                devIsOpen = true;
                cout << endl;
                break;

            case WRITEONLY:
                cout << "function 'Device::openDevice' worked as WRITEONLY" << endl;
                fd.open(devName, ios::out | ios::trunc);

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << devName << ": " << std::strerror(errno) << std::endl;
                    break;
                } 
                
                else
                    cout << devName << " is opened successfully as WRITEONLY" << endl;

                devIsOpen = true;
                cout << endl;
                break;

            default:
                cout << "function 'Device::openDevice' worked as DEFAULT" << endl << endl;
                fd.open(devHandler->devName);

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << devName << ": " << std::strerror(errno) << std::endl;
                    openDevice(WRITEONLY);
                } else
                    cout << devName << " is opened successfully as DEFAULT" << endl;

                devIsOpen = true;
                cout << endl;
                break;

            }
        
    }

    void Device::closeDevice() {

        cout << "function 'Device::closeDevice' worked" << endl;

        if (fd.is_open()) {
            cout << devName << " is closed successfully" << endl;
            fd.close();
        } else 
            cout << devName << " is already not open" << endl;

        devIsOpen = false;
        cout << endl;
    }

    string Device::DeviceContent::config(command request) {

        string dir = devHandler->getDefaultDir();

        cout << "function 'Device::DeviceContent::config' worked" << endl;
        devHandler->openDevice(WRITEONLY);

        if (request == DEFAULT) {
            cout << dir << endl;
            devHandler->parse();
        }

        devHandler->closeDevice();
        return "true";
    }

    string Device::DeviceContent::show() {
        ofstream temp;
        temp.open("temp", ios::app);

        cout << "function 'Device::DeviceContent::show' worked" << endl << endl;
        
        string dir = devHandler->getDefaultDir();

        vector<string> pack = devHandler->getPack();
        int packSize = devHandler->getPackSize();
        //cout<< "Length of pack:" << devHandler->getPackSize() << endl;

        devHandler->openDevice(READONLY);

        //cout << "Chip info is being shown ..." << endl << endl;

        // Get the maximum length of each column
        int max_length[packSize];
        for (int i = 0; i < packSize; i++)
            max_length[i] = 0;

        int Length[packSize];

        while (true) {
            string line, word;
            for (int i = 0; i < packSize; i++) {
                devHandler->getFd() >> word;
                line += word + "/";
            }

            if (devHandler->getFd().eof()) break;

            auto *buffers = new string[packSize + 1];
            Split(line, "/", buffers);

            for (int i = 0; i < packSize; i++) {
                Length[i] = buffers[i].length();
                if (Length[i] > max_length[i]) max_length[i] = Length[i];
            }
            delete[] buffers;
        }

        devHandler->getFd().clear();
        devHandler->getFd().seekg(0, ios::beg);

        //cout << "Data was shown for '" << devHandler->devName << "':" << endl;
        while (true) {
            string line, word;
            for (int i = 0; i < packSize; i++) {
                devHandler->getFd() >> word;
                line += word + "/";
            }

            if (devHandler->getFd().eof()) break;

            auto *buffers = new string[packSize + 1];
            Split(line, "/", buffers);


            for (int i = 0; i < packSize; i++) {
                /*cout << pack[i] << ": ";
                cout << setw(max_length[i]) << left << buffers[i] << "   ";*/

                temp << pack[i] << ": ";
                temp << setw(max_length[i]) << left << buffers[i] << "   ";

            }

            //cout << endl;
            temp << endl;
            delete[] buffers;
        }


        return "true";
    }

    string Device::DeviceContent::read(const int offset, const string property) {

        cout << "function 'Device::DeviceContent::read' worked" << endl << endl;

        string dir = devHandler->getDefaultDir();

        vector<string> pack = devHandler->getPack();
        int packSize = devHandler->getPackSize();

        /**
         * Check whether the property is exist in the device or not
         */
        int request = -1;
        for (int i = 0; i < packSize; i++) {
            if (property == pack[i]) {
                request = i;
                break;
            }
        }

        if (request == -1)
            return "false";

        devHandler->openDevice(DEFAULT);

        GotoLine(devHandler->getFd(), offset + 1);

        string word, empty;

        for (int i = 0; i < request; i++) {
            devHandler->getFd() >> empty;
        }
        devHandler->getFd() >> word;

        devHandler->closeDevice();
        return word;
    }

    string Device::DeviceContent::write(const int offset, const string property, const string newValue) {

        cout << "function 'Device::DeviceContent::write' worked" << endl << endl;

        string dir = devHandler->getDefaultDir();

        vector<string> pack = devHandler->getPack();
        int packSize = devHandler->getPackSize();

        int request = -1;

        for (int i = 0; i < packSize; i++) {
            if (property == pack[i]) {
                request = i;
                break;
            }
        }
        if (request == -1) 
            return "false";

        devHandler->openDevice(DEFAULT);
        GotoLine(devHandler->getFd(), offset + 1);

        string line;
        getline(devHandler->getFd(), line);
        
        auto *buffers = new string[packSize + 1];
            
        Split(line, " ", buffers);
        
        string new_line;

        for (int i = 0; i < packSize; i++) {

            if (i == request) {
                new_line += newValue + " ";
            } else
                new_line += buffers[i] + " ";
        }
        rtrim(new_line);

        GotoLine(devHandler->getFd(), 1);

        fstream newfile;
        ofstream outfile(NEW_FILE);
        outfile.close();

        newfile.open(NEW_FILE);
        int i = 0;

        while (true) {
            if (i != offset) {
                getline(devHandler->getFd(), line);
                newfile << line;
            } else {
                newfile << new_line;
                GotoLine(devHandler->getFd(), i + 2);
            }

            i++;

            if (!devHandler->getFd().eof()) {
                newfile << '\n'; // Write a newline only if it's not the last line
            } else {
                break;
            }
        }

        newfile.close();

        devHandler->closeDevice();

        remove(devHandler->devName.c_str());
        rename(NEW_FILE, devHandler->devName.c_str());

        return "true";
    }
    
    void Device::parseDevice(const string& dir, fstream& outputFile) {
        // Read the JSON file
        ifstream input_file(dir);

        // Parse the JSON file
        json json_data;
        input_file >> json_data;

        // Get the "Devices" array from the JSON
        json devices_array = json_data["Devices"];
        json keys = json_data["keys"];

        // Iterate over the "Devices" array
        for (const auto& device : devices_array) {
            // Iterate over the keys in order
            size_t key_count = keys.size();
            for (size_t i = 0; i < key_count; i++) {
                const auto& key = keys[i];
                // Write the value for the key to the output file, followed by a space
                auto it = device.find(key.get<std::string>());
                if (it != device.end()) {
                    json value = it.value();
                    if (value.is_string()) {
                        outputFile << value.get<std::string>();
                    } else {
                        outputFile << value;
                    }
                } else {
                    outputFile << "-";
                }
                
                if (i != key_count - 1) {
                    outputFile << " "; // Add a space only if it's not the last key
                }
            }
            // Write a newline after processing each device
            outputFile << std::endl;
        }
    }

    vector<string> Device::getDeviceKeys(const string& deviceType) {
        string json_config_file = "dev-config/config_json/" + deviceType + "_config.json";

        // Read the JSON file
        ifstream input_file(json_config_file);
        json json_data;
        input_file >> json_data;

        // Get the keys array from the JSON
        json keys_array = json_data["keys"];

        // Convert the keys to a vector of strings and return them
        vector<string> keys;
        for (const auto& key : keys_array) {
            keys.push_back(key);
        }
        return keys;
    }

    string execute_command(const enum command_request request_type, const string dev_type, const string command, ofstream& register_file) {

        string driver_response = "failure";
        
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
                driver_response = "success";
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
}
