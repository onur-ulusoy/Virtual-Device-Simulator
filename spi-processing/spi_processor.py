import argparse
import hashlib
import json
from collections import defaultdict
import os

class SpiFileProcessor:
    def __init__(self, input_file):
        self.json_file = "spi_data.json"
        self.input_file = input_file
        self.spi_write_data = ""
        self.spi_data = defaultdict(list)

    def process_spi(self):
        with open(self.input_file, 'r') as infile:
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
        table_name = encrypt_write_data(spi_write_data)

        if os.path.exists(self.json_file):
            with open(self.json_file, "r") as infile:
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

        with open(self.json_file, "w") as outfile:
            json.dump(self.spi_data, outfile, indent=4)


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

# class SpiReadFinder:
#     def __init__(self, json_file):
#         self.json_file = json_file
#         self.spi_data = {}

#     def load_spi_data(self):
#         with open(self.json_file, 'r') as infile:
#             self.spi_data = json.load(infile)

#     def find_spi_read_line(self, spi_write_data):
#         self.load_spi_data()

#         table_name = encrypt_write_data(spi_write_data)

#         if table_name not in self.spi_data:
#             return None

#         rows = self.spi_data[table_name]
#         for row in rows:
#             if row["spi_write_line"] == spi_write_data:
#                 return row["spi_read_line"]

#         return None
    
def encrypt_write_data(spi_write_data):
    # Create a hash of the spi_write line to use as the table name
    table_name = "spi_" + hashlib.sha1(spi_write_data.encode()).hexdigest()
    return table_name

def main(input_file, display_flag=False, find_flag=False, print_tree_flag=False):
    spi_processor = SpiFileProcessor(input_file)

    if not display_flag and not find_flag and not print_tree_flag and input_file is not None:
        spi_processor.process_spi()
    elif display_flag:
        spi_processor.display_spi_data()
    elif find_flag:
        spi_processor.create_tree_from_json()
        ##
    elif print_tree_flag:
        spi_processor.create_tree_from_json()
        spi_processor.print_spi_tree()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process input file.")
    parser.add_argument("-i", "--input", help="The input text file.", default=None)
    parser.add_argument("-d", "--display", help="Display the SPI data.", action="store_true")
    parser.add_argument("-f", "--find", help="Find the associated SPI read line.", action="store_true")
    parser.add_argument("-p", "--print-tree", help="Print the SPI tree.", action="store_true")

    args = parser.parse_args()

    if not args.display and not args.find and not args.print_tree and args.input is None:
        print("Error: An input file is required when not using the --display, --find, or --print-tree flags.")
        parser.print_help()
    else:
        main(args.input, args.display, args.find, args.print_tree)

