import subprocess, sys, csv

if len(sys.argv) < 2:
    print("Please give test count as an argument")
    exit()

test_count = sys.argv[1]

if not test_count.isdigit():
    print("Please provide an integer as an argument")
    exit()

# Define the command to be executed
cmd = ["python3", "run_itests.py"]

test_count = int(test_count)

# Define the columns to print
all_columns = [[""], [""], [""], [""]]
test_set = 4

# Print the columns
""" while True:
    # Clear the console
    #os.system('cls' if os.name == 'nt' else 'clear')
    # Wait for 1 second
    os.system('cls' if os.name == 'nt' else 'clear') """
    
    
import os

for i in range(0, test_count):

    # Use subprocess to start the process in the background
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Wait for the process to complete
    process.wait()

    os.system('cls' if os.name == 'nt' else 'clear')

    # Check the return code of the process
    if process.returncode == 0:
        print("Serial tests {} x {} = {} completed successfully.\n".format(i+1, test_set, (i+1)*test_set))
    else:
        print("Process failed with return code:", process.returncode)

    # Read the contents of the text file and print each column
    with open("test_results.txt") as f:
        reader = csv.reader(f, delimiter='\t')
        columns = zip(*reader)
        for column in columns:
            i = 0
            while (i < len(column)):
                    #print(column[i])
                    all_columns[i].append(column[i])
                    print(" ".join(all_columns[i]))
                    i += 1
                    
        print()
    