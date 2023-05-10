/**
 * @file testermain.cpp
 * @brief The main file of the tester program.
 *
 * @author Onur Ulusoy
 * @date 03.02.2023
 */
#include "testercommapi.hpp"
#include <vector>
#include "SpiProcessorWrapper.hpp"
#include "SpiProcessorUtil.hpp"

using namespace Tester;
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

    // SpiDevRequest spi_dev_request("SPI_A.txt", 0);
    // spi_dev_request.rawToJson();

    // spi_dev_request.processAndSaveJson();
    // spi_dev_request.parseJsonFile();
    // spi_dev_request.parseProcessedJsonFile();
    // // spi_dev_request.getDevEntryProcessed().print();
    // // spi_dev_request.getDevEntry().print(); 

    // // Open the output file
    // std::ofstream outfile("commandsText");

    // // Write the output to the file instead of the console
    // for (const auto& group : spi_dev_request.getDevEntryProcessed().getSpiWrite()) {
    //     for (const auto& write : group) {
    //         outfile << write << std::endl;
    //     }
    // }

    // // Close the output file
    // outfile.close();

    // vector<string> commands;
    // string line;
    // ifstream infile("commandsText");
    // if(infile.is_open()) {
    //     while(getline(infile, line)) {
    //         commands.push_back(line);
    //     }
    //     infile.close();
    // }

    // else {
    //     cout << "Unable to open file" << endl;
    //     return 1;
    // }

    ofstream log_file_ofs("driver_log");
    
    std::string comm_register_file_name = "communication-register"; // -> remove, write_master_command and write_slave_response will be used with onyl ofs
    std::string real_comm_reg_file_name = "real-communication-register";

    ofstream comm_register_file_ofs(comm_register_file_name);
    ofstream real_comm_reg_file_ofs(real_comm_reg_file_name);

    // Pipeline of commands from tester to driver
    string commands_topic = "tcp://localhost:6000";

    // Pipeline of responses to a command from driver to tester
    string responses_topic = "tcp://localhost:6002";

    Publisher driver_speaker(commands_topic, "tester");
    Subscriber driver_listener(responses_topic);

    string response;

    std::string spi_processor_workpath = "../spi-processing/cpp_wrapper/src";
    std::string current_workpath = []() { char buffer[FILENAME_MAX]; return getcwd(buffer, FILENAME_MAX) ? std::string(buffer) : ""; }();

    chdir(spi_processor_workpath.c_str());
    SpiProcessorWrapper spi_wrapper;    
    spi_wrapper.runWithFFlag();
    chdir(current_workpath.c_str());

    // Define communication topics to test script
    std::string signal_topic = "tcp://localhost:5555";
    std::string write_data_topic = "tcp://localhost:5557";
    std::string read_data_topic = "tcp://localhost:5559";

    // Create Publisher objects for the signal topic and read data topic
    Publisher data_requester(signal_topic, "tester");
    Publisher data_supplier(read_data_topic, "tester");

    // Create a Subscriber object to listen for write data
    Subscriber data_listener(write_data_topic);

    const int regular_timeout_ms = 100;
    const int first_msg_timeout_ms = 10000;
    const int sleep_ms = 100;
    bool first_msg = true;
    bool communication_running = true;
    int consecutive_timeouts = 0;
    
    usleep(0.5 * 1000000);

    while (consecutive_timeouts < 2 && communication_running) {
        // Send a request for data
        data_requester.publish("Requesting data");
        usleep(sleep_ms * 1000);

        // Receive and process messages, with a timeout
        size_t group_index = 0;
        while (true) {
            // Set timeout for the first message or regular messages
            int current_timeout = first_msg ? first_msg_timeout_ms : regular_timeout_ms;
            auto message = data_listener.receive(current_timeout);

            if (message.empty()) {
                if (first_msg) {
                    // If the first message didn't arrive within the specified time, exit the loop
                    communication_running = false;
                    cout << "First message could not be received." << endl;
                    break;
                } else {
                    // If it's not the first message, increment the consecutive timeouts counter and break the inner loop
                    consecutive_timeouts++;
                    break;
                }
            } 
            
            else {

                std::ofstream file("SPI_A_Oneshot.txt");
                if (file.is_open()) {
                    file << message << endl;
                    file.close();
                } else {
                    std::cerr << "Unable to open file";
                }
                
                SpiDevRequest spi_dev_request("SPI_A_Oneshot.txt", 0);
                spi_dev_request.rawToJson();

                spi_dev_request.processAndSaveJson();
                spi_dev_request.parseJsonFile();
                spi_dev_request.parseProcessedJsonFile();
                
                vector<vector<string>> spi_write_groups = spi_dev_request.getDevEntry().getSpiWrite();

                // Open the output file
                std::ofstream outfile("commandsText_Oneshot");

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
                ifstream infile("commandsText_Oneshot");
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

                first_msg = false;
                consecutive_timeouts = 0;

                sleep(1);
                // First cycle
                string command1 = commands[2 * group_index];
                write_master_command(comm_register_file_ofs, command1);
                send_command(driver_speaker, command1);
                string response1 = receive_response(driver_listener);
                write_slave_response(comm_register_file_ofs, response1);
                write_driver_log(command1, response1, log_file_ofs);
                
                sleep(1);
                // Second cycle
                string command2 = commands[2 * group_index + 1];
                write_master_command(comm_register_file_ofs, command2);
                send_command(driver_speaker, command2);
                string response2 = receive_response(driver_listener);
                write_slave_response(comm_register_file_ofs, response2);
                write_driver_log(command2, response1, log_file_ofs);


                // Check if both responses are "success"
                if (response1 == "success" && response2 == "success") {
                    // Process and print the SPI write group
                    const auto& group = spi_write_groups[group_index];
                    std::string write;
                    std::string separator = "\n";

                    for (size_t i = 0; i < group.size(); ++i) {
                        write += group[i];
                        if (i < group.size() - 1) {
                            write += separator;
                        }
                    }

                    cout << write << std::endl;

                    chdir(spi_processor_workpath.c_str());
                    std::string read_line = spi_wrapper.requestReadLine(write);
                    cout << read_line << endl;  
                    // Publish the read line
                    data_supplier.publish(read_line); 
                    chdir(current_workpath.c_str());

                    write_master_command(real_comm_reg_file_ofs, write);
                    write_slave_response(real_comm_reg_file_ofs, read_line);
             
       
                } else {
                    throw std::runtime_error("One of the responses is 'failure'");
                }
                group_index++;         
            }
        }

        std::cout << "*******************************" << std::endl;
        usleep(sleep_ms * 1000);
    }

    
    
    sleep(1);
    chdir(spi_processor_workpath.c_str());
    spi_wrapper.requestReadLine("TERMINATE");
    cout << "Tester is being terminated .." << endl;
    return 0;
}


