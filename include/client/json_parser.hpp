#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <json.hpp>
#include <optional>
#include <vector>

#include "messages.pb.h"

using json = nlohmann::json;

/** A class that parses the JSON config file
 * This class mostly concerns itself with parsing the given JSON config into
 * the various protobuf Messages.
 */
class json_parser {
public:
    json_parser();
    std::optional<std::vector<messages::Config>> parse_config(json j);
private:
    template <typename T> std::optional<T> optional_get(json j, std::string field);
    void fill_required_fields(messages::ConfigMessage& msg, messages::ConfigMessage_ConfigType type, json j);
    std::string auto_generate_name(messages::ConfigMessage_ConfigType type, std::string address, std::string port);
    std::optional<messages::ConfigMessage> parse_tcp_json(json tcp_json);
    std::optional<messages::ConfigMessage> parse_udp_json(json udp_json);
    std::optional<messages::ConfigMessage> parse_http_json(json http_json);
    std::optional<messages::ConfigMessage> parse_pop3_json(json pop3_json);
    std::optional<messages::ConfigMessage> parse_telnet_json(json telnet_json);
    std::optional<messages::ConfigMessage> parse_json_to_config_message(json service);
};

#endif