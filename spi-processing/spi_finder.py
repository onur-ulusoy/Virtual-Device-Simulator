import sqlite3
import argparse
from spi_processor import encrypt_write_data

class SpiReadFinder:
    def __init__(self, db_file):
        self.db_file = db_file

    def find_spi_read_line(self, spi_write_data):
        conn = sqlite3.connect(self.db_file)
        cursor = conn.cursor()

        # Create a hash of the spi_write line to use as the table name
        table_name = encrypt_write_data(spi_write_data)

        # Strip any leading or trailing whitespace or non-printable characters
        spi_write_line = spi_write_data.strip()

        cursor.execute(f"SELECT spi_read_line FROM {table_name} WHERE spi_write_line = ?", (spi_write_line,))
        spi_read_line = cursor.fetchone()

        conn.close()

        if spi_read_line:
            return spi_read_line[0]
        else:
            return None



def main(input_file):
    spi_read_finder = SpiReadFinder("spi_data.db")

    with open(input_file, 'r') as infile:
        spi_write_data = infile.read().strip()
        print(spi_write_data.__len__())
        spi_read_line = spi_read_finder.find_spi_read_line(spi_write_data)

        print(f"Searching for:\n{spi_write_data}")
        if spi_read_line:
            print("Associated spi_read line:")
            print(spi_read_line)
        else:
            print("No such key found.")
        print("------")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process input file with spi_write data.")
    parser.add_argument("input", help="The input text file.")
    
    args = parser.parse_args()
    
    main(args.input)
