#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
@file test_suite.py

This module provides a set of functions for a series of test scenarios on a program that interacts with
an SPI device. The test suite is responsible for preparing test data, running the program to be tested, sending data to
the program, and verifying the received responses against expected responses.

@author: Onur Ulusoy
@date: 22/04/2023
@license: MIT
"""

import os
import subprocess
import shutil
import time
import sys
from pathlib import Path
import signal

sys.path.append('process-communication')
from comm_interface import *

def prepare_data(spi_write_file, remote_directory, local_directory=os.getcwd(), time_out=10, sleep_time=1):
    start_time = time.time()
    
    while True:
        elapsed_time = time.time() - start_time
        
        if elapsed_time > time_out:
            print(f"time_out of {time_out} seconds exceeded. {spi_write_file} not found in {remote_directory}.")
            return False
        
        remote_file_path = os.path.join(remote_directory, spi_write_file)
        local_file_path = os.path.join(local_directory, spi_write_file)

        if os.path.exists(remote_file_path):
            shutil.copyfile(remote_file_path, local_file_path)
            print(f"{spi_write_file} has been copied from {remote_directory} to {local_directory}")
            
            # Duplicate the file
            duplicated_file_path = os.path.join(local_directory, "SPI_Log.txt")
            shutil.copyfile(local_file_path, duplicated_file_path)
            print(f"{spi_write_file} has been duplicated as SPI_Log.txt in {local_directory}")
            break
        else:
            print(f"{spi_write_file} not found in {remote_directory}. Retrying in {sleep_time} seconds...")
            time.sleep(sleep_time)
    
    return True

def prepare_data_b(spi_a_file, spi_b_file):
    run_sp_with_f_flag()

    # Read lines from spi_a_file and group them into solid parts
    solid_parts = []
    current_part = []
    with open(spi_a_file, "r") as file:
        for line in file:
            line = line.strip()
            if line:
                current_part.append(line)
            else:
                if current_part:
                    solid_parts.append(current_part)
                    current_part = []

    # Add the last solid part if it's not empty
    if current_part:
        solid_parts.append(current_part)

    # Process and append each solid part to spi_b_file
    with open(spi_b_file, "w") as file:
        for solid_part in solid_parts:
            spi_write_lines = "\n".join(solid_part)
            read_line = request_sp_read_line(spi_write_lines)

            time.sleep(0.1)
            
            file.write(read_line + "\n")

            # Add an empty line after processing each solid part
            file.write("\n")

    time.sleep(0.1)

    request_sp_read_line("TERMINATE")

def run_process(directory_path, process_name):
    """
    Execute the specified program in the specified directory.
    """
    program_path = os.path.join(directory_path, process_name)

    command_template = 'cd {} && exec {}'

    program_command = command_template.format(directory_path, program_path)
    program_process = subprocess.Popen(['gnome-terminal', '--', 'bash', '-c', program_command])

    return program_process.pid

def continue_process(directory_path):
    # Define the path of the verification file
    verification_file = os.path.join(directory_path, "verification")

    # Continuously write "verification OK" to the file until interrupted
    while True:
        with open(verification_file, "w") as f:
            f.write("OK")
            time.sleep(0.1)

def stop_process(directory_path):
    # Define the path of the verification file
    verification_file = os.path.join(directory_path, "verification")

    # Write "EXIT" to the file to stop the process
    with open(verification_file, "w") as f:
        f.write("EXIT")

def send_data_when_asked(spi_write_file, subscriber, publisher, local_directory=os.getcwd()):
    """
    Send the spi_write commands (spi_a) to the program to be tested.

    @param spi_write_file: The file containing spi_write commands (spi_a)
    @param subscriber: The Subscriber object to receive the signal to send data
    @param publisher: The Publisher object to send the spi_write commands (spi_a)
    @param local_directory: The local directory where the file is located (default: current working directory)
    """

    # Wait for the signal to ask for data
    subscriber.receive()

    # Read data from the spi_write_file
    local_file_path = os.path.join(local_directory, spi_write_file)

    data_to_send = []
    with open(local_file_path, "r") as file:
        while True:
            line = file.readline()
            if line == "\n":
                break
            data_to_send.append(line)
        remaining_file_lines = file.readlines()

    # Delete the sent part from the file
    with open(local_file_path, "w") as file:
        file.writelines(remaining_file_lines)

    # Join the data_to_send list into a single string
    data_to_send_str = "".join(data_to_send)

    # Publish the data
    publisher.publish(data_to_send_str)
    time.sleep(0.1)

def expect(spi_read_file, subscriber, local_directory=os.getcwd()):
    # Read data from the spi_read_file
    local_file_path = os.path.join(local_directory, spi_read_file)
    with open(local_file_path, "r") as file:
        file_lines = file.readlines()

    # Find the first empty line
    first_empty_line_index = file_lines.index("\n")

    # Read data from the beginning to the first empty line
    expected_responses = file_lines[:first_empty_line_index]

    # Delete the read part from the file
    file_lines = file_lines[first_empty_line_index+1:]
    with open(local_file_path, "w") as file:
        file.writelines(file_lines)

    received_responses = []

    # SPI_Log.txt file path
    spi_log_file_path = os.path.join(local_directory, 'SPI_Log.txt')

    # Listen to the channel and collect received messages
    for _ in range(len(expected_responses)):
        message = subscriber.receive()
        received_responses.append(message)
        write_received_responses_to_log(spi_log_file_path, message)

    # Compare received messages with expected responses and print the results
    for expected, received in zip(expected_responses, received_responses):
        expected_parts = expected.strip().split(':', 1)
        received_parts = received.split(':', 1)
        
        if len(expected_parts) == 2 and len(received_parts) == 2 and expected_parts[1].strip() == received_parts[1].strip():
            print(f"Match: Expected: {expected.strip()} | Received: {received}")
        else:
            print(f"Mismatch: Expected: {expected.strip()} | Received: {received}")

    # Check if SPI_B.txt is empty
    with open(local_file_path, 'r') as file:
        if not file.read(1):
            os.remove(local_file_path)  # Remove the empty file

def write_received_responses_to_log(log_file_path, received_response):
    with open(log_file_path, 'r') as log_file:
        log_lines = log_file.readlines()

    for i, line in enumerate(log_lines):
        if line == "\n":
            log_lines[i] = received_response + '\n'
            break

    with open(log_file_path, 'w') as log_file:
        log_file.writelines(log_lines)

def run_assembly(local_directory=os.getcwd()):
    """
    Starts processes tester and driver ./tester.out and ./driver.out in separate consoles.
    Returns the PIDs of the processes.
    """
    tester_path = os.path.join(local_directory, "tester.out")
    driver_path = os.path.join(local_directory, "driver.out")

    command_template = 'exec {}'

    tester_command = command_template.format(tester_path)
    driver_command = command_template.format(driver_path)

    driver_process = subprocess.Popen(['gnome-terminal', '--', 'bash', '-c', driver_command])
    time.sleep(0.5)
    tester_process = subprocess.Popen(['gnome-terminal', '--', 'bash', '-c', tester_command])

    return tester_process.pid, driver_process.pid

def kill_assembly():
    """
    Terminates the tester.out and driver.out processes.
    """
    # Give the processes some time to start and write their PIDs to the files
    time.sleep(1)

    # Read the PIDs from the files
    with open('tester.pid', 'r') as f:
        tester_pid = int(f.read().strip())

    with open('driver.pid', 'r') as f:
        driver_pid = int(f.read().strip())

    # Kill the processes
    os.kill(tester_pid, signal.SIGTERM)
    os.kill(driver_pid, signal.SIGTERM)

def wait_response():
    """
    Waits response from front-end of the program to be tested.
    """
    pass

def request_sp_read_line(write_line):
    working_path = os.getcwd()

    # Change directory to spi processor workpath
    spi_processor_workpath = "../spi-processing"
    abs_spi_processor_workpath = Path(spi_processor_workpath).resolve()
    os.chdir(abs_spi_processor_workpath)

    in_filepath = Path("in")
    out_filepath = Path("out")

    # Save write_line to "in" file
    with in_filepath.open("w") as in_file:
        in_file.write(write_line)

    read_line = ""
    identifier = ""

    # Loop until a valid spi_read line is found
    while True:
        if out_filepath.exists():
            with out_filepath.open("r") as out_file:
                read_line = out_file.readline().strip()
            identifier = read_line[:8]
            if identifier == "spi_read" or identifier == "TERMINAT":
                break
        time.sleep(0.01)

    # Recreate the "in" file
    with in_filepath.open("w"):
        pass
    
    os.chdir(working_path)
    return read_line

def run_sp_with_f_flag():
    working_path = os.getcwd()
    
    # Change directory to spi processor workpath
    spi_processor_workpath = "../spi-processing"
    abs_spi_processor_workpath = Path(spi_processor_workpath).resolve()
    os.chdir(abs_spi_processor_workpath)

    # Create or recreate "in" and "out" files
    in_filepath = Path("in")
    out_filepath = Path("out")

    with open(in_filepath, 'w') as _:
        pass
    with open(out_filepath, 'w') as _:
        pass

    # Run spi_processor.py with -f flag in the background without console output
    command = "python3 spi_processor.py -f > /dev/null 2>&1 &"
    try:
        subprocess.Popen(command, shell=True)
    except Exception as e:
        print(f"Failed to run spi_processor.py: {e}")

    os.chdir(working_path)

def copy_spi_log_to_destination(destination_directory):
    source_file = 'SPI_Log.txt'
    
    if not os.path.exists(destination_directory):
        os.makedirs(destination_directory)
        
    destination_file = os.path.join(destination_directory, 'SPI_Log.txt')
    shutil.copy2(source_file, destination_file)

# Define communication topics
signal_topic = "tcp://localhost:5555"
write_data_topic = "tcp://localhost:5557"
read_data_topic = "tcp://localhost:5559"

# Create a Subscriber object to listen for the signal to send data
signal_listener = Subscriber(signal_topic)

# Create a Publisher object to send the data
data_supplier = Publisher(write_data_topic, "test")

# Create a Subscriber object to receive the read data
data_listener = Subscriber(read_data_topic)

# Define the spi_write and spi_read file names
spi_write_file = "SPI_A.txt"
spi_read_file = "SPI_B.txt"

# Get the current working directory
local_directory = os.getcwd()