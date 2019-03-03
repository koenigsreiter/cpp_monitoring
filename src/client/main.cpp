#include <CLI11.hpp>
#include <thread>
#include <optional>
#include <json.hpp>
#include <asio.hpp>

std::string LOGGER_NAME = "monitoring_client";
#include "logger.hpp"
#include "networking.hpp"
#include "json_parser.hpp"
#include "messages.pb.h"

using json = nlohmann::json;

bool check_server_string(std::string inp) {
    return inp.find(":") != std::string::npos;
}

std::optional<std::tuple<std::string, std::string>> split_server_name(const std::string& s) {
    std::vector<std::string> parts;
    char delimiter{':'};
    std::string tmp;
    std::istringstream strm(s);
    while (std::getline(strm, tmp, delimiter)) {
        parts.push_back(tmp);
    }

    try {
        return {{parts.at(0), parts.at(1)}};
    } catch (...) { 
        return {}; 
    }
}

bool health_check_server(std::string server_string) {
    
    logger::log->info("Checking server {}", server_string);
    auto opt = split_server_name(server_string);
    
    if (!opt) {
        return false;
    }

    auto [address, port] = opt.value();


    using namespace asio::ip;
    asio::io_context ctx;
    tcp::resolver resolver{ctx};

    try {
        auto results = resolver.resolve(address, port);
        tcp::socket sock{ctx};

        asio::connect(sock, results);

        messages::HealthCheck req;
        req.set_msg("HEALTH");
        networking::send_protobuf(sock, req);

        messages::HealthMessage res;
        return networking::receive_protobuf(sock, res) && 
            (res.status() == messages::HealthMessage_Status_UP);

    } catch(...) {
        logger::log->info("Server {} was not available!", server_string);
        return false;
    }
}

std::optional<std::vector<std::string>> check_available_servers(json j) {
    if (j["servers"].is_null() || !j["servers"].is_array()) {
        return {};
    }

    // TODO: Implement actual healthcheck
    std::vector<std::string> servers = j["servers"];
    std::vector<std::string> applicable_servers;
    std::copy_if(std::begin(servers), std::end(servers), 
        std::back_inserter(applicable_servers), check_server_string);

    if (applicable_servers.size() == 0) {
        return {};
    }

    std::vector<std::string> reachable_servers;
    std::copy_if(std::begin(applicable_servers), std::end(applicable_servers),
        std::back_inserter(reachable_servers), health_check_server);

    logger::log->info("{} servers are actually healthy/reachable.", reachable_servers.size());

    if (reachable_servers.size() == 0) {
        return {};
    }

    return reachable_servers;
}

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
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::string infile = "";
    std::string debuglevel = "debug";
    unsigned short int listener_port = 9696;
    unsigned short int grpc_port = 4200;

    CLI::App app{"Monitoring tool client to start servers and receive messages."};

    app.add_option("-c,--config-file", infile, "The JSON config file to load.")->required()->check(CLI::ExistingFile);
    app.add_option("-v,--verbosity", debuglevel, "Debuglevel for the application.", true)->envname("MONITOR_CLIENT_LOG_LEVEL");
    app.add_option("-p,--port", listener_port, "The port where the monitoring messages will be listened on.", true);
    app.add_option("-g,--grpc-port", grpc_port, "Port to which GRPC will connect.", true)->envname("MONITOR_GRPC_PORT");


    CLI11_PARSE(app, argc, argv);
    logger::log->set_level(spdlog::level::from_str(debuglevel));

    json j;
    {
        std::ifstream i{infile};
        i >> j;
    }

    auto servers = check_available_servers(j);
    if (!servers) {
        logger::log->critical("No server appear to be valid; Exiting now...");
        return 0;
    }

    json_parser jp;
    auto configs = jp.parse_config(j);
    if (!configs) {
        logger::log->critical("No config appears to be valid; Exiting now...");
        return 0;
    }
    logger::log->debug("{} valid configs!", configs->size());
    for (auto& cfg : configs.value()) {
        logger::log->debug("{}", cfg.DebugString());
    }


    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
