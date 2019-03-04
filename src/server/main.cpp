#include <string>
#include <asio.hpp>
#include <CLI11.hpp>

std::string LOGGER_NAME = "monitoring_server";
#include "logger.hpp"
#include "monitor.hpp"
#include "health_checker.hpp"


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

    std::thread health_checker_thread{health_checker{}, listen_port};

    health_checker_thread.join();    
    return 0;
}
