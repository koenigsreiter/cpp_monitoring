#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include "logger.hpp"
#include <asio.hpp>
#include <google/protobuf/message.h>

class networking
{
public:

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