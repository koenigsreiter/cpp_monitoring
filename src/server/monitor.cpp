#include "monitor.hpp"

#include "logger.hpp"
#include <asio.hpp>
#include "networking.hpp"

messages::HealthMessage monitor::check(messages::ConfigMessage) {
    messages::HealthMessage ret;
    ret.set_status(messages::HealthMessage_Status::HealthMessage_Status_INVALID);
    ret.set_message("THE CONFIG YOU PROVIDED WAS INVALID!");
    logger::log->debug("DEFAULT IMPLEMENTATION OF MONITOR CALLED! NOT RECOMMENDED!");
    return ret;
} 

void monitor::operator()(std::string callback_address, 
    std::string callback_port, messages::ConfigMessage config) {

    int client_not_available = 0;
    int current_request_interval = config.interval();

    using namespace asio::ip;

    asio::io_context ctx;
    tcp::resolver resolver{ctx};

    while (client_not_available <= 5) {

        auto results = resolver.resolve(callback_address, callback_port);
        tcp::socket sock{ctx};

        asio::connect(sock, results);

        while (sock.is_open()) {


            auto res = check(config);
            if (res.status() == messages::HealthMessage_Status::HealthMessage_Status_UP) {
                current_request_interval = config.interval();
            } else {
                current_request_interval *= 2;
            }
            res.set_name(config.name());

            networking::send_protobuf(sock, res);

            messages::HealthCheck valid;
            if (!(networking::receive_protobuf(sock, valid) && valid.msg() == std::string{"RECEIVED"})) {
                logger::log->error("{} didn't receive a complete message!", config.name());
            }

            logger::log->trace("{} sleeping for {}s", config.name(), current_request_interval);
            std::this_thread::sleep_for(std::chrono::seconds(current_request_interval));
        }

        client_not_available += 1;
        logger::log->error("Connection to {} was closed! Attempt {}/5", callback_address, client_not_available);

    }
}