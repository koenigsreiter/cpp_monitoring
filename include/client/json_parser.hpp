#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <json.hpp>
#include <optional>
#include <vector>

#include "messages.pb.h"

using json = nlohmann::json;

/** A class that parses the JSON config file.
 * The job of this class is to parse an `service`
 * JSON object into a `messages::Config` object.
 */
class json_parser {
public:
    /** Default constructor.
     * This is a simple default constructor that simply does nothing.
     */
    json_parser();
    /** Method that parses the `services` array of the JSON config.
     * @param j A JSON array containing service objects
     * 
     * This method takes an JSON array as an input and converts it into
     * `messages::Config` objects for transmission to the servers. 
     * 
     * Uses `parse_json_to_config_message` to convert the JSON service object into
     * a protobuf message.
     * 
     * @returns An optional containing all the valid configurations; Is automatically
     *          empty if `callback_address` and `callback_port` is not specified.
     */
    std::optional<std::vector<messages::Config>> parse_config(json j);
private:
    /** Wraps the value of given field in an optional for easy default value extraction.
     * @param j JSON service object
     * @param field String specifying the field read from @p j
     * 
     * This method is used to have an easy way of getting an optional value of the JSON
     * service object. If the value of the field is null the method will return an optional
     * containing the value; otherwise it will be empty. 
     * 
     * Example call: 
     * ```cpp
     * int interval = optional_get<int>(j, "interval").value_or(10);
     * ```
     * 
     * @returns An empty optional if the specified fiel is null, otherwise the value of that field.
     */
    template <typename T> std::optional<T> optional_get(json j, 
                                                        std::string field);
    /** Puts the required values into the `ConfigMessage`
     * @param msg Reference to the protobuf ConfigMessage used to configure the server
     * @param type Enum value specifying the type of the message
     * @param j The JSON service object
     * 
     * This method takes the specified JSON object and parses it into the protobuf message.
     * The only required fields in the JSON are:
     * 
     * - address: The address to reach the remote host
     * - port: The remote port to monitor
     * 
     * Other fields that are common to all configs but are not required as they have
     * default values:
     * 
     * - interval: Time in seconds how long to wait between requests; Default 10s
     * - name: A name identifying this specific service in the output; Defaults to 
     * an auto-generated name
     * 
     * @see json_parser::auto_generate_name for the auto generated service name
     * 
     */
    void fill_required_fields(messages::ConfigMessage& msg, 
                              messages::ConfigMessage_ConfigType type, 
                              json j);
    /** Utitity method to auto generate a service name.
     * @param type The type of the Config
     * @param address Remote address of the host to monitor
     * @param port The port to monitor on the remote host
     * 
     * This method automatically generates a name in case the JSON config doesn't specify one.
     * It is structured like the pattern: `<TYPE>@<ADDRESS>:<PORT>`.
     * 
     */
    std::string auto_generate_name(messages::ConfigMessage_ConfigType type, 
                                   std::string address, 
                                   std::string port);
    /** Parses the JSON containing a TCP config.
     * @param tcp_json A JSON object containing all the information for monitoring of a TCP port
     * 
     * This method parses the TCP config. As the TCP config doesn't have any additional
     * required parameters it will simply call fill the default required fields and then return
     * 
     * @see json_parser::fill_required_fields for required fields and their default values
     * 
     * @returns An optional containing the valid ConfigMessage.
     */
    std::optional<messages::ConfigMessage> parse_tcp_json(json tcp_json);
    /** Parses the JSON containing a UDP config.
     * @param udp_json A JSON object containing information to monitor a UDP port
     * 
     * This method parses a UDP config. The UDP config has an additional
     * field `message` that is required. The common configuration values
     * are filled automatically.
     * 
     * @see json_parser::fill_required_fields for required fields and their default values
     * 
     * @returns An optional containing the UDP Config or an empty if the `message`
     *          field was not defined. 
     */
    std::optional<messages::ConfigMessage> parse_udp_json(json udp_json);
    /** Parses the HTTP JSON config.
     * @param http_json A JSON Object containing HTTP related configuration parameters
     * 
     * Parses @p http_json and populates common config values. This type
     * requires a specific field `path` that defines the path to a ressource on the 
     * remote server. 
     * 
     * @see json_parser::fill_required_fields for required fields and their default values
     * 
     * @returns An optional containing the HTTP config; Or an empty if `path` was not 
     *          specified.
     */
    std::optional<messages::ConfigMessage> parse_http_json(json http_json);
    /** Parses the POP3 config.
     * @param pop3_json A JSON service containing configuration values
     * 
     * Takes the @p pop3_json and parses the common values. POP3 requires 2 additional
     * config values:
     * - username: Username on the POP3 server for authentication
     * - password: Password for user authentication on the POP3 server
     * 
     * @see json_parser::fill_required_fields for required fields and their default values
     * 
     * @returns An optional either containing the Config; Or an empty of `username` or 
     *          `password` was not specified.
     */
    std::optional<messages::ConfigMessage> parse_pop3_json(json pop3_json);
    /** Config parsing method for TELNET services.
     * @param telnet_json The JSON config for telnet services
     * 
     * This config requires, beside the common config fields, the `message` field.
     * This message defines the input that is sent to the TELNET server.
     * 
     * @see json_parser::fill_required_fields for required fields and their default values
     * 
     * @returns An optional containing the Config; Or an empty optional if `message` was
     *          not specified.
     */
    std::optional<messages::ConfigMessage> parse_telnet_json(json telnet_json);
    /** Determines the type of service and passes the JSON to the corresponding parser method.
     * @param service The JSON Service where the type is unknown
     * 
     * This method takes a JSON object of undefined type and parses it based on the `type` field.
     * This method requires the `type` field to be defined and to be a string.
     * 
     * @see json_parser::parse_tcp_json If the config is of type "TCP"
     * @see json_parser::parse_udp_json If the config is of type "UDP"
     * @see json_parser::parse_http_json If the config is of type "HTTP"
     * @see json_parser::parse_pop3_json If the config is of type "POP3"
     * @see json_parser::parse_telnet_json If the config is of type "TELNET"
     * 
     * @returns An optional containing the ConfigMessage; Alwas empty if the `type` field is
     *          not specified.
     */
    std::optional<messages::ConfigMessage> parse_json_to_config_message(json service);
};

#endif