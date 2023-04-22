/**
 * @file CommInterface.cpp
 * @brief Contains the Publisher and Subscriber classes for communication between processes using ZeroMQ.
 * 
 * Provides a C++ implementation of the ZeroMQ publisher-subscriber communication pattern. 
 * The Publisher class sends messages to one or more subscribers using a publisher socket, and the 
 * Subscriber class receives messages from a publisher using a subscriber socket.
 * 
 * @author Onur Ulusoy
 * @date 22.04.2023
 * @license MIT
 */

#include "CommInterface.hpp"

Publisher::Publisher(const std::string& localAddress, const std::string& processName)
    : context(1), socket(context, ZMQ_PUB), processName(processName) {
    socket.bind(localAddress);
}

void Publisher::publish(const std::string& message) {
    std::string messageWithProcessName = processName + ": " + message;
    msgpack::sbuffer buffer;
    msgpack::pack(buffer, messageWithProcessName);
    zmq::message_t packedMessage(buffer.data(), buffer.size());
    socket.send(packedMessage, zmq::send_flags::none);
    std::cout << "Sent: " << messageWithProcessName << std::endl;
}

Subscriber::Subscriber(const std::string& localAddress)
    : context(1), socket(context, ZMQ_SUB) {
    socket.connect(localAddress);
    socket.set(zmq::sockopt::subscribe, "");  // Updated function call
}

void Subscriber::receive() {
    zmq::message_t packedMessage;
    auto result = socket.recv(packedMessage, zmq::recv_flags::none);  // Handle the return value
    if (result) {
        msgpack::object_handle oh = msgpack::unpack(static_cast<const char*>(packedMessage.data()), packedMessage.size());
        msgpack::object deserialized = oh.get();
        std::string message;
        deserialized.convert(message);
        std::cout << "Received: " << message << std::endl;
    }
}