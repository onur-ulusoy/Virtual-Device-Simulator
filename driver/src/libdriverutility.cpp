/**
 * @file libdriverutility.cpp
 * @brief Library of utility functions used in the driver such as string or file manipulation.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#include "libdriverutility.hpp"
namespace DeviceSim{

    fstream& GotoLine(std::fstream& file, unsigned int num){
        file.seekg(std::ios::beg);
        for(int i=0; i < num - 1; ++i){
            file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
        return file;
    }

    void Split(string s, string del, string* buffers)
    {
        int i = 0;

        int start, end = -1*del.size();
        do {
            start = end + del.size();
            end = s.find(del, start);
            buffers[i] = s.substr(start, end - start);
            i++;
        } while (end != -1);
    }

    string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    string now(){
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm *tm = std::localtime(&now_c);

        // Create a stringstream object
        std::stringstream ss;

        // Use the stringstream's operator << to format the time
        ss << std::put_time(tm, "%c");

        // Get the string from the stringstream
        return ss.str();
    }

    char* generateDevName (string path) {
        char* dev_name = new char[path.length() + 1];
        strcpy(dev_name, path.c_str());
        return dev_name;
    }

    int count_lines(const string& filename) {
        ifstream infile(filename);  // Open the input file
        string line;
        int line_count = 0;
        
        while (getline(infile, line)) {
            ++line_count;
        }
        
        return line_count;
    }
}

