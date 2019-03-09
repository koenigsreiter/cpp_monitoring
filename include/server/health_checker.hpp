#ifndef HEALTH_CHECK_HPP
#define HEALTH_CHECK_HPP

/** Thread that listens HealthCheck messages.
 * 
 * Seperate thread to check whether the whether the 
 * master is available or not.
 * Creates a TCP socket on the specified port and
 * continously listens for protobuf messages.
 * 
 */
class health_checker {
public:
    /** Operator to start the thread easily.
     * @param port The port to listen on for incoming connections
     * 
     * Creates a TCP socket and infinetely listens for
     * `messages::HealthCheck` messages. If 
     * the server is online it will reply with a 
     * `messages::HealthMessage` where it says it is
     * up.
     */
    void operator()(unsigned short int port);

};

#endif