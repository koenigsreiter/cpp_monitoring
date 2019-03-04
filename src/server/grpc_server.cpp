#include "grpc_server.hpp"

grpc::Status grpc_server::start(grpc::ServerContext* ctx, grpc::ServerReader<messages::Config>* reader, messages::Reply* repl) {
    messages::Config* cfg = new messages::Config();

    while (reader->Read(cfg)) {
        monitor mon;
        if (cfg->config().type() == messages::ConfigMessage_ConfigType_TCP) {
            // Star TCP Monitor
            mon = tcp_monitor();
        } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_UDP) {
            // Start UDP Monitor
        } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_HTTP) {
            // Start HTTP Monitor
        } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_POP3) {
            // Start POP3 Monitor
        } else if (cfg->config().type() == messages::ConfigMessage_ConfigType_TELNET) {
            // Start Telnet Monitor
        } else {
            // Invalid default impl.
            mon = monitor{};
        }
        // Start thread & detach
        std::thread monitoring_thread{mon, cfg->callback_address(), cfg->callback_port(), cfg->config()};
        monitoring_thread.detach();

    }

    return grpc::Status::OK;
}
