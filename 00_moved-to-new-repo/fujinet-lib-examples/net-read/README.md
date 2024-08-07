# net-read

This is a cross platform application using network-library to test reading resources from network.

## Running REST API service

See [alphabet rest api docs](alphabet-rest-api/README.md)

## Building the application

Because the application will send requests to the REST service, it needs to know where it is located.
If you are building on the same machine as the service runs on, you can just run make.

If you are running the REST API on a different machine to where you are building the application, you will need to pass a parameter for the target machine that the service is running on, and port if it is not 8080.

NOTE: I've not been able to get the fujinet application to see the hostname of the app server via its `hostname`, so I use the 2nd form below with the IP address of the REST server.

```shell
# Building on machine where REST service will run, and on default port of 8080, just do:
make

# Building on different machine to where REST service will run, do following - uses obnoxious amount of backslashes required to force quote symbols into make
make REST_SERVER_ADDRESS=\\\"IP-of-rest-server\\\" REST_SERVER_PORT=\\\"8081\\\"
```

Change the port value as required. The default is 8080, so you do not need the parameter at all if you're running it on that port.

And then make a target file:

```shell
# Build an ATR for atari
make TARGETS=atari atr

# Build a PO for apple2
make TARGETS=apple2 po
```
