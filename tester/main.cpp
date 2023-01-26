#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <time.h>
/*#include <unistd.h>
#include <ctime>*/
#include <iomanip>

using namespace std;
string now();
void master_writing(ofstream& com, string _command);

int main() {
    ofstream com ("communication-register");
    com.close();

    cout << "tester started working" << endl;
    string _command;
    string emptyString = "---";
    float delay = .4;

    cout << endl;

    cout << "Commands: " << endl;
    cout << "show-gpiochipx: shows gpiox set" << endl;
    cout << "read-gpiochipx-offset-property: reads gpiox spesific preperty from offset" << endl;
    cout << "write-gpiochipx-offset-property-newValue: writes new value to gpiox spesific property from offset" << endl;
    cout << "fill-gpiochipx: fills gpiox set from default" << endl;
    cout << ".commandSet-textfile: Executes commands from file" << endl;

    cout << endl;

    cout << "Enter command (-1 to terminate): " << endl;
    while (_command != "-1"){
        cin >> _command;

        ofstream outfile ("command");
        outfile.close();

        fstream file;
        file.open("command");

        file << _command;

        outfile.close();

        master_writing(com, _command);

        com.close();

        if (_command == "-1") break;

        usleep(1000000 * delay);
        //cout << emptyString << endl;
        emptyString.clear();
        while (emptyString != "&"){
            usleep(1000000 * delay);
            file.open("command");

            file >> emptyString;

            //cout << emptyString.length() << endl;
            file.close();
            //usleep(1000000);
        }

        usleep(1000000 * delay);

        cout << "Enter command (-1 to terminate): " << endl;

    }
    return 0;
}

string now(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *tm = std::localtime(&now_c);

    // Create a stringstream object
    std::stringstream ss;

    // Use the stringstream's operator << to format the time
    ss << std::put_time(tm, "%c");

    // Get the string from the stringstream
    return ss.str();
}

void master_writing(ofstream& com, string _command){
    com.open("communication-register", ios::app);
    com << now() << "\tMASTER LINE COMMAND: master write" << endl;
    com << now() << "\t     master writing: " << _command << endl;
    com << now() << "\tMASTER LINE COMMAND: master read" << endl;
    com.close();
}
