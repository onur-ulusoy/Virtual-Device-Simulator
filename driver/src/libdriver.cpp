/**
 * @file libdriver.cpp
 * @brief Driver library contains device classes simulating their attributes and behaviors to create virtual devices.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023 Reworked: 28/03/2022
 *
 * @section LICENSE
 *
 * Licensed under the MIT License. See LICENSE file in the root directory for details.
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

}
