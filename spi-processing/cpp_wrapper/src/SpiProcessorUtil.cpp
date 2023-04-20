#include "SpiProcessorUtil.hpp"

std::string SpiDevEntry::get_device_type() const {
    return device_type;
}

int SpiDevEntry::get_offset_number() const {
    return offset_number;
}

std::vector<std::vector<std::string>> SpiDevEntry::get_spi_write() const {
    return spi_write;
}

void SpiDevEntry::set_device_type(const std::string &new_device_type) {
    device_type = new_device_type;
}

void SpiDevEntry::set_offset_number(const int new_offset_number) {
    offset_number = new_offset_number;
}

void SpiDevEntry::push_back_spi_write(const std::vector<std::string> &group) {
    spi_write.push_back(group);
}

void SpiDevEntry::print() const {
    std::cout << "Device Type: " << device_type << std::endl;
    std::cout << "Offset Number: " << offset_number << std::endl;
    std::cout << "SPI Write: " << std::endl;
    for (const auto& group : spi_write) {
        for (const auto& write : group) {
            std::cout << "  " << write << std::endl;
        }
        std::cout << std::endl;
    }
}

SpiDevRequest::SpiDevRequest(const std::string &filename, const int offset)
: filename(filename), offset(offset) { setRawName(); }

void SpiDevRequest::raw_to_json() {
    std::ifstream infile(filename);
    std::string line;
    nlohmann::json output;

    output["device_type"] = dev_type;
    output["offset_number"] = offset;

    std::vector<std::vector<std::string>> spi_write_groups;
    std::vector<std::string> current_group;

    while (std::getline(infile, line)) {
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

    std::ofstream outfile(raw_filename + ".json");
    outfile << output.dump(4);
}

void SpiDevRequest::parse_json_file() {
    std::ifstream infile(raw_filename + ".json");
    nlohmann::json input;

    infile >> input;

    devEntry.set_device_type(input["device_type"]);
    devEntry.set_offset_number(input["offset_number"]);

    for (const auto &group : input["spi_write"]) {
        std::vector<std::string> current_group;
        for (const auto &write : group) {
            current_group.push_back(write);
        }
        devEntry.push_back_spi_write(current_group);
    }
}

void SpiDevRequest::parse_processed_json_file() {
    std::ifstream infile(raw_filename + "_processed" + ".json");
    nlohmann::json input;

    infile >> input;

    devEntryProcessed.set_device_type(input["device_type"]);
    devEntryProcessed.set_offset_number(input["offset_number"]);

    for (const auto &group : input["spi_write"]) {
        std::vector<std::string> current_group;
        for (const auto &write_pair : group) {
            for (const auto &write : write_pair) {
                current_group.push_back(write);
            }
        }
        devEntryProcessed.push_back_spi_write(current_group);
    }
}

void SpiDevRequest::process_and_save_json() {
    nlohmann::json input, output;
    std::ifstream infile(raw_filename + ".json");
    infile >> input;

    output["device_type"] = input["device_type"];
    output["offset_number"] = input["offset_number"];

    std::vector<std::vector<std::vector<std::string>>> spi_write_groups;
    for (const auto &group : input["spi_write"]) {
        std::vector<std::vector<std::string>> current_group;
        for (const auto &write : group) {
            std::stringstream ss(write.get<std::string>());
            std::string temp;
            std::vector<std::string> tokens;

            while (ss >> temp) {
                tokens.push_back(temp);
            }

            int data_size = std::stoi(tokens[3].substr(0, tokens[3].find(':')));
            std::string buffer = tokens[5];

            for (size_t i = 6; i < tokens.size(); i++) {
                buffer += " " + tokens[i];
            }

            std::string new_write_1 = "write-" + output["device_type"].get<std::string>() + "device-" + std::to_string(output["offset_number"].get<int>()) + "-buffer-" + buffer;
            std::string new_write_2 = "write-" + output["device_type"].get<std::string>() + "device-" + std::to_string(output["offset_number"].get<int>()) + "-data_size-" + std::to_string(data_size);

            std::vector<std::string> write_pair = {new_write_1, new_write_2};
            current_group.push_back(write_pair);
        }
        spi_write_groups.push_back(current_group);
    }

    output["spi_write"] = spi_write_groups;

    std::ofstream outfile(raw_filename + "_processed" + ".json");
    outfile << output.dump(4);
}

SpiDevEntry SpiDevRequest::getDevEntry() const {
    return devEntry;
}

SpiDevEntry SpiDevRequest::getDevEntryProcessed() const {
    return devEntryProcessed;
}

void SpiDevRequest::setRawName() {
    size_t pos = filename.find_last_of(".");

    if (pos != std::string::npos) {
        raw_filename = filename.substr(0, pos);
    } else {
        raw_filename = filename;
    }
}
