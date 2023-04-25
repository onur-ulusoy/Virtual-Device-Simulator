"""
@file comm_interface.py
@brief Contains the Publisher and Subscriber classes for communication between processes using ZeroMQ.

This module provides a Python implementation of the ZeroMQ publisher-subscriber communication pattern. 
The Publisher class sends messages to one or more subscribers using a publisher socket, and the 
Subscriber class receives messages from a publisher using a subscriber socket.

@author Onur Ulusoy
@date 22.04.2023
@license MIT
"""

import zmq
import msgpack

class Publisher:
    """
    Class representing a ZeroMQ publisher.
    @details This class allows for sending messages to a ZeroMQ subscriber using a publisher socket.
    """
    def __init__(self, local_address, process_name):
        """
        Constructor for Publisher object.
        
        @param local_address The local address to bind the socket to.
        @param process_name The name of the process that is publishing the message.
        """
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUB)
        self.socket.bind(f"tcp://*:{local_address.split(':')[-1]}")  # Bind using the wildcard
        self.process_name = process_name

    def publish(self, message):
        """
        Sends a message using the Publisher object's socket.
        
        @param message The message to be sent.
        """
        message_with_process_name = f"{self.process_name}: {message}"
        packed_message = msgpack.packb(message_with_process_name)
        self.socket.send(packed_message)
        print(f"Sent: {message_with_process_name}")
    
    def close(self):
        """
        @brief Close the publisher socket and terminate the ZeroMQ context.

        @details This method closes the socket associated with the Publisher object and
        terminates the ZeroMQ context, which releases any resources being used by the socket
        and context.
        """
        self.socket.close()
        self.context.term()

class Subscriber:
    """
    Class representing a ZeroMQ subscriber.
    @details This class allows for receiving messages from a ZeroMQ publisher using a subscriber socket.
    """
    def __init__(self, local_address):
        """
        Constructor for Subscriber object.
        
        @param local_address The local address to connect the socket to.
        """
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.SUB)
        self.socket.connect(local_address)
        self.socket.setsockopt_string(zmq.SUBSCRIBE, "")

    def receive(self, timeout=None):
        """
        Receives a message using the Subscriber object's socket.
        
        @details The received message is deserialized using the msgpack library and printed to the console.
        @param timeout: The maximum time to wait for a message, in milliseconds. None (default) means wait indefinitely.
        """
        self.socket.setsockopt(zmq.RCVTIMEO, timeout if timeout is not None else -1)
        try:
            packed_message = self.socket.recv()
            message = msgpack.unpackb(packed_message, raw=False)
            print(f"Received: {message}")
            
            message = message.split(':', 1)[-1].strip()
            return message
        
        except zmq.error.Again:
            print("Timeout occurred while waiting for a message.")
            return None
    
    def close(self):
        """
        @brief Close the subscriber socket and terminate the ZeroMQ context.

        @details This method closes the socket associated with the Subscriber object and
        terminates the ZeroMQ context, which releases any resources being used by the socket
        and context.
        """
        self.socket.close()
        self.context.term()


