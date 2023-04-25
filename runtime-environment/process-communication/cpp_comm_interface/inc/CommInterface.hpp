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
     * @param processName The name of the process publishes the message.
     */
    void publish(const std::string& message);
    /**
     * @brief Close the publisher socket and terminate the ZeroMQ context.
     * This method closes the socket associated with the Publisher object and
     * terminates the ZeroMQ context, which releases any resources being used by the socket
     * and context.
     */
    void close();

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
     * @brief Receives a message using the Subscriber object's socket and returns the received message.
     * The received message is deserialized using the msgpack library and printed to the console.
     * If no message is received, an empty string is returned.
     * @return The received message as a std::string or an empty string if no message is received.
     */
    std::string receive();
    /**
     * @brief Close the subscriber socket and terminate the ZeroMQ context.
     * This method closes the socket associated with the Subscriber object and
     * terminates the ZeroMQ context, which releases any resources being used by the socket
     * and context.
     */
    void close();

private:
    zmq::context_t context; /**< ZeroMQ context object for creating sockets. */
    zmq::socket_t socket; /**< ZeroMQ socket for receiving messages from publishers. */
};

#endif // COMM_INTERFACE_HPP
