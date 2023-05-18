# Virtual Device Simulator

The main objective of this project is to develop a software platform that enables the creation, manipulation, and use of virtual devices in the user space of an operating system. The goal is to make it easier for developers to create and test embedded software without having to use actual physical hardware. This will save time and resources and allow for more efficient and streamlined software development processes. The platform is designed to be flexible and user-friendly, making it accessible to a wide range of programmers who develop embedded linux applications. Virtual Device Simulator also includes features for automated testing and validation. This allows developers to automatically test communication between the embedded software and the virtual peripheral device, verify the accuracy of data transmission, and ensure the correct functioning of all software components with arrangable test scenerios while providing realistic log files for easy debug.
## Table of Contents

- [Virtual Device Simulator](#virtual-device-simulator)
  - [Table of Contents](#table-of-contents)
  - [Understanding Linux Devices](#understanding-linux-devices)
    - [Linux devices operating scenerio](#linux-devices-operating-scenerio)
    - [Fake device operating scenerio](#fake-device-operating-scenerio)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
      - [Installation for Debian-based OS](#installation-for-debian-based-os)
    - [Build instructions](#build-instructions)
    - [Usage instructions](#usage-instructions)
    - [Output files software produces](#output-files-software-produces)
  - [Software Architecture](#software-architecture)
    - [Directories](#directories)
    - [Components](#components)
      - [Driver](#driver)
    - [Tester](#tester)
    - [Test Scenerio Scripts and Test Framework](#test-scenerio-scripts-and-test-framework)
  - [Documentation](#documentation)
  - [Technologies Used](#technologies-used)
  - [Contributing](#contributing)
  - [Versioning](#versioning)
  - [Authors](#authors)
  - [License](#license)

## Understanding Linux Devices
Linux, as an open-source operating system kernel, provides a unique and versatile structure for managing devices. Devices in Linux are treated as files and are accessed through file systems, illustrating the philosophy of "everything is a file." This structure includes various types of devices such as hard drives, RAM, USBs, printers, and even keyboards. The device files are typically housed in the "/dev" directory, each having specific permissions and ownership. Understanding Linux devices involves a comprehension of these file-based device interactions, the different categories of devices, and the permissions that control their accessibility. This foundational knowledge is key to effective system administration and troubleshooting within a Linux environment.
### Linux devices operating scenerio
In the Linux operating scenario, an arbitrary program interacts with a device file, in this case "/dev/spi", located within the user space. This file essentially represents a physical SPI (Serial Peripheral Interface) hardware in the system. The communication between the user space and the kernel space is facilitated through the kernel's SPI driver. When the program writes to or reads from "/dev/spi", it is, in reality, communicating with the physical SPI hardware via this kernel SPI driver. The "/dev/spi" file acts as an interface between the program and the SPI hardware, encapsulating the complexities of hardware interaction. Any communication log generated in this process, such as spi_log.txt, records the intricate details of these transactions, providing valuable insights for debugging and system optimization purposes.

![Real device schematic](/schematics/real_dev_schematic.png "Real Device Operating Scenerio Schematic")
<p style="text-align: center;"><em>Real Device Operating Scenerio Schematic</em></p>


### Fake device operating scenerio
In the scenario of a fake device operating within a Linux user space, a systematic and interactive process is automated. This process is initiated by a test framework, such as Robot Framework, that starts running different test scenarios sequentially. Each test scenario launches a driver and tester processes, which interact with each other to emulate the actions on the fake device.

The operation begins with the back end of an arbitrary program providing an SPI write sequence to the test scenario. This sequence is then sent to the tester process, which executes these SPI write commands through the driver. The driver, in turn, simulates actions on the fake device, like writing to the device. During this procedure, a log of communications between the driver and tester processes is created. This log, consisting of details such as driver-log and communication registers, is a valuable asset for later analysis and debugging.

Simultaneously, the tester process generates corresponding read lines for each write line. These read lines, when merged with the SPI write lines, form an SPI log. This log is sent back to the back end of the arbitrary program. Based on this SPI log, the front end of the arbitrary program can either manipulate the test process or allow the test to continue as per the scenario. This elaborate interplay between various components provides a high level of control and traceability for the testing process in a user space environment.

![Fake device schematic](/schematics/fake_dev_schematic.png "Fake Device Operating Scenerio Schematic")
<p style="text-align: center;"><em>Fake Device Operating Scenerio Schematic</em></p>


## Getting Started

This guide will assist you in cloning, setting up, and running the project on your local machine for the purposes of development and testing. 

### Prerequisites

In order to successfully install and run this project, you will need to ensure the availability of several tools and libraries on your local development environment. These include:

- **C++ Compiler**, with support for C++ 17 at minimum, along with its default libraries. If you have not installed it yet, you can download it from the [GNU GCC Compiler official website](https://gcc.gnu.org/install/index.html).
- **CMake**, version 3.5 or above, is essential for managing the build process of this software. It can be obtained from the [official CMake website](https://cmake.org/download/).
- **nlohmann/json**, a modern C++ library for parsing and manipulating JSON data structures. Can be downloaded from [its official repository](https://github.com/nlohmann/json)
- **Python3 interpreter** along with its default libraries. It can be downloaded from the [official Python website](https://www.python.org/downloads/).
- **ZeroMQ and MsgPack** libraries for **both C++ and Python**. ZeroMQ is a high-performance asynchronous messaging library, and MessagePack is a binary serialization format. They can be found at their respective official websites.
- Python libraries for handling **JSON** and **YAML** data formats.
- **Robot Framework**, a generic open-source automation framework for acceptance testing, acceptance test-driven development (ATDD), and robotic process automation (RPA). It can be installed via pip, Python's package manager.

Please ensure all these prerequisites are met before proceeding to the installation of the project.

#### Installation for Debian-based OS
Using the bash script below, all the dependencies can easily be installed for Debian-based operation systems.
```bash
sudo apt install build-essential  # C++ Compiler
sudo apt install cmake  # CMake
sudo apt install nlohmann-json3-dev  # nlohmann/json
sudo apt install python3  # Python3 interpreter
sudo apt install libzmq3-dev  # ZeroMQ for C++
pip install pyzmq  # ZeroMQ for Python
pip install msgpack  # MsgPack for Python
pip install pyyaml  # Python libraries for handling JSON and YAML data formats
pip install robotframework  # Robot Framework
```

### Build instructions
1. Install the required libraries and dependencies following the instructions from [Prerequisites](#prerequisites)
2. Clone the repository to your local machine using command `git clone` (Later steps it is assumed you cloned repository to your home directory)
3. Build spi processor C++ wrapper to work with spi devices.
   ```bash
    cd ~/Virtual-Device-Simulator/spi-processing/cpp_wrapper/src/
    cmake -B build
    cd build/
    make
   ```
4. Build process communication interface for C++.
   ```bash
    cd ~/Virtual-Device-Simulator/runtime-environment/process-communication/cpp_comm_interface/src/
    cmake -B build
    cd build/
    make
   ```

5. Build the assembly contains fundamental components of the system.
   ```bash
    cd ~/Virtual-Device-Simulator/runtime-environment/
    python3 build.py 
   ```
6. Build alternative mock process of program to be tested, if you just want to try and look around the system and have no external program.
   ```bash
    cd ~/Virtual-Device-Simulator/program_tested_mock/src/
    cmake -B build
    cd build/
    make
   ```


### Usage instructions

1. Navigate to the runtime environment which is the directory where program runs:
   ```bash
   cd ~/Virtual-Device-Simulator/runtime-environment/
   ```

2. Create your test scenerios copying and modifying example script of test1.py or just write from stratch using the functions in test_suite.py
3. Add your test scenerio scripts to run_tests.py by modifying it to make them visible for robot script.
4. Set pythonpath in order robot script to see the run_tests module.
    ```bash
    export PYTHONPATH=${PYTHONPATH}:$PWD
    ```
5. Run the robot script to perform all the tests while saving logs to /robot_log folder.
   ```bash
    robot --outputdir $PWD/robot_log test_runner.robot
    ```

### Usage notes

* As of the version of 1.0, the software only supports spi device testing with the real spi commands be sent by the computer.
* For using it with a process producing real spi commands, make sure that the external process saves the commands in a file by leaving an empty space between them and at the end, like:
  ```
  spi_write: Bytes written: 6: 0x02 0x00 0x03 0x01 0x00 0xB2
  spi_write: Bytes written: 5: 0x01 0x04 0x03 0x04 0x00

  spi_write: Bytes written: 21: 0x02 0x40 0x04 0x10 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x08 0x00 0x00 0x00 0x00 0x00
  spi_write: Bytes written: 6: 0x02 0x00 0x03 0x01 0x00 0xB4
  spi_write: Bytes written: 18: 0x02 0x30 0x04 0x02 0x00 0x02 0x00 0x34 0x04 0x02 0x00 0x48 0x03 0x00 0x03 0x01 0x00 0xB3
  spi_write: Bytes written: 5: 0x01 0x20 0x05 0x04 0x00

  spi_write: Bytes written: 5: 0x01 0x08 0x05 0x04 0x00

  spi_write: Bytes written: 6: 0x02 0x00 0x03 0x01 0x00 0xAC
  spi_write: Bytes written: 5: 0x01 0x04 0x03 0x04 0x00

  ```
  and also make sure to set the input command sequence file name, directory, and  process executable file name in test scenerio scripts correctly in the functions **prepare_data**, **start_process**.

* There is mock process to mimic the external program and example script test1 is set to perform the test with it. This process is arranged to produce random commands sequence until the test script terminates it. Check out test1.py to see further details.

### Output files software produces

The software generates several output files for debugging purposes. These files provide insights into the software's behavior and can be accessed within the `runtime-environment` directory.

1. **Device File**: You can access the device file, which is manipulated by the software and read data from it, in the `runtime-environment/dev/spi/` directory during runtime.

2. **Log Files**: The software also generates various log files in the `runtime-environment` directory. These logs are helpful for debugging and understanding the software's execution.

    - **real-communication-register**: This log file captures the communication between the computer and the simulated SPI device with timestamps.

    - **communication-register**: This log file resembles the real-communication-register log but contains the commands in the format of the driver.

    - **driver_log**: This file allows you to keep track of the driver movements in a detailed way, including timestamps.

    - **SPI_Log.txt**: This final file contains all the commands and responses to commands sent to the directory of the external program at the end.

These output files serve as valuable resources for debugging and analyzing the software's performance and interactions with the simulated SPI device.

## Software Architecture

The software consists of several components and layers that work together to achieve its functionality, the main 3 of components are briefly:

- **Driver**: The driver component controls the behavior of the fake devices. It interacts with the devices and simulates their responses.

- **Test Scenarios**: The test scenarios define specific test cases and conditions to be executed by the software.

- **Tester**: The tester component executes the defined test scenarios and logs the results for analysis and debugging.

### Directories

The project includes the following directories, each serving a specific purpose:

- `~/runtime-environment`: This directory serves as the central location for building the driver and tester components, as well as executing the test scripts. It is also where the output files are generated.

- `~/runtime-environment/dev-config`: This directory contains various device templates that define the content of the fake devices generated at runtime. By modifying the configuration files (`config.yaml`), you can easily customize the features of the devices. The `device_config_creator.py` script should be executed to generate the device configurations based on the updated templates.

- `~/runtime-environment/dev`: The driver process creates this directory at runtime, and it contains the fake device files. The files are organized into folders such as `/spi`, `/gpio`, etc., representing different types of devices.

- `~/runtime-environment/process-communication`: This directory contains the source code for the communication interfaces in both C++ and Python. The interfaces facilitate efficient communication between processes using the Publisher and Subscriber objects instantiated from the ZeroMQ library. Communication plays a vital role in this multi-layered software architecture.

- `~/spi-processing`: This directory houses the `spi_processor` Python script and its C++ wrapper. It represents the essential part of modeling SPI devices, as it handles real responses and converts between real SPI commands and the driver format.

- `~/driver`: This directory contains the source code for the driver component. Additionally, it includes the following subdirectories for testing:

    - `~/driver/driver-int-tests`: Contains integration tests for the driver component.

    - `~/driver/driver-unit-tests`: Contains unit tests for the driver component.
     
      **Please note that** the test source code is not up to date, currently being reworked and cannot be utilized in this version.

- `~/tester`: This directory contains the source code for the tester component.

- `~/program_tested_mock`: This directory contains the source code of the mock program that utilizes the software. It is built and produces output in the `/env` folder.

- `~/runtime-environment/integration-test`: This directory provides unit and integration tests utilizing the Google Test framework. These tests are designed to ensure the reliability and robustness of the software. The tests can be built and executed using various Python scripts. However, **please note that** the test source code is not up to date, currently being reworked and cannot be utilized in this version.

These directories and components collectively contribute to the functionality and structure of the software, enabling effective control, testing, and analysis of the simulated devices and their interactions.


### Components

#### Driver

The driver component in this software plays a crucial role in simulating and controlling the properties of virtual devices. Although it can be utilized and developed for various purposes, the driver in this specific project is primarily focused on handling writing operations for SPI devices. It is responsible for performing tasks such as writing data, reading data, and configuring the device.

The driver component operates within the `dev/gpio` or `dev/spi` directories, creating and manipulating virtual devices. In the current design, the driver is launched by the test script and maintained and communicated with by the tester component.

During the design stage, special consideration has been given to ensure the driver is memory efficient and maintainable. The intention is to facilitate further development and enhancements in a streamlined manner. In the current version, the driver accepts commands applied to the device as a sequence in its own unique format.

By focusing on writing operations for SPI devices and incorporating features for configuration and data exchange, the driver component enables efficient simulation and control of the virtual devices within the software.

![Driver Working Schematic](/schematics/driver_working.png "Driver Working Schematic")
  <p style="text-align: center;"><em>Driver Working Schematic</em></p>

### Tester

The tester component in this software plays a crucial role in managing the test system. It acts as an intermediary between the test scenario scripts and the driver, ensuring controlled and coordinated execution of the tests. The tester receives inputs from the test scenario scripts through the communication interface and passes them to the driver component in a controlled manner.

One of the important features of the tester is its ability to produce log files that aid in the debugging process. The log files, such as driver_log, communication-register, and SPI_Log.txt, are generated with timestamps. These log files not only facilitate efficient debugging but also provide valuable information to the program being tested, based on the responses received from the driver.

Internally, the tester component utilizes the SPI processor for various utilities. This includes functionalities such as retrieving the correct spi_read response corresponding to a spi_write command and converting real SPI commands to the driver's format. By leveraging the SPI processor, the tester enhances the overall functionality and effectiveness of the test system.

The tester component plays a critical role in ensuring proper coordination, logging, and communication between the test scenario scripts and the driver, ultimately contributing to the successful execution and evaluation of the software under test.

![Tester Working Schematic](/schematics/tester_working.png "Tester Working Schematic")
  <p style="text-align: center;"><em>Tester Working Schematic</em></p>

### Test Scenerio Scripts and Test Framework

The test scenario scripts and test framework are vital components in the software's testing process. The test scripts define how scenarios are executed and under which conditions the fake devices are controlled based on specific embedded programs. The scenarios implemented in the test script utilize the functions provided by test_suite.py.

These test scripts communicate with the tester component through the communication interfaces, enabling the execution of test commands. Each test script represents a specific sequence of commands designed for a particular device, such as turning off the device or configuring it while connected to the computer.

To execute the test scenarios in a systematic and organized manner, the Robot Framework is employed. The Robot Framework allows for the execution of test scripts in a sequential order while providing comprehensive test logs. This framework facilitates the creation and execution of tests with ease, ensuring reliable and thorough testing of the software's functionality.

By combining the test scenario scripts, the test framework, and the communication interfaces, the software can be thoroughly tested under various conditions, generating comprehensive logs for analysis and debugging purposes. This robust testing process helps ensure the reliability and effectiveness of the software.

![Test Scripting Schematic](/schematics/test_scripting.png "Test Scripting Schematic")
  <p style="text-align: center;"><em>Test Scripting Schematic</em></p>



## Documentation

Comprehensive documentation of all the source code can be accessed the directory below in html format.
`Virtual-Device-Simulator\docs\html\index`

## Technologies Used

The project utilizes a range of technologies and frameworks, including the following latest versions:

- [Visual Studio Code 1.75](https://code.visualstudio.com): A powerful source code editor with essential addons, providing a seamless development experience.

- [CMake 3.5](https://cmake.org): A cross-platform build system thReal Device Operating Scenerio Schematicat enables efficient and consistent project compilation and configuration.

- [Doxygen](https://www.doxygen.nl): Used for generating comprehensive documentation, Doxygen helps to automatically generate well-structured and easily accessible documentation for the project.

- [Git](https://git-scm.com): A widely used version control system that provides efficient and reliable management of source code, facilitating collaboration and tracking changes throughout the development process.

- [GPT](https://openai.com/product/gpt-4) 3.5 (Davinci) and 4 models from OpenAI: A powerful language model that utilizes deep learning techniques to generate human-like text, enabling natural language processing and understanding.
  
- [CLion 2022.3](https://www.jetbrains.com/clion/): A cross-platform integrated development environment (IDE) specifically designed for C and C++ development. CLion offers advanced features such as intelligent code completion, debugging capabilities, and seamless integration with the CMake build system.

- [PyCharm 2022.3](https://www.jetbrains.com/pycharm/): An integrated development environment (IDE) for Python, PyCharm offers a rich set of features including code analysis, debugging tools, and support for popular frameworks.

Additionally, the project incorporates the following technologies and frameworks:

- [ZeroMQ](https://zeromq.org): A high-performance messaging library that enables distributed and scalable communication between various components of the system.

- [Msgpack](https://msgpack.org): A binary serialization format that efficiently encodes data structures, facilitating fast and compact data interchange.

- [Robot Framework](https://robotframework.org): A generic open-source automation framework for acceptance testing and robotic process automation (RPA). It offers a simple, keyword-driven syntax and supports testing across different platforms and systems.

- [nlohmann/json 3.11.2](https://github.com/nlohmann/json): A C++ library for working with JSON data, offering modern and intuitive functionality for JSON manipulation.

- [Google Test Framework](https://github.com/google/googletest): A popular C++ testing framework that provides a comprehensive set of tools for writing and running unit tests, enabling efficient and reliable software testing.

These technologies collectively contribute to the development and functionality of the project, ensuring efficient communication, robust testing, comprehensive documentation, and seamless integration within the development environment.



## Contributing

Please read [CONTRIBUTING.md](link) for details on the code of conduct and the process for submitting pull requests.

## Versioning

This project uses [SemVer](http://semver.org/) for versioning. For the available versions, see the [tags on this repository](link).

## Authors

* **[Onur Ulusoy](https://github.com/onurulusoy4)** 

See the full list of [contributors](link) who participated in this project.

## License

This project is licensed under the [MIT License](/LICENSE).