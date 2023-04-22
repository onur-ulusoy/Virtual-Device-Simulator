import zmq
import msgpack

class Publisher:
    def __init__(self, local_address, process_name):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUB)
        self.socket.bind(local_address)
        self.process_name = process_name

    def publish(self, message):
        message_with_process_name = f"{self.process_name}: {message}"
        packed_message = msgpack.packb(message_with_process_name)
        self.socket.send(packed_message)
        print(f"Sent: {message_with_process_name}")

class Subscriber:
    def __init__(self, local_address):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.SUB)
        self.socket.connect(local_address)
        self.socket.setsockopt_string(zmq.SUBSCRIBE, "")

    def receive(self):
        packed_message = self.socket.recv()
        message = msgpack.unpackb(packed_message, raw=False)
        print(f"Received: {message}")