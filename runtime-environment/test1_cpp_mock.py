import sys
import time

sys.path.append('process-communication')
from comm_interface import *

def main():
    signal_topic = "tcp://localhost:5555"
    data_topic = "tcp://localhost:5557"

    # Create a Publisher object and send the expected signal
    publisher = Publisher(signal_topic, "mock")
    # Create a Subscriber object and subscribe to the send_data topic
    subscriber = Subscriber(data_topic)
    
    time_out = 0
    time.sleep(1)

    while time_out < 2:
        
        publisher.publish("Requesting data")

        time.sleep(1)
        
        # Receive and print the message
        while True:
            message = subscriber.receive(timeout=100)
            if (not message):
                time_out += 1
                break
            else:
                time_out = 0

        print("*****")
            
        time.sleep(0.1)
    
    publisher.close()
    subscriber.close()

if __name__ == "__main__":
    main()
