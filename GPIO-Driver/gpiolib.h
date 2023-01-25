#ifndef UNTITLED_GPIOLIB_H
#define UNTITLED_GPIOLIB_H

//#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>
//#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

#define NEW_FILE "dev/newfile"

using namespace std;
enum command {DEFAULT, READONLY, WRITEONLY};
enum feature {NAME, CONSUMER, FLAG_IS_OUT, FLAG_ACTIVE_LOW, FLAG_OPEN_DRAIN, FLAG_OPEN_SOURCE, FLAG_KERNEL};
enum command_request {ONESHOT, RECURSIVE};

fstream& GotoLine(std::fstream& file, unsigned int num);
void Split(string s, string del, string* buffers);
static inline std::string &rtrim(std::string &s);

class GPIO_Device{
protected:
    fstream fd;
private:
    const char *dev_name;
    fstream hist;
    string defaultDir = "dev/default_GPIO_chipInfo.json";
    int packSize = 8;
    string pack[8] = {"offset", "name", "consumer", "FLAG_IS_OUT", "FLAG_ACTIVE_LOW", "FLAG_OPEN_DRAIN", "FLAG_OPEN_SOURCE", "FLAG_KERNEL"};

public:
    GPIO_Device(const char* dev_name);
    void device_open(command request, GPIO_Device* gpioDevHandler);
    void device_close();
    virtual string getDefaultDir() { return defaultDir; }
    virtual string* getPack(){ return pack; }
    virtual int getPackSize(){ return packSize; }

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
        void fill(command request, GPIO_Device* gpioDevHandler);
        static void show (GPIO_Device* gpioDevHandler);
        static string read(int offset, enum feature request, GPIO_Device* gpioDevHandler);
        void write (int offset, enum feature request, string new_value, GPIO_Device* gpioDevHandler);

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

    SPI_Device(const char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_SPI_chipInfo.json";
    };

    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_SPI(dir, gpioDevHandler);
    }

};

class I2C_Device : public GPIO_Device {
private:
    string defaultDir;
    fstream fd;

public:
    string getDefaultDir() { return defaultDir; }
    I2C_Device(const char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_I2C_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_I2C(dir, gpioDevHandler);
    }
};

class ETHERNET_Device : public GPIO_Device {
private:
    string defaultDir;
    fstream fd;

public:
    string getDefaultDir() { return defaultDir; }
    ETHERNET_Device(const char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_ETHERNET_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_ETHERNET(dir, gpioDevHandler);
    }
};

class USART_Device : public GPIO_Device {
private:
    string defaultDir;
    fstream fd;

public:
    string getDefaultDir() { return defaultDir; }
    USART_Device(const char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_USART_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_USART(dir, gpioDevHandler);
    }
};

class UART_Device : public GPIO_Device {
private:
    string defaultDir;
    fstream fd;

public:
    string getDefaultDir() { return defaultDir; }
    UART_Device(const char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_UART_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_UART(dir, gpioDevHandler);
    }
};

class CAN_Device : public GPIO_Device {
private:
    string defaultDir;
    fstream fd;

public:
    string getDefaultDir() { return defaultDir; }
    CAN_Device(const char* dev_name) : GPIO_Device(dev_name) {
        this->defaultDir = "dev/default_CAN_chipInfo.json";
    };
    void parse(string dir, GPIO_Device* gpioDevHandler){
        parse_CAN(dir, gpioDevHandler);
    }
};

#endif //UNTITLED_GPIOLIB_H
