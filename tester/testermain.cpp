#include "testercommapi.h"

using namespace driverTester;

int main() {
    fstream log;
    string directories[] = {"dev/gpio", "dev/spi", "dev/i2c", "dev/ethernet", "dev/usart", "dev/uart", "dev/can"};
    create_directories(directories, 7);

    ofstream com ("communication-register");
    com.close();

    cout << "tester started working" << endl;
    string _command;

    cout << endl;

    cout << "Commands: " << endl;
    cout << "show-gpiochipx: shows gpiox set" << endl;
    cout << "read-gpiochipx-offset-property: reads gpiox spesific preperty from offset" << endl;
    cout << "write-gpiochipx-offset-property-newValue: writes new value to gpiox spesific property from offset" << endl;
    cout << "config-gpiochipx: configs gpiox set from default" << endl;
    cout << ".commandSet-textfile: Executes commands from file" << endl;
    cout << endl;

    cout << "Enter command (-1 to terminate): " << endl;

    get_and_transmit_command(_command, com, log);

    return 0;
}


