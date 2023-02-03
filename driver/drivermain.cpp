#include "drivercommapi.h"

using namespace DriverSim;

int main() {
    fstream receiver;
    ofstream _com;
    string _command;

    float sleep_time = 0.2;

    cout << "driver simulator started working."<< endl;

    receiver.open("command", ios::in);
    string device;

    receiver >> device;

    cout << device << " device is being simulated." << endl;

    receiver.close();

    receiver.open("command", ios::out);
    receiver << "start";

    receiver.close();

    ofstream logFile;
    logFile.open("log");
    logFile.close();

    usleep(1000000 * sleep_time);

    while (receive_command(ONESHOT, device, receiver, _command, _com) != -1)
        usleep(1000000 * sleep_time);

    cout << "Program is terminating .." << endl;
    return 0;
}




