#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>
#include <map>
#include "messages.pb.h"
#include <asio.hpp>

class listener {
public:
    listener();
    void operator()(short unsigned int port);
private:
    std::map<std::string, messages::HealthMessage> health_messages;

    std::string construct_string(messages::HealthMessage);
    void render();
    void del(std::string key);
    bool update(std::string key, messages::HealthMessage);
    void serve(asio::ip::tcp::socket sock);
};

#endif // LISTENER_HPP