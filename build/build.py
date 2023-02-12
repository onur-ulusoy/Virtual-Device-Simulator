"""
The script implements a program for compiling source files for a driver and a tester concurrently by running different processes.
"""

##
# \mainpage Virtual Device Simulator Documentation
#
# The main objective of this project is to develop a software platform that enables the creation, manipulation, and use of virtual devices in the user space of an operating system. The goal is to make it easier for developers to create and test embedded software without having to use physical hardware. This will save time and resources and allow for more efficient and streamlined software development processes. The platform will be designed to be flexible and user-friendly, making it accessible to a wide range of developers with varying skill levels.
#
#

## @namespace build 
# @brief Builds the software into machine code and create .out files

import sys
from os import system, path
from threading import Thread

def build(cpp_files, exe_file, dir):
    ##
    # @brief Compiles the given cpp files.
    # The compiled program is stored in the exe_file.
    #
    # @param cpp_files: list of cpp files to compile
    # @param exe_file: output file name for the compiled program
    # @param dir: directory of the cpp files
    #
    cpp_file = str()
    
    for i in cpp_files:
        cpp_file += "../" + dir + "/" + i + " "
        
    cpp_file.rstrip()

    system('g++ ' + cpp_file + ' -o ' + exe_file)
    print(dir + " build finished.")

## Main function for the program
# 
# Builds all the source file of the software. 
def main():
    ## List of paths of source folders to append to the system path
    paths = ['driver', 'tester']

    for PATH in paths:
        sys.path.append(path.join(path.dirname(__file__), PATH))
    ## List of tester source files to compile
    tester_cpp_files = [  "testermain.cpp",
                   "libtesterutility.cpp", 
                   "libtesterutility.h", 
                   "testercommapi.cpp", 
                   "testercommapi.h" ]
    ## Output file name for the compiled tester               
    tester_exe_file = "tester.out"

    print("\ntester is being built ...")

    tester = Thread(target=build, args=(tester_cpp_files, tester_exe_file, "tester"))
    tester.start()
    
    ## List of driver source files to compile
    driver_cpp_files = [ "drivermain.cpp",
                   "libdriver.cpp", 
                   "libdriver.h", 
                   "libdriverutility.cpp", 
                   "libdriverutility.h", 
                   "drivercommapi.cpp", 
                   "drivercommapi.h" ]
    ## Output file name for the compiled driver
    driver_exe_file = "driver.out"

    print("driver is being built ...")

    program = Thread(target=build, args=(driver_cpp_files, driver_exe_file, "driver"))
    program.start()

if __name__ == "__main__":
    main()

    


    

