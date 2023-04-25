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
subscriber = Subscriber(signal_topic)

# Create a Publisher object to send the data
publisher_write = Publisher(write_data_topic, "test")

# Create a Subscriber object to receive the read data
subscriber_read = Subscriber(read_data_topic)

# Define the spi_write and spi_read file names
spi_write_file = "SPI_A.txt"
spi_read_file = "SPI_B.txt"

# Get the current working directory
local_directory = os.getcwd()

# Continuously send data and expect read data
while True:
    # Check if SPI_A.txt is empty
    local_file_path = os.path.join(local_directory, spi_write_file)
    with open(local_file_path, 'r') as file:
        if not file.read(1):
            os.remove(local_file_path)  # Remove the empty file
            break

    # Send data when asked
    send_data_when_asked(spi_write_file, subscriber, publisher_write, local_directory)
    # Expect and compare received data with the data in spi_read_file
    expect(spi_read_file, subscriber_read, local_directory)
    print("\n", "*************************")

# Close the subscriber and publisher sockets
subscriber.close()
subscriber_read.close()
publisher_write.close()
