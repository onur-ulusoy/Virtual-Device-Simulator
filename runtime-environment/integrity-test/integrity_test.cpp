#include <fstream>
#include <gtest/gtest.h>
#include "libdriver.hpp"
#include "libtesterutility.hpp"

using namespace DeviceSim;

TEST(IntegrityTest, Case1){
    
    string filename = "case-1/case_1_input_arguments";
    ifstream infile(filename.c_str());
    if (!infile.good()) {
        cerr << "Error: could not open file " << filename << endl;
    }
    string argv;
    getline(infile, argv);
    infile.close();
    
    string shell_command = "cd .. && python3 " + argv;

    char* shell_command_ = const_cast<char*>(shell_command.c_str());
    cout << shell_command_ << endl;
    // Change to the parent directory and execute the command with arguments
    system(shell_command_);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}