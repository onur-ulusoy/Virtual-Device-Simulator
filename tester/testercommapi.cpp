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

    void get_and_transmit_command(string _command, ofstream& com, fstream& log){

        string emptyString = "---";
        float delay = .4;
        bool recursived = false;

        while (_command != "-1") {
            //cin >> _command;
            fstream getcommand;
            getcommand.open("command");
            getcommand >> _command;

            cout << _command << endl;
            ofstream outfile ("command");
            outfile.close();

            fstream transmitter;
            
            transmitter.open("command");

            transmitter << _command;

            outfile.close();

            master_writing(com, _command);

            com.close();

            if (_command == "-1") break;

            usleep(1000000 * delay);
            emptyString.clear();

            while (emptyString != "&"){
                usleep(1000000 * delay);
                transmitter.open("command");

                transmitter >> emptyString;
                if (emptyString == "-1") break;
                //RECURSIVE MODE
                if (emptyString.find("&&") != -1){
                    recursived = true;
                    _command = emptyString.erase(0, 2);
                    string last_word = get_last_word("communication-register");
                    evaluate_slave(_command, last_word, log);

                    ofstream outfile ("command");      

                    outfile << "-2";
                    outfile.close();
                }

                transmitter.close();
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

    }
}