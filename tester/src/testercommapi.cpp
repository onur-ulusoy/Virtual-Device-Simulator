/**
 * @file testercommapi.cpp
 * @brief Communication API for tester to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023, Reworked: 18/05/2023
 *
 * @section LICENSE
 * Licensed under the MIT License.
 */
#include "testercommapi.hpp"

namespace Tester{

    void write_master_command(std::ofstream& com, const std::string& command) {
        com << now() << "\tMASTER LINE COMMAND: master write" << std::endl;
        com << now() << "\t     master writing: " << command << std::endl;
        com << now() << "\tMASTER LINE COMMAND: slave write" << std::endl;
    }

    void write_slave_response(std::ofstream& com, const std::string& message) {
        com << now() << "\t      slave writing: " << message << std::endl;
    }

    void write_driver_log(const std::string& command, const std::string& response, std::ofstream& log) {
        if (command.find("read-") != string::npos) {
            log << "Date: " << now() << endl << "Command: " << command << endl;
            log << "Output: " << endl;
            cout << "Chip info has been read successfully" << endl << endl;
            cout << "Data has been stored: " << response << endl;
            log << "Data has been stored: " << response << endl;
            log << endl << endl;
        }
        else if (command.find("config-") != string::npos) {
            if (response == "true") {
                log << "Date: " << now() << endl << "Command: " << command << endl;
                log << "Output: " << endl;
                cout << "Default chip info is written to the file successfully" << endl;
                log << "Default chip info is written to the file successfully" << endl;
                log << endl << endl;
            }
        }
        else if (command.find("show-") != string::npos) {
            log << "Date: " << now() << endl << "Command: " << command << endl;
            log << "Output: " << endl;
            cout << "Data is shown:" << endl;
            ifstream temp("temp");
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
            remove("temp");
        }
        else if (command.find("write-") != string::npos) {
            log << "Date: " << now() << endl << "Command: " << command << endl;
            log << "Output: " << endl;
            cout << endl;
            cout << "Chip info is changed successfully" << endl;
            log << "Chip info is changed successfully" << endl;
            log << endl << endl;
        }
        else {
            cout << "Command is not valid." << endl;
        }
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