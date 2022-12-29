#include "gpiolib.h"

using namespace std;

#define DEV_NAME "/home/onur/CLionProjects/bbx15/dev/gpiochip0"

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

int main() {

    std::ifstream jsonFile("/home/onur/CLionProjects/bbx15/dev/chipinfo.json");
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["gpioDevices"];
    unsigned long dataSize = data.size();

    cout << "Number of items in gpioDevices: " << data.size() << endl;

    struct chipInfo chipxInfo[dataSize];

    for (int i = 0; i<dataSize; i++) {
        /*
        cout << data.at(i).value("offset", 0)<< "  ";
        cout << data.at(i).value("name", "-")<< "  ";
        cout << data.at(i).value("consumer", "-")<< "  ";
        cout << data.at(i).value("FLAG_IS_OUT", "-")<< "  ";
        cout << data.at(i).value("FLAG_ACTIVE_LOW", "-")<< "  ";
        cout << data.at(i).value("FLAG_OPEN_DRAIN", "-")<< "  ";
        cout << data.at(i).value("FLAG_OPEN_SOURCE", "-")<< "  ";
        cout << data.at(i).value("FLAG_KERNEL", "-")<< "  ";

        cout << endl;
         */
        chipxInfo[i] = {
                .offset = data.at(i).value("offset", 0),
                .name = data.at(i).value("name", "-"),
                .consumer = data.at(i).value("consumer", "-"),
                .FLAG_IS_OUT = data.at(i).value("FLAG_IS_OUT", "-"),
                .FLAG_ACTIVE_LOW = data.at(i).value("FLAG_ACTIVE_LOW", "-"),
                .FLAG_OPEN_DRAIN = data.at(i).value("FLAG_OPEN_DRAIN", "-"),
                .FLAG_OPEN_SOURCE = data.at(i).value("FLAG_OPEN_SOURCE", "-"),
                .FLAG_KERNEL = data.at(i).value("FLAG_KERNEL", "-")
        };
    }


    GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);


    gpioDevHandler->device_open();

    gpioDevHandler->device_close();

    gpioDevHandler->gpio_list();
    /*
    gpioDevHandler->gpio_write(4, 1);
    gpioDevHandler->gpio_read(4);
    */
    delete gpioDevHandler;
    return 0;
}





