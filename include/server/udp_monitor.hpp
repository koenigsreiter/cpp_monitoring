#ifndef UDP_MONITOR_HPP
#define UDP_MONITOR_HPP

#include "monitor.hpp"

class udp_monitor : public monitor {
public:
    udp_monitor() {};
    messages::HealthMessage check(messages::ConfigMessage) override;
};

#endif