#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

int main() {

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
    cout << "tester bitti";
    return 0;
}
