# Virtual Device Simulator

The main objective of this project is to develop a software platform that enables the creation, manipulation, and use of virtual devices in the user space of an operating system. The goal is to make it easier for developers to create and test embedded software without having to use physical hardware. This will save time and resources and allow for more efficient and streamlined software development processes. The platform will be designed to be flexible and user-friendly, making it accessible to a wide range of developers with varying skill levels.
## Table of Contents

- [Virtual Device Simulator](#virtual-device-simulator)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
  - [Usage](#usage)
  - [Documentation](#documentation)
  - [Built With](#built-with)
  - [Contributing](#contributing)
  - [Versioning](#versioning)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The following tools are required to get started with this project:

- [C++ Compiler](https://gcc.gnu.org/install/index.html)
- [nlohmann/json](https://github.com/nlohmann/json) header-only library for JSON
- [Python](https://www.python.org/downloads/)

Make sure you have the latest versions of these tools installed on your machine before proceeding with the installation.


### Installation

1. Clone the repository to your local machine using the following command:
```
$ git clone https://github.com/onurulusoy4/Virtual-Device-Simulator.git
```
2. Install the required libraries and dependencies by running the following command:
```
$ git clone https://github.com/nlohmann/json.git
$ cd json
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

## Understanding Devices

...

### Linux devices operating scenerio
...

![Real device schematic](schematics/real_device_schematic.png)

### Fake device operating scenerio
...

![Fake device schematic](schematics/fake_device_schematic.png)





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
