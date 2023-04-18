import argparse
import hashlib
import json
import os
import time
import yaml
class SpiFileProcessor:
    def __init__(self):
        self.config = self.load_config()

        self.spi_data_file = self.config["spi_data_file"]
        self.input_file_find_mode = self.config["input_file_find_mode"]
        self.output_file_find_mode = self.config["output_file_find_mode"]
        self.input_file_input_mode = self.config["input_file_input_mode"]

        self.sleep_time_find_mode = self.config["sleep_time_find_mode"]

        self.spi_write_data = ""

    def load_config(self):
        config_file = "config.yaml"

        if os.path.exists(config_file):
            with open(config_file, 'r') as yaml_file:
                config = yaml.safe_load(yaml_file)
        else:
            assert("Config file cannot be found.")

        return config

    def process_spi(self):
        with open(self.input_file_input_mode, 'r') as infile:
            for line in infile:
                line = line.strip()  # Remove extra spaces and newlines
                if line.startswith("spi_write"):
                    self.spi_write_data += line + '\n'  # Add newline after each line
                elif line.startswith("spi_read"):
                    self.save_to_json(self.spi_write_data.strip(), line)  # Remove extra spaces and newlines
                    self.spi_write_data = ""
            # Save the remaining spi_write_data
            if self.spi_write_data:
                self.save_to_json(self.spi_write_data.strip(), "")

    def save_to_json(self, spi_write_data, spi_read_line):
        table_name = self.encrypt_write_data()

        if os.path.exists(self.spi_data_file):
            with open(self.spi_data_file, "r") as infile:
                self.spi_data = json.load(infile)
        else:
            self.spi_data = {}

        if table_name not in self.spi_data:
            self.spi_data[table_name] = []

        if not self.spi_data[table_name] or self.spi_data[table_name][-1]["spi_read_line"] != spi_read_line:
            new_entry = {
                "spi_write_line": spi_write_data,
                "spi_read_line": spi_read_line,
                "entry_count": 1,
            }
            self.spi_data[table_name].append(new_entry)
        else:
            self.spi_data[table_name][-1]["entry_count"] += 1

        with open(self.spi_data_file, "w") as outfile:
            json.dump(self.spi_data, outfile, indent=4)

    def get_associated_spi_read(self, spi_write_data):
        spi_read_lines = []

        if spi_write_data in self.spi_tree:
            all_zero = all(iteration_number == 0 for _, _, iteration_number in self.spi_tree[spi_write_data])

            if all_zero:
                spi_read_line, entry_count, _ = self.spi_tree[spi_write_data][0]
                self.spi_tree[spi_write_data][0] = (spi_read_line, entry_count, 1)

            for i, (spi_read_line, entry_count, iteration_number) in enumerate(self.spi_tree[spi_write_data]):
                if iteration_number != 0:
                    spi_read_lines.append(spi_read_line)
                    # Increment iteration_number by 1
                    new_iteration_number = iteration_number + 1

                    # If iteration count == entry count, make it 0 and set the next cell's iteration to 1
                    if new_iteration_number > entry_count:
                        new_iteration_number = 0
                        if i + 1 < len(self.spi_tree[spi_write_data]):
                            next_spi_read_line, next_entry_count, _ = self.spi_tree[spi_write_data][i + 1]
                            self.spi_tree[spi_write_data][i + 1] = (next_spi_read_line, next_entry_count, 1)

                    self.spi_tree[spi_write_data][i] = (spi_read_line, entry_count, new_iteration_number)
                    break  # exit the loop once we've found the entry with a non-zero iteration number
        self.print_spi_tree()
        return spi_read_lines
    
    def create_tree_from_json(self):
        json_file = 'spi_data.json'
        with open(json_file, 'r') as f:
            self.spi_data = json.load(f)

        self.spi_tree = {}
        for table_name, rows in self.spi_data.items():
            for i, row in enumerate(rows):
                spi_write_line, spi_read_line, entry_count = row.values()
                if spi_write_line not in self.spi_tree:
                    self.spi_tree[spi_write_line] = []
                iteration_number = 1 if i == 0 else 0
                self.spi_tree[spi_write_line].append((spi_read_line, entry_count, iteration_number))

    def print_spi_tree(self):
        # ANSI escape codes for colors
        GREEN = "\033[32m"
        RESET = "\033[0m"
        YELLOW = "\033[33m"

        for spi_write_line, associated_spi_read_lines in self.spi_tree.items():
            print(f"{spi_write_line}:")
            for spi_read_line, entry_count, iteration_number in associated_spi_read_lines:
                print(f"  {spi_read_line} {GREEN}x{entry_count}, {YELLOW} iteration: {iteration_number}{RESET}")
            print()
    
    def get_associated_spi_read_from_file(self):
        with open(self.input_file_find_mode, 'r') as infile:
            spi_write_lines = infile.read().strip()

        with open(self.output_file_find_mode, 'w') as outfile:
            if not spi_write_lines:
                print("spi_write buffer is empty.")
            else:
                print(spi_write_lines, "*")

                if spi_write_lines == "TERMINATE":
                    print("spi_processor script is terminating")
                    exit()
                
                associated_spi_read_lines = self.get_associated_spi_read(spi_write_lines)

                print(f"Searching for:\n{spi_write_lines}")
                if associated_spi_read_lines:
                    print("Associated spi_read lines:")
                    for spi_read_line in associated_spi_read_lines:
                        print(spi_read_line)
                        outfile.write(f"{spi_read_line}\n")
                else:
                    print("No such key found.")
                print("------")

    def display_spi_data(self):
        # ANSI escape codes for colors
        GREEN = "\033[32m"
        YELLOW = "\033[33m"
        RESET = "\033[0m"

        spi_write_data_color = YELLOW
        spi_read_line_color = GREEN

        self.create_tree_from_json()
        for table_name, rows in self.spi_data.items():
            for row in rows:
                spi_write_line, spi_read_line, entry_count = row.values()

                # Add color to the strings
                colored_spi_write_data = f"{spi_write_data_color}{spi_write_line.strip()}{RESET}"
                colored_spi_read_line = f"{spi_read_line_color}{spi_read_line.strip()}{RESET}"

                print("\n")
                print(f"{colored_spi_write_data}\nassociates\n{colored_spi_read_line}\n\n(entry count: {entry_count})")
                print("\n")

    def encrypt_write_data(self):
        # Create a hash of the spi_write line to use as the table name
        table_name = "spi_" + hashlib.sha1(self.spi_write_data.encode()).hexdigest()
        return table_name
    
    def sleep(self):
        time.sleep(self.sleep_time_find_mode)
        

def main(display_flag=False, find_flag=False, print_tree_flag=False):
    spi_processor = SpiFileProcessor()

    if not display_flag and not find_flag and not print_tree_flag:
        spi_processor.process_spi()
    elif display_flag:
        spi_processor.display_spi_data()
    elif find_flag:
        spi_processor.create_tree_from_json()
        while True:
            spi_processor.get_associated_spi_read_from_file()
            spi_processor.sleep()
    elif print_tree_flag:
        spi_processor.create_tree_from_json()
        spi_processor.print_spi_tree()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process input file.")
    parser.add_argument("-i", "--input", help="The input text file.", action="store_true")
    parser.add_argument("-d", "--display", help="Display the SPI data.", action="store_true")
    parser.add_argument("-f", "--find", help="Find the associated SPI read line.", action="store_true")
    parser.add_argument("-p", "--print-tree", help="Print the SPI tree.", action="store_true")

    args = parser.parse_args()

    if not args.display and not args.find and not args.print_tree and not args.input:
        print("Error: An input file is required when not using the --display, --find, or --print-tree flags.")
        parser.print_help()
    else:
        main(args.display, args.find, args.print_tree)


