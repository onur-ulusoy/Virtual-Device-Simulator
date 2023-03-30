/**
 * @file libdriver.cpp
 * @brief Driver library contains device classes simulating their attributes and behaviors to create virtual devices.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#include "libdriver.hpp"

namespace DeviceSim {

    GPIO_Device& GPIO_Device::getInstance(string dev_name){
        static GPIO_Device instance("");
        if (dev_name.length() > 0) {
            instance.dev_name = dev_name;
        }
        return instance;
    }
       
    // Device::Device(string dev_name) {
    //     // this->dev_name = new char[strlen(dev_name) + 1];
    //     // strcpy(this->dev_name, dev_name);
    //     log.open("log", ios::app);
    // }

    void Device::device_open(command request) {

        switch (request) {

            case READONLY:
                cout << "function 'Device::device_open' worked as READONLY" << endl;


                fd.open(dev_name, ios::in);
                //cout << fd.is_open() << endl;

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
                } else
                    cout << dev_name << " is opened successfully as READONLY" << endl;

                cout << endl;
                break;

            case WRITEONLY:
                cout << "function 'Device::device_open' worked as WRITEONLY" << endl;
                fd.open(dev_name, ios::out | ios::trunc);

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << dev_name << ": " << std::strerror(errno) << std::endl;
                    break;
                } else
                    cout << dev_name << " is opened successfully as WRITEONLY" << endl;

                cout << endl;
                break;

            default:
                cout << "function 'Device::device_open' worked as DEFAULT" << endl << endl;
                //device_close();
                fd.open(devHandler->dev_name);

                if (!fd.is_open()) {
                    std::cerr << "Unable to open " << dev_name << ": " << std::strerror(errno) << std::endl;
                    device_open(WRITEONLY);
                } else
                    cout << dev_name << " is opened successfully as DEFAULT" << endl;

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

        cout << endl;
    }

    string Device::DeviceContent::config(command request) {

        string dir = devHandler->getDefaultDir();

        cout << "function 'Device::DeviceContent::config' worked" << endl;
        devHandler->device_open(WRITEONLY);

        if (request == DEFAULT) {
            cout << dir << endl;

            //devHandler->parse(dir, devHandler);

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

    string Device::DeviceContent::read(int offset, string property) {

        cout << "function 'Device::DeviceContent::read' worked" << endl << endl;

        string dir = devHandler->getDefaultDir();

        vector<string> pack = devHandler->getPack();
        int packSize = devHandler->getPackSize();

        int request = -2;

        for (int i = 0; i < packSize; i++) {
            if (property == pack[i]) {
                request = i;
                break;
            }
        }

        if (request == -2) {
            return "false";
        }

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

    string Device::DeviceContent::write(int offset, string property, string new_value) {

        //cout << devHandler->dev_name << endl;
        cout << "function 'Device::DeviceContent::write' worked" << endl << endl;

        string dir = devHandler->getDefaultDir();

        vector<string> pack = devHandler->getPack();
        int packSize = devHandler->getPackSize();

        int request = -2;

        for (int i = 0; i < packSize; i++) {
            if (property == pack[i]) {
                request = i;
                break;
            }
        }
        if (request == -2) {
            return "false";
            //return 0;
        }
        //cout << devHandler->dev_name << endl;
        devHandler->device_open(DEFAULT);

        GotoLine(devHandler->getFd(), offset + 1);

        string line;
        getline(devHandler->getFd(), line);
        //cout << line << endl;

        auto *buffers = new string[packSize];
        Split(line, " ", buffers);

        string new_line;

        for (int i = 0; i < packSize; i++) {

            if (i == request) {
                new_line += new_value + " ";
            } else
                new_line += buffers[i] + " ";
        }
        rtrim(new_line);
        //cout << new_line << endl << endl;


        GotoLine(devHandler->getFd(), 1);

        fstream newfile;
        ofstream outfile(NEW_FILE);
        outfile.close();

        newfile.open(NEW_FILE);
        int i = 0;

        while (true) {

            if (i != offset) {
                getline(devHandler->getFd(), line);
                newfile << line << endl;
            } else {
                newfile << new_line << endl;
                GotoLine(devHandler->getFd(), i + 2);
            }

            i++;
            if (devHandler->getFd().eof()) break;
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
            for (const auto& key : keys) {
                // Write the value for the key to the output file, followed by a space
                auto it = device.find(key.get<std::string>());
                if (it != device.end()) {
                    json value = it.value();
                    if (value.is_string()) {
                        output_file << value.get<std::string>() << " ";
                    } else {
                        output_file << value << " ";
                    }
                } else {
                    output_file << "- ";
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