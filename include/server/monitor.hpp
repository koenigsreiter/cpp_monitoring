#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
#include "messages.pb.h"

class monitor {
public:
    monitor(){};
    void operator()(std::string callback_address, 
        std::string callback_port, messages::ConfigMessage config);
    virtual ~monitor() = default;
    virtual messages::HealthMessage check(messages::ConfigMessage cfg);
private:
    messages::HealthMessage exec_check(messages::ConfigMessage& config, int& current_request_interval);
};

#endif // MONITOR_HPP
