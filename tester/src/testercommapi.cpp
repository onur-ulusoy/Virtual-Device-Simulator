/**
 * @file testercommapi.cpp
 * @brief Communication API for tester to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#include "testercommapi.hpp"

namespace DriverTester{

    void write_master_command(std::ofstream& com, const std::string& command, const std::string& file_name) {
        com.open(file_name, std::ios::app);
        com << now() << "\tMASTER LINE COMMAND: master write" << std::endl;
        com << now() << "\t     master writing: " << command << std::endl;
        com << now() << "\tMASTER LINE COMMAND: slave write" << std::endl;
        com.close();
    }

    void write_slave_response(std::ofstream& com, const std::string& message, const std::string& file_name) {
        com.open(file_name, std::ios::app);
        com << now() << "\t      slave writing: " << message << std::endl;
        com.close();
    }

    void evaluate_slave(string _command, string word, fstream& log){
        ifstream temp;
        if (_command.find("read-") != -1){
            log.open("log", ios::app);

            log << "Date: " << now() << endl << "Command: " << _command << endl;
            log << "Output: " << endl;
            cout << "Chip info has been read successfully" << endl << endl;
            cout << "Data has been stored: " << word << endl;
            log << "Data has been stored: " << word << endl;
            log << endl << endl;
            log.close();
        }

        else if (_command.find("config-") != -1){
            if (word == "true"){
                log.open("log", ios::app);

                log << "Date: " << now() << endl << "Command: " << _command << endl;
                log << "Output: " << endl;

                cout << "Default chip info is written to the file successfully" << endl;
                log << "Default chip info is written to the file successfully" << endl;
                log << endl << endl;

                log.close();
            }
        }

        else if (_command.find("show-") != -1){
            log.open("log", ios::app);

            log << "Date: " << now() << endl << "Command: " << _command << endl;
            log << "Output: " << endl;
            cout << "Data is shown:" << endl;

            temp.open("temp");

            string line;
            while (getline(temp, line)) {
                cout << line << endl;
                log << line << endl;
            }

            cout << endl;
            cout << "Chip info is shown successfully" << endl;

            log << endl << endl;
            cout << endl << endl;

            temp.close();
            log.close();
            remove( "temp" );
        }

        else if (_command.find("write-") != -1){
            log.open("log", ios::app);

            log << "Date: " << now() << endl << "Command: " << _command << endl;
            log << "Output: " << endl;
            cout << endl;
            cout << "Chip info is changed successfully" << endl;
            log << "Chip info is changed successfully" << endl;

            log << endl << endl;
            log.close();
        }

        else
            cout << "Command is not valid." << endl;


    }

    void send_command(Publisher& publisher, const std::string& command) {
        publisher.publish(command);
    }

    string receive_response(Subscriber& subscriber){
        // receive a response from the topic
        std::string response = subscriber.receive();

        // purify the message
        std::string delimiter = ": ";
        size_t pos = response.find(delimiter);
        if (pos != std::string::npos) {
            response = response.substr(pos + delimiter.length());
        }

        return response;
    }
}