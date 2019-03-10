#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>
#include <map>
#include "messages.pb.h"
#include <asio.hpp>

/** The listener class listens for incoming `HealthMessages` 
 *  and is responsible to disply them. 
 * 
 * The class listener starts a TCP socket on the specified 
 * CLI port (default 9696) an listens for incoming connections.
 * After an incomming connection a new thread will be started
 * and detached that takes care of updating the state and rerendering. 
 */
class listener {
public:
    /** Default constructor that does nothing.
     */
    listener();
    /** Operator for easy threading; Takes listening port as an input.
     * @param port Port to listen on for incoming connections
     * 
     * Used to start this class easily as a thread. The method opens
     * a new socket on @p port and starts new threads upon new connections. 
     */
    void operator()(short unsigned int port);
private:
    /** Class level state containing the status of each service.
     * 
     * This map stores the status of each service identified by its name.
     * 
     * - key: Service name
     * - value: `messages::HealthMessage` used for rendering
     */
    std::map<std::string, messages::HealthMessage> health_messages;

    /** Constructs the string for printing to the console.
     * @param msg The input message used for constructing the string
     * 
     * Constructs a string in the format:
     * ```
     * <SERVICE_NAME> - <STATUS>
     * <MESSAGE_OF_SERVICE>
     * ``` 
     * where `<MESSAGE_OF_SERVICE>` is specific to the type of service.
     * 
     * - TCP, HTTP, POP3: Return the string "Service is UP" if the 
     *   service is up, or "Service is DOWN" if that is not the case
     * - TELNET, UDP: The response of the service based on `message` in the config
     * 
     * @returns A string representing the state of the service. 
     */
    std::string construct_string(messages::HealthMessage msg);
    /** Renders the state to the console.
     * 
     * Clears the console first and then iterates through the state
     * and prints each service seperated by 10 dashes. 
     * 
     * @see listener::health_messages The global state
     * @see listener::construct_string Information about the generated string
     */
    void render();
    /** Deletes a key of the state in case a thread loses the connection.
     * @param key The key to delete from the state
     * 
     * Takes the @p key and deletes it from the state. This operation also 
     * forces a rerender.
     * 
     * @see listener::render For details about rendering
     * @see listener::health_messages Information about the state of the class
     */
    void del(std::string key);
    /** Updates the value of the state identified by the key.
     * @param key The key which service to update
     * @param msg The new value for @p key
     * 
     * Looks at the state and compares the values of @p msg. If the
     * message been updated it will be updated and true will be returned
     * indicating a rerender of the application. If the @p msg has not
     * been changed since the last iteration nothing will be changed.
     * 
     * Example call:
     * ```cpp
     * if (update(healthmessage.name(), healthmessage)) {
     *     render();
     * }
     * ```
     * 
     * @see listener::render Rendering logic
     * @see listener::health_messages State of this class
     * 
     * @returns True if there was a state in the change and the application
     *          needs a rerender.
     */
    bool update(std::string key, messages::HealthMessage msg);
    /** Method responsible for serving a socket.
     * @param sock A tcp socket serving a server
     * 
     * This method is started in a thread and is responsible to 
     * read incoming messages and update the state accordingly.
     * If a rerender of this application is necessary it will 
     * also trigger a rerender.
     * 
     * If the connection to the server dies it will also call `del`
     * and remove the service from the state.
     * 
     * @see listener::del For the deletion from the state
     * @see listener::health_messages Reference to the class state
     * 
     */
    void serve(asio::ip::tcp::socket sock);
};

#endif // LISTENER_HPP