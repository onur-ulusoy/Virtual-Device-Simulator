#!/usr/bin/env python3

import argparse
import sqlite3
from cryptography.fernet import Fernet
from base64 import urlsafe_b64encode

class SpiFileProcessor:
    def __init__(self, input_file):
        self.input_file = input_file
        self.spi_write_data = ""

    def encrypt(self, data):
        key = Fernet.generate_key()
        f = Fernet(key)
        encrypted_data = f.encrypt(data.encode())
        return key, encrypted_data


    def process_spi(self):
        with open(self.input_file, 'r') as infile:
            for line in infile:
                if line.startswith("spi_write"):
                    self.spi_write_data += line
                elif line.startswith("spi_read"):
                    key, encrypted_data = self.encrypt(self.spi_write_data)
                    self.save_to_db(key, encrypted_data, line)
                    self.spi_write_data = ""

    def save_to_db(self, key, encrypted_data, spi_read_line):
        conn = sqlite3.connect("spi_data.db")
        cursor = conn.cursor()
        cursor.execute(
            "CREATE TABLE IF NOT EXISTS spi_data (encryption_key BLOB, encrypted_value BLOB, spi_read_line TEXT UNIQUE)"
        )

        cursor.execute("SELECT COUNT(*) FROM spi_data WHERE spi_read_line = ?", (spi_read_line,))
        count = cursor.fetchone()[0]

        if count == 0:
            cursor.execute("INSERT INTO spi_data (encryption_key, encrypted_value, spi_read_line) VALUES (?, ?, ?)", (key, encrypted_data, spi_read_line))
            conn.commit()

        conn.close()


def main(input_file):
    spi_processor = SpiFileProcessor(input_file)
    spi_processor.process_spi()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process input file.")
    parser.add_argument("input", help="The input text file.")
    
    args = parser.parse_args()
    
    main(args.input)
