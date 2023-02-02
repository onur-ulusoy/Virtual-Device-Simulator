#ifndef DRIVER_SIMULATOR_H
#define DRIVER_SIMULATOR_H

#include <cstring>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "AuxiliaryTools.h"

#define NEW_FILE "dev/newfile"

using namespace std;

enum command {DEFAULT, READONLY, WRITEONLY};
enum command_request {ONESHOT, RECURSIVE};

class GPIO_Device{
protected:
    fstream fd;
private:
    char *dev_name;
    fstream log;
    string defaultDir = "dev/default_GPIO_chipInfo.json";
    int packSize = 8;
    string pack[8] = {"offset", "name", "consumer", "FLAG_IS_OUT", "FLAG_ACTIVE_LOW", "FLAG_OPEN_DRAIN", "FLAG_OPEN_SOURCE", "FLAG_KERNEL"};

public:
    GPIO_Device(char* dev_name);
    void device_open(command request, GPIO_Device* gpioDevHandler);
    void device_close();
    virtual string getDefaultDir() { return defaultDir; }
    virtual string* getPack(){ return pack; }
    virtual int getPackSize(){ return packSize; }
    //virtual char* getDevName() { return dev_name; }

    friend void parse_GPIO(string dir, GPIO_Device* gpioDevHandler);
    friend void parse_SPI(string dir, GPIO_Device* gpioDevHandler);
    friend void parse_I2C(string dir, GPIO_Device* gpioDevHandler);
    friend void parse_ETHERNET(string dir, GPIO_Device* gpioDevHandler);
    friend void parse_USART(string dir, GPIO_Device* gpioDevHandler);
    friend void parse_UART(string dir, GPIO_Device* gpioDevHandler);
    friend void parse_CAN(string dir, GPIO_Device* gpioDevHandler);


    virtual void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_GPIO(dir, gpioDevHandler);
    }

    class DeviceContent {
    public:
        string config(command request, GPIO_Device* gpioDevHandler);
        static string show (GPIO_Device* gpioDevHandler);
        static string read(int offset, string property, GPIO_Device* gpioDevHandler);
        string write (int offset, string property, string new_value, GPIO_Device* gpioDevHandler);

    };

    DeviceContent devContent;

};

class SPI_Device : public GPIO_Device {
private:
    string defaultDir;
    string pack[9] = {"offset", "name", "consumer", "cpol", "cpha", "lsb_first", "cs_high", "3wire", "loopback"};
    int packSize = 9;

public:
    string getDefaultDir() { return defaultDir; }
    string* getPack(){ return pack; }
    int getPackSize(){ return packSize; }

    SPI_Device(char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_SPI_chipInfo.json";
    };

    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_SPI(dir, gpioDevHandler);
    }

};

class I2C_Device : public GPIO_Device {
private:
    string defaultDir;
    string pack[7] = {"offset", "name", "consumer", "clock_speed", "address_mode", "10bit_mode", "sda_hold_time"};
    int packSize = 7;

public:
    string getDefaultDir() { return defaultDir; }
    string* getPack(){ return pack; }
    int getPackSize(){ return packSize; }
    I2C_Device(char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_I2C_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_I2C(dir, gpioDevHandler);
    }
};

class ETHERNET_Device : public GPIO_Device {
private:
    string defaultDir;
    string pack[9] = {"offset", "name", "mac_address", "ip_address", "netmask", "gateway", "vlan_enable", "vlan_id", "link_speed"};
    int packSize = 9;

public:
    string getDefaultDir() { return defaultDir; }
    string* getPack(){ return pack; }
    int getPackSize(){ return packSize; }

    ETHERNET_Device( char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_ETHERNET_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_ETHERNET(dir, gpioDevHandler);
    }
};

class USART_Device : public GPIO_Device {
private:
    string defaultDir;
    string pack[13] = {"offset", "name", "consumer", "baud_rate", "data_bits", "stop_bits", "parity", "flow_control", "fifo_depth", "synchronous_mode", "clock_polarity", "clock_phase", "clock_rate"};
    int packSize = 13;

public:
    string getDefaultDir() { return defaultDir; }
    string* getPack(){ return pack; }
    int getPackSize(){ return packSize; }

    USART_Device( char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_USART_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_USART(dir, gpioDevHandler);
    }
};

class UART_Device : public GPIO_Device {
private:
    string defaultDir;
    string pack[9] = {"offset", "name", "consumer", "baud_rate", "data_bits", "stop_bits", "parity", "flow_control", "fifo_depth"};
    int packSize = 9;

public:
    string getDefaultDir() { return defaultDir; }
    string* getPack(){ return pack; }
    int getPackSize(){ return packSize; }

    UART_Device( char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_UART_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_UART(dir, gpioDevHandler);
    }
};

class CAN_Device : public GPIO_Device {
private:
    string defaultDir;
    string pack[8] = {"offset", "name", "consumer", "bitrate", "acceptance_filter", "loopback_mode", "listen_only_mode", "transceiver_type", };
    int packSize = 8;

public:
    string getDefaultDir() { return defaultDir; }
    string* getPack(){ return pack; }
    int getPackSize(){ return packSize; }

    CAN_Device( char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_CAN_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_CAN(dir, gpioDevHandler);
    }
};

#endif //DRIVER_SIMULATOR_H
