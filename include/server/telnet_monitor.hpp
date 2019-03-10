/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#ifndef TELNET_MONITOR_HPP
#define TELNET_MONITOR_HPP

#include "monitor.hpp"

/** Monitors TELNET ressources by connecting to the ressource via TCP.
 * 
 * Creates a TCP connection to the server
 * and sends the specified command to the server
 * as specified by @p cfg.message. 
 * 
 * @see monitor Monitor superclass that has additional information.
 */
class telnet_monitor : public monitor {
public:
    /** Default constructor; Does nothing.
     */
    telnet_monitor() {};
    /** Check funtion for TELNET monitor.
     * @param cfg Configuration containing the `message` 
     *            to send to the server
     * 
     * Connects to the remote server via a `asio::ip::tcp::tcstream`
     * and then sends the @p cfg.message to the server, stores the response
     * and sends it back to the client.
     * 
     * @returns A `messages::HealthMessage` with the message containing
     *          the response of the server and the status `UP` if the
     *          server was available; The status `DOWN` if the 
     *          service was not available and a message saying that 
     *          the service is not available.
     */
    messages::HealthMessage check(messages::ConfigMessage cfg) override;
    /** Empty class destructor.
     */
    ~telnet_monitor();
private:
};

#endif