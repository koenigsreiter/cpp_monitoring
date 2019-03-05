#ifndef TCP_MONITOR_HPP
#define TCP_MONITOR_HPP

#include "monitor.hpp"

class tcp_monitor : public monitor {
public:
    tcp_monitor() {};
    messages::HealthMessage check(messages::ConfigMessage) override;
};

#endif