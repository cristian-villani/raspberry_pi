# Running the Client and Server

This project demonstrates a simple TCP client-server connection between a PC and a Raspberry Pi, exchanging simulated sensor readings.

1. Compile the program on client and server

```
make
```

2. Start the Server (Raspberry Pi)

```
./server
```

The server listens on port 5000 (default).

It prints incoming readings to the console.

Make sure the Raspberry Pi is reachable from your PC. You can do this via:

 -`/etc/hosts` entry

 - Static IP on the Raspberry Pi

3. Start the Client (PC)

```
./client [hostname_or_ip] [port]
```

hostname_or_ip (optional): The target Raspberry Pi hostname or IP address.

Default: raspberrypi (matches /etc/hosts entry on the PC)

port (optional): The server port. Default is 5000.

**Note:** the port argument is optional, but can only be set if a hostname/IP is also provided.

Example 1: default host and port

```
./client
```

Example 2: specify host

```
./client raspberrypi
```

Example 3: specify host and port

```
./client raspberrypi 5000
```

The client sends a simulated sensor reading (random number) every
second to the server.

## Hostname resolution

The client resolves the Raspberry Pi hostname using `/etc/hosts`
for reliability.

Example `/etc/hosts` entry:

```
192.168.0.52   raspberrypi
```

## Design Notes and Scope

This project intentionally implements a simple, blocking TCP server that handles one client connection at a time.

The goal is to keep the focus on:

-TCP socket programming in C

-Client–server protocol design

-Hostname resolution and identification

-Simulated sensor data generation

-Structured logging and debugging

While the server can accept multiple connection attempts, it processes clients sequentially and blocks while receiving data from a connected client. This behavior is intentional and helps keep the implementation easy to understand and reason about.

Supporting multiple concurrent clients would require additional mechanisms such as:

-Process-based concurrency (fork())

-Thread-based concurrency (pthread)

-Event-driven I/O (select(), poll())

These approaches are deliberately out of scope for this project and are planned as a separate follow-up project to clearly demonstrate system evolution and scalability.

## Sensor Simulation

Sensor readings are simulated on the client side using a pseudo-random number generator seeded at program start. This ensures:

-Realistic, changing values across program executions

-No hardware dependency

-Clear separation between networking logic and sensor logic

Currently, a single humidity sensor is simulated, but the design allows easy extension to additional sensors.

## Future Work

Possible extensions include:

-Multi-client server using fork() or threads

-Structured CSV or JSON logging to file

-Support for multiple sensors per client

-MQTT-based communication for IoT-style deployments

-Event-driven server implementation for scalability
