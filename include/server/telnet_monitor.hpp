#ifndef TELNET_MONITOR_HPP
#define TELNET_MONITOR_HPP

#include "monitor.hpp"
#include <curl/curl.h>

class telnet_monitor : public monitor {
public:
    telnet_monitor() {};
    messages::HealthMessage check(messages::ConfigMessage) override;
    ~telnet_monitor();
private:
};

#endif