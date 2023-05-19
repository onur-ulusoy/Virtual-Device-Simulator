"""
The script implements a program for compiling source files for a driver and a tester concurrently by running different processes.
"""

##
# \mainpage Virtual Device Simulator Documentation
#
# The main objective of this project is to develop a software platform that enables the creation, manipulation, and use of virtual devices in the user space of an operating system. The goal is to make it easier for developers to create and test embedded software without having to use actual physical hardware. This will save time and resources and allow for more efficient and streamlined software development processes. The platform is designed to be flexible and user-friendly, making it accessible to a wide range of programmers who develop embedded linux applications. Virtual Device Simulator also includes features for automated testing and validation. This allows developers to automatically test communication between the embedded software and the virtual peripheral device, verify the accuracy of data transmission, and ensure the correct functioning of all software components with arrangable test scenerios while providing realistic log files for easy debug.
#

## @namespace build 
# @brief Builds the software into machine code and create .out files

from os import system, chdir

def build():
    ## Run cmake command
    cmake_command = 'cmake -B build'
    system(cmake_command)

    ## Change directory to build/
    chdir('./build')

    ## Run make command
    make_command = 'make'
    system(make_command)

## Main function for the program
# 
# Builds all the source file of the software. 
def main():
    
    #print("\ntester is being built ...")
    #print("driver is being built ...\n")

    build()

    print("\ntester build finished.\n")
    print("driver build finished.\n")


if __name__ == "__main__":
    main()

    


    

