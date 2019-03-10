# Client

The client is the main user interface to communicate with the master and is responsible to display the availability of the various services.

## Usage

The client requires the user to specify at least a `JSON` config file with the `-c` commandline flag that contains various configuration settings:

```text
Monitoring tool client to start servers and receive messages.
Usage: ./monitoring_client [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -c,--config-file FILE REQUIRED
                              The JSON config file to load.
  -v,--verbosity TEXT=info (Env:MONITOR_CLIENT_LOG_LEVEL)
                              Debuglevel for the application.
  -p,--port UINT=9696         The port where the monitoring messages will be listened on.
  -g,--grpc-port UINT=4200 (Env:MONITOR_GRPC_PORT)
                              Port to which GRPC will connect.
```

## Preparations

Before the client configures the server it will beforehand do some cleanup work that mostly consists of the validation of the `JSON` config and the healthcheck of the servers.

The first thing the client application does is the healthcheck of the servers. Therefor the client requires the `servers` string-array in the `JSON` config file to be defined. The array contains the address:port mappings of the required servers:

```json
"servers": [
    "localhost:6969"
]
```

The client will afterwards check this server for its availability. If at least one server is available it will check whether the rest of the `JSON` is correct. The `JSON` must at least contain one service and the fields `callback_address` and `callback_port` that define how the server can reach the client:

```json
"callback_address": "localhost",
"callback_port": "9696",
"services": [
    {
        "type": "HTTP",
        "address": "example.com",
        "port": "80",
        "path": "/"
    }
]
```

The valid configuration keys for each service differ and can be either found in the code documentation or in the `example.json` in the root of the project. **Warning! The `example.json` file is not valid but rather pseudo `JSON` containing the various keys for each service!**

If all the previous requirements were met the client will start to configure the servers via *gRPC*.

An activity diagram describing the whole process can be found in Figure \ref{fig:start_sequence}.

![An activity diagram showing the start sequence of the client. First it will start the listener; Check the servers for their health, parse the `JSON` and then configure them via `protobuf`][start_sequence]

## The listener

The listener is a seperate component of the client responsible for listening to `HealthMessages` from the server. It is a seperate thread that also renders the state of the class. The state is realized as a `map<string, HealthMessage>` and contains the most recent changed `HealthMessage`. The listener also has a method `bool update(string key, HealthMessage value)` that compares the message and returns true if the application needs a re-render. This was implemented this way to prevent a lot of unneccessary re-renders.

The `void render()` method is responsible to render the whole state to the console. It uses the console code `\033c` to clear the whole console.

## Relations

The following figure \ref{fig:client_class_hierarchy} is going to explain the class relationships in a UML class diagram.

![A class diagram showing the relations between the client and the listener.][client_class_hierarchy]
