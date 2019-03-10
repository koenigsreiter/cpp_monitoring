# Server

The server is the actual monitor of each service. It will consistently check a service for its availability and will report the status to the client. The Server has defaults for all important parameter set so it can be started with its default values. All parameters can optionally be set via environment variables.

## Usage

```text
Monitor for various services in C++ using gRPC and protobuf
Usage: ./monitoring_server [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -v,--verbosity TEXT=info (Env:MONITOR_SERVER_LOG_LEVEL)
                              Debuglevel for the application.
  -p,--port UINT=6969 (Env:MONITOR_SERVER_HEALTHCHECK_PORT)
                              Port the healthchecker listens on.
  -g,--grpc-port UINT=4200 (Env:MONITOR_GRPC_PORT)
                              Port for the GRPC server.
```

## Server monitoring algorithm

The server is realized as a set of multiple classes responsible for the monitoring of the respective service. The general algorithm can be found in Figure \ref{fig:server_mainloop}.

![A sequence diagram visualizing the steps executed of `monitor` to monitor a single service.][server_mainloop]

This functionality is implemented in the `monitor` class in its `operator()()`. The step *check for availability of service* is a special step as it is implemented by each subclass. This is the virtual method `check` of the monitor.

The class hierarchy of the various monitors can be found in Figure \ref{fig:monitor_variations}.

![Class hierarchy of the monitor. Monitor is a general class where each subclass implements the `check` function that takes care of actually checking for the availability of the service.][monitor_variations]

### Client communication

The communication to the client is a simple request/response protocol. The server sends the result of the `check` function as a protobuf message via the network to the client. If the client can successfully parse the received message it will reply with a simple protobuf message containing a string that states **RECEIVED**.

### Error handling

If the client loses the network connectivity the server will try to reconnect to the client five times. Each time the client does not respond it will increase the time to the next connection attempt.

## Health checker

The server also contains a component called the `health_checker`. The health checker is used by the client to perform some checks before actually starting. This component is a separate thread on the server that is continuously listening for incoming connections and send a `HealthMessage` as soon as a client connects to indicate that the server is up and running.

## Relations

For additional information about the relations between the various functions see figure \ref{fig:server_class_hierarchy}.

![An UML showing the relationships between the various server-side classes. For additional information about the monitor relations see figure \ref{fig:monitor_variations}][server_class_hierarchy]
