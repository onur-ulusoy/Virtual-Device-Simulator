#include "gpiolib.h"
//#include <unistd.h>

using namespace std;

#define DEV_NAME "dev/gpiochip0"
//#define PATH "home/onur/CLionProjects/bbx15"
int main() {
    //cout << chdir(PATH) << endl;
    //const char* DEV_NAME = "dev/gpiochip0";
    GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);

    gpioDevHandler->device_open();
    gpioDevHandler->device_close();

    gpioDevHandler->gpio_list();

    gpioDevHandler->gpio_write(4, 1);
    gpioDevHandler->gpio_read(4);

    delete gpioDevHandler;
    return 0;
}





