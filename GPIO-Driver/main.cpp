#include "gpiolib.h"
#include <unistd.h>
#include <regex>

using namespace std;

//#define DEV_NAME "dev/gpiochip0"

fstream receiver;
string _command;

int receive_command(enum command_request req);
void throw_command();

int main() {

    //GPIO_Device* gpioDevHandler = new GPIO_Device(DEV_NAME);

    //gpioDevHandler->device_open(READONLY, gpioDevHandler);

    //gpioDevHandler->device_close();

    //gpioDevHandler->devContent.fill(DEFAULT, gpioDevHandler);

    //gpioDevHandler->devContent.show(gpioDevHandler);

    //int GPIO_offset = 0;
    //string GPIO_status = gpioDevHandler->devContent.read(GPIO_offset, FLAG_IS_OUT,  gpioDevHandler);
    //gpioDevHandler->devContent.write(1, FLAG_IS_OUT, "[OUTPUT]", gpioDevHandler);
    //feature inputlar için enum ayarlanabilir write için string input vermektense
    //cout << "GPIO Device " << GPIO_offset << " " << "FLAG_IS_OUT is " << GPIO_status << endl;

    //gpioDevHandler->devContent.show(gpioDevHandler);


    float sleep_time = 0.2;
    cout << "Build finished."<< endl;
    cout << "program started working."<< endl;

    //usleep(2000000);

    receiver.open("command");
    receiver << "start";

    receiver.close();

//    fstream hFile;
//    hFile.open("history");
//    hFile.close();


    usleep(1000000 * sleep_time);

    while (receive_command(ONESHOT) != -1) {

        usleep(1000000 * sleep_time);


    }

    cout << "Program is terminating .." << endl;
    return 0;
}

int receive_command(enum command_request req) {

    if (req == ONESHOT){
        receiver.open("command");
        receiver >> _command;
        //cout << _command << endl;

        receiver.close();
        //cout << _command.length();
    }

    else if (req == RECURSIVE)
        ;

    if (_command == "-1")
        return -1;

    else if (_command == "&"){
        return 0;
    }

    else {
        string delimiter = "-";
        vector<string> substrings;
        stringstream ss(_command);
        string item;

        while (getline(ss, item, delimiter[0])) {
            substrings.push_back(item);
        }

        if (substrings[0] == "show") {

            const char* dev_name = ("dev/" + substrings[1]).c_str();
            GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
            gpioDevHandler->devContent.show(gpioDevHandler);

        }

        else if (substrings[0] == "read") {

            const char* dev_name = ("dev/" + substrings[1]).c_str();
            GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
            int offset = stoi(substrings[2]);

            enum feature property;

            if (substrings[3] == "NAME")
                property = NAME;
            else if (substrings[3] == "CONSUMER")
                property = CONSUMER;
            else if (substrings[3] == "FLAG_IS_OUT")
                property = FLAG_IS_OUT;
            else if (substrings[3] == "FLAG_ACTIVE_LOW")
                property = FLAG_ACTIVE_LOW;
            else if (substrings[3] == "FLAG_OPEN_DRAIN")
                property = FLAG_OPEN_DRAIN;
            else if (substrings[3] == "FLAG_OPEN_SOURCE")
                property = FLAG_OPEN_SOURCE;
            else if (substrings[3] == "FLAG_KERNEL")
                property = FLAG_KERNEL;
            else{
                cout << "Property is not valid" << endl;
                return 0;
            }

            string gpio_data = gpioDevHandler->devContent.read(offset, property, gpioDevHandler);
            cout << "Data has been read:" << gpio_data << endl;

        }

        else if (substrings[0] == "write") {
            const char* dev_name = ("dev/" + substrings[1]).c_str();
            //cout << dev_name << " * " << endl;
            GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);

            int offset = stoi(substrings[2]);

            enum feature property;
            //write-d-2-NAME-fdf
            if (substrings[3] == "NAME")
                property = NAME;
            else if (substrings[3] == "CONSUMER")
                property = CONSUMER;
            else if (substrings[3] == "FLAG_IS_OUT")
                property = FLAG_IS_OUT;
            else if (substrings[3] == "FLAG_ACTIVE_LOW")
                property = FLAG_ACTIVE_LOW;
            else if (substrings[3] == "FLAG_OPEN_DRAIN")
                property = FLAG_OPEN_DRAIN;
            else if (substrings[3] == "FLAG_OPEN_SOURCE")
                property = FLAG_OPEN_SOURCE;
            else if (substrings[3] == "FLAG_KERNEL")
                property = FLAG_KERNEL;
            else{
                cout << "Property is not valid" << endl;
                return 0;
            }

            //cout << substrings.size() << endl;
            //string new_val = substrings[4];
            //string gpio_data = gpioDevHandler->devContent.read(offset, property, gpioDevHandler);
            //cout << "Data has read:" << gpio_data << endl;
            //string val = substrings[4];
            //gpioDevHandler->devContent.show(gpioDevHandler);
            gpioDevHandler->devContent.write(offset, property, substrings[4], gpioDevHandler);
            //gpioDevHandler->devContent.write(1, FLAG_IS_OUT, "[OUTPUT]", gpioDevHandler);

        }

        else if (substrings[0] == "fill") {

            const char* dev_name = ("dev/" + substrings[1]).c_str();
            GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
            gpioDevHandler->devContent.fill(DEFAULT, gpioDevHandler);

        }
        //.commandSet-commandsText
        else if (substrings[0] == ".commandSet") {

            fstream f;
            f.open(substrings[1]);

            while (true) {
                if(f.eof()) break;
                _command.clear();
                f >> _command;
                if (_command.length()  == 0)
                    break;
                cout << "Command is running: " << _command << endl;
                receive_command(RECURSIVE);
            }

        }

        else {
            cout << "Command is not valid." << endl;
        }

    }
    //cout << "3322" << endl;

    //_command = "&";
    //usleep(1000000 * 3);
    throw_command();
    //usleep(10000000);
    //exit(0);
    return 0;
}

void throw_command(){
    ofstream outfile ("command");
    outfile << "&";
    outfile.close();
}



