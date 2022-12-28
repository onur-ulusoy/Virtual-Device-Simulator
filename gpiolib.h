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
#include <iostream>

using namespace std;

class GPIO_Device{

private:
    const char *dev_name;
    int fd, ret;
public:
    GPIO_Device(const char* dev_name);
    int device_open();
    void device_close() const;

};




#endif //UNTITLED_GPIOLIB_H
