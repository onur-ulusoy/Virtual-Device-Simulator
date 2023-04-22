from comm_interface import Publisher, Subscriber
import time

# subscriber = Subscriber("tcp://localhost:5555")
# counter = 0

# while counter < 5:
#     subscriber.receive()
#     counter += 1

local_address = "tcp://127.0.0.1:5555"
publisher = Publisher(local_address, "Python Script")
counter = 0

while counter < 5:
    time.sleep(2)
    message = "Python publisher"
    publisher.publish(message)
    counter += 1