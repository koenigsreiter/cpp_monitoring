#include "listener.hpp"

#include "networking.hpp"
#include <iostream>

listener::listener() {

}

std::string listener::construct_string(messages::HealthMessage msg) {
    return msg.name() + " - " + messages::HealthMessage_Status_Name(msg.status()) + " {" + msg.message() + "}";
}

void listener::render() {
    std::cout << "\033c";
    for (const auto& pair : health_messages) {
        std::cout << construct_string(pair.second) << std::endl;
        for (int i{0}; i < 10; i++) {
            std::cout << "-";
        }
        std::cout << std::endl;
    }
}

bool listener::update(std::string key, messages::HealthMessage msg) {

    if (health_messages.find(key) == std::end(health_messages)) {
        health_messages[key] = msg;
        return true;
    }

    auto old_msg = health_messages[key];

    if (construct_string(old_msg) != construct_string(msg)) {
        health_messages[key] = msg;
        return true;
    }

    return false;

}

void listener::serve(asio::ip::tcp::socket sock) {
    messages::HealthMessage hm;
    messages::HealthCheck repl;
    repl.set_msg("RECEIVED");
    while (true) {
        networking::receive_protobuf(sock, hm);
        if (update(hm.name(), hm)) {
            render();
        }

        networking::send_protobuf(sock, repl);
    }
}

void listener::operator()(short unsigned int port) {
    logger::log->info("Monitoring message receiver is listening on port {}", port);
    using namespace asio::ip;

    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), port};
    tcp::acceptor acceptor{ctx, ep};
    acceptor.listen();
    while (true) {
        tcp::socket sock{ctx};

        acceptor.accept(sock);

        std::thread list_thread{&listener::serve, this, std::move(sock)};
        list_thread.detach();
        
    }
}