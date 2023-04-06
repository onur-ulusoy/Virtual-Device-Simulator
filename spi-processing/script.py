import sqlite3
import sys

def read_log_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    return lines

def save_to_database(data, db_name="spi_logs.db"):
    conn = sqlite3.connect(db_name)
    cursor = conn.cursor()
    cursor.execute('''CREATE TABLE IF NOT EXISTS logs (line TEXT)''')
    
    for line in data:
        cursor.execute("INSERT INTO logs (line) VALUES (?)", (line,))
    
    conn.commit()
    conn.close()

def generate_output_from_db(output_file, db_name="spi_logs.db"):
    conn = sqlite3.connect(db_name)
    cursor = conn.cursor()
    cursor.execute("SELECT line FROM logs")
    
    lines = cursor.fetchall()
    with open(output_file, 'w') as file:
        for line in lines:
            file.write(line[0])
    
    conn.close()

def main():
    mode = input("Enter mode (learning/output): ").lower()

    if mode == "learning":
        input_file = sys.argv[1]
        lines = read_log_file(input_file)
        save_to_database(lines)
        print(f"Data saved to database from {input_file}")
    elif mode == "output":
        output_file = "output.txt"
        generate_output_from_db(output_file)
        print(f"Generated {output_file} from database")
    else:
        print("Invalid mode. Please enter 'learning' or 'output'.")

if __name__ == "__main__":
    main()
