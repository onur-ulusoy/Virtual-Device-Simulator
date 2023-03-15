#include "lib-drv-itest.hpp"

namespace DriverIntTestSuite{

    vector<std::string> getFileNames(){
        return {"test_device", "log", "command", "communication-register", "temp"};
    }

    void deleteGarbage(vector<string> filenames){
        // iterate over the filenames and delete each file
        for (const auto& filename : filenames) {
            remove(filename.c_str());
        }
    }

    bool areFilesEqual(const std::string& filePath1, const std::string& filePath2) {
        std::ifstream file1(filePath1, std::ios::binary);
        std::ifstream file2(filePath2, std::ios::binary);

        if (!file1 || !file2) {
            // Failed to open one or both files
            return false;
        }

        // Read the files into two strings
        std::stringstream buffer1, buffer2;
        buffer1 << file1.rdbuf();
        buffer2 << file2.rdbuf();
        std::string contents1 = buffer1.str();
        std::string contents2 = buffer2.str();

        // Replace end-of-line characters with a common format
        for (char& c : contents1) {
            if (c == '\r') {
                c = '\n';
            }
        }
        for (char& c : contents2) {
            if (c == '\r') {
                c = '\n';
            }
        }

        // Compare the strings
        return contents1 == contents2;
    }
}