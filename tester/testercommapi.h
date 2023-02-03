#ifndef TESTERCOMMAPI_H
#define TESTERCOMMAPI_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <time.h>
#include <iomanip>
#include <sys/stat.h>
#include "libtesterutility.h"

using namespace std;

namespace driverTester{
    void master_writing(ofstream& com, string _command);
    void evaluate_slave(string _command, string word, fstream& log);
    void get_and_transmit_command(string _command, ofstream& com, fstream& log);
}

#endif //TESTERCOMMAPI_H
