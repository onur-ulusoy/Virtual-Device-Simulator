#include "gpiolib.h"

using namespace std;

#define DEV_NAME "dev/gpiochip0"
int main() {

    GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);

    gpioDevHandler->device_open();


    return 0;
}





