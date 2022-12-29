#include "gpiolib.h"

GPIO_Device::GPIO_Device(const char *dev_name) {
    dev_name = dev_name;
}

int GPIO_Device::device_open() {

    fd = open(dev_name, O_RDONLY);
    if (fd < 0)
    {
        printf("Unabled to open %s: %s\n", dev_name, strerror(errno));
        return 0;
    }

    else
        cout << dev_name << " is opened successfully" << endl;

    return 0;
}

void GPIO_Device::device_close() const {
    (void)close(fd);
}

int GPIO_Device::gpio_list()
{
    device_open();

    ret = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info);

    if (ret == -1)
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

        ret = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &line_info);

        if (ret == -1)
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

    device_close();

    return 0;
}

int GPIO_Device::gpio_write(int offset, uint8_t value)
{
    printf("Write value %d to GPIO at offset %d (OUTPUT mode) on chip %s\n", value, offset, dev_name);

    device_open();

    rq.lineoffsets[0] = offset;

    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    rq.lines = 1;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &(rq));

    device_close();

    if (ret == -1)
    {
        printf("Unable to line handle from ioctl : %s\n", strerror(errno));
        return 0;
    }

    data.values[0] = value;

    ret = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s\n", strerror(errno));
    }

    else

    {

        usleep(2000000);

    }

    close(rq.fd);
    return 0;
}



int GPIO_Device::gpio_read(int offset) {


    rq.lineoffsets[0] = offset;

    rq.flags = GPIOHANDLE_REQUEST_INPUT;

    rq.lines = 1;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);

    device_close();

    if (ret == -1) {
        printf("Unable to get line handle from ioctl : %s", strerror(errno));

        return 0;
    }

    ret = ioctl(rq.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

    if (ret == -1) {
        printf("Unable to get line value using ioctl : %s", strerror(errno));
    } else {
        printf("Value of GPIO at offset %d (INPUT mode) on chip %s: %d\n", offset, dev_name, data.values[0]);
    }

    close(rq.fd);
    return 0;
}