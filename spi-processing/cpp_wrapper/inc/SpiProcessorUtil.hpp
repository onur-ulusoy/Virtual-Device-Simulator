/**
 * @file SpiProcessorUtil.hpp
 * @brief Spi processor utility library.
 * 
 * Spi processor utility library that provides solutions for the conversion
 * between arbitrary user space process' and driver process' spi_write messages.
 * 
 * @author Onur Ulusoy
 * @date 20/04/2023
 * @license MIT
 */

#ifndef SPI_PROCESSOR_UTIL_HPP
#define SPI_PROCESSOR_UTIL_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * @class SpiDevEntry
 * @brief A class representing a single SPI device entry.
 *
 * This class is used to store information about a specific SPI device entry,
 * including the device type, offset number, and SPI write data. It provides
 * methods for printing the stored data and modifying the SPI write data.
 */
class SpiDevEntry {
public:
    /**
     * @brief Get the device type for the SpiDevEntry object.
     * 
     * @return std::string The device type for the SpiDevEntry object.
     */
    std::string getDeviceType() const;
    /**
     * @brief Get the offset number for the SpiDevEntry object.
     * 
     * @return int The offset number for the SpiDevEntry object.
     */
    int getOffsetNumber() const;
    /**
     * @brief Get the SPI write data for the SpiDevEntry object.
     * 
     * @return std::vector<std::vector<std::string>> The SPI write data for the SpiDevEntry object.
     */
    std::vector<std::vector<std::string>> getSpiWrite() const;
    /**
     * @brief Set the device type for the SpiDevEntry object.
     * 
     * @param newDeviceType The new device type to set.
     */
    void setDeviceType(const std::string &newDeviceType);
    /**
     * @brief Set the offset number for the SpiDevEntry object.
     * 
     * @param newOffsetNumber The new offset number to set.
     */
    void setOffsetNumber(const int newOffsetNumber);
    /**
     * @brief Add a new group of SPI write data to the SpiDevEntry object.
     * 
     * @param group The group of SPI write data to add.
     */
    void pushBackSpiWrite(const std::vector<std::string> &group);
    /**
     * @brief Print the SpiDevEntry data in a human-readable format.
     *
     * This method prints the device type, offset number, and SPI write data
     * for the current SpiDevEntry object.
     */
    void print() const;

private:
    /// A string representing the type of the SPI device, such as "spi".
    std::string deviceType; 
    /// An integer representing the offset number for the SPI device that specifies what offset of device user space process wants to control.
    int offsetNumber; 
    /// Vector of string vector representing the SPI write data for the device, organized into groups of writes.
    std::vector<std::vector<std::string>> spiWrite;
};

/**
 * @class SpiDevRequest
 * @brief A class to process SPI device requests from raw data.
 *
 * This class is responsible for processing raw SPI device request data,
 * converting it to JSON format, parsing and processing the JSON data, and
 * saving the processed data as a new JSON file. It also provides methods to
 * access the SpiDevEntry objects created from the parsed and processed JSON data.
 */
class SpiDevRequest {
public:
    /**
     * @brief Construct a new SpiDevRequest object.
     * @param fileName The name of the input file containing raw SPI device request data (SPI_A.txt).
     * @param offset An integer representing the offset number for the SPI device that specifies what offset of device user space process wants to control.
     * 
     */
    SpiDevRequest(const std::string &fileName, const int offset);
    /**
     * @brief Get the SpiDevEntry object representing the raw data.
     * 
     * @return SpiDevEntry The SpiDevEntry object representing the raw data.
     */
    SpiDevEntry getDevEntry() const;
    /**
     * @brief Get the SpiDevEntry object representing the processed data.
     * 
     * @return SpiDevEntry The SpiDevEntry object representing the processed data.
     */
    SpiDevEntry getDevEntryProcessed() const;
    /**
     * @brief Convert the raw SPI device request data to JSON format and save it as a new file.
     *
     * This method reads the input file (as in this context such as SPI_A, consists of spi_write lines)
     * line by line, extracts relevant information, and constructs a JSON object representing the SPI device request. 
     * The JSON object is then saved to a new file with the same name as the input file but with a ".json" extension.
     */
    void rawToJson();
    /**
     * @brief Process the JSON data and save it as a new processed (formatted for driver software) JSON file.
     *
     * This method processes the JSON data by modifying the format of the SPI write commands
     * and then saves the processed data as a new JSON file with a "_processed.json" extension.
     */
    void processAndSaveJson();
    /**
     * @brief Parse the JSON file and create an SpiDevEntry object from the parsed data.
     *
     * This method reads the JSON file created by rawToJson(), extracts the relevant data,
     * and initializes an SpiDevEntry object with the extracted data.
     */
    void parseJsonFile();
    /**
     * @brief Parse the processed JSON file and create an SpiDevEntry object from the parsed data.
     *
     * This method reads the processed JSON file, extracts the relevant data,
     * and initializes an SpiDevEntry object with the extracted data.
     */
    void parseProcessedJsonFile();

private:
    /**
     * @brief Set the rawFileName attribute based on the input fileName.
     *
     * This method extracts the raw fileName from the input fileName by removing the file extension
     * and assigns it to the rawFileName attribute.
     */
    void setRawFileName();
    /// The name of the input file containing raw SPI device request data.
    std::string fileName; 
    /// The raw file name derived from the input fileName, without the file extension.
    std::string rawFileName; 
    /// The device type, fixed as "spi" as in this case spi communication is processed.
    const std::string deviceType = "spi"; 
    /// An integer representing the offset number for the SPI device that specifies what offset of device user space process wants to control.
    int offset; 
    /// The SpiDevEntry object created from the parsed JSON data.
    SpiDevEntry devEntry; 
    /// The SpiDevEntry object created from the parsed processed JSON data.
    SpiDevEntry devEntryProcessed; 
};


#endif // SPI_PROCESSOR_UTIL_HPP