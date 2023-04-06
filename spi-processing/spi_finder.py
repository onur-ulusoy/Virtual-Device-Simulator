import sqlite3
from cryptography.fernet import Fernet
import argparse

class SpiReadFinder:
    def __init__(self, db_file):
        self.db_file = db_file

    def find_spi_read_line(self, spi_write_data):
        conn = sqlite3.connect(self.db_file)
        cursor = conn.cursor()

        cursor.execute("SELECT encrypted_value, encryption_key FROM spi_data")
        rows = cursor.fetchall()

        for row in rows:
            encrypted_data, key = row
            f = Fernet(key)
            decrypted_data = f.decrypt(encrypted_data)

            if decrypted_data.decode() == spi_write_data:
                cursor.execute("SELECT spi_read_line FROM spi_data WHERE encryption_key = ? AND encrypted_value = ?", (key, encrypted_data))
                spi_read_line = cursor.fetchone()[0]
                conn.close()
                return spi_read_line

        conn.close()
        return None

def main(input_file):
    spi_read_finder = SpiReadFinder("spi_data.db")
    
    with open(input_file, 'r') as infile:
        for spi_write_data in infile:
            spi_write_data = spi_write_data.strip()
            spi_read_line = spi_read_finder.find_spi_read_line(spi_write_data)
            
            print(f"Searching for: {spi_write_data}")
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
