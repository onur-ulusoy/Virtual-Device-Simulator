#ifndef DRIVER_DRIVERCOMMAPI_H
#define DRIVER_DRIVERCOMMAPI_H

#include "libdriver.h"

namespace DriverSim {
    int receive_command(enum command_request req, string devType, fstream& receiver, string& _command, ofstream& _com);
    void throw_command();
    void slave_writing(ofstream& com, string message);
}


#endif //DRIVER_DRIVERCOMMAPI_H
