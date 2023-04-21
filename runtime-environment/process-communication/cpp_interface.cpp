// publisher_subscriber.hpp
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <msgpack.hpp>

class Publisher {
public:
    Publisher(const std::string& local_address, const std::string& process_name)
        : context(1), socket(context, ZMQ_PUB), process_name(process_name) {
        socket.bind(local_address);
    }

    void publish(const std::string& message) {
        std::string message_with_process_name = process_name + ": " + message;
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, message_with_process_name);
        zmq::message_t packed_message(buffer.data(), buffer.size());
        socket.send(packed_message, zmq::send_flags::none);
        std::cout << "Sent: " << message_with_process_name << std::endl;
    }

private:
    zmq::context_t context;
    zmq::socket_t socket;
    std::string process_name;
};

class Subscriber {
public:
    Subscriber(const std::string& local_address)
        : context(1), socket(context, ZMQ_SUB) {
        socket.connect(local_address);
        socket.set(zmq::sockopt::subscribe, "");  // Updated function call
    }

    void receive() {
        zmq::message_t packed_message;
        auto result = socket.recv(packed_message, zmq::recv_flags::none);  // Handle the return value
        if (result) {
            msgpack::object_handle oh = msgpack::unpack(static_cast<const char*>(packed_message.data()), packed_message.size());
            msgpack::object deserialized = oh.get();
            std::string message;
            deserialized.convert(message);
            std::cout << "Received: " << message << std::endl;
        }
    }

private:
    zmq::context_t context;
    zmq::socket_t socket;
};


int main(){
    
}