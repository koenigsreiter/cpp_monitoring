/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#ifndef UDP_MONITOR_HPP
#define UDP_MONITOR_HPP

#include "monitor.hpp"

/** Monitor responsible for monitoring UDP Ressources.
 * 
 * Connects to a UDP port and checks whether the socket is open
 * and receives input.
 * 
 * @see monitor Monitor superclass that has additional information.
 */
class udp_monitor : public monitor {
public:
    /** Default constructor; Does nothing.
     */
    udp_monitor() {};
    /** Checks the UDP service for availability.
     * @param cfg Config object for the monitor
     * 
     * Connects via UDP to the remote port and sends
     * the specified @p cfg.message. Returns the 
     * output if any was received.
     * 
     * @returns A `HealthMessage` with the status of `UP` 
     *          and the output of the service if any was received.
     *          Otherwise the status is `DOWN`.
     */
    messages::HealthMessage check(messages::ConfigMessage cfg) override;
};

#endif