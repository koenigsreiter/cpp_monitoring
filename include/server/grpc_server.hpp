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
};

#endif