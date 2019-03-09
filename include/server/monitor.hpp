#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
#include "messages.pb.h"

/** Super class for all monitors; Handles error correction and service checks.
 * 
 * The monitor class is the base for all monitors. The virtual @p check method
 * takes care of the actual service checks.
 *
 */  
class monitor {
public:
    /** Default monitor; Does nothing.
     */
    monitor(){};
    /** Operator for easy threading.
     * @param callback_address The address of the remote client
     * @param callback_port The port of the remote client
     * @param config The service configuration to check
     * 
     * This is the main entry of the class and responsible for 
     * calling the actual implementation of @p check.
     * 
     * # Error handling
     * 
     * The monitor has some primitive type of error handling. 
     * If the service is not available it will increase the delay
     * between requests in case the service needs some time to
     * recover itself. After the service has recovered itself 
     * it will default back to its original request delay.
     * 
     * If the client appears to be down it will try to 
     * reconnect to the client **five** times before stopping
     * to contact the server.
     */
    void operator()(std::string callback_address, 
        std::string callback_port, messages::ConfigMessage config);
    /** Default destructor for monitor
     */
    virtual ~monitor() = default;
    /** Virtual function that checks the actual service.
     * @param cfg The config for the service
     * 
     * A virtual function that is doing the actual checking
     * of the service. The implementation is depending based
     * on the service to check. See the respective subclass
     * for the implementation details.
     * 
     * @returns A `messagse::HealthMessage` with the status of `UP` 
     *          if the service is reachable; The status is `DOWN`
     *          in the other case.
     */
    virtual messages::HealthMessage check(messages::ConfigMessage cfg);
private:
    /** Utility method to increase readability.
     * 
     * A simple method that actually calls the service checking
     * but due to error handling via try/catch has been
     * moved into its own function.
     * 
     * @returns A `messagse::HealthMessage` with the status of `UP` 
     *          if the service is reachable; The status is `DOWN`
     *          in the other case.
     */
    messages::HealthMessage exec_check(messages::ConfigMessage& config, int& current_request_interval);
};

#endif // MONITOR_HPP
