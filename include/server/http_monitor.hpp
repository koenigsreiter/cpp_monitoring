#ifndef HTTP_MONITOR_HPP
#define HTTP_MONITOR_HPP

#include "monitor.hpp"
#include <string>
#include <curl/curl.h>

class http_monitor : public monitor {
public:
    http_monitor();
    messages::HealthMessage check(messages::ConfigMessage) override;
    ~http_monitor();
    static size_t curl_cb(char* character, size_t size, size_t nmemb, std::string* writer_data);
private:
    CURL* curl_handle;
    char error_buffer[CURL_ERROR_SIZE];
    std::string buf;
};

#endif