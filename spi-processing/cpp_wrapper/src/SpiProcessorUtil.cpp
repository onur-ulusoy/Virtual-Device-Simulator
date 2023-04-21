/**
 * @file SpiProcessorUtil.cpp
 * @brief Spi processor utility library.
 * 
 * Spi processor utility library that provides solutions for the conversion
 * between arbitrary user space process' and driver process' spi_write messages.
 * 
 * @author Onur Ulusoy
 * @date 20/04/2023
 * @license MIT
 */

#include "SpiProcessorUtil.hpp"

std::string SpiDevEntry::getDeviceType() const {
    return deviceType;
}

int SpiDevEntry::getOffsetNumber() const {
    return offsetNumber;
}

std::vector<std::vector<std::string>> SpiDevEntry::getSpiWrite() const {
    return spiWrite;
}

void SpiDevEntry::setDeviceType(const std::string &newDeviceType) {
    deviceType = newDeviceType;
}

void SpiDevEntry::setOffsetNumber(const int newOffsetNumber) {
    offsetNumber = newOffsetNumber;
}

void SpiDevEntry::pushBackSpiWrite(const std::vector<std::string> &group) {
    spiWrite.push_back(group);
}

void SpiDevEntry::print() const {
    std::cout << "Device Type: " << deviceType << std::endl;
    std::cout << "Offset Number: " << offsetNumber << std::endl;
    std::cout << "SPI Write: " << std::endl;
    for (const auto& group : spiWrite) {
        for (const auto& write : group) {
            std::cout << "  " << write << std::endl;
        }
        std::cout << std::endl;
    }
}

SpiDevRequest::SpiDevRequest(const std::string &fileName, const int offset)
: fileName(fileName), offset(offset) { setRawFileName(); }

void SpiDevRequest::rawToJson() {
    std::ifstream infile(fileName);
    std::string line;
    nlohmann::json output;

    output["device_type"] = deviceType;
    output["offset_number"] = offset;

    std::vector<std::vector<std::string>> spiWriteGroups;
    std::vector<std::string> currentGroup;

    while (std::getline(infile, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        if (line.empty()) {
            if (!currentGroup.empty()) {
                spiWriteGroups.push_back(currentGroup);
                currentGroup.clear();
            }
        } else if (line.find("spi_write:") != std::string::npos) {
            currentGroup.push_back(line);
        }
    }

    if (!currentGroup.empty()) {
        spiWriteGroups.push_back(currentGroup);
    }

    output["spi_write"] = spiWriteGroups;

    std::ofstream outfile(rawFileName + ".json");
    outfile << output.dump(4);
}

void SpiDevRequest::processAndSaveJson() {
    nlohmann::json input, output;
    std::ifstream infile(rawFileName + ".json");
    infile >> input;

    output["device_type"] = input["device_type"];
    output["offset_number"] = input["offset_number"];

    std::vector<std::vector<std::vector<std::string>>> spiWriteGroups;
    for (const auto &group : input["spi_write"]) {
        std::vector<std::vector<std::string>> currentGroup;
        for (const auto &write : group) {
            std::stringstream ss(write.get<std::string>());
            std::string temp;
            std::vector<std::string> tokens;

            while (ss >> temp) {
                tokens.push_back(temp);
            }

            int dataSize = std::stoi(tokens[3].substr(0, tokens[3].find(':')));
            std::string buffer = tokens[5];

            for (size_t i = 6; i < tokens.size(); i++) {
                buffer += " " + tokens[i];
            }

            std::string new_write_1 = "write-" + output["device_type"].get<std::string>() + "device-" + std::to_string(output["offset_number"].get<int>()) + "-buffer-" + buffer;
            std::string new_write_2 = "write-" + output["device_type"].get<std::string>() + "device-" + std::to_string(output["offset_number"].get<int>()) + "-dataSize-" + std::to_string(dataSize);

            std::vector<std::string> writePair = {new_write_1, new_write_2};
            currentGroup.push_back(writePair);
        }
        spiWriteGroups.push_back(currentGroup);
    }

    output["spi_write"] = spiWriteGroups;

    std::ofstream outfile(rawFileName + "_processed" + ".json");
    outfile << output.dump(4);
}

void SpiDevRequest::parseJsonFile() {
    std::ifstream infile(rawFileName + ".json");
    nlohmann::json input;

    infile >> input;

    devEntry.setDeviceType(input["device_type"]);
    devEntry.setOffsetNumber(input["offset_number"]);

    for (const auto &group : input["spi_write"]) {
        std::vector<std::string> currentGroup;
        for (const auto &write : group) {
            currentGroup.push_back(write);
        }
        devEntry.pushBackSpiWrite(currentGroup);
    }
}

void SpiDevRequest::parseProcessedJsonFile() {
    std::ifstream infile(rawFileName + "_processed" + ".json");
    nlohmann::json input;

    infile >> input;

    devEntryProcessed.setDeviceType(input["device_type"]);
    devEntryProcessed.setOffsetNumber(input["offset_number"]);

    for (const auto &group : input["spi_write"]) {
        std::vector<std::string> currentGroup;
        for (const auto &writePair : group) {
            for (const auto &write : writePair) {
                currentGroup.push_back(write);
            }
        }
        devEntryProcessed.pushBackSpiWrite(currentGroup);
    }
}

SpiDevEntry SpiDevRequest::getDevEntry() const {
    return devEntry;
}

SpiDevEntry SpiDevRequest::getDevEntryProcessed() const {
    return devEntryProcessed;
}

void SpiDevRequest::setRawFileName() {
    size_t pos = fileName.find_last_of(".");

    if (pos != std::string::npos) {
        rawFileName = fileName.substr(0, pos);
    } else {
        rawFileName = fileName;
    }
}
