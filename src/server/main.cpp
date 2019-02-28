#include <CLI11.hpp>
#include "messages.pb.h"
#include <asio.hpp>

std::string LOGGER_NAME = "monitoring_server";
#include "logger.hpp"
#include "networking.hpp"

void test() {
    using namespace asio::ip;

    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), 6969};
    tcp::acceptor acceptor{ctx, ep};
    acceptor.listen();
    tcp::socket sock{ctx};

    acceptor.accept(sock);

    messages::HealthMessage hm;
    networking::receive_protobuf(sock, hm);
    logger::log->debug("Received: {}", hm.SerializeAsString());
}

int main(int argc, char const *argv[])
{
    std::string debuglevel = "debug";
    short unsigned int listen_port = 6969;

    CLI::App app{"Monitor for various services in C++ using gRPC and protobuf"};

    app.add_option("-v,--verbosity", debuglevel, "Debuglevel for the application.", true)->envname("MONITOR_SERVER_LOG_LEVEL");
    app.add_option("-p,--port", listen_port, "Port the healthchecker listens on.", true)->envname("MONITOR_SERVER_HEALTHCHECK_PORT");

    CLI11_PARSE(app, argc, argv);
    logger::log->set_level(spdlog::level::from_str(debuglevel));

    test();
    
    return 0;
}
