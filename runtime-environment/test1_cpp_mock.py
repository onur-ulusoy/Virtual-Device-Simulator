import sys
import time
import os
import shutil

sys.path.append('process-communication')
from comm_interface import *

def main():
    # Set up file paths for copying SPI_B.txt to the working directory
    desktop_path = os.path.expanduser("~/Desktop")
    source_file = os.path.join(desktop_path, "SPI_B.txt")
    destination_file = os.path.join(os.getcwd(), "SPI_B.txt")

    # Copy SPI_B.txt from desktop to the working directory
    shutil.copyfile(source_file, destination_file)

    # Define communication topics
    signal_topic = "tcp://localhost:5555"
    write_data_topic = "tcp://localhost:5557"
    read_data_topic = "tcp://localhost:5559"

    # Define a mock read line
    mock_read_line = "spi_read: Bytes read: 5: 0xA5 0xB9 0x01 0x00 0x00"

    # Create Publisher objects for the signal topic and read data topic
    publisher_write = Publisher(signal_topic, "mock")
    publisher_read = Publisher(read_data_topic, "mock")
    
    # Create a Subscriber object to listen for write data
    subscriber = Subscriber(write_data_topic)
    
    # Initialize timeout counter
    time_out = 0
    time.sleep(1)

    # Continuously request data and receive messages
    while time_out < 2:
        # Send a request for data
        publisher_write.publish("Requesting data")

        time.sleep(0.2)
        
        # Receive and process messages, with a timeout
        while True:
            message = subscriber.receive(timeout=100)
            if (not message):
                time_out += 1
                break
            else:
                # Publish the mock read line
                publisher_read.publish(mock_read_line)
                time_out = 0

        print("*****")
            
        time.sleep(0.1)
    
    # Close the publisher and subscriber sockets
    publisher_write.close()
    publisher_read.close()
    subscriber.close()

if __name__ == "__main__":
    main()
