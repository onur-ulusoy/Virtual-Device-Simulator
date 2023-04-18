#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

class SpiProcessorUtil {
public:
    static void raw_to_json(const std::string &filename, const std::string &device, int offset) {
        std::ifstream infile(filename);
        std::string line;
        nlohmann::json output;

        output["device_type"] = device;
        output["offset_number"] = offset;

        std::vector<std::vector<std::string>> spi_write_groups;
        std::vector<std::string> current_group;

        while (std::getline(infile, line)) {
            // Remove any '\r' characters that might be present
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            
            if (line.empty()) {
                if (!current_group.empty()) {
                    spi_write_groups.push_back(current_group);
                    current_group.clear();
                }
            } else if (line.find("spi_write:") != std::string::npos) {
                current_group.push_back(line);
            }
        }

        if (!current_group.empty()) {
            spi_write_groups.push_back(current_group);
        }

        output["spi_write"] = spi_write_groups;

        std::string raw_file_name = getRawName(filename);
        std::ofstream outfile(raw_file_name + ".json");
        outfile << output.dump(4);
    }

    static std::string getRawName(std::string fileName) {
        // Find the position of the last dot (.)
        size_t pos = fileName.find_last_of(".");
        
        // If the dot is found, remove the file extension
        if (pos != std::string::npos) {
            return fileName.substr(0, pos);
        }
        
        // If the dot is not found, return the original file name
        return fileName;
    }

};

int main() {
    SpiProcessorUtil::raw_to_json("SPI_A.txt", "spi", 0);
    return 0;
}
