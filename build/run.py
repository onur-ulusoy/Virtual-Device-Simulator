"""
The script implements a program for running compiled files for a driver and a tester concurrently by running different processes.
It also allows the user to input a command.
"""
## @namespace run 
# @brief Runs the machine codes belongs to driver and tester concurrently and manages them.
import sys
from os import path
from subprocess import Popen
from threading import Thread


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

def main():
    ## Main function for the program
    # 
    # Runs the machine codes belongs to driver and tester. The function uses
    # threads to perform actions in parallel and communicates with other threads using a file.
    # It also creates proper directories for virtual devices that will be simulated.

    ## List of paths of source folders to append to the system path
    paths = ['driver', 'tester']
    
    for PATH in paths:
        sys.path.append(path.join(path.dirname(__file__), PATH))

    ## Output file names for the compiled tester and driver
    files = ['driver.out', 'tester.out']

    for file in files:
        if not path.exists(file):
            print("Out files are missing.")
            sys.exit()

    device = input("Select device to simulate: gpio, spi, i2c, ethernet, usart, uart, can\n")

    file = open("command", "w", encoding="utf-8")
    file.write(device)
    file.close()
    ## Output file name for the compiled driver 
    exe_file = "driver.out"

    program = Thread(target=run, args=(exe_file, True))
    program.start()

    file = open("command", "r", encoding="utf-8")

    while file.readline() != "start":
        file.seek(0, 0)

    file.close()
    file = open("command", "w", encoding="utf-8")
    file.write("&")
    file.close()
    ## Output file name for the compiled tester 
    exe_file = "tester.out"

    tester = Thread(target=run, args=(exe_file, True))
    tester.start()

if __name__ == "__main__":
    main()
    

