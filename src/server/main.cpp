#include <CLI11.hpp>
#include "messages.pb.h"
#include <asio.hpp>
#include <thread>

std::string LOGGER_NAME = "monitoring_server";
#include "logger.hpp"
#include "networking.hpp"


void health_check(unsigned short int port) {
    using namespace asio::ip;

    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), port};
    tcp::acceptor acceptor{ctx, ep};
    acceptor.listen();
    while (true) {
        tcp::socket sock{ctx};

        acceptor.accept(sock);

        messages::HealthCheck hm;
        networking::receive_protobuf(sock, hm);
        logger::log->debug("Received: {}", hm.SerializeAsString());

        messages::HealthMessage res;
        res.set_status(messages::HealthMessage_Status_UP);
        res.set_message("UP");
        res.set_name("HealthCheck");

        networking::send_protobuf(sock, res);
    }
}

int main(int argc, char const *argv[])
{
    std::string debuglevel = "debug";
    short unsigned int listen_port = 6969;
    short unsigned int grpc_port = 4200;

    CLI::App app{"Monitor for various services in C++ using gRPC and protobuf"};

    app.add_option("-v,--verbosity", debuglevel, "Debuglevel for the application.", true)->envname("MONITOR_SERVER_LOG_LEVEL");
    app.add_option("-p,--port", listen_port, "Port the healthchecker listens on.", true)->envname("MONITOR_SERVER_HEALTHCHECK_PORT");
    app.add_option("-g,--grpc-port", grpc_port, "Port for the GRPC server.", true)->envname("MONITOR_GRPC_PORT");

    CLI11_PARSE(app, argc, argv);
    logger::log->set_level(spdlog::level::from_str(debuglevel));

    std::thread health_checker{health_check, listen_port};

    health_checker.join();    
    return 0;
}
