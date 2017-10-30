'''Client
Your client will accept command-line parameters that include:

Hostname (or IP address in decimal dot notation) of server
Port number of server
Transport protocol: TCP or UDP
Acknowledgement protocol: streaming or stop-and-wait
Message size
The server must be started first, of course, and will receive messages from the client. Prior to starting the data transfer, the client sends a message to the server with a message that, at a minimum, identifies the number of bytes that it will transfer. See the server section for possible extensions. It then waits for an acknowledgement from the server. This acknowledgement can be a single byte.

Prior to transferring the data, you will note the start time. After the last message was sent, you will note the end time to compute the overall elapsed time.

Output
======
At the end of execution, the client will print

Number of messages sent
Number of bytes sent
Total transmit time
'''

import sys
from sock_tester import TCPClient, UDPClient



def str_to_bool(str):
    if str == 'False':
        return False
    elif str == 'True':
        return True

def main():
    ip_address = input("Enter hostname or IP address: ")
    port_no = int(input("Enter port number: "))
    transport_protocol = input("TCP or UDP: " )
    acks = input("Streaming (False) or stop-and-wait (True): ")
    acks = str_to_bool(acks)
    message_size = int(input("Enter message size: "))

    if transport_protocol == 'TCP':
        client = TCPClient(ip_address, port_no, acks)
        client.run(message_size, 2 ** 20)
    else:
        client = UDPClient(ip_address, port_no, acks)
        client.run(message_size, 2 ** 20)


if __name__ == "__main__":
    main()
