/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#include "udp_monitor.hpp"

#include "logger.hpp"
#include <asio.hpp>
#include <sstream>
#include <thread>
#include <chrono>
#include <future>

messages::HealthMessage udp_monitor::check(messages::ConfigMessage cfg) {
    messages::HealthMessage res;

    using namespace asio::ip;

    asio::io_context ctx;
    udp::resolver resolver{ctx};
    udp::endpoint receiver_ep = *resolver.resolve(udp::v4(), cfg.address(), cfg.port()).begin();

    udp::socket sock{ctx};
    sock.open(udp::v4());
    if (sock.is_open()) {

        sock.send_to(asio::buffer(cfg.message()), receiver_ep);
        udp::endpoint sender;
        std::string msg(sock.available(), ' ');

        if (sock.available() != 0) {
            sock.receive_from(asio::buffer(msg), sender);
            res.set_message(msg);
            res.set_status(messages::HealthMessage_Status::HealthMessage_Status_UP);

        } else {
            res.set_message("Service didn't respond");
            res.set_status(messages::HealthMessage_Status::HealthMessage_Status_DOWN);
        }

    } else {
        res.set_message("Service is DOWN");
        res.set_status(messages::HealthMessage_Status::HealthMessage_Status_DOWN);
    }

    return res;
    
}