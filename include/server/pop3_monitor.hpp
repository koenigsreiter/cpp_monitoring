#ifndef POP3_MONITOR_HPP
#define POP3_MONITOR_HPP

#include "monitor.hpp"
#include <curl/curl.h>

/** Monitors POP3 ressources.
 * 
 * Responsible for checking a POP3 service on a remote
 * server using cURL.  
 * 
 * @see monitor Monitor superclass that has additional information.
 */
class pop3_monitor : public monitor {
public:
    /** Default constructor; Sets some fields of the cURL library.
     * @see http_monitor::http_monitor For additional information
     */
    pop3_monitor();
    /** Checks for the status of the POP3 ressource.
     * @param cfg Config for the POP3 server
     * 
     * Connects to the remote POP3 ressource via cURL 
     * and checks whether it is available or not. 
     * 
     * The decision whether a service is "healthy" is 
     * defined by cURL as the return type of the cURL
     * call is used to determine the status of the service. 
     * 
     * @returns A HealthMessage with the status `UP` if the
     *          service is available.
     */
    messages::HealthMessage check(messages::ConfigMessage) override;
    /** Empty default destructor.
     */
    ~pop3_monitor();
    /**
     * @see http_monitor::curl_cb For additional information.
     */
    static size_t curl_cb(char* character, size_t size, size_t nmemb, std::string* writer_data);
private:
    /**
     * @see http_monitor::curl_handle For additional information
     */
    CURL* curl_handle;
    /**
     * @see http_monitor::error_buffer For additional information
     */
    char error_buffer[CURL_ERROR_SIZE];
    /**
     * @see http_monitor::buf For additional information
     */
    std::string buf;
};

#endif