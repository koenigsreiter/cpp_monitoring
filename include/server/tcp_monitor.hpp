#ifndef TCP_MONITOR_HPP
#define TCP_MONITOR_HPP

#include "monitor.hpp"

/** Class responsible for monitoring TCP services.
 * 
 * Creates a `asio::ip::tcp::tcpstream` and checks 
 * whether the connection was successful. If the
 * connections was successful the service is considered
 * healthy.
 *  
 * @see monitor Monitor superclass that has additional information.
 */
class tcp_monitor : public monitor {
public:
    /** Default constructor; Does nothing.
     */
    tcp_monitor() {};
    /** Check implementaion for TCP services.
     * @param cfg Configuration for the TCP service
     * 
     * Connects to the service via a `asio::ip::tcp::tcpstream`.
     * If the boolean operator of the stream returns true the
     * service is considered healthy.
     * 
     * @returns A `HealthMessage` with the status of up if the stream
     *          could successfully connect to the remote server.
     */
    messages::HealthMessage check(messages::ConfigMessage cfg) override;
};

#endif