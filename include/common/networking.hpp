#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include "logger.hpp"
#include <asio.hpp>
#include <google/protobuf/message.h>

/** Static class containing various networking functionality.
 * 
 * This class is responsible for handling protobuf transportation
 * over the network.
 */
class networking
{
public:

    /** Sends the specified protobug message over the socket to the remote host.
     * @param sock The TCP socket to the remote host used for transport
     * @param msg The message to serialize and send
     * 
     * Sends the length in bytes first and the actual message afterwards.
     * if an error occurs it will be captured and logged.
     * 
     * @returns True if the message was successfully sent; False otherwise. 
     */
    static inline bool send_protobuf(asio::ip::tcp::socket& sock, google::protobuf::Message& msg) {
        try {
            u_int64_t msg_size{msg.ByteSizeLong()};

            asio::write(sock, asio::buffer(&msg_size, sizeof(msg_size)));

            asio::streambuf buf;
            std::ostream os(&buf);
            msg.SerializeToOstream(&os);
            asio::write(sock, buf);
        } catch (...) {
            logger::log->debug("An error occured while writing Message {}!", msg.SerializeAsString());
            return false;
        }

        return true;
    }

    /** Receives a protobuf message from the socket.
     * @param sock TCP socket to the remote host
     * @param msg Message that is going to be serialized
     * 
     * Reads the message size first from the socket and then serializes the message from
     * the socket.
     * 
     * @returns True if the message could successfully be serialized; False otherwise.
     */
    static inline bool receive_protobuf(asio::ip::tcp::socket& sock, google::protobuf::Message& msg) {
        u_int64_t msg_size;
        sock.receive(asio::buffer(&msg_size, sizeof(msg_size)), 0);

        asio::streambuf sb;
        asio::streambuf::mutable_buffers_type mb{sb.prepare(msg_size)};

        sb.commit(asio::read(sock, mb));

        std::istream is{&sb};
        return msg.ParseFromIstream(&is);
    }

};

#endif