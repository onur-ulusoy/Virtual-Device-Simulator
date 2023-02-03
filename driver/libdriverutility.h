#ifndef LIB_DRIVER_UTILITY_H
#define LIB_DRIVER_UTILITY_H

#include <fstream>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <nlohmann/json.hpp>

using namespace std;

namespace DriverSim{
    fstream& GotoLine(std::fstream& file, unsigned int num);
    void Split(string s, string del, string* buffers);
    string &rtrim(std::string &s);
    string now();
}


#endif //LIB_DRIVER_UTILITY_H
