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
            break
        else:
            print(f"{spi_write_file} not found in {remote_directory}. Retrying in {sleep_time} seconds...")
            time.sleep(sleep_time)
    
    return True

def prepare_data_b(spi_a_file, spi_b_file):
    # Read the first line from spi_a_file
    with open(spi_a_file, "r") as file:
        spi_write_line = file.readline().strip()

    read_line = request_sp_read_line(spi_write_line)
    print(read_line)
    
    # Append spi_write_line and a newline character to spi_b_file
    with open(spi_b_file, "a") as file:
        file.write(read_line + "\n\n")
    
    file.close()
    time.sleep(0.1)
    return read_line

def run_program_to_be_tested():
    """
    Execute the program with the prepared test data.
    """
    pass

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
    with open(local_file_path, "r") as file:
        file_lines = file.readlines()

    # Find the first empty line
    first_empty_line_index = file_lines.index("\n")

    # Read data from the beginning to the first empty line
    data_to_send = file_lines[:first_empty_line_index]

    # Delete the sent part from the file
    file_lines = file_lines[first_empty_line_index+1:]
    with open(local_file_path, "w") as file:
        file.writelines(file_lines)

    # Publish the data
    for line in data_to_send:
        publisher.publish(line.strip())
        time.sleep(0.1)

def expect(spi_read_file, subscriber, local_directory=os.getcwd()):
    """
    Compare the received spi_read responses (spi_b) from the tester to the expected spi_read responses to determine
    if the program behaves correctly.

    @param spi_read_file: The file containing expected spi_read responses (spi_b)
    @param subscriber: A Subscriber object to receive messages from the channel
    @param local_directory: The local directory where the spi_read_file is located (default: current working directory)
    """
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

    # Listen to the channel and collect received messages
    for _ in range(len(expected_responses)):
        message = subscriber.receive()
        received_responses.append(message)

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

def run_tester_and_driver(local_directory=os.getcwd()):
    """
    Starts processes tester and driver ./tester.out and ./driver.out in separate consoles.
    """
    tester_path = os.path.join(local_directory, "tester.out")
    driver_path = os.path.join(local_directory, "driver.out")

    command_template = 'gnome-terminal -- bash -c "{}; exec bash"'

    tester_command = command_template.format(tester_path)
    driver_command = command_template.format(driver_path)

    subprocess.run(tester_command, shell=True, cwd=local_directory)
    subprocess.run(driver_command, shell=True, cwd=local_directory)


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
