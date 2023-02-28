#include <fstream>
#include <gtest/gtest.h>
#include "libdriver.hpp"
#include "libtesterutility.hpp"

using namespace DeviceSim;

enum ComparisonMode {
    LOG,
    REGISTER_LOG,
    DEFAULT_MODE
};

vector<std::string> filenames = {"../case_1_input_commands"};

bool areFilesEqual(const string& filePath1, const string& filePath2, ComparisonMode mode);
void deleteGarbage(vector<string> filenames);

TEST(IntegrityTest, Case1){
    
    string arg_filename = "case-1/case_1_input_arguments";
    ifstream argfile(arg_filename.c_str());
    if (!argfile.good()) {
        throw runtime_error("Error: could not open file ");

    }
    string argv;
    getline(argfile, argv);
    argfile.close();
    cout << argv << endl;
    
    auto *buffers = new string[4];

    Split(argv, " ", buffers);
    string device_type = buffers[1];

    cout << device_type << endl;
    
    string commands_dir = "case-1/case_1_input_commands";
    string commands_targetDir = "../case_1_input_commands";
    ifstream comfile(commands_dir.c_str());
    if (!comfile.good()) {
        throw runtime_error("Error: could not open file ");
    }
    string set_commands = "cp " + commands_dir + " " + commands_targetDir;
    char* set_commands_shell = const_cast<char*>(set_commands.c_str());
    system(set_commands_shell);
    cout << set_commands_shell << endl;

    std::ifstream file(commands_targetDir.c_str());
    if (!file.good())
        throw runtime_error("Commands file could not be moved");

    string start_test = "cd .. && python3 " + argv;

    char* start_test_shell = const_cast<char*>(start_test.c_str());
    cout << start_test_shell << endl;
    // Change to the parent directory and execute the command with arguments
    system(start_test_shell);

    string output_file1 = "../log";
    string output_file2 = "../communication-register";
    string output_file3 = "../dev/" + device_type + "/" + device_type + "device";

    ifstream o_file1(output_file1);
    ifstream o_file2(output_file2);
    ifstream o_file3(output_file3);

    if (!o_file1.good())
        throw runtime_error("Output 'log' could not be found.");

    if (!o_file2.good())
        throw runtime_error("Output 'communication-register' could not be found.");

    if (!o_file3.good())
        throw runtime_error("Output dev could not be found.");

    string expected_file1 = "case-1/case-1-output/log";
    string expected_file2 = "case-1/case-1-output/communication-register";
    string expected_file3 = "case-1/case-1-output/" + device_type + "device";

    ifstream e_file1(expected_file1);
    ifstream e_file2(expected_file2);
    ifstream e_file3(expected_file3);

    if (!e_file1.good())
        throw runtime_error("Expected file 'log' could not be found.");

    if (!e_file2.good())
        throw runtime_error("Expected file 'communication-register' could not be found.");

    if (!e_file3.good())
        throw runtime_error("Expected file dev could not be found.");
    
    EXPECT_TRUE(areFilesEqual(output_file1, expected_file1, LOG));
    EXPECT_TRUE(areFilesEqual(output_file2, expected_file2, REGISTER_LOG));
    EXPECT_TRUE(areFilesEqual(output_file3, expected_file3, DEFAULT_MODE));

    deleteGarbage(filenames);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}

bool areFilesEqual(const string& filePath1, const string& filePath2, ComparisonMode mode) {
    ifstream file1(filePath1);
    ifstream file2(filePath2);

    if (!file1 || !file2) {
        // Failed to open one or both files
        return false;
    }

    string line1, line2;
    while (getline(file1, line1) && getline(file2, line2)) {
        if (mode == LOG && line1.compare(0, 5, "Date:") == 0) {
            continue;  // Skip lines starting with "Date:"
        }
        size_t pos1 = line1.find(": ");
        size_t pos2 = line2.find(": ");
        if (mode == REGISTER_LOG && pos1 != string::npos && pos2 != string::npos) {
            if (line1.substr(pos1+2) != line2.substr(pos2+2)) {
                return false;
            }
        } else {
            if (line1 != line2) {
                return false;
            }
        }
    }
    return true;
}

void deleteGarbage(vector<string> filenames){
    // iterate over the filenames and delete each file
    for (const auto& filename : filenames) {
      remove(filename.c_str());
    }
}


