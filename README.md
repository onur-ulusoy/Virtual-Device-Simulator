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
    - [For Debian-based OS](#for-debian-based-os)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Starting modes:](#starting-modes)
    - [After program has run,](#after-program-has-run)
      - [There are commands below user can call:](#there-are-commands-below-user-can-call)
    - [Example Usage](#example-usage)
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

## Prerequisites

In order to successfully install and run this project, you will need to ensure the availability of several tools and libraries on your local development environment. These include:

- **C++ Compiler**, with support for C++ 17 at minimum, along with its default libraries. If you have not installed it yet, you can download it from the [GNU GCC Compiler official website](https://gcc.gnu.org/install/index.html).
- **CMake**, version 3.5 or above, is essential for managing the build process of this software. It can be obtained from the [official CMake website](https://cmake.org/download/).
- **nlohmann/json**, a modern C++ library for parsing and manipulating JSON data structures. Can be downloaded from [its official repository](https://github.com/nlohmann/json)
- **Python3 interpreter** along with its default libraries. It can be downloaded from the [official Python website](https://www.python.org/downloads/).
- **ZeroMQ and MsgPack** libraries for **both C++ and Python**. ZeroMQ is a high-performance asynchronous messaging library, and MessagePack is a binary serialization format. They can be found at their respective official websites.
- Python libraries for handling **JSON** and **YAML** data formats.
- **Robot Framework**, a generic open-source automation framework for acceptance testing, acceptance test-driven development (ATDD), and robotic process automation (RPA). It can be installed via pip, Python's package manager.

Please ensure all these prerequisites are met before proceeding to the installation of the project.

### For Debian-based OS
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

## Installation
1. Install the required libraries and dependencies.
2. Clone the repository to your local machine.
3. 





## Usage

1. Navigate to the project directory:
```
$ cd test-environment/
```
2. Run the Python script
```
$ python3 start.py buildrun
```

### Starting modes:
There are three arguments that can be used when running the Python script:
- `compile`: Builds the driver and tester
- `run`: Runs the driver and tester
- `compilerun`: Builds and runs the driver and tester

Example:
```
$ python3 start.py compile
```

### After program has run,

#### There are commands below user can call:
Assuming that gpio device is simulated,
- `show-gpiochipx`: shows the `gpiox` set
- `read-gpiochipx-offset-property`: reads the `gpiox` specific property from an offset
- `write-gpiochipx-offset-property-newValue`: writes a new value to the `gpiox` specific property from an offset
- `config-gpiochipx`: configures the `gpiox` set from the default
- `.commandSet-textfile`: executes commands from a file

Note that you should create command set text file in the the directory of test-environment to make the program run commands from text file.

All the commands after it is excecuted successfully, are committed to `log` file to futher examination.

All the communication between driver and tester is committed to `communication-register` file.

Virtual devices can be accessed in the directory `test-environment\dev\devfolder\device` after device is initialized.

### Example Usage

This field will be filled later

## Documentation

Comprehensive documentation of all the source code can be accessed the directory below in html format.
`Virtual-Device-Simulator\docs\html\index`

## Built With

- [Clion 2022.3](https://www.jetbrains.com/clion/) - Cross-platform IDE for C and C++
- [Pycharm 2022.3](https://www.jetbrains.com/pycharm/) - Python IDE
- [Visual Studio Code 1.75](https://code.visualstudio.com) - Source code editor
- [Cmake 3.25](https://cmake.org) - Cross-platform build system
- [nlohmann/json 3.11.2](https://github.com/nlohmann/json) - JSON for Modern C++
- [Doxygen](https://www.doxygen.nl) - Used for generating documentation
- [Git](https://git-scm.com) - Version Control System


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
