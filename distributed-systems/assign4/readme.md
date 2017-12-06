# Distributed Systems - Programming Assignment 1

## Socket Benchmarking

Group Members

- Mike Botti
- Farah Sultana
- Zachary Blanco

## Pre-Requisites

- Python 3+

(Already installed on iLab machines)

## Benchmark Instructions

To benchmark you need to start a server on a separate machine.

```
python3 better_server.py
```

The server runs two threads. One for TCP and one for UDP benchmarking. The acknowledgement protocol is used determined when the client begins sending messages.

From here you can either write a custom python script to run the client benchmarks or you can use the `better_client.py` to run benchmarks from the command line.

`sock_tester.py` must reside in the same directory as `better\_client.py` and `better\_server.py`.

### `better_client.py`

Simply run `python3 better_client.py` and you will be prompted to enter the benchmark parameters such as TCP, UDP, port number, and acknowledgement protocol.

The the final parameters of the benchmark will be printed out after the transfer has occurred.

### `better_server.py`

Simply run `python3 better_server.py` and you will be prompted to run the server on a specific port. The port chosen will be used for TCP connections. UDP connections will run on port + 1.

### Custom Benchmarking Script

The `sock_tester.py` file contains the TCPClient and UDPClient classes. So you can write a custom benchmarking script instantiating each type of client to run benchmarks.


Example Script:

```
from sock_tester import TCPClient, UDPClient

if __name__ == "__main__":
    client = TCPClient('127.0.0.1', 8080, acks=False) # (host, port, acknowledgement_protocol)
    for x in range(100):
        client.run(2 ** x,  (2 ** x) * 1000) # (msg_size, transfer_size)
```