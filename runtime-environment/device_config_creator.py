import yaml
import json
import sys

def create_device_config(device_amount, config_file, output_file):
    with open(config_file, 'r') as yaml_file:
        config_data = yaml.safe_load(yaml_file)

    devices = []
    for i in range(device_amount):
        devices.append(config_data)

    keys = list(config_data.keys())

    output_data = {
        "Devices": devices,
        "keys": keys
    }

    with open(output_file, 'w') as json_file:
        json.dump(output_data, json_file, indent=2)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 device_config_creator.py <device_amount>")
        sys.exit(1)

    device_amount = int(sys.argv[1])

    config_file = "dev-config/yaml_configs/gpio_config.yaml"
    output_file = "dev-config/json_configs/gpio_config.json"

    create_device_config(device_amount, config_file, output_file)
