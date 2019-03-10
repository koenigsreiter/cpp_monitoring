/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#ifndef GRPC_SERVER_HPP
#define GRPC_SERVER_HPP

#include "messages.grpc.pb.h"

#include <grpc++/grpc++.h>
#include <thread>
#include "monitor.hpp"
#include "tcp_monitor.hpp"

/** Handles incoming GRPC calls.
 * 
 * This is a class that handles incoming GRPC calls.
 * The actual definition of the server is defined in 
 * `src/messages.proto`.
 * 
 * The class derives from the auto generated class
 * `messages::Monitor::Service` and handles the `start`
 * method.
 * 
 * @see https://grpc.io/ GRPC Website
 */
class grpc_server final : public messages::Monitor::Service {
public:
    /** Starts the actual monitor based on @p cfg
     * @param ctx ServerContext of GRPC
     * @param cfg Protobuf configuration message
     * @param repl Output message; Empty reply
     * 
     * Responsible for starting a `monitor`. Depending on the 
     * type of the server the respective monitor will be started.
     * 
     * @returns GRPC internal datastructure; 
     *          `Status::Ok` if the `monitor` has been 
     *          started and detached.
     */
    grpc::Status start(grpc::ServerContext* ctx, 
                       const messages::Config* cfg, 
                       messages::Reply* repl) override;
    /** Default destructor. 
     */
    ~grpc_server() = default;
private:
    /** Utility method to start the thread and detach it
     * @param inp The `monitor` class that is going to be started as a thread
     * @param cfg Config message containing necessary information for the monitor
     * 
     * Generic method that takes a `monitor` and starts it in a seperate thread.
     * It also extracts necessary information of the @p cfg parameter and 
     * passes it to the thread.
     */
    template <class T> 
    void start_monitoring_thread(T& inp, const messages::Config* cfg) {
        std::thread monitoring_thread{inp, cfg->callback_address(), cfg->callback_port(), cfg->config()};
        monitoring_thread.detach();
    }
};

#endif