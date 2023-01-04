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

using namespace std;
enum command {DEFAULT, READONLY, WRITEONLY};


class GPIO_Device{

private:
    const char *dev_name;
    ofstream fd;
    ifstream fdi;
    /*
    struct gpiochip_info info;
    struct gpioline_info line_info;

    struct gpiohandle_request rq;
    struct gpiohandle_data data;
     */

public:
    GPIO_Device(const char* dev_name);
    void device_open(command request, GPIO_Device* gpioDevHandler);
    void device_close();
    //void fillDeviceContent(command request);

    class DeviceContent {
    public:
        void fill(command request, GPIO_Device* gpioDevHandler);
        void show (GPIO_Device* gpioDevHandler);
        ofstream fd;
    };

    DeviceContent devContent;

    //int device_write(int offset, uint8_t value);
    /*
    int gpio_list();

    int gpio_read(int offset);
     */

};


struct chipInfo{
    int offset;
    string name;
    string consumer;
    string FLAG_IS_OUT;
    string FLAG_ACTIVE_LOW;
    string FLAG_OPEN_DRAIN;
    string FLAG_OPEN_SOURCE;
    string FLAG_KERNEL;
};

#endif //UNTITLED_GPIOLIB_H
