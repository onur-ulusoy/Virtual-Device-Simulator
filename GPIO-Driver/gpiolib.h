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

private:
    fstream fd;
    const char *dev_name;
    fstream hist;
public:
    GPIO_Device(const char* dev_name);
    void device_open(command request, GPIO_Device* gpioDevHandler);
    void device_close();

    class DeviceContent {
    public:
        void fill(command request, GPIO_Device* gpioDevHandler);
        static void show (GPIO_Device* gpioDevHandler);
        static string read(int offset, enum feature request, GPIO_Device* gpioDevHandler);
        void write (int offset, enum feature request, string new_value, GPIO_Device* gpioDevHandler);
    };

    DeviceContent devContent;

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
