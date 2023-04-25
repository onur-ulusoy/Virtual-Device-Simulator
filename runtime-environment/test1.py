from test_suite import *

prepare_data(spi_write_file="SPI_A.txt",
             remote_directory="/home/onur/Desktop/program_tested",
             time_out=10,
             sleep_time=1)

# run_tester_and_driver()

signal_topic="tcp://localhost:5555"
data_topic="tcp://localhost:5557"

# Create a Subscriber object to listen for the signal to send data
subscriber = Subscriber(signal_topic)

# Create a Publisher object to send the data
publisher = Publisher(data_topic, "test")

spi_write_file = "SPI_A.txt"
local_directory = os.getcwd()

while True:
    # Check if SPI_A.txt is empty
    local_file_path = os.path.join(local_directory, spi_write_file)
    with open(local_file_path, 'r') as file:
        if not file.read(1):
            os.remove(local_file_path)  # Remove the empty file
            break

    send_data_when_asked(spi_write_file, subscriber, publisher, local_directory)
    print("\n", "*************************")

# Close the subscriber and publisher sockets
subscriber.close()
publisher.close()

publisher.close()
subscriber.close()
