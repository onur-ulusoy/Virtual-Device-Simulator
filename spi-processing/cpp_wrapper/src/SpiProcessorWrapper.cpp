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

    void run_with_f_flag() {
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


    std::string request_read_line(const std::string& write_line) {
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

private:
    std::ifstream input_file;
};

#include <random>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    SpiProcessorWrapper spi_wrapper("SPI_Log_Trial.txt");
    // std::string output_file_path = "../../SPI_Log.txt";

    // spi_wrapper.save_to_file(output_file_path);
    // spi_wrapper.run_with_i_flag();

    spi_wrapper.run_with_f_flag();

    for (size_t i = 0; i < 30; i++)
    {
        std::string write_line = "spi_write: Bytes written: 5: 0x01 0x04 0x03 0x04 0x00";
        std::string read_line = spi_wrapper.request_read_line(write_line);

        std::cout << "Read line: " << read_line << std::endl;
        std::uniform_real_distribution<float> dis(0.1f, 2.0f);
         float random_num = dis(gen);
        float stime = random_num;
        usleep((int) (stime * 1000000));

    }
    
    

    return 0;
}
