#include "gpiolib.h"

using namespace std;

#define DEV_NAME "dev/gpiochip0"
void GPIO_Access();

int main() {

    thread gpio_access (GPIO_Access);

    //gpio_access.join();
    ::getchar();

    return 0;
}

void GPIO_Access() {

    GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);


    gpioDevHandler->device_open(READONLY, gpioDevHandler);

    gpioDevHandler->device_close();

    //gpioDevHandler->devContent.fill(DEFAULT, gpioDevHandler);

    gpioDevHandler->devContent.show(gpioDevHandler);

    int GPIO_offset = 0;
    string GPIO_status = gpioDevHandler->devContent.read(GPIO_offset, FLAG_IS_OUT,  gpioDevHandler);
    //gpioDevHandler->devContent.write(1, FLAG_IS_OUT, "[OUTPUT]", gpioDevHandler);
    //feature inputlar için enum ayarlanabilir write için string input vermektense
    cout << "GPIO Device " << GPIO_offset << " " << "FLAG_IS_OUT is " << GPIO_status << endl;

    //gpioDevHandler->devContent.show(gpioDevHandler);

    delete gpioDevHandler;


}



