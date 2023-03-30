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
        @var char *dev_name
        @brief Stores the name of the device.
        @var fstream log
        @brief File stream object for logging information.
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack vector.
        @var vector<string> pack
        @brief Vector of strings storing information about the device's properties, such as offset, name, consumer, and flags.
        */
        char *dev_name;
        string dev_type;
        fstream log;
        string defaultDir;
        int packSize;
        vector<string> pack;

    public:
        /**
        * @brief Constructor for the Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Initializes the `dev_name` member variable with the provided `dev_name` and opens the log file for appending.
        */
        Device(char *dev_name);

        virtual ~Device() = default;
        
        char* getDevName() const { return dev_name; }
        fstream& getLog() { return log; }
        fstream& getFd() { return fd; }

        /**
        @brief Opens the device with the specified request (READONLY, WRITEONLY, DEFAULT)
        @param request Enum representing the request type (READONLY, WRITEONLY, DEFAULT)
        @param devHandler Pointer to the Device object
        */
        virtual void device_open(command request, Device *devHandler);
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
        virtual string getDefaultDir() = 0;
        /**
        * @brief Returns the pack of the device
        * 
        * @return string* Pointer to the pack of the device
        */
        virtual vector<string> getPack() = 0;
        /**
        * @brief Returns the size of the pack of the device
        * 
        * @return int The size of the pack of the device
        */
        virtual int getPackSize() = 0;
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
        vector<string> get_device_keys(const string& device_type);

        /**
        @class DeviceContent
        @brief Includes device beheviours referenced when it is simulated.
        */
        class DeviceContent {
        public:
            /**
            * @brief Configures the device according to the default device information with specified request from JSON file.
            * 
            * If the request is DEFAULT, it opens the device in write mode, retrieves the default configuration directory, 
            * and calls the parse() method on the default directory. The device is then closed. The function returns the 
            * string "true".
            * 
            * @param request The type of request to be processed.
            * @param gpioDevHandler A pointer to a Device object.
            * 
            * @return returns "true" if operation is successful.
            */
            string config(command request, Device *gpioDevHandler);
            /**
            * @brief function to show the device content.
            * @param gpioDevHandler pointer to Device object.
            * @return returns "true" if operation is successful.
            */
            static string show(Device *gpioDevHandler);
            /**
            * @brief function to read device content at a given offset.
            * @param offset line number from which data should be read.
            * @param property property to be read.
            * @param gpioDevHandler pointer to Device object.
            * @return returns the value of the specified property.
            */
            static string read(int offset, string property, Device *gpioDevHandler);
            /**
            * @brief function to manipulate device content at a given offset.
            *
            * This method is used to write the property of a device with a new value.
            *
            * @param offset      An int representing the offset of the property in the device file
            * @param property    A string representing the property of the device to be updated
            * @param new_value   A string representing the new value to be set for the property
            * @param gpioDevHandler  A pointer to a Device instance representing the device to be updated
            *
            * @return "true" if write operation is successful, "false" otherwise
            */
            string write(int offset, string property, string new_value, Device *gpioDevHandler);

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

        static GPIO_Device& getInstance(char* dev_name){
            char* defaultDevName;
            static GPIO_Device instance(defaultDevName);
            if (strlen(dev_name) > 0) {
                instance.dev_name = dev_name;
            }
            return instance;
        }


        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
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
        GPIO_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "gpio";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
    };   

    /**
    * @class SPI_Device
    * @brief Models a SPI device that is being simulated, implements pure virtual class Device.
    */
    class SPI_Device : public Device {

    public:
        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
        /**
        * @brief Constructor for the SPI_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `SPI_Device` and initializes attributes to start device
        */
        SPI_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "spi";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
        
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    };  

    /**
    * @class I2C_Device
    * @brief Models a I2C device that is being simulated, implements pure virtual class Device.
    */
    class I2C_Device : public Device {

    public:
        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
        /**
        * @brief Constructor for the I2C_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `I2C_Device` and initializes attributes to start device
        */
        I2C_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "i2c";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
        
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    };  

    /**
    * @class UART_Device
    * @brief Models a UART device that is being simulated, implements pure virtual class Device.
    */
    class UART_Device : public Device {

    public:
        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
        /**
        * @brief Constructor for the UART_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `UART_Device` and initializes attributes to start device
        */
        UART_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "uart";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
        
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    };  

    /**
    * @class USART_Device
    * @brief Models a USART device that is being simulated, implements pure virtual class Device.
    */
    class USART_Device : public Device {

    public:
        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
        /**
        * @brief Constructor for the USART_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `USART_Device` and initializes attributes to start device
        */
        USART_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "usart";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
        
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    };  

    /**
    * @class Ethernet_Device
    * @brief Models a Ethernet device that is being simulated, implements pure virtual class Device.
    */
    class Ethernet_Device : public Device {

    public:
        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
        /**
        * @brief Constructor for the Ethernet_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `Ethernet_Device` and initializes attributes to start device
        */
        Ethernet_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "ethernet";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
        
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    };  

    /**
    * @class CAN_Device
    * @brief Models a CAN device that is being simulated, implements pure virtual class Device.
    */
    class CAN_Device : public Device {

    public:
        string getDefaultDir() override { return defaultDir; }

        vector<string> getPack() override { return pack; }

        int getPackSize() override { return packSize; }
        /**
        * @brief Constructor for the CAN_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `CAN_Device` and initializes attributes to start device
        */
        CAN_Device(char *dev_name) : Device(dev_name) {
            this->dev_type = "can";
            this->defaultDir = "dev-config/config_json/" + dev_type + "_config.json";
            this->pack = get_device_keys(this->dev_type);
            this->packSize = this->pack.size();
        };
        
        void parse() override {
            parse_device(this->defaultDir, this->fd);
        }

    };  

   
    
}
#endif //LIB_DRIVER_HPP
