from test_suite import *

# Prepare data by splitting the original data file into separate SPI_A.txt and SPI_B.txt files
prepare_data(spi_write_file="SPI_A.txt",
             remote_directory="/home/onur/Desktop/program_tested",
             time_out=10,
             sleep_time=1)

# run_tester_and_driver()

# Define communication topics
signal_topic = "tcp://localhost:5555"
write_data_topic = "tcp://localhost:5557"
read_data_topic = "tcp://localhost:5559"

# Create a Subscriber object to listen for the signal to send data
signal_listener = Subscriber(signal_topic)

# Create a Publisher object to send the data
data_supplier = Publisher(write_data_topic, "test")

# Create a Subscriber object to receive the read data
data_listener = Subscriber(read_data_topic)

# Define the spi_write and spi_read file names
spi_write_file = "SPI_A.txt"
spi_read_file = "SPI_B.txt"

# Get the current working directory
local_directory = os.getcwd()

# Prepare expected data b
prepare_data_b(spi_write_file, spi_read_file)

tester_pid, driver_pid = run_assembly()
time.sleep(0.1)
# Continuously send data and expect read data
while True:
    # Check if SPI_A.txt is empty
    local_file_path = os.path.join(local_directory, spi_write_file)
    with open(local_file_path, 'r') as file:
        if not file.read(1):
            os.remove(local_file_path)  # Remove the empty file
            break
    # Send data when asked
    send_data_when_asked(spi_write_file, signal_listener, data_supplier, local_directory)
    # Expect and compare received data with the data in spi_read_file
    expect(spi_read_file, data_listener, local_directory)
    print("\n" + "*************************")

# Close the subscriber and publisher sockets
signal_listener.close()
data_listener.close()
data_supplier.close()

time.sleep(0.1)
    
#request_sp_read_line("TERMINATE")

#os.remove(spi_write_file)
#os.remove(spi_read_file)