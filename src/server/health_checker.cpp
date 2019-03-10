/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#include "health_checker.hpp"

#include <asio.hpp>
#include "networking.hpp"
#include "logger.hpp"
#include "messages.pb.h"

void health_checker::operator()(unsigned short int port) {
    logger::log->info("Healthchecker is listening on port {}", port);
    using namespace asio::ip;

    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), port};
    tcp::acceptor acceptor{ctx, ep};
    acceptor.listen();
    while (true) {
        tcp::socket sock{ctx};

        acceptor.accept(sock);

        messages::HealthCheck hm;
        networking::receive_protobuf(sock, hm);
        logger::log->debug("Received: {}", hm.SerializeAsString());

        messages::HealthMessage res;
        res.set_status(messages::HealthMessage_Status_UP);
        res.set_message("UP");
        res.set_name("HealthCheck");

        networking::send_protobuf(sock, res);
    }
}