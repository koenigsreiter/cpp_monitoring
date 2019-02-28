#include <CLI11.hpp>
#include <thread>

#include <asio.hpp>

std::string LOGGER_NAME = "monitoring_client";
#include "logger.hpp"
#include "networking.hpp"

#include "messages.pb.h"

void test() {

    using namespace asio::ip;

    asio::io_context ctx;
    tcp::resolver resolver{ctx};

    try {
        auto results = resolver.resolve("localhost", "6969");
        tcp::socket sock{ctx};

        asio::connect(sock, results);

        messages::HealthMessage hm;
        hm.set_status(messages::HealthMessage_Status_UP);
        hm.set_message("Hello world!");

        logger::log->debug("Sending msg: {}", hm.SerializeAsString());
        networking::send_protobuf(sock, hm);
    } catch (...) {
        // TODO
    }

}

int main(int argc, char const *argv[])
{
    std::string infile = "";
    std::string debuglevel = "debug";
    unsigned short int listener_port = 9696;

    CLI::App app{"Monitoring tool client to start servers and receive messages."};

    app.add_option("-c,--config-file", infile, "The JSON config file to load.")->required()->check(CLI::ExistingFile);
    app.add_option("-v,--verbosity", debuglevel, "Debuglevel for the application.", true)->envname("MONITOR_CLIENT_LOG_LEVEL");
    app.add_option("-p,--port", listener_port, "The port where the monitoring messages will be listened on.", true);


    CLI11_PARSE(app, argc, argv);
    logger::log->set_level(spdlog::level::from_str(debuglevel));

    test();

    return 0;
}
