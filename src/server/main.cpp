/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#include <string>
#include <asio.hpp>
#include <CLI11.hpp>

std::string LOGGER_NAME = "monitoring_server";
#include "logger.hpp"
#include "monitor.hpp"
#include "health_checker.hpp"
#include "messages.grpc.pb.h"
#include "grpc_server.hpp"
#include <curl/curl.h>

int main(int argc, char const *argv[])
{
    std::string debuglevel = "info";
    short unsigned int listen_port = 6969;
    short unsigned int grpc_port = 4200;

    CLI::App app{"Monitor for various services in C++ using gRPC and protobuf"};

    app.add_option("-v,--verbosity", debuglevel, "Debuglevel for the application.", true)->envname("MONITOR_SERVER_LOG_LEVEL");
    app.add_option("-p,--port", listen_port, "Port the healthchecker listens on.", true)->envname("MONITOR_SERVER_HEALTHCHECK_PORT");
    app.add_option("-g,--grpc-port", grpc_port, "Port for the GRPC server.", true)->envname("MONITOR_GRPC_PORT");

    CLI11_PARSE(app, argc, argv);
    logger::log->set_level(spdlog::level::from_str(debuglevel));
    curl_global_init(CURL_GLOBAL_ALL);

    grpc_server server;

    std::string grpc_addr{"0.0.0.0:" + std::to_string(grpc_port)};
    grpc::ServerBuilder server_builder;
    server_builder.AddListeningPort(grpc_addr, grpc::InsecureServerCredentials());
    server_builder.RegisterService(&server);
    auto grpc_srv{server_builder.BuildAndStart()};
    logger::log->info("GRPC is listening on {}", grpc_addr);

    std::thread health_checker_thread{health_checker{}, listen_port};

    grpc_srv->Wait();
    health_checker_thread.join();    
    curl_global_cleanup();
    return 0;
}
