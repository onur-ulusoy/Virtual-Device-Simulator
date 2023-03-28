#!/usr/bin/env python3
"""
@brief This script reads a YAML configuration file for a specific device and generates a JSON file
containing the configuration information for multiple instances of that device.

@file device_config_creator.py
@author Onur Ulusoy
@date 28/03/2023
@license MIT
"""

import sys
import os.path
import yaml
import json


class DeviceConfigCreator:
    """
    @brief Class that creates device configurations from a YAML file and writes them to a JSON file
    """

    def __init__(self, config_file):
        """
        @brief Constructor that reads the YAML configuration file for the device
        @param config_file Path to the YAML configuration file
        """
        with open(config_file, 'r') as file:
            self.config = yaml.safe_load(file)

    def create_devices(self, device_amount, device_type):
        """
        @brief Creates device configurations for multiple instances of the device
        @param device_amount Number of device instances to create
        @return List of device configurations
        """
        devices = []
        for i in range(device_amount):
            device = {}
            for key in self.config:
                device[key] = self.config[key]
            device["offset"] = i
            device["name"] = device_type.upper() + "_" + str(i)
            devices.append(device)
        return devices

    def write_json_file(self, devices, output_file):
        """
        @brief Writes the device configurations to a JSON file
        @param devices List of device configurations
        @param output_file Path to the output JSON file
        """
        data = {"Devices": devices, "keys": list(self.config.keys())}
        with open(output_file, 'w') as file:
            json.dump(data, file, indent=2)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 device_config_creator.py <device_type> <device_amount>")
        sys.exit(1)

    device_type = sys.argv[1]
    device_amount = int(sys.argv[2])

    # Check if the device configuration file exists
    config_file = f"dev-config/config_yaml/{device_type}_config.yaml"
    if not os.path.exists(config_file):
        print(f"Error: Configuration file {config_file} does not exist.")
        sys.exit(1)

    output_file = f"dev-config/config_json/{device_type}_config.json"

    creator = DeviceConfigCreator(config_file)
    devices = creator.create_devices(device_amount, device_type)
    creator.write_json_file(devices, output_file)
