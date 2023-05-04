/**
 * @file libdriver.hpp
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

    void Device::setDevName(const string dev_name){
        this->dev_name = dev_name;
    }

    void Device::device_open(command request) {
        if (dev_is_open){
            device_close();
        }

        switch (request) {

            case READONLY:
                cout << "function 'Device::device_open' worked as READONLY" << endl;

                fd.open(dev_name, ios::in);

                if (!fd.is_open()) {
                    Device::device_open(WRITEONLY);

                    devContent.config(DEFAULT);

                    Device::device_close();
                    Device::device_open(READONLY);
                    break;

                }

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << dev_name << ": " << std::strerror(errno) << std::endl;
                    break;
                } 
                
                else
                    cout << dev_name << " is opened successfully as READONLY" << endl;

                dev_is_open = true;
                cout << endl;
                break;

            case WRITEONLY:
                cout << "function 'Device::device_open' worked as WRITEONLY" << endl;
                fd.open(dev_name, ios::out | ios::trunc);

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << dev_name << ": " << std::strerror(errno) << std::endl;
                    break;
                } 
                
                else
                    cout << dev_name << " is opened successfully as WRITEONLY" << endl;

                dev_is_open = true;
                cout << endl;
                break;

            default:
                cout << "function 'Device::device_open' worked as DEFAULT" << endl << endl;
                fd.open(devHandler->dev_name);

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << dev_name << ": " << std::strerror(errno) << std::endl;
                    device_open(WRITEONLY);
                } else
                    cout << dev_name << " is opened successfully as DEFAULT" << endl;

                dev_is_open = true;
                cout << endl;
                break;

            }
        
    }

    void Device::device_close() {

        cout << "function 'Device::device_close' worked" << endl;

        if (fd.is_open()) {
            cout << dev_name << " is closed successfully" << endl;
            fd.close();
        } else 
            cout << dev_name << " is already not open" << endl;

        dev_is_open = false;
        cout << endl;
    }

    string Device::DeviceContent::config(command request) {

        string dir = devHandler->getDefaultDir();

        cout << "function 'Device::DeviceContent::config' worked" << endl;
        devHandler->device_open(WRITEONLY);

        if (request == DEFAULT) {
            cout << dir << endl;
            devHandler->parse();
        }

        devHandler->device_close();
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

        devHandler->device_open(READONLY);

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

        //cout << "Data was shown for '" << devHandler->dev_name << "':" << endl;
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

        devHandler->device_open(DEFAULT);

        GotoLine(devHandler->getFd(), offset + 1);

        string word, empty;

        for (int i = 0; i < request; i++) {
            devHandler->getFd() >> empty;
        }
        devHandler->getFd() >> word;

        devHandler->device_close();
        return word;
    }

    string Device::DeviceContent::write(const int offset, const string property, const string new_value) {

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

        devHandler->device_open(DEFAULT);
        GotoLine(devHandler->getFd(), offset + 1);

        string line;
        getline(devHandler->getFd(), line);
        
        auto *buffers = new string[packSize + 1];
            
        Split(line, " ", buffers);
        
        string new_line;

        for (int i = 0; i < packSize; i++) {

            if (i == request) {
                new_line += new_value + " ";
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

        devHandler->device_close();

        remove(devHandler->dev_name.c_str());
        rename(NEW_FILE, devHandler->dev_name.c_str());

        return "true";
    }
    
    void Device::parse_device(const string& dir, fstream& output_file) {
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
                        output_file << value.get<std::string>();
                    } else {
                        output_file << value;
                    }
                } else {
                    output_file << "-";
                }
                
                if (i != key_count - 1) {
                    output_file << " "; // Add a space only if it's not the last key
                }
            }
            // Write a newline after processing each device
            output_file << std::endl;
        }
    }


    vector<string> Device::get_device_keys(const string& device_type) {
        string json_config_file = "dev-config/config_json/" + device_type + "_config.json";

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
