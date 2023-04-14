#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <unistd.h>

class SpiProcessorWrapper {
public:
    SpiProcessorWrapper(const std::string& input_file_path) : input_file(input_file_path) {}

    void save_to_file(const std::string& output_file_path) {
        if (!input_file.is_open()) {
            std::cerr << "Unable to open input file." << std::endl;
            return;
        }

        std::ofstream output_file(output_file_path);

        if (output_file.is_open()) {
            std::string line;
            while (std::getline(input_file, line)) {
                output_file << line << std::endl;
            }
            output_file.close();
        } else {
            std::cerr << "Unable to open output file." << std::endl;
        }
    }

    void run_with_i_flag() {

        // Change to the objective directory
        std::string workpath = "../../";
        chdir(workpath.c_str());

        std::string command = "python3 spi_processor.py -i ";
        std::system(command.c_str());
    }

private:
    std::ifstream input_file;
};

int main() {
    SpiProcessorWrapper spi_wrapper("SPI_Log_Trial.txt");
    std::string output_file_path = "../../SPI_Log.txt";

    spi_wrapper.save_to_file(output_file_path);
    spi_wrapper.run_with_i_flag();

    return 0;
}
