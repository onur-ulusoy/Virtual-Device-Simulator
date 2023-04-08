#!/usr/bin/env python3

import argparse
import sqlite3
import hashlib

class SpiFileProcessor:
    def __init__(self, input_file):
        self.input_file = input_file
        self.spi_write_data = ""

    def process_spi(self):
        with open(self.input_file, 'r') as infile:
            for line in infile:
                if line.startswith("spi_write"):
                    self.spi_write_data += line
                elif line.startswith("spi_read"):
                    self.save_to_db(self.spi_write_data, line)
                    self.spi_write_data = ""
            # Save the remaining spi_write_data
            if self.spi_write_data:
                self.save_to_db(self.spi_write_data, "")

    def save_to_db(self, spi_write_data, spi_read_line):
        conn = sqlite3.connect("spi_data.db")
        cursor = conn.cursor()

        # Create a hash of the spi_write line to use as the table name
        table_name = "spi_" + hashlib.sha1(spi_write_data.encode()).hexdigest()

        cursor.execute(
            f"""CREATE TABLE IF NOT EXISTS {table_name} (
                spi_write_line TEXT, spi_read_line TEXT, entry_count INTEGER
            )"""
        )

        cursor.execute(f"SELECT *, ROWID FROM {table_name} ORDER BY ROWID DESC LIMIT 1")
        last_row = cursor.fetchone()

        if last_row is None or last_row[1] != spi_read_line:
            cursor.execute(
                f"""INSERT INTO {table_name} (spi_write_line, spi_read_line, entry_count)
                VALUES (?, ?, ?)""",
                (spi_write_data, spi_read_line, 1),
            )
        else:
            entry_count = last_row[2] + 1
            row_id = last_row[3]
            cursor.execute(
                f"""UPDATE {table_name} SET entry_count = ? WHERE ROWID = ?""",
                (entry_count, row_id),
            )

        conn.commit()
        conn.close()

    def display_spi_data(self):
        # ANSI escape codes for colors
        GREEN = "\033[32m"
        YELLOW = "\033[33m"
        RESET = "\033[0m"

        # Choose the color you want to use for each string
        spi_write_data_color = YELLOW
        spi_read_line_color = GREEN

        db = SQLiteDatabase("spi_data.db")
        table_names = db.get_all_tables()
        for table_name in table_names:
            rows = db.read_table(table_name)
            for row in rows:
                spi_write_line, spi_read_line, entry_count = row

                # Add color to the strings
                colored_spi_write_data = f"{spi_write_data_color}{spi_write_line.strip()}{RESET}"
                colored_spi_read_line = f"{spi_read_line_color}{spi_read_line.strip()}{RESET}"

                print("\n")
                print(f"{colored_spi_write_data}\nassociates\n{colored_spi_read_line}\n\n(entry count: {entry_count})")
                print("\n")

class SQLiteDatabase:
    def __init__(self, db_path):
        self.conn = sqlite3.connect(db_path)

    def get_all_tables(self):
        cursor = self.conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        tables = [table[0] for table in cursor.fetchall()]
        return tables

    def read_table(self, table_name):
        cursor = self.conn.cursor()
        cursor.execute(f"SELECT * FROM {table_name};")
        return cursor.fetchall()

def main(input_file, display_flag=False):
    spi_processor = SpiFileProcessor(input_file)
    if not display_flag:
        spi_processor.process_spi()
    else:
        spi_processor.display_spi_data()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process input file.")
    parser.add_argument("-i", "--input", help="The input text file.", default=None)
    parser.add_argument("-d", "--display", help="Display the SPI data.", action="store_true")
    
    args = parser.parse_args()

    if not args.display and args.input is None:
        print("Error: An input file is required when not using the --display flag.")
        parser.print_help()
    else:
        main(args.input, args.display)
