"""
The script implements a program for compiling and running source files for a driver and a tester concurrently by running different processes.
It also allows the user to choose which device to simulate among several options.
"""

##
# \mainpage Virtual Device Simulator Documentation
#
# The main objective of this project is to develop a software platform that enables the creation, manipulation, and use of virtual devices in the user space of an operating system. The goal is to make it easier for developers to create and test embedded software without having to use physical hardware. This will save time and resources and allow for more efficient and streamlined software development processes. The platform will be designed to be flexible and user-friendly, making it accessible to a wide range of developers with varying skill levels.
#
#

## @namespace start 
# @brief Runs the compiling and running processes of the system
import sys
from os import system, path
from subprocess import Popen
from threading import Thread

## List of paths of source folders to append to the system path
paths = ['driver', 'tester']
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
## List of tester source files to compile
tester_cpp_files = cpp_files = [ "testermain.cpp",
                                 "libtesterutility.cpp", 
                                 "libtesterutility.h", 
                                 "testercommapi.cpp", 
                                 "testercommapi.h" ]
## Output file name for the compiled tester
tester_exe_file = "tester.out"

def compile_and_run(cpp_files, exe_file, sh, dir):
    ##
    # @brief Compile and run the given cpp files.
    # The compiled program is stored in the exe_file.
    #
    # @param cpp_files: list of cpp files to compile
    # @param exe_file: output file name for the compiled program
    # @param sh: boolean indicating whether to run the program in shell
    # @param dir: directory of the cpp files
    #
    cpp_file = str()
    
    for i in cpp_files:
        cpp_file += "../" + dir + "/" + i + " "
        
    cpp_file.rstrip()

    system('g++ ' + cpp_file + ' -o ' + exe_file)
    print("Build finished.")
    process = Popen("./{}".format(exe_file), shell=sh)

    out, err = process.communicate()
    errcode = process.returncode

    process.kill()
    process.terminate()

def compile(cpp_files, exe_file, dir):
    ##
    # @brief Compile the given cpp files.
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

def run(exe_file, sh):
    ##
    # @brief Runs the specified executable file.
    #
    # This function opens a new process for the specified executable file using Popen, sets the shell
    # parameter based on the value of the `sh` argument, communicates with the process to retrieve the
    # standard output and standard error, and terminates the process.
    #
    # @param exe_file The path to the executable file.
    # @param sh Whether or not to run the process in a shell.
    #
    # @return The standard output and standard error from the process, and the return code.
    #
    process = Popen("./{}".format(exe_file), shell=sh)

    out, err = process.communicate()
    errcode = process.returncode

    process.kill()
    process.terminate()

## Main function for the program
# 
# This function provides three options to the user: compile, run, and compile_run.
# Depending on the user's choice, the appropriate action is performed. The function uses
# threads to perform actions in parallel and communicates with other threads using a file.
# It also creates proper directories for virtual devices that will be simulated.
def main():
    try:
        request = sys.argv[1]
    except IndexError:
        request = input("Enter one of them:\ncompile\nrun\ncompile_run\n")

    for PATH in paths:
        sys.path.append(path.join(path.dirname(__file__), PATH))
            
    if request == "compile":
        print("\ntester is being built ...")

        tester = Thread(target=compile, args=(tester_cpp_files, tester_exe_file, "tester"))
        tester.start()

        print("driver is being built ...")

        program = Thread(target=compile, args=(driver_cpp_files, driver_exe_file, "driver"))
        program.start()

    elif request == "run":
        files = ['driver.out', 'tester.out']

        for file in files:
            if not path.exists(file):
                print("Out files are missing.")
                sys.exit()

        device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

        file = open("command", "w", encoding="utf-8")
        file.write(device)
        file.close()

        program = Thread(target=run, args=(driver_exe_file, True))
        program.start()

        file = open("command", "r", encoding="utf-8")

        while file.readline() != "start":
            file.seek(0, 0)

        file.close()
        file = open("command", "w", encoding="utf-8")
        file.write("&")
        file.close()

        program = Thread(target=run, args=(tester_exe_file, True))
        program.start()

    else:
        device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

        file = open("command", "w", encoding="utf-8")
        file.write(device)
        file.close()

        print("driver is being built ...")

        program = Thread(target=compile_and_run, args=(driver_cpp_files, driver_exe_file, True, "driver"))
        program.start()

        file = open("command", "r", encoding="utf-8")

        while file.readline() != "start":
            file.seek(0, 0)

        file.close()
        file = open("command", "w", encoding="utf-8")
        file.write("&")
        file.close()

        print("\ntester is being built ...")

        program = Thread(target=compile_and_run, args=(tester_cpp_files, tester_exe_file, True, "tester"))
        program.start()

if __name__ == "__main__":
    main()




    

    

    

