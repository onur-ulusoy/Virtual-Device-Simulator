/**
 * @file testermain.cpp
 * @brief The main file of the tester program.
 *
 * @author Onur Ulusoy
 * @date 03.02.2023
 */
#include "testercommapi.hpp"
#include <vector>
#include "SpiProcessorUtil.hpp"

using namespace DriverTester;
/**
 * @brief The main function of the tester program.
 *
 * This function creates the required device directories, prepares the communication-register file to be committed, outputs introductory messages,
 * and takes user commands through the `get_and_transmit_command` function and transmits it to the device simulator.
 *
 * @return Returns 0 if the program terminates successfully.
 * @param log Stream object to manage the log file that is log of communication between driver and tester
 * @param directories Directories that devices take place when they are being simulated
 * @param com Stream object to manage the communicator register file that contains the communication between tester and driver
*  @param _command String to store the received command
 */
int main() {
    // fstream log;
    // string directories[] = {"dev/gpio", "dev/spi", "dev/i2c", "dev/ethernet", "dev/usart", "dev/uart", "dev/can"};
    // create_directories(directories, 7);

    // ofstream com ("communication-register");
    // com.close();

    // cout << "tester started working" << endl;
    // string _command;

    // cout << endl;

    SpiDevRequest spi_dev_request("commands.txt", 0);
    spi_dev_request.rawToJson();

    spi_dev_request.processAndSaveJson();
    spi_dev_request.parseJsonFile();
    spi_dev_request.parseProcessedJsonFile();
    //spi_dev_request.getDevEntryProcessed().print();

    // Open the output file
    std::ofstream outfile("commandsText");

    // Write the output to the file instead of the console
    for (const auto& group : spi_dev_request.getDevEntryProcessed().getSpiWrite()) {
        for (const auto& write : group) {
            outfile << write << std::endl;
        }
    }

    // Close the output file
    outfile.close();

    vector<string> commands;
    string line;
    ifstream infile("commandsText");
    if(infile.is_open()) {
        while(getline(infile, line)) {
            commands.push_back(line);
        }
        infile.close();
    }

    else {
        cout << "Unable to open file" << endl;
        return 1;
    }
    
    // Pipeline of commands from tester to driver
    string commands_topic = "tcp://localhost:6000";

    // Pipeline of responses to a command from driver to tester
    string responses_topic = "tcp://localhost:6002";

    Publisher driver_speaker(commands_topic, "tester");

    for(auto& command : commands) {
        sleep(1);
        
        //cout << command << endl;

        send_command(driver_speaker, command);
    }

    

    cout << "Tester is terminating .." << endl;
    return 0;
}


