#ifndef UNTITLED_GPIOLIB_H
#define UNTITLED_GPIOLIB_H

#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;

class GPIO_Device{

private:
    const char *dev_name;
    int fd = 0, ret;

    struct gpiochip_info info;
    struct gpioline_info line_info;

    struct gpiohandle_request rq;
    struct gpiohandle_data data;

public:
    GPIO_Device(const char* dev_name);
    int device_open();
    void device_close();
    int gpio_list();
    int gpio_write(int offset, uint8_t value);
    int gpio_read(int offset);

};

int _open(const char* dev_name, int flag);


#endif //UNTITLED_GPIOLIB_H
