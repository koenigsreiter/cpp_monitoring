#ifndef GRPC_SERVER_HPP
#define GRPC_SERVER_HPP

#include "messages.grpc.pb.h"

#include <grpc++/grpc++.h>
#include <thread>
#include "monitor.hpp"
#include "tcp_monitor.hpp"

class grpc_server final : public messages::Monitor::Service {
public:
    grpc::Status start(grpc::ServerContext* ctx, const messages::Config* cfg, messages::Reply* repl) override;
    ~grpc_server() = default;
private:
    template <class T> 
    void start_monitoring_thread(T& inp, const messages::Config* cfg) {
        std::thread monitoring_thread{inp, cfg->callback_address(), cfg->callback_port(), cfg->config()};
        monitoring_thread.detach();
    }
};

#endif