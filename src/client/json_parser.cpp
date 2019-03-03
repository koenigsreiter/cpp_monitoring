#include "json_parser.hpp"

#include <string>
#include <algorithm>

#include "logger.hpp"

json_parser::json_parser() {}

template <typename T> std::optional<T> json_parser::optional_get(json j, std::string field) {
    if (j[field].is_null()) {
        return {};
    }
    return j[field];
}

std::string json_parser::auto_generate_name(messages::ConfigMessage_ConfigType type, std::string address, std::string port) {
    return messages::ConfigMessage_ConfigType_Name(type) + "@" + address + ":" + port;
}

void json_parser::fill_required_fields(messages::ConfigMessage& msg, messages::ConfigMessage_ConfigType type, json j) {
    msg.set_type(type);
    msg.set_interval(optional_get<int>(j, "interval").value_or(10));
    msg.set_address(j["address"]);
    msg.set_port(j["port"]);
    msg.set_name(optional_get<std::string>(j, "name").value_or(
        auto_generate_name(type, j["address"], j["port"])
    ));
}

std::optional<messages::ConfigMessage> json_parser::parse_tcp_json(json tcp_json) {

    auto tcp_msg = messages::ConfigMessage_ConfigType::ConfigMessage_ConfigType_TCP;

    messages::ConfigMessage ret;
    fill_required_fields(ret, tcp_msg, tcp_json);

    return ret;
}

std::optional<messages::ConfigMessage> json_parser::parse_udp_json(json udp_json) {
    if (udp_json["message"].is_null()) {
        return {};
    }
    
    auto udp_msg = messages::ConfigMessage_ConfigType::ConfigMessage_ConfigType_UDP;

    messages::ConfigMessage ret;
    fill_required_fields(ret, udp_msg, udp_json);
    ret.set_message(udp_json["message"]);

    return ret;
}

std::optional<messages::ConfigMessage> json_parser::parse_http_json(json http_json) {
    if (http_json["path"].is_null()) {
        return {};
    }

    auto http_msg = messages::ConfigMessage_ConfigType::ConfigMessage_ConfigType_HTTP;

    messages::ConfigMessage ret;
    fill_required_fields(ret, http_msg, http_json);
    ret.set_path(http_json["path"]);

    return ret;
}

std::optional<messages::ConfigMessage> json_parser::parse_pop3_json(json pop3_json) {
    if (pop3_json["username"].is_null() || pop3_json["password"].is_null()) {
        return {};
    }

    auto pop3_msg = messages::ConfigMessage_ConfigType::ConfigMessage_ConfigType_POP3;

    messages::ConfigMessage ret;
    fill_required_fields(ret, pop3_msg, pop3_json);
    ret.set_username(pop3_json["username"]);
    ret.set_password(pop3_json["password"]);

    return ret;
}

std::optional<messages::ConfigMessage> json_parser::parse_telnet_json(json telnet_json) {
    
    auto telnet_msg = messages::ConfigMessage_ConfigType::ConfigMessage_ConfigType_TELNET;

    messages::ConfigMessage ret;
    fill_required_fields(ret, telnet_msg, telnet_json);

    return ret;
}


std::optional<messages::ConfigMessage> json_parser::parse_json_to_config_message(json service) {
    std::string type = service["type"];
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    std::string tcp{"tcp"};
    std::string udp{"udp"};
    std::string http{"http"};
    std::string telnet{"telnet"};
    std::string pop3{"pop3"};
 
    if (type == tcp) {
        return parse_tcp_json(service);
    } else if (type == udp) {
        return parse_udp_json(service);
    } else if (type == http) {
        return parse_http_json(service);
    } else if (type == telnet) {
        return parse_telnet_json(service);
    } else if (type == pop3) {
        return parse_telnet_json(service);
    } else {
        return {};
    }
}


std::optional<std::vector<messages::Config>> json_parser::parse_config(json j) {
    if (j["callback_address"].is_null() || j["callback_port"].is_null()) {
        logger::log->critical("No callback address or port was given; Please specify them in the config");
        return {};
    }

    std::vector<messages::Config> ret;



    for (auto& service : j["services"]) {
        if (service["address"].is_null() || service["port"].is_null()) {
            logger::log->critical("Service {} has no address or port specified; Please correct the issue.", service.dump());
            continue;
        }

        messages::Config config;
        config.set_callback_address(j["callback_address"]);
        config.set_callback_port(j["callback_port"]);

        auto cfg_msg = parse_json_to_config_message(service);
        if (cfg_msg) {
            config.set_allocated_config(&cfg_msg.value());
        } else {
            logger::log->error("Could not parse service: {}", service.dump());
            continue;
        }
        ret.push_back(config);
        config.release_config();
    }

    if (ret.size() == 0) {
        return {};
    }

    return ret;
}