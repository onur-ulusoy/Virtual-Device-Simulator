#include "SpiProcessorWrapper.hpp"

void SpiProcessorWrapper::copy_file(const std::string& input_file_path) {
    const fs::path target_path = "../../SPI_Log.txt";
        
    try {
        fs::copy_file(input_file_path, target_path, fs::copy_options::overwrite_existing);
        std::cout << "File copied successfully." << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
    }
}

void SpiProcessorWrapper::run_with_i_flag() {
    // Change to the objective directory
    std::string workpath = "../../";
    chdir(workpath.c_str());

    std::string command = "python3 spi_processor.py -i ";
    std::system(command.c_str());

    std::cout << "Data saved to json successfully." << std::endl;
}

void SpiProcessorWrapper::run_with_f_flag() {
    // Create "in" and "out" files
    std::ofstream in_file("../../in");
    std::ofstream out_file("../../out");

    in_file.close();
    out_file.close();

    // Run spi_processor.py with -f flag in the background without console output
    std::string command = "cd ../.. && python3 spi_processor.py -f > /dev/null 2>&1 &";
    FILE* proc = popen(command.c_str(), "r");
    if (proc == nullptr) {
        std::cerr << "Failed to run spi_processor.py" << std::endl;
    } else {
        pclose(proc);
    }
}

std::string SpiProcessorWrapper::request_read_line(const std::string& write_line) {
    // Save write_line to "in" file
    std::ofstream in_file("../../in");
    in_file << write_line;
    in_file.close();

    std::string read_line;

    // Loop until a valid spi_read line is found
    while (true) {
        std::ifstream out_file("../../out");

        if (out_file.is_open()) {
            std::getline(out_file, read_line);
            out_file.close();

            if (read_line.substr(0, 8) == "spi_read") {
                break;
            }
        }
        usleep(10000);
    }

    // Recreate the "in" file
    in_file.open("../../in", std::ofstream::out | std::ofstream::trunc);
    in_file.close();

    return read_line;
}
