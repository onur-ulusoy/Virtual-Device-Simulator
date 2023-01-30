#ifndef PROGRAM_AUXILIARYTOOLS_H
#define PROGRAM_AUXILIARYTOOLS_H

#include <fstream>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <nlohmann/json.hpp>

using namespace std;

fstream& GotoLine(std::fstream& file, unsigned int num);
void Split(string s, string del, string* buffers);
string &rtrim(std::string &s);
string now();

#endif //PROGRAM_AUXILIARYTOOLS_H
