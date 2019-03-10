/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#include "telnet_monitor.hpp"

#include "logger.hpp"

#include <asio.hpp>

messages::HealthMessage telnet_monitor::check(messages::ConfigMessage config) {
    messages::HealthMessage res;

    asio::ip::tcp::iostream tcpstream{config.address(), config.port()};
    if (tcpstream) {
        res.set_status(messages::HealthMessage_Status::HealthMessage_Status_UP);
        tcpstream << config.message();
        std::string tmp;
        std::string msg;

        while (std::getline(tcpstream, tmp)) {
            msg += tmp;
        }

        res.set_message(msg);
    } else {
        res.set_status(messages::HealthMessage_Status::HealthMessage_Status_DOWN);
        logger::log->debug("{}:{} appears to be down.", config.address(), config.port());
    }

    return res;
}

telnet_monitor::~telnet_monitor() {
    // curl_easy_cleanup(curl_handle);
}