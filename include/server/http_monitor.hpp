#ifndef HTTP_MONITOR_HPP
#define HTTP_MONITOR_HPP

#include "monitor.hpp"

#include <curl/curl.h>

class http_monitor : public monitor {
public:
    http_monitor();
    messages::HealthMessage check(messages::ConfigMessage) override;
    ~http_monitor();
private:
    CURL* curl_handle;
    char error_buffer[CURL_ERROR_SIZE];
};

#endif