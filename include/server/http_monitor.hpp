#ifndef HTTP_MONITOR_HPP
#define HTTP_MONITOR_HPP

#include "monitor.hpp"
#include <string>
#include <curl/curl.h>

/** Monitors HTTP ressources.
 * 
 * A monitor subclass that is responsibles 
 * for monitoring HTTP ressources.
 * 
 * Uses the cURL library to to connect
 * to the remote server.
 * 
 * @see monitor Monitor superclass that has additional information.
 */
class http_monitor : public monitor {
public:
    /** Initializes the cURL library.
     * 
     * Sets the cURL callback to @p curl_cb and sets some 
     * additional fields.
     */
    http_monitor();
    /** Checks for the status of the HTTP ressource.
     * @param cfg Config for the HTTP server
     * 
     * Connects to the remote HTTP ressource via cURL 
     * and checks whether it is available or not. 
     * 
     * The decision whether a service is "healthy" is 
     * defined by cURL as the return type of the cURL
     * call is used to determine the status of the service. 
     * 
     * @returns A HealthMessage with the status `UP` if the
     *          service is available.
     */
    messages::HealthMessage check(messages::ConfigMessage cfg) override;
    /** Empty default destructor.
     */
    ~http_monitor();
    /** Callback for cURL to prevent output of response to stdout.
     * @param character Unused parameter
     * @param size Size of the response
     * @param nmemb Number of memory blocks
     * @param writer_data Unused parameter
     * 
     * @returns Always returns @p size * @p nmemb.
     */
    static size_t curl_cb(char* character, size_t size, size_t nmemb, std::string* writer_data);
private:
    /** A reference to the cURL instance.
     */
    CURL* curl_handle;
    /** Error buffer that gets populated upon an error.
     */
    char error_buffer[CURL_ERROR_SIZE];
    /** Neccessary for cURL.
     */
    std::string buf;
};

#endif