/**
 * @file libdriver.hpp
 * @brief Driver library contains device classes simulating their attributes and behaviors to create virtual devices.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023 Reworked: 28/03/2022
 */
#ifndef LIB_DRIVER_HPP
#define LIB_DRIVER_HPP

#include <cstring>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "libdriverutility.hpp"
/**
@brief Macro to specify the directory of temporary file while device is being manipulated.
*/
#define NEW_FILE "dev/newfile"

using namespace std;

namespace DeviceSim {

    /**
    @enum command_request 
    @brief Enumeration of command requests for processing the received command.
    @var ONESHOT
        Process a command only once
    @var RECURSIVE
        Process the commands recursively that have been read from commands text
    */
    enum command_request {
        ONESHOT, 
        RECURSIVE
    };
    /**
    @enum command 
    @brief Lists different modes to open a device.

    @var DEFAULT
        Open device for both read and write operations.
    @var READONLY
        Open device for read-only operations.
    @var WRITEONLY
        Open device for write-only operations.
    */
    enum command {
        DEFAULT, READONLY, WRITEONLY
    };

    /**
    * @class Device 
    * @brief Pure virtual class to interface devices that is being simulated.
    */
    class Device {
    protected:
        /**
        @var fd
        @brief A stream object to control the device file.
        */
        fstream fd;
        
        /**
        @var string dev_name
        @brief Stores the name of the device.
        @var fstream log
        @brief File stream object for logging information.
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack vector.
        @var vector<string> pack
        @brief Vector of strings storing information about the device's properties, such as offset, name, consumer, and flags.
        @var dev_is_open
        @brief Vector of strings storing information about the device's properties, such as offset, name, consumer, and flags.
        */
        string dev_name;
        string dev_type;
        fstream log;
        string defaultDir;
        int packSize;
        vector<string> pack;
        bool dev_is_open;

    public:
        /**
         * @var devHandler 
         * @brief Pointer to the Device object
         */
        Device* devHandler = this;
        /**
        * @brief Constructor for the Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Initializes the `dev_name` member variable with the provided `dev_name` and opens the log file for appending.
        */
        Device(string dev_name) : devContent(this) {
            log.open("log", ios::app);
            dev_is_open = false;
        }
        
        virtual ~Device() = default;
        
        const string getDevName() const { return dev_name; }
        fstream& getLog() { return log; }
        fstream& getFd() { return fd; }

        /**
        @brief Opens the device with the specified request (READONLY, WRITEONLY, DEFAULT)
        @param request Enum representing the request type (READONLY, WRITEONLY, DEFAULT)
        */
        virtual void device_open(command request);
        /**
        * @brief Closes the file stream 'fd' associated with the device.
        * 
        * If the file stream 'fd' is open, it closes the stream and outputs a message indicating that the file stream 
        * is closed successfully. If the file stream 'fd' is not open, it outputs a message indicating that the file 
        * stream is already not open.
        */
        virtual void device_close();
        /**
        * @brief Returns the default directory of the device
        * 
        * @return string The default directory of the device
        */
        virtual const string getDefaultDir() const = 0;
        /**
        * @brief Returns the pack of the device
        * 
        * @return string* Pointer to the pack of the device
        */
        virtual vector<string> getPack() const = 0;
        /**
        * @brief Returns the size of the pack of the device
        * 
        * @return int The size of the pack of the device
        */
        virtual const int getPackSize() const = 0;
        /**
        * @brief Virtual function to parse data from a given directory
        * 
        * This function uses the function related to the class to parse the data from the given directory and store it in the devHandler object.
        */
        virtual void parse() = 0;
        /**
         * @brief Parses a JSON device file and outputs the device information to a file
         * in the order of the keys specified in the file.
         *
         * @param dir The directory of the JSON device file to parse.
         * @param output_file The file to write the parsed device information to.
         */
        void parse_device(const std::string& dir, std::fstream& output_file);
        /**
         * @brief Returns the keys of a device as a vector of strings in the order specified in the JSON config file
         *
         * @param device_type The type of the device (e.g. "ethernet", "spi", "i2c")
         * @return std::vector<string> The keys of the device in the order specified in the JSON config file
         */
        vector<string> get_device_keys (const string& device_type);
        /**
        @class DeviceContent
        @brief Includes device beheviours referenced when it is simulated.
        */
        class DeviceContent {
        public:
            DeviceContent(Device* devHandler) : devHandler(devHandler) {}
            /**
            * @brief Configures the device according to the default device information with specified request from JSON file.
            * 
            * If the request is DEFAULT, it opens the device in write mode, retrieves the default configuration directory, 
            * and calls the parse() method on the default directory. The device is then closed. The function returns the 
            * string "true".
            * 
            * @param request The type of request to be processed.       
            * @return returns "true" if operation is successful.
            */
            string config(command request);
            /**
            * @brief function to show the device content.
            * @return returns "true" if operation is successful.
            */
            string show();
            /**
            * @brief function to read device content at a given offset.
            * @param offset line number from which data should be read.
            * @param property property to be read.
            * @return returns the value of the specified property.
            */
            string read(const int offset, const string property);
            /**
            * @brief function to manipulate device content at a given offset.
            *
            * This method is used to write the property of a device with a new value.
            *
            * @param offset      An int representing the offset of the property in the device file
            * @param property    A string representing the property of the device to be updated
            * @param new_value   A string representing the new value to be set for the property            
            * @return "true" if write operation is successful, "false" otherwise
            */
            string write(const int offset, const string property, const string new_value);
        
        private:
            Device* devHandler;
        };
        
        DeviceContent devContent;
    };

    using json = nlohmann::json;

    /**
    * @class GPIO_Device
    * @brief Models a GPIO device that is being simulated, implements pure virtual class Device.
    */
    class GPIO_Device : public Device {

    public:

        static GPIO_Device& getInstance(string dev_name);
        
        const string getDefaultDir() const override { return defaultDir; }

        vector<string> getPack() const override { return pack; }

        int const getPackSize() const override { return packSize; }
        /**
        * @brief Constructor for the SPI_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `SPI_Device` and initializes the `defaultDir` member variable to "dev/default_SPI_chipInfo.json".
        */
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    private:
        GPIO_Device(string dev_name) : Device(dev_name) {
            this->dev_type = "gpio";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
    };   
    
}
#endif //LIB_DRIVER_HPP
