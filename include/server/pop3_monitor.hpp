#ifndef POP3_MONITOR_HPP
#define POP3_MONITOR_HPP

#include "monitor.hpp"
#include <curl/curl.h>

class pop3_monitor : public monitor {
public:
    pop3_monitor();
    messages::HealthMessage check(messages::ConfigMessage) override;
    ~pop3_monitor();
private:
    CURL* curl_handle;
    char error_buffer[CURL_ERROR_SIZE];
};

#endif