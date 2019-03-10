#include "listener.hpp"

#include "networking.hpp"
#include <iostream>
#include <mutex>
#include <fmt/format.h>

std::mutex render_mtx;

listener::listener() {
}

std::string listener::construct_string(messages::HealthMessage msg) {
    return fmt::format("{} - {}\n{}\n",  
            msg.name(), 
            messages::HealthMessage_Status_Name(msg.status()), 
            msg.message());
}

void listener::render() {
    fmt::print("\033c");
    for (const auto& pair : health_messages) {
        fmt::print("{}", construct_string(pair.second));
        for (int i{0}; i < 10; i++) {
            fmt::print("-");
        }
        fmt::print("\n");
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

void listener::del(std::string key) {

    health_messages.extract(key);

    std::cout << "\033c";
    render();
}

void listener::serve(asio::ip::tcp::socket sock) {
    messages::HealthMessage hm;
    messages::HealthCheck repl;
    std::string name; 
    repl.set_msg("RECEIVED");
    while (true) {
        try {
            networking::receive_protobuf(sock, hm);
            {
                std::lock_guard<std::mutex> lk{render_mtx};
                if (update(hm.name(), hm)) {
                    name = hm.name();
                    render();
                }
            }

            networking::send_protobuf(sock, repl);
        } catch (...) {
            std::lock_guard<std::mutex> lk{render_mtx};
            del(name);
            return; // Thread dies and it is going to be recreated on the next connection
        }
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