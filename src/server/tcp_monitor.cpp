/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#include "tcp_monitor.hpp"

#include "logger.hpp"
#include <asio.hpp>

messages::HealthMessage tcp_monitor::check(messages::ConfigMessage config) {
    messages::HealthMessage res;

    asio::ip::tcp::iostream tcpstream{config.address(), config.port()};
    if (tcpstream) {
        res.set_status(messages::HealthMessage_Status::HealthMessage_Status_UP);
    } else {
        res.set_status(messages::HealthMessage_Status::HealthMessage_Status_DOWN);
        logger::log->debug("{}:{} appears to be down.", config.address(), config.port());
    }

    return res;
    
}