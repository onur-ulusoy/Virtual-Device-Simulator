/**
 * @file libdriver.hpp
 * @brief Driver library contains command operation functions and device classes simulating their attributes and behaviors to create virtual devices.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Reworked: 28/03/2023, 18/05/2023
 * 
 * @section LICENSE
 * Licensed under the MIT License.
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
        @var string devName
        @brief Stores the name of the device.
        @var string defaultDir
        @brief The directory path to the device default information file.
        @var int packSize
        @brief The number of elements in the pack vector.
        @var vector<string> pack
        @brief Vector of strings storing information about the device's properties, such as offset, name, consumer, and flags.
        @var devIsOpen
        @brief Boolean value specifies whether the device is open or not.
        */
        string devName;
        string devType;
        string defaultDir;
        int packSize;
        vector<string> pack;
        bool devIsOpen;
        /**
        * @brief Constructor for the Device class
        *
        * @param devName A character array representing the device name
        *
        * Initializes the object with `devName`.
        */
        Device(string devName) : devContent(this) {
            devIsOpen = false;
        }
        /**
         * @brief Virtual destructor for the Device class. Uses default behavior.
         */
        virtual ~Device() = default; 
        /**
         * @var devHandler 
         * @brief Pointer to the Device object for the base class itself
         */
        Device* devHandler = this;

    public:
        /**
         * @brief Retrieves the device's name.
         * @return const string - A string representing the device name.
         */
        const string getDevName() const { return devName; }
        /**
         * @brief Provides access to the device's file descriptor.
         * @return fstream& - A reference to the fstream object representing the file descriptor.
         */
        fstream& getFd() { return fd; }
        /**
        @brief Opens the device with the specified request (READONLY, WRITEONLY, DEFAULT)
        @param request Enum representing the request type (READONLY, WRITEONLY, DEFAULT)
        */
        virtual void openDevice(command request);
        /**
        * @brief Closes the file stream 'fd' associated with the device.
        * 
        * If the file stream 'fd' is open, it closes the stream and outputs a message indicating that the file stream 
        * is closed successfully. If the file stream 'fd' is not open, it outputs a message indicating that the file 
        * stream is already not open.
        */
        virtual void closeDevice();
        /**
        * @brief Returns the default directory of the device
        * 
        * @return string The default directory of the device
        */
        virtual const string getDefaultDir() const { return defaultDir; }
        /**
        * @brief Returns the pack of the device
        * 
        * @return string* Pointer to the pack of the device
        */
        virtual vector<string> getPack() const { return pack; }
        /**
        * @brief Returns the size of the pack of the device
        * 
        * @return int The size of the pack of the device
        */
        virtual const int getPackSize() const { return packSize; }
        /**
        * @brief Sets the device name for the Device object.
        * @param devName - A string representing the device name to set.
        */
        virtual void setDevName(const string devName);
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
         * @param outputFile The file to write the parsed device information to.
         */
        void parseDevice(const std::string& dir, std::fstream& outputFile);
        /**
         * @brief Returns the keys of a device as a vector of strings in the order specified in the JSON config file
         *
         * @param deviceType The type of the device (e.g. "ethernet", "spi", "i2c")
         * @return std::vector<string> The keys of the device in the order specified in the JSON config file
         */
        vector<string> getDeviceKeys (const string& deviceType);
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
            * @param newValue   A string representing the new value to be set for the property            
            * @return "true" if write operation is successful, "false" otherwise
            */
            string write(const int offset, const string property, const string newValue);
        
        private:
            /**
             * @var devHandler 
             * @brief Pointer to the Device object for child class to be able to access to the base class
             */
            Device* devHandler;
        };
        /**
         * @var devContent
         * @brief Object to the DeviceContent belongs to the main class to use main behaivors of the device
         */
        DeviceContent devContent;

        private:
            /**
             * @brief Copy constructor and copy assignment operator are deleted to prevent copying of Device objects.
             *
             * Copying of Device objects is not allowed because each device should be uniquely identified by its name and ID.
             * Copying a Device object would result in two instances with the same name and ID, which is not allowed.
             * Therefore, the copy constructor and copy assignment operator are deleted.
             */
            Device(const Device&) = delete;
            Device& operator=(const Device&) = delete;
    };
    /**
     * @brief Template class to implement the Singleton design pattern for any type T.
     *
     * @tparam T The type for which a single instance is required of. Can be any device class (Ex: GPIO_Device).
     */
    template <typename T>
    class Singleton {
    public:
        /**
         * @brief Gets the single instance of the type T.
         *
         * If the input `devName` is non-empty, it sets the device name for the instance.
         *
         * @param devName A string representing the device name. If not provided or empty, the device name is not modified.
         * @return T& A reference to the singleton instance of type T.
         */
        static T& getInstance(const string& devName){
            static T instance(devName);
            if (!devName.empty()) {
                instance.setDevName(devName);
            }
            return instance;
        }

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    private:
        /**
         * @brief The copy constructor of the Singleton class is deleted to prevent copying.
         */
        Singleton(const Singleton&) = delete;

        /**
         * @brief The assignment operator of the Singleton class is deleted to prevent copying.
         * @return Singleton&
         */
        Singleton& operator=(const Singleton&) = delete;
    };

    using json = nlohmann::json;

    /**
    * @class GPIO_Device    
    * @brief Models a GPIO device that is being simulated, implements pure virtual class Device.
    */
    class GPIO_Device : public Device, public Singleton<GPIO_Device> {

    public:
        friend class Singleton<GPIO_Device>;
        
        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the GPIO_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `GPIO_Device` and initializes the main attributes.
        */
        GPIO_Device(string devName) : Device(devName) {
            this->devType = "gpio";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
        
    };   

    /**
    * @class SPI_Device
    * @brief Models an SPI device that is being simulated, implements pure virtual class Device.
    */
    class SPI_Device : public Device, public Singleton<SPI_Device>{

    public:
        friend class Singleton<SPI_Device>;
        
        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the SPI_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `SPI_Device` and initializes the the main attributes
        */
        SPI_Device(string devName) : Device(devName) {
            this->devType = "spi";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
    };  

    /**
    * @class I2C_Device
    * @brief Models a I2C device that is being simulated, implements pure virtual class Device.
    */
    class I2C_Device : public Device, public Singleton<I2C_Device> {

    public:
        friend class Singleton<I2C_Device>;
        
        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the I2C_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `I2C_Device` and initializes the main attributes
        */
        I2C_Device(string devName) : Device(devName) {
            this->devType = "I2C";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
    };  

    /**
    * @class UART_Device
    * @brief Models a UART device that is being simulated, implements pure virtual class Device.
    */
    class UART_Device : public Device, public Singleton<UART_Device> {

    public:
        friend class Singleton<UART_Device>;

        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the UART_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `UART_Device` and initializes the main attributes.
        */
        UART_Device(string devName) : Device(devName) {
            this->devType = "UART";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
    };  

    /**
    * @class USART_Device
    * @brief Models a USART device that is being simulated, implements pure virtual class Device.
    */
    class USART_Device : public Device, public Singleton<USART_Device> {

    public:
        friend class Singleton<USART_Device>;
        
        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the USART_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `USART_Device` and initializes the main attributes.
        */
        USART_Device(string devName) : Device(devName) {
            this->devType = "USART";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
    };  

    /**
    * @class CAN_Device
    * @brief Models a CAN device that is being simulated, implements pure virtual class Device.
    */
    class CAN_Device : public Device, public Singleton<CAN_Device> {

    public:
        friend class Singleton<CAN_Device>;
        
        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the CAN_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `CAN_Device` and initializes the main attributes.
        */
        CAN_Device(string devName) : Device(devName) {
            this->devType = "CAN";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
    };  

    /**
    * @class Ethernet_Device
    * @brief Models a Ethernet device that is being simulated, implements pure virtual class Device.
    */
    class Ethernet_Device : public Device, public Singleton<Ethernet_Device> {

    public:
        friend class Singleton<Ethernet_Device>;
        
        void parse() override {
            parseDevice(this->defaultDir, this->fd);
        }

    private:
        /**
        * @brief Constructor for the Ethernet_Device class
        *
        * @param devName A character array representing the device name
        *
        * Calls the constructor of the child class `Ethernet_Device` and initializes the main attributes.
        */
        Ethernet_Device(string devName) : Device(devName) {
            this->devType = "Ethernet";
            this->defaultDir = "dev-config/config_json/" + devType + "_config.json";
            this->pack = getDeviceKeys(this->devType);
            this->packSize = this->pack.size();
        };
    };  
    /**
     * @brief Executes a command for a specific device.
     *
     * This function executes a command for a specific device based on the provided parameters. Currently, this function is not complete
     * and will be completed in future updates.
     *
     * @param request The type of command request.
     * @param dev_type The type of device.
     * @param command The command to be executed.
     * @param register_file The file to store register information.
     * @return The response of the driver after executing the command.
     *
     * @todo Complete the implementation of this function in future updates.
     */
    string execute_command(const enum command_request request, const string dev_type, const string command, ofstream& register_file);
    /**
     * @brief Creates a device object based on the device type and name.
     *
     * This function creates and returns the appropriate device object based on the provided device type and name. It handles various device types
     * such as GPIO, SPI, I2C, UART, USART, CAN, and Ethernet. If the device type is unknown, an error message is displayed, and the program exits.
     *
     * @param dev_type The type of the device.
     * @param dev_name The name of the device.
     * @return The created device object.
     *
     * @throws std::runtime_error if the device type is unknown.
     */
    Device& create_device(const std::string& dev_type, const std::string& dev_name);
}
#endif //LIB_DRIVER_HPP
