#include "logger.hpp"
#include <CLI11.hpp>

#include "messages.pb.h"
#include <asio.hpp>

int main(int argc, char const *argv[])
{
    std::string debuglevel = "debug";
    short unsigned int listen_port = 6969;

    CLI::App app{"Monitor for various services in C++ using gRPC and protobuf"};

    app.add_option("-v,--verbosity", debuglevel, "Debuglevel for the application.", true)->envname("MONITOR_SERVER_LOG_LEVEL");
    app.add_option("-p,--port", listen_port, "Port the healthchecker listens on.", true)->envname("MONITOR_SERVER_HEALTHCHECK_PORT");

    CLI11_PARSE(app, argc, argv);
    logger::log->set_level(spdlog::level::from_str(debuglevel));
    
    return 0;
}
