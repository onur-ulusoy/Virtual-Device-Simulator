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
    std::string port = localAddress.substr(localAddress.find_last_of(':') + 1);
    std::string endpoint = "tcp://*:" + port;
    socket.bind(endpoint);
}

void Publisher::publish(const std::string& message) {
    std::string messageWithProcessName = processName + ": " + message;
    msgpack::sbuffer buffer;
    msgpack::pack(buffer, messageWithProcessName);
    zmq::message_t packedMessage(buffer.data(), buffer.size());
    socket.send(packedMessage, zmq::send_flags::none);
    std::cout << "Sent: " << messageWithProcessName << std::endl;
}

void Publisher::close() {
    socket.close();
    context.close();
}

Subscriber::Subscriber(const std::string& localAddress)
    : context(1), socket(context, ZMQ_SUB) {
    socket.connect(localAddress);
    socket.set(zmq::sockopt::subscribe, "");  // Updated function call
}

std::string Subscriber::receive(int timeout) {
    zmq::message_t packedMessage;
    socket.set(zmq::sockopt::rcvtimeo, timeout);

    auto result = socket.recv(packedMessage, zmq::recv_flags::none);  // Handle the return value
    if (result) {
        msgpack::object_handle oh = msgpack::unpack(static_cast<const char*>(packedMessage.data()), packedMessage.size());
        msgpack::object deserialized = oh.get();
        std::string message;
        deserialized.convert(message);
        std::cout << "Received: " << message << std::endl;

        size_t delimiterPos = message.find(':');
        if (delimiterPos != std::string::npos) {
            message = message.substr(delimiterPos + 1);
            message = message.substr(message.find_first_not_of(' '));
        }
        return message;
    } else if (timeout > 0 && zmq_errno() == EAGAIN) {
        std::cout << "Timeout occurred while waiting for a message." << std::endl;
        return "";
    }

    return "";
}

void Subscriber::close() {
    socket.close();
    context.close();
}