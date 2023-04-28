/**
 * @file testercommapi.cpp
 * @brief Communication API for tester to provide functionality of transmitting and receiveing the commands between tester and driver.
 *
 * @author Onur Ulusoy
 * @date 03/02/2023
 */
#include "testercommapi.hpp"

namespace DriverTester{

    void master_writing(ofstream& com, string _command){
        com.open("communication-register", ios::app);
        com << now() << "\tMASTER LINE COMMAND: master write" << endl;
        com << now() << "\t     master writing: " << _command << endl;
        com << now() << "\tMASTER LINE COMMAND: slave write" << endl;
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

    void get_and_transmit_command(string& _command, ofstream& com, fstream& log){
        string emptyString = "---";
        float delay = .01;
        bool recursived = false;

        while (_command != "-1") {
            //cin >> _command;
            ifstream getcommand;
            getcommand.open("command3");
            getcommand >> _command;
            getcommand.close();

            cout << _command << endl;
            cout << "---" << endl;
            
            ofstream outfile ("command2");
            outfile.close();

            fstream transmitter;
            
            transmitter.open("command2");

            transmitter << _command;

            transmitter.close();

            master_writing(com, _command);

            com.close();

            if (_command == "-1") break;

            //usleep(1000000 * delay);
            emptyString.clear();

            while (emptyString != "&"){ 
                usleep(1000000 * delay);
                transmitter.open("command0");
                transmitter >> emptyString;
                //cout << emptyString << "*" <<  endl;

                if (emptyString == "&1"){
                    cout << "tester is terminating .." << endl;
                    ofstream file;
                    file.open("command4");
                    file << "exit";
                    file.close();
                    exit(0);
                }

                if (emptyString == "-1") break;
                //RECURSIVE MODE
                transmitter.close();
                ofstream ofile("command0");
                cout << _command  << "***" << endl;
                if (_command == ""){
                    cout << "*|*" << endl;
                    break;
                }
                if (emptyString.find("&&") != -1){
                    recursived = true;
                    string _commandd = emptyString.erase(0, 2);
                    string last_word = get_last_word("communication-register");
                    evaluate_slave(_commandd, last_word, log);

                    ofstream outfile ("command");      
                    outfile << "-2";
                    outfile.close();
                    //emptyString.clear();
                }

                
            }

            if (!recursived){
                string last_word = get_last_word("communication-register");
                evaluate_slave(_command, last_word, log);
            }

            else
                recursived = false;

            usleep(1000000 * delay);

            cout << "Enter command (-1 to terminate): " << endl;
    
        }

        ofstream file;
        file.open("command4");
        file << "exit";
        file.close();
            
        cout << "tester is terminating .." << endl;
        exit(0);

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