#include "gpiolib.h"

using namespace std;

#define DEV_NAME "/home/onur/CLionProjects/bbx15/dev/gpiochip0"


int main() {

    std::ifstream jsonFile("/home/onur/CLionProjects/bbx15/dev/chipinfo.json");
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["gpioDevices"];
    cout << "Number of items in gpioDevices: " << data.size() << endl;

    //cout << data.at(2).value("FLAG_ACTIVE_LOW", "-")<< endl;

    for (int i = 0; i<data.size(); i++) {
        cout << data.at(i).value("offset", 0)<< "  ";
        cout << data.at(i).value("name", "-")<< "  ";
        cout << data.at(i).value("consumer", "-")<< "  ";
        cout << data.at(i).value("FLAG_IS_OUT", "-")<< "  ";
        cout << data.at(i).value("FLAG_ACTIVE_LOW", "-")<< "  ";
        cout << data.at(i).value("FLAG_OPEN_DRAIN", "-")<< "  ";
        cout << data.at(i).value("FLAG_OPEN_SOURCE", "-")<< "  ";
        cout << data.at(i).value("FLAG_KERNEL", "-")<< "  ";

        cout << endl;
    }
    /*
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        cout << it.key() << ": " << it.value() << endl;
    }
    
    /*
    std::ifstream f("/home/onur/CLionProjects/bbx15/dev/chipinfo.json");
    json data = json::parse(f);
    //cout << data << endl;


    /*
    string name = data.value("name", "not found");
    string FLAG_ACTIVE_LOW = data.value("FLAG_ACTIVE_LOW", "not found");

    cout << "Name: " << name << endl;
    cout << "FLAG_ACTIVE_LOW: " << FLAG_ACTIVE_LOW << endl;
    */


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





