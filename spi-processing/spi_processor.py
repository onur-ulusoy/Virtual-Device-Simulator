#!/usr/bin/env python3

import argparse
import sqlite3
from cryptography.fernet import Fernet
from base64 import urlsafe_b64encode
import hashlib

class SpiFileProcessor:
    def __init__(self, input_file):
        self.input_file = input_file
        self.spi_write_data = ""

    def encrypt(self, data):
        key = Fernet.generate_key()
        f = Fernet(key)
        encrypted_data = f.encrypt(data.encode())
        return key, encrypted_data

    def decrypt(self, key, encrypted_data):
        f = Fernet(key)
        return f.decrypt(encrypted_data).decode()

    def process_spi(self):
        with open(self.input_file, 'r') as infile:
            saved_spi_writes = set()
            for line in infile:
                if line.startswith("spi_write"):
                    self.spi_write_data += line
                elif line.startswith("spi_read"):
                    key, encrypted_data = self.encrypt(self.spi_write_data)
                    self.save_to_db(key, encrypted_data, line)
                    saved_spi_writes.add(self.spi_write_data)
                    self.spi_write_data = ""
            # Save the remaining spi_write_data if it has not been saved yet
            if self.spi_write_data and self.spi_write_data not in saved_spi_writes:
                key, encrypted_data = self.encrypt(self.spi_write_data)
                self.save_to_db(key, encrypted_data, "")


    def save_to_db(self, key, encrypted_data, spi_read_line):
        conn = sqlite3.connect("spi_data.db")
        cursor = conn.cursor()

        # Create a hash of the spi_write and spi_read lines to use as the table name
        table_name = "spi_" + hashlib.sha1((self.spi_write_data + spi_read_line).encode()).hexdigest()

        cursor.execute(
            f"""CREATE TABLE IF NOT EXISTS {table_name} (
                encryption_key BLOB, encrypted_value BLOB, spi_write_line TEXT, spi_read_line TEXT, entry_count INTEGER
            )"""
        )

        cursor.execute(f"SELECT COUNT(*), COALESCE(entry_count, 0) FROM {table_name} WHERE spi_write_line = ?", (self.spi_write_data,))
        count, entry_count = cursor.fetchone()

        if count == 0:
            cursor.execute(
                f"""INSERT INTO {table_name} (encryption_key, encrypted_value, spi_write_line, spi_read_line, entry_count)
                VALUES (?, ?, ?, ?, ?)""",
                (key, encrypted_data, self.spi_write_data, spi_read_line, 1),
            )
        else:
            entry_count += 1
            cursor.execute(
                f"""UPDATE {table_name} SET encryption_key = ?, encrypted_value = ?, spi_read_line = ?, entry_count = ?
                WHERE spi_write_line = ?""",
                (key, encrypted_data, spi_read_line, entry_count, self.spi_write_data),
            )

        conn.commit()
        conn.close()


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

def main(input_file, decrypt_flag=False):
    spi_processor = SpiFileProcessor(input_file)
    if not decrypt_flag:
        spi_processor.process_spi()
    else:
        db = SQLiteDatabase("spi_data.db")
        rows = db.read_table("spi_data")
        for row in rows:
            print("\n****************")
            key, encrypted_data, spi_read_line = row
            decrypted_data = spi_processor.decrypt(key, encrypted_data)
            print(f"{decrypted_data}->\n{spi_read_line.strip()}")
            print("****************\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process input file.")
    parser.add_argument("-i", "--input", help="The input text file.", default=None)
    parser.add_argument("-d", "--decrypt", help="Decrypt and display the data.", action="store_true")
    
    args = parser.parse_args()

    if not args.decrypt and args.input is None:
        print("Error: An input file is required when not using the --decrypt flag.")
        parser.print_help()
    else:
        main(args.input, args.decrypt)