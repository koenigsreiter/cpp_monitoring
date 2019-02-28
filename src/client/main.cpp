#include <CLI11.hpp>
#include <thread>

std::string LOGGER_NAME = "monitoring_client";
#include "logger.hpp"

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

    return 0;
}
