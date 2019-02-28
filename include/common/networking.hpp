#include <asio.hpp>

namespace networking
{

    using socket = asio::ip::tcp::socket;

    bool send_protobuf(socket& sock, google::protobuf::Message& msg) {
        try {
            u_int64_t msg_size{msg.ByteSize()};

            asio::write(sock, buffer(&msg_size, sizeof(msg_size)));

            asio::streambuf buf;
            std::ostream os(&buf);
            msg.SerializeToOstream(&os);
            asio::write(sock, buf);
        } catch (...) {
            logger::log->debug("An error occured while writing Message {}!", msg.SerializeToString());
            return false;
        }

        return true;
    }

    bool receive_protobuf(socket& sock, google::protobuf::Message& msg) {
        u_int64_t msg_size;
        socket.receive(asio::buffer(&msg_size, sizeof(msg_size)), 0);

        asio::streambuf sb;
        asio::streambuf::mutable_buffers_type mb{sb.prepare(msg_size)};

        sb.commit(asio::read(sock, mb));

        std::istream is{&sb};
        return message.ParseFromIstream(&is);
    }

} // networking
