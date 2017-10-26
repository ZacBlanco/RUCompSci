'''Better  Server

Your server will be given the following parameters on the command line:

- Port number for requests
- Transport protocol: TCP or UDP
- Acknowledgement protocol: streaming or stop-and-wait

The server will start up, create a socket, and read messages from the client. The first message will contain the size. This can be assumed to be a fixed number of bytes (e.g., four bytes). This message is acknowledged (e.g., send back a single byte containing a 1 or whatever). Subsequent bytes read will contain the data.

The server will continue reading the data until all the bytes have been consumed or an error occurs.

Better servers
A problem with the basic server is that you need to restart it each time you restart the client. You can make your server smarter by having the client communicate some more information.

The command line accepts:

Port number for requests
Transport protocol: TCP or UDP
The client, in its initial message, will identify the acknowledgement protocol to be used.

You can make the server even smarter by omitting the need to specify a transport protocol and have the server create two threads, each of which is responsible for one protocol. This will take a bit more work but save you some time in running your tests.

Output
======
Server
At the end of each server session (when the maximum number of bytes has been read or a TCP connection has been closed) the server will print:

Acknowledgement protocol used
Number of messages read
Number of bytes read
'''

import sys
from sock_tester import Server


def main():
    port_no = int(input("Enter a port number: "))
    s1 = Server('0.0.0.0', port_no)
    s1.run()
    s1.wait_forever()

if __name__ == "__main__":
    main()
