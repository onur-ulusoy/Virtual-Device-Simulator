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
  - [Documentation](#documentation)
  - [Built With](#built-with)
  - [Contributing](#contributing)
  - [Versioning](#versioning)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Understanding Linux Devices
Linux, as an open-source operating system kernel, provides a unique and versatile structure for managing devices. Devices in Linux are treated as files and are accessed through file systems, illustrating the philosophy of "everything is a file." This structure includes various types of devices such as hard drives, RAM, USBs, printers, and even keyboards. The device files are typically housed in the "/dev" directory, each having specific permissions and ownership. Understanding Linux devices involves a comprehension of these file-based device interactions, the different categories of devices, and the permissions that control their accessibility. This foundational knowledge is key to effective system administration and troubleshooting within a Linux environment.
### Linux devices operating scenerio
In the Linux operating scenario, an arbitrary program interacts with a device file, in this case "/dev/spi", located within the user space. This file essentially represents a physical SPI (Serial Peripheral Interface) hardware in the system. The communication between the user space and the kernel space is facilitated through the kernel's SPI driver. When the program writes to or reads from "/dev/spi", it is, in reality, communicating with the physical SPI hardware via this kernel SPI driver. The "/dev/spi" file acts as an interface between the program and the SPI hardware, encapsulating the complexities of hardware interaction. Any communication log generated in this process, such as spi_log.txt, records the intricate details of these transactions, providing valuable insights for debugging and system optimization purposes.

![Real device schematic](/schematics/real_dev_schematic.png)


### Fake device operating scenerio
In the scenario of a fake device operating within a Linux user space, a systematic and interactive process is automated. This process is initiated by a test framework, such as Robot Framework, that starts running different test scenarios sequentially. Each test scenario launches a driver and tester processes, which interact with each other to emulate the actions on the fake device.

The operation begins with the back end of an arbitrary program providing an SPI write sequence to the test scenario. This sequence is then sent to the tester process, which executes these SPI write commands through the driver. The driver, in turn, simulates actions on the fake device, like writing to the device. During this procedure, a log of communications between the driver and tester processes is created. This log, consisting of details such as driver-log and communication registers, is a valuable asset for later analysis and debugging.

Simultaneously, the tester process generates corresponding read lines for each write line. These read lines, when merged with the SPI write lines, form an SPI log. This log is sent back to the back end of the arbitrary program. Based on this SPI log, the front end of the arbitrary program can either manipulate the test process or allow the test to continue as per the scenario. This elaborate interplay between various components provides a high level of control and traceability for the testing process in a user space environment.

![Fake device schematic](/schematics/fake_dev_schematic.png)


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

## How it works









## Documentation

Comprehensive documentation of all the source code can be accessed the directory below in html format.
`Virtual-Device-Simulator\docs\html\index`

## Technologies Used

The project utilizes a range of technologies and frameworks, including the following latest versions:

- [Visual Studio Code 1.75](https://code.visualstudio.com): A powerful source code editor with essential addons, providing a seamless development experience.

- [CMake 3.5](https://cmake.org): A cross-platform build system that enables efficient and consistent project compilation and configuration.

- [Doxygen](https://www.doxygen.nl): Used for generating comprehensive documentation, Doxygen helps to automatically generate well-structured and easily accessible documentation for the project.

- [Git](https://git-scm.com): A widely used version control system that provides efficient and reliable management of source code, facilitating collaboration and tracking changes throughout the development process.

- [ChatGPT](https://chat.openai.com/) 3.5 (Davinci) and 4 models from OpenAI: A powerful language model that utilizes deep learning techniques to generate human-like text, enabling natural language processing and understanding.
  
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

This project is licensed under the [MIT License](https://opensource/MIT).
## Acknowledgments

This field will be filled later
