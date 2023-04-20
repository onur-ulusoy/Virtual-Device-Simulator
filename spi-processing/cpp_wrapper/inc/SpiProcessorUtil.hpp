#ifndef SPI_PROCESSOR_UTIL_HPP
#define SPI_PROCESSOR_UTIL_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

class SpiDevEntry {
public:
    // Getters
    std::string get_device_type() const;
    int get_offset_number() const;
    std::vector<std::vector<std::string>> get_spi_write() const;

    // Setters
    void set_device_type(const std::string &new_device_type);
    void set_offset_number(const int new_offset_number);
    void push_back_spi_write(const std::vector<std::string> &group);

    void print() const;

private:
    std::string device_type;
    int offset_number;
    std::vector<std::vector<std::string>> spi_write;
};

class SpiDevRequest {
public:
    SpiDevRequest(const std::string &filename, const int offset);

    void raw_to_json();
    void parse_json_file();
    void parse_processed_json_file();
    void process_and_save_json();

    SpiDevEntry getDevEntry() const;
    SpiDevEntry getDevEntryProcessed() const;

private:
    std::string filename;
    std::string raw_filename;

    const std::string dev_type = "spi";
    int offset;
    SpiDevEntry devEntry;
    SpiDevEntry devEntryProcessed;

    void setRawName();
};

#endif // SPI_PROCESSOR_UTIL_HPP
