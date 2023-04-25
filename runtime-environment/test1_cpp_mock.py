import sys
import time

sys.path.append('process-communication')
from comm_interface import *

def main():
    signal_topic = "tcp://localhost:5555"
    publish_topic = "tcp://localhost:5556"

    # Create a Publisher object and send the expected signal
    publisher = Publisher(signal_topic, "mock")
    time.sleep(1)
    publisher.publish("Requesting data")

    # Create a Subscriber object and subscribe to the send_data topic
    subscriber = Subscriber(publish_topic)

    while True:
        # Receive and print the message
        message = subscriber.receive()

        # Terminate the process when a message is received
        if message:
            print("Message received. Terminating process.")
            break

if __name__ == "__main__":
    main()
