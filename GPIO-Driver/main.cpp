#include "gpiolib.h"

using namespace std;

#define DEV_NAME "dev/gpiochip0"

int main() {


    GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);



    gpioDevHandler->device_open(READONLY, gpioDevHandler);

    gpioDevHandler->device_close();

    //gpioDevHandler->devContent.fill(DEFAULT, gpioDevHandler);

    gpioDevHandler->devContent.show(gpioDevHandler);

    int GPIO_offset = 0;
    string GPIO_status = gpioDevHandler->devContent.read(GPIO_offset, FLAG_IS_OUT,  gpioDevHandler);
    gpioDevHandler->devContent.write(7, FLAG_ACTIVE_LOW, "[ACTIVE_LOW]", gpioDevHandler);
    //feature inputlar için enum ayarlanabilir write için string input vermektense
    cout << "GPIO Device " << GPIO_offset << " " << "FLAG_IS_OUT is " << GPIO_status << endl;

    gpioDevHandler->devContent.show(gpioDevHandler);
    /*
    gpioDevHandler->device_open();

    gpioDevHandler->device_close();

    gpioDevHandler->gpio_list();

    gpioDevHandler->gpio_write(4, 1);
    gpioDevHandler->gpio_read(4);
    */
    delete gpioDevHandler;
    return 0;
}





