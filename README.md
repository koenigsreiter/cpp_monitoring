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