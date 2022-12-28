#include "gpiolib.h"

GPIO_Device::GPIO_Device(const char *dev_name) {
    this->dev_name = dev_name;
}

int GPIO_Device::device_open() {

    this->fd = open(this->dev_name, O_RDONLY);
    if (this->fd < 0)
    {
        printf("Unabled to open %s: %s", this->dev_name, strerror(errno));
        return 0;
    }


    else
        cout << this->dev_name << " is opened successfully";

    return 0;
}

void GPIO_Device::device_close() const {
    (void)close(this->fd);
}
