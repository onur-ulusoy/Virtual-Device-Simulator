import sqlite3
import argparse
import os
import shutil
from spi_processor import encrypt_write_data

class SpiReadFinder:
    def __init__(self, db_file):
        self.db_file = db_file

    def find_spi_read_line(self, spi_write_data):
        conn = sqlite3.connect(self.db_file)
        cursor = conn.cursor()

        table_name = encrypt_write_data(spi_write_data)

        cursor.execute(f"SELECT spi_read_line FROM {table_name} WHERE spi_write_line = ?", (spi_write_data,))
        spi_read_line = cursor.fetchone()

        conn.close()

        if spi_read_line:
            return spi_read_line[0]
        else:
            return None

    def duplicate_db_and_update(self):
        new_db_file = "spi_data_copy.db"
        shutil.copy(self.db_file, new_db_file)

        conn = sqlite3.connect(new_db_file)
        cursor = conn.cursor()

        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        table_names = [table[0] for table in cursor.fetchall()]

        for table_name in table_names:
            cursor.execute(f"ALTER TABLE {table_name} ADD COLUMN iteration INTEGER")
            cursor.execute(f"UPDATE {table_name} SET iteration = 1 WHERE ROWID = 1")
            cursor.execute(f"UPDATE {table_name} SET iteration = 0 WHERE ROWID > 1")

        conn.commit()
        conn.close()
        return new_db_file

def main():
    spi_read_finder = SpiReadFinder("spi_data.db")
    new_db_file = spi_read_finder.duplicate_db_and_update()
    spi_read_finder_new = SpiReadFinder(new_db_file)

    while True:
        input_file = input("Enter the path to the input file: ").strip()

        try:
            with open(input_file, 'r') as infile:
                spi_write_data = infile.read().strip()
                spi_read_line = spi_read_finder_new.find_spi_read_line(spi_write_data)

                print(f"Searching for:\n{spi_write_data}")
                if spi_read_line:
                    print("Associated spi_read line:")
                    print(spi_read_line)
                else:
                    print("No such key found.")
                print("------")
        except FileNotFoundError:
            print(f"Error: File '{input_file}' not found.")
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    main()
