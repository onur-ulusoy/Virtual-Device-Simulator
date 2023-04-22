/**
 * @file CommInterface.hpp
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

#ifndef COMM_INTERFACE_HPP
#define COMM_INTERFACE_HPP

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

/**
 * @brief Class representing a ZeroMQ publisher.
 * @details This class allows for sending messages to a ZeroMQ subscriber using a publisher socket.
 */
class Publisher {
public:
    /**
     * @brief Constructor for Publisher object.
     * @param localAddress The local address to bind the socket to.
     * @param processName The name of the process that is publishing the message.
     */
    Publisher(const std::string& localAddress, const std::string& processName);
    /**
     * @brief Sends a message using the Publisher object's socket.
     * @param message The message to be sent.
     */
    void publish(const std::string& message);

private:
    zmq::context_t context; /**< ZeroMQ context object for creating sockets. */
    zmq::socket_t socket; /**< ZeroMQ socket for sending messages to subscribers. */
    std::string processName; /**< The name of the process that is publishing the message. */
};

/**
 * @brief Class representing a ZeroMQ subscriber.
 * This class allows for receiving messages from a ZeroMQ publisher using a subscriber socket.
 */
class Subscriber {
public:
    /**
     * @brief Constructor for Subscriber object.
     * @param localAddress The local address to connect the socket to.
     */
    Subscriber(const std::string& localAddress);
    /**
     * @brief Receives a message using the Subscriber object's socket.
     * The received message is deserialized using the msgpack library and printed to the console.
     */
    void receive();

private:
    zmq::context_t context; /**< ZeroMQ context object for creating sockets. */
    zmq::socket_t socket; /**< ZeroMQ socket for receiving messages from publishers. */
};

#endif // COMM_INTERFACE_HPP
