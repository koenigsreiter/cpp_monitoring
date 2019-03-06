#include "http_monitor.hpp"

#include "logger.hpp"

http_monitor::http_monitor() : curl_handle{curl_easy_init()} {

    if (curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, error_buffer) != CURLE_OK) {
        logger::log->error("Failed to set error buffer");
        return;
    }

    if (curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L) != CURLE_OK) {
        logger::log->error("Failed to set FOLLOWLOCATION! {}", error_buffer);
        return;
    }

    if (curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1L) != CURLE_OK) {
        logger::log->error("Failed to set FAILONERROR! {}", error_buffer);
        return;
    } 

}

messages::HealthMessage http_monitor::check(messages::ConfigMessage config) {
    messages::HealthMessage hm;
    std::string url{"http://" + config.address() + ":" + config.port() + config.path()};

    if (curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str()) != CURLE_OK) {
        hm.set_status(messages::HealthMessage_Status::HealthMessage_Status_INVALID);
        hm.set_message("COULD NOT SET CURL URL!");
        return hm;
    }

    if (curl_easy_perform(curl_handle) == CURLE_OK) {
        hm.set_status(messages::HealthMessage_Status::HealthMessage_Status_UP);
        hm.set_message("Service is UP");
    } else {
        hm.set_status(messages::HealthMessage_Status::HealthMessage_Status_DOWN);
        hm.set_message("Service is DOWN");
    }
    return hm;
}

http_monitor::~http_monitor() {
    // curl_easy_cleanup(curl_handle);
}