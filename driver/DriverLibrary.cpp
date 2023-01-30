#include "DriverLibrary.h"

GPIO_Device::GPIO_Device(char *dev_name) {
    this->dev_name = new char[strlen(dev_name) + 1];
    strcpy(this->dev_name, dev_name);
    log.open("log", ios::app);
    //cout << this->dev_name << endl;
}

void GPIO_Device::device_open(command request, GPIO_Device* gpioDevHandler) {

    switch (request) {

        case READONLY:
            cout << "function 'GPIO_Device::device_open' worked as READONLY" << endl;


            fd.open(dev_name, ios::in);
            //cout << fd.is_open() << endl;

            if (!fd.is_open()){
                GPIO_Device::device_open(WRITEONLY, gpioDevHandler);

                devContent.config(DEFAULT, gpioDevHandler);

                GPIO_Device::device_close();
                GPIO_Device::device_open(READONLY, gpioDevHandler);
                break;

            }

            if (!fd.is_open())
            {
                printf("Unabled to open %s: %s\n", dev_name, strerror(errno));
                break;
            }

            else
                cout << dev_name << " is opened successfully as READONLY" <<endl;

            cout << endl;
            break;

        case WRITEONLY:
            cout << "function 'GPIO_Device::device_open' worked as WRITEONLY" << endl;

            fd.open(dev_name, ios::out | ios::trunc);

            if (!fd.is_open())
            {
                printf("Unabled to open %s: %s\n", dev_name, strerror(errno));
                break;
            }

            else
                cout << dev_name << " is opened successfully as WRITEONLY" << endl;

            cout << endl;
            break;

        default:
            cout << "function 'GPIO_Device::device_open' worked as DEFAULT" << endl << endl;
            //device_close();
            fd.open(gpioDevHandler->dev_name);

            if (!fd.is_open())
            {
                printf("Unabled to open %s: %s\n", dev_name, strerror(errno));
                break;
            }

            else
                cout << dev_name << " is opened successfully as DEFAULT" << endl;

            cout << endl;
            break;

    }
}

void GPIO_Device::device_close() {

    cout << "function 'GPIO_Device::device_close' worked" << endl;

    if (fd.is_open()){
        cout << dev_name << " is closed successfully" << endl;
        fd.close();
    }

    else{
        //fd = 0;
        cout << dev_name << " is already not open" << endl;
    }

    cout << endl;
}

string GPIO_Device::DeviceContent::config(command request, GPIO_Device* gpioDevHandler) {

    string dir = gpioDevHandler->getDefaultDir();

    cout << "function 'GPIO_Device::DeviceContent::config' worked" << endl;
    gpioDevHandler->device_open(WRITEONLY, gpioDevHandler);

    if (request == DEFAULT){
        cout << dir << endl;

        gpioDevHandler->parse(dir, gpioDevHandler);

        //return "true";

    }
    gpioDevHandler->device_close();
    return "true";
}

string GPIO_Device::DeviceContent::show(GPIO_Device* gpioDevHandler) {
    ofstream temp;
    temp.open("temp", ios::app);

    cout << "function 'GPIO_Device::DeviceContent::show' worked" << endl << endl;

    string dir = gpioDevHandler->getDefaultDir();

    string* pack = gpioDevHandler->getPack();
    int packSize = gpioDevHandler->getPackSize();
    //cout<< "Length of pack:" << gpioDevHandler->getPackSize() << endl;

    gpioDevHandler->device_open(READONLY, gpioDevHandler);

    //cout << "Chip info is being shown ..." << endl << endl;

    // Get the maximum length of each column
    int max_length[packSize];
    for (int i=0; i<packSize; i++)
        max_length[i] = 0;

    int Length[packSize];

    while (true) {
        string line, word;
        for (int i=0; i<packSize ;i++){
            gpioDevHandler->fd >> word;
            line += word + "/";
        }

        if( gpioDevHandler->fd.eof() ) break;

        auto* buffers = new string[packSize+1];
        Split(line, "/", buffers);

        for (int i=0; i<packSize; i++){
            Length[i] = buffers[i].length();
            if (Length[i] > max_length[i]) max_length[i] = Length[i];
        }
        delete[] buffers;
    }

    gpioDevHandler->fd.clear();
    gpioDevHandler->fd.seekg(0, ios::beg);

    //cout << "Data was shown for '" << gpioDevHandler->dev_name << "':" << endl;
    while (true) {
        string line, word;
        for (int i=0; i<packSize;i++){
            gpioDevHandler->fd >> word;
            line += word + "/";
        }

        if( gpioDevHandler->fd.eof() ) break;

        auto* buffers = new string[packSize+1];
        Split(line, "/", buffers);


        for (int i=0; i<packSize; i++){
            /*cout << pack[i] << ": ";
            cout << setw(max_length[i]) << left << buffers[i] << "   ";*/

            temp << pack[i] << ": ";
            temp << setw(max_length[i]) << left << buffers[i] << "   ";

        }

        //cout << endl;
        temp << endl;
        delete[] buffers;
    }


    return "true";
}

string GPIO_Device::DeviceContent::read(int offset, string property, GPIO_Device* gpioDevHandler){

    cout << "function 'GPIO_Device::DeviceContent::read' worked" << endl << endl;

    string dir = gpioDevHandler->getDefaultDir();

    string* pack = gpioDevHandler->getPack();
    int packSize = gpioDevHandler->getPackSize();

    int request = -2;

    for (int i=0; i<packSize; i++) {
        if (property == pack[i]){
            request = i;
            break;
        }
    }

    if (request == -2){
        return "false";
    }

    gpioDevHandler->device_open(DEFAULT, gpioDevHandler);

    GotoLine(gpioDevHandler->fd, offset+1);

    string word, empty;

    for (int i=0; i<request;i++){
        gpioDevHandler->fd >> empty;
    }
    gpioDevHandler->fd >> word;

    /*cout << endl;
    cout << "Chip info is read successfully" << endl << endl;*/

    //cout << "Data has been stored: " << word << endl;

    //gpioDevHandler->log << "Data has been stored: " << word << endl;
    //fstream log;



    return word;
}

string GPIO_Device::DeviceContent::write (int offset, string property, string new_value, GPIO_Device* gpioDevHandler){

    //cout << gpioDevHandler->dev_name << endl;
    cout << "function 'GPIO_Device::DeviceContent::write' worked" << endl << endl;

    string dir = gpioDevHandler->getDefaultDir();

    string* pack = gpioDevHandler->getPack();
    int packSize = gpioDevHandler->getPackSize();

    int request = -2;

    for (int i=0; i<packSize; i++) {
        if (property == pack[i]){
            request = i;
            break;
        }
    }
    if (request == -2){
        return "false";
        //return 0;
    }
    //cout << gpioDevHandler->dev_name << endl;
    gpioDevHandler->device_open(DEFAULT, gpioDevHandler);

    GotoLine(gpioDevHandler->fd, offset+1);

    string line;
    getline(gpioDevHandler->fd, line);
    //cout << line << endl;

    auto* buffers = new string[packSize];
    Split(line, " ", buffers);

    string new_line;

    for (int i=0; i<packSize; i++){

        if (i == request){
            new_line += new_value + " ";
        }

        else
        new_line += buffers[i] + " ";
    }
    rtrim(new_line);
    //cout << new_line << endl << endl;


    GotoLine(gpioDevHandler->fd, 1);

    fstream newfile;
    ofstream outfile (NEW_FILE);
    outfile.close();

    newfile.open(NEW_FILE);
    int i = 0;

    while (true) {

        if (i != offset){
            getline(gpioDevHandler->fd, line);
            newfile << line << endl;
        }

        else {
            newfile << new_line << endl;
            GotoLine(gpioDevHandler->fd, i+2);
        }

        i++;
        if( gpioDevHandler->fd.eof() ) break;
    }

    newfile.close();

    gpioDevHandler->device_close();

    remove(gpioDevHandler->dev_name);
    rename(NEW_FILE, gpioDevHandler->dev_name);

    return "true";
}

void parse_GPIO(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);

    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {

        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("consumer", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("FLAG_IS_OUT", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("FLAG_ACTIVE_LOW", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("FLAG_OPEN_DRAIN", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("FLAG_OPEN_SOURCE", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("FLAG_KERNEL", "-");
        gpioDevHandler->fd << endl;

    }
}

void parse_SPI(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {
        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("consumer", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("cpol", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("cpha", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("lsb_first", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("cs_high", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("3wire", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("loopback", "false");

        gpioDevHandler->fd << endl;

    }
}

void parse_I2C(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {

        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("consumer", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("clock_speed", 100000) << " ";
        gpioDevHandler->fd << data.at(i).value("address_mode", 7) << " ";
        gpioDevHandler->fd << data.at(i).value("10bit_mode", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("sda_hold_time", 10);
        gpioDevHandler->fd << endl;
    }
}

void parse_ETHERNET(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {

        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("mac_address", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("ip_address", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("netmask", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("gateway", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("vlan_enable", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("vlan_id", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("link_speed", 1000);
        gpioDevHandler->fd << endl;
    }
}

void parse_USART(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {

        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("consumer", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("baud_rate", 115200) << " ";
        gpioDevHandler->fd << data.at(i).value("data_bits", 8) << " ";
        gpioDevHandler->fd << data.at(i).value("stop_bits", 1) << " ";
        gpioDevHandler->fd << data.at(i).value("parity", "none") << " ";
        gpioDevHandler->fd << data.at(i).value("flow_control", "none") << " ";
        gpioDevHandler->fd << data.at(i).value("fifo_depth", 64) << " ";
        gpioDevHandler->fd << data.at(i).value("synchronous_mode", "true") << " ";
        gpioDevHandler->fd << data.at(i).value("clock_polarity", "low") << " ";
        gpioDevHandler->fd << data.at(i).value("clock_phase", "first") << " ";
        gpioDevHandler->fd << data.at(i).value("clock_rate", 16000000);
        gpioDevHandler->fd << endl;
    }
}

void parse_UART(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {

        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("consumer", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("baud_rate", 115200) << " ";
        gpioDevHandler->fd << data.at(i).value("data_bits", 8) << " ";
        gpioDevHandler->fd << data.at(i).value("stop_bits", 1) << " ";
        gpioDevHandler->fd << data.at(i).value("parity", "none") << " ";
        gpioDevHandler->fd << data.at(i).value("flow_control", "none") << " ";
        gpioDevHandler->fd << data.at(i).value("fifo_depth", 64);
        gpioDevHandler->fd << endl;
    }
}

void parse_CAN(string dir, GPIO_Device* gpioDevHandler){
    std::ifstream jsonFile(dir);
    nlohmann::json commands;
    jsonFile >> commands;

    nlohmann::json data = commands["Devices"];

    unsigned long dataSize = data.size();

    cout << "Number of device: " << data.size() << endl << endl;

    for (int i = 0; i<dataSize; i++) {

        gpioDevHandler->fd << data.at(i).value("offset", 0) << " ";
        gpioDevHandler->fd << data.at(i).value("name", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("consumer", "-") << " ";
        gpioDevHandler->fd << data.at(i).value("bitrate", 500000) << " ";
        gpioDevHandler->fd << data.at(i).value("acceptance_filter", "standard") << " ";
        gpioDevHandler->fd << data.at(i).value("loopback_mode", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("listen_only_mode", "false") << " ";
        gpioDevHandler->fd << data.at(i).value("transceiver_type", "SN65HVD230");
        gpioDevHandler->fd << endl;
    }
}


