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

def expect(spi_read_file):
    """
    Compare the received spi_read responses (spi_b) from the tester to the expected spi_read responses to determine
    if the program behaves correctly.

    @param spi_read_file: The file containing expected spi_read responses (spi_b)
    """
    pass

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