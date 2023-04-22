#ifndef COMM_INTERFACE_HPP
#define COMM_INTERFACE_HPP

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

class Publisher {
public:
    Publisher(const std::string& localAddress, const std::string& processName);
    void publish(const std::string& message);

private:
    zmq::context_t context;
    zmq::socket_t socket;
    std::string processName;
};

class Subscriber {
public:
    Subscriber(const std::string& localAddress);
    void receive();

private:
    zmq::context_t context;
    zmq::socket_t socket;
};

#endif // COMM_INTERFACE_HPP
