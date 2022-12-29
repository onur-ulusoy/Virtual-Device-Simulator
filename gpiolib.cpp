#include "gpiolib.h"

GPIO_Device::GPIO_Device(const char *dev_name) {
    this->dev_name = dev_name;
}

int GPIO_Device::device_open() {

    this->fd = open(this->dev_name, O_RDONLY);
    if (this->fd < 0)
    {
        printf("Unabled to open %s: %s\n", this->dev_name, strerror(errno));
        return 0;
    }

    else
        cout << this->dev_name << " is opened successfully" << endl;

    return 0;
}

void GPIO_Device::device_close() const {
    (void)close(this->fd);
}

int GPIO_Device::device_list()
{
    device_open();

    this->ret = ioctl(this->fd, GPIO_GET_CHIPINFO_IOCTL, &info);

    if (this->ret == -1)
    {

        printf("Unable to get chip info from ioctl: %s\n", strerror(errno));

        device_close();

        return 0;
    }

    printf("Chip name: %s\n", info.name);

    printf("Chip label: %s\n", info.label);

    printf("Number of lines: %d\n", info.lines);


    for (int i = 0; i < info.lines; i++)
    {

        line_info.line_offset = i;

        this->ret = ioctl(this->fd, GPIO_GET_LINEINFO_IOCTL, &line_info);

        if (this->ret == -1)
        {
            printf("Unable to get line info from offset %d: %s\n", i, strerror(errno));
        }

        else
        {

            printf("offset: %d, name: %s, consumer: %s. Flags:\t[%s]\t[%s]\t[%s]\t[%s]\t[%s]\n",

                   i,

                   line_info.name,

                   line_info.consumer,

                   (line_info.flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT",

                   (line_info.flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW" : "ACTIVE_HIGHT",

                   (line_info.flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN" : "...",

                   (line_info.flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPENSOURCE" : "...",

                   (line_info.flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "");

        }

    }

    close(this->fd);

    return 0;
}