#include "grpc_server.hpp"

#include "logger.hpp"
#include "http_monitor.hpp"

grpc::Status grpc_server::start(grpc::ServerContext* ctx, const messages::Config* cfg, messages::Reply*) {
    if (!ctx) {
        return grpc::Status::CANCELLED;
    }

    logger::log->debug("Received config {}", cfg->DebugString());
    if (cfg->config().type() == messages::ConfigMessage_ConfigType_TCP) {
        logger::log->debug("Got a TCP config!");
        // Star TCP Monitor
        tcp_monitor tcp_mon;
        start_monitoring_thread(tcp_mon, cfg);
    } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_UDP) {
        // Start UDP Monitor
    } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_HTTP) {
        logger::log->debug("Got a HTTP config!");
        // Start HTTP Monitor
        http_monitor http_mon;
        start_monitoring_thread(http_mon, cfg);
    } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_POP3) {
        // Start POP3 Monitor
    } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_TELNET) {
        // Start Telnet Monitor
    } else {
        // Invalid default impl.
    }
    // Start thread & detach
    

    return grpc::Status::OK;
}
