#ifndef LIBTESTERUTILITY_H
#define LIBTESTERUTILITY_H

#include "testercommapi.h"

using namespace std;

namespace driverTester {
    string now();
    string get_last_word(const string& file_name);
    void create_directories(string directories[], int size);


}

#endif //LIBTESTERUTILITY_H
