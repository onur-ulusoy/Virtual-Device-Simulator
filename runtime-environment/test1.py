from test_suite import *

prepare_data(spi_write_file="SPI_A.txt",
             remote_directory="/home/onur/Desktop/program_tested",
             time_out=10,
             sleep_time=1)

# run_tester_and_driver()

signal_topic = "tcp://localhost:5555"
write_data_topic = "tcp://localhost:5557"
read_data_topic = "tcp://localhost:5559"

# Create a Subscriber object to listen for the signal to send data
subscriber = Subscriber(signal_topic)

# Create a Publisher object to send the data
publisher_write = Publisher(write_data_topic, "test")

# Create a Publisher object to send the data
subscriber_read = Subscriber(read_data_topic)

spi_write_file = "SPI_A.txt"
spi_read_file = "SPI_B.txt"

local_directory = os.getcwd()

while True:
    # Check if SPI_A.txt is empty
    local_file_path = os.path.join(local_directory, spi_write_file)
    with open(local_file_path, 'r') as file:
        if not file.read(1):
            os.remove(local_file_path)  # Remove the empty file
            break

    send_data_when_asked(spi_write_file, subscriber, publisher_write, local_directory)
    expect(spi_read_file, subscriber_read, local_directory)
    print("\n", "*************************")

# Close the subscriber and publisher sockets
subscriber.close()
publisher_write.close()