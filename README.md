# Monitoring tool

This is a simple monitoring tool used to monitor the following types of services:

- TCP
- UDP
- HTTP
- POP3
- TELNET

The tool communicates via the `asio` library and uses `protobuf` and `GRPC`.

The architecture is designed that each client can connect to *n* servers that each can monitor various services.

In general it has been tried to keep the code as "functional" as possible although this was not always possible.

@dir include/common Includes common header files used by both clients and servers. These files are header-only and concern themselves with logging and networking.
@dir include/client Includes the class definitions for the client. The files are the `JSON` config parser and the `listener` that is constantly listening for incoming health messages.
@dir include/server Contains header files for the server part. Contains both the `GRPC` server definition and all the different monitors. Also contains the `health_checker` class that is used to determine the availability of each master.