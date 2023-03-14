/**
 * @file libdriver.hpp
 * @brief Driver library contains device classes simulating their attributes and behaviors to create virtual devices.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
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
    * @class GPIO_Device
    * @brief Models a GPIO device that is being simulated.
    */
    class GPIO_Device {
    protected:
        /**
        @var fd
        @brief A stream object to control the device file.
        */
        fstream fd;
    private:
        /**
        @var char *dev_name
        @brief Stores the name of the device.
        @var fstream log
        @brief File stream object for logging information.
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[8]
        @brief Array of strings storing information about the device's properties, such as offset, name, consumer, and flags.
        */
        char *dev_name;
        fstream log;
        string defaultDir = "dev/default_GPIO_chipInfo.json";
        int packSize = 8;
        string pack[8] = {"offset", "name", "consumer", "FLAG_IS_OUT", "FLAG_ACTIVE_LOW", "FLAG_OPEN_DRAIN",
                          "FLAG_OPEN_SOURCE", "FLAG_KERNEL"};

    public:
        /**
        * @brief Constructor for the GPIO_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Initializes the `dev_name` member variable with the provided `dev_name` and opens the log file for appending.
        */
        GPIO_Device(char *dev_name);

        char* getDevName() const { return dev_name; }
        fstream& getLog() { return log; }
        fstream& getFd() { return fd; }

        /**
        @brief Opens the device with the specified request (READONLY, WRITEONLY, DEFAULT)
        @param request Enum representing the request type (READONLY, WRITEONLY, DEFAULT)
        @param gpioDevHandler Pointer to the GPIO_Device object
        */
        void device_open(command request, GPIO_Device *gpioDevHandler);
        /**
        * @brief Closes the file stream 'fd' associated with the device.
        * 
        * If the file stream 'fd' is open, it closes the stream and outputs a message indicating that the file stream 
        * is closed successfully. If the file stream 'fd' is not open, it outputs a message indicating that the file 
        * stream is already not open.
        */
        void device_close();
        /**
        * @brief Returns the default directory of the device
        * 
        * @return string The default directory of the device
        */
        virtual string getDefaultDir() { return defaultDir; }
        /**
        * @brief Returns the pack of the device
        * 
        * @return string* Pointer to the pack of the device
        */
        virtual string *getPack() { return pack; }
        /**
        * @brief Returns the size of the pack of the device
        * 
        * @return int The size of the pack of the device
        */
        virtual int getPackSize() { return packSize; }

        friend void parse_GPIO(string dir, GPIO_Device *gpioDevHandler);

        friend void parse_SPI(string dir, GPIO_Device *gpioDevHandler);

        friend void parse_I2C(string dir, GPIO_Device *gpioDevHandler);

        friend void parse_ETHERNET(string dir, GPIO_Device *gpioDevHandler);

        friend void parse_USART(string dir, GPIO_Device *gpioDevHandler);

        friend void parse_UART(string dir, GPIO_Device *gpioDevHandler);

        friend void parse_CAN(string dir, GPIO_Device *gpioDevHandler);
        /**
        * @brief Virtual function to parse data from a given directory
        * 
        * This function uses the parse_DEVICE function related to the class to parse the data from the given directory and store it in the gpioDevHandler object.
        * @param dir The directory where the data to be parsed is located
        * @param gpioDevHandler The GPIO_Device object where the parsed data will be stored
        */
        virtual void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_GPIO(dir, gpioDevHandler);
        }
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
            * @param gpioDevHandler A pointer to a GPIO_Device object.
            * 
            * @return returns "true" if operation is successful.
            */
            string config(command request, GPIO_Device *gpioDevHandler);
            /**
            * @brief function to show the device content.
            * @param gpioDevHandler pointer to GPIO_Device object.
            * @return returns "true" if operation is successful.
            */
            static string show(GPIO_Device *gpioDevHandler);
            /**
            * @brief function to read device content at a given offset.
            * @param offset line number from which data should be read.
            * @param property property to be read.
            * @param gpioDevHandler pointer to GPIO_Device object.
            * @return returns the value of the specified property.
            */
            static string read(int offset, string property, GPIO_Device *gpioDevHandler);
            /**
            * @brief function to manipulate device content at a given offset.
            *
            * This method is used to write the property of a device with a new value.
            *
            * @param offset      An int representing the offset of the property in the device file
            * @param property    A string representing the property of the device to be updated
            * @param new_value   A string representing the new value to be set for the property
            * @param gpioDevHandler  A pointer to a GPIO_Device instance representing the device to be updated
            *
            * @return "true" if write operation is successful, "false" otherwise
            */
            string write(int offset, string property, string new_value, GPIO_Device *gpioDevHandler);

        };

        DeviceContent devContent;

    };
    /**
    * @class SPI_Device
    * @brief Models an SPI device that is being simulated.
    */
    class SPI_Device : public GPIO_Device {
    private:
        /**
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[8]
        @brief Array of strings storing information about the device's properties, such as offset, name, consumer, and flags.
        */
        string defaultDir;
        string pack[9] = {"offset", "name", "consumer", "cpol", "cpha", "lsb_first", "cs_high", "3wire", "loopback"};
        int packSize = 9;

    public:
        string getDefaultDir() { return defaultDir; }

        string *getPack() { return pack; }

        int getPackSize() { return packSize; }
        /**
        * @brief Constructor for the SPI_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `SPI_Device` and initializes the `defaultDir` member variable to "dev/default_SPI_chipInfo.json".
        */
        SPI_Device(char *dev_name) : GPIO_Device(dev_name) {
            this->defaultDir = "dev/default_SPI_chipInfo.json";
        };

        void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_SPI(dir, gpioDevHandler);
        }

    };
    /**
    * @class I2C_Device
    * @brief Models an I2C device that is being simulated.
    */
    class I2C_Device : public GPIO_Device {
    private:
        /**
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[7]
        @brief Array of strings storing information about the device's properties, such as offset, name, consumer or clock_speed.
        */
        string defaultDir;
        string pack[7] = {"offset", "name", "consumer", "clock_speed", "address_mode", "10bit_mode", "sda_hold_time"};
        int packSize = 7;

    public:
        string getDefaultDir() { return defaultDir; }

        string *getPack() { return pack; }

        int getPackSize() { return packSize; }
        /**
        * @brief Constructor for the I2C_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `I2C_Device` and initializes the `defaultDir` member variable to "dev/default_I2C_chipInfo.json".
        */
        I2C_Device(char *dev_name) : GPIO_Device(dev_name) {
            this->defaultDir = "dev/default_I2C_chipInfo.json";
        };

        void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_I2C(dir, gpioDevHandler);
        }
    };
    /**
    * @class ETHERNET_Device
    * @brief Models an ETHERNET device that is being simulated.
    */
    class ETHERNET_Device : public GPIO_Device {
    private:
        /**
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[9]
        @brief Array of strings storing information about the device's properties, such as offset, name, mac_address, ip_address.
        */
        string defaultDir;
        string pack[9] = {"offset", "name", "mac_address", "ip_address", "netmask", "gateway", "vlan_enable", "vlan_id",
                          "link_speed"};
        int packSize = 9;

    public:
        string getDefaultDir() { return defaultDir; }

        string *getPack() { return pack; }

        int getPackSize() { return packSize; }
        /**
        * @brief Constructor for the ETHERNET_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `ETHERNET_Device` and initializes the `defaultDir` member variable to "dev/default_ETHERNET_chipInfo.json".
        */
        ETHERNET_Device(char *dev_name) : GPIO_Device(dev_name) {
            this->defaultDir = "dev/default_ETHERNET_chipInfo.json";
        };

        void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_ETHERNET(dir, gpioDevHandler);
        }
    };
    /**
    * @class USART_Device
    * @brief Models a USART device that is being simulated.
    */
    class USART_Device : public GPIO_Device {
    private:
        /**
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[13]
        @brief Array of strings storing information about the device's properties, such as offset, name, consumer, baud_rate.
        */
        string defaultDir;
        string pack[13] = {"offset", "name", "consumer", "baud_rate", "data_bits", "stop_bits", "parity",
                           "flow_control", "fifo_depth", "synchronous_mode", "clock_polarity", "clock_phase",
                           "clock_rate"};
        int packSize = 13;

    public:
        string getDefaultDir() { return defaultDir; }

        string *getPack() { return pack; }

        int getPackSize() { return packSize; }
        /**
        * @brief Constructor for the USART_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `USART_Device` and initializes the `defaultDir` member variable to "dev/default_USART_chipInfo.json".
        */
        USART_Device(char *dev_name) : GPIO_Device(dev_name) {
            this->defaultDir = "dev/default_USART_chipInfo.json";
        };

        void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_USART(dir, gpioDevHandler);
        }
    };
    /**
    * @class UART_Device
    * @brief Models a UART device that is being simulated.
    */
    class UART_Device : public GPIO_Device {
    private:
        /**
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[9]
        @brief Array of strings storing information about the device's properties, such as offset, name, consumer, baud_rate.
        */
        string defaultDir;
        string pack[9] = {"offset", "name", "consumer", "baud_rate", "data_bits", "stop_bits", "parity", "flow_control",
                          "fifo_depth"};
        int packSize = 9;

    public:
        string getDefaultDir() { return defaultDir; }

        string *getPack() { return pack; }

        int getPackSize() { return packSize; }
        /**
        * @brief Constructor for the UART_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `UART_Device` and initializes the `defaultDir` member variable to "dev/default_UART_chipInfo.json".
        */
        UART_Device(char *dev_name) : GPIO_Device(dev_name) {
            this->defaultDir = "dev/default_UART_chipInfo.json";
        };

        void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_UART(dir, gpioDevHandler);
        }
    };
    /**
    * @class CAN_Device
    * @brief Models a CAN device that is being simulated.
    */
    class CAN_Device : public GPIO_Device {
    private:
        /**
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack array.
        @var string pack[8]
        @brief Array of strings storing information about the device's properties, such as offset, name, consumer, bitrate.
        */
        string defaultDir;
        string pack[8] = {"offset", "name", "consumer", "bitrate", "acceptance_filter", "loopback_mode",
                          "listen_only_mode", "transceiver_type",};
        int packSize = 8;

    public:
        string getDefaultDir() { return defaultDir; }

        string *getPack() { return pack; }

        int getPackSize() { return packSize; }
        /**
        * @brief Constructor for the CAN_Device class
        *
        * @param dev_name A character array representing the device name
        *
        * Calls the constructor of the child class `CAN_Device` and initializes the `defaultDir` member variable to "dev/default_CAN_chipInfo.json".
        */
        CAN_Device(char *dev_name) : GPIO_Device(dev_name) {
            this->defaultDir = "dev/default_CAN_chipInfo.json";
        };

        void parse(string dir, GPIO_Device *gpioDevHandler) {
            parse_CAN(dir, gpioDevHandler);
        }
    };
    
}
#endif //LIB_DRIVER_HPP
