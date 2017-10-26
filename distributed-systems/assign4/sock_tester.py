#! /usr/bin/env python3
import socket
import threading
import time
import os
import math

M = 2 ** 20
G = 2 ** 20

def create_socket(udp=True):
    '''Logic to simply creating a basic TCP/UDP socket w/ IPv4'''
    if udp:
        return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    else:
        return socket.socket(socket.AF_INET, socket.SOCK_STREAM)

class Server(object):
    '''Server object listens on two different threads for UDP and TCP testing.

    You shouldn't have to restart the server ever as the beginning of each client test will specify
    all of the necessary parameters (msg size, total transfer size, and ack protocol) before 
    initiating the test.
    
    The TCP thread will always listen on the specified port
    The UDP thread will always listen on the specified port + 1

    Basic usage example

    srv = Server('0.0.0.0', 8080)
    srv.run() # Starts two separate threads to listen on UDP/TCP
    srv.wait_forever() # Listen forever
    # or
    srv.stop_server() # both threads exit after timing out
    '''

    def __init__(self, listen_addr, port):
        self.listen_addr = listen_addr
        self.port = port
        self.stop = False

    def __run_tcp__(self):
        '''Run the TCP thread'''
        self._sock = create_socket(udp=False)
        self._sock.settimeout(1)
        self._sock.bind((self.listen_addr, self.port))
        self._sock.listen(5) # 5 is the number of backlog (waiting incoming) TCP connections. Specified for python 3.4 support
        while not self.stop:
            msgs = 0
            byte_count = 0
            try:
                conn, addr = self._sock.accept()
                msg_size = conn.recv(4)
                msg_size = int.from_bytes(msg_size, 'little')
                use_acks = conn.recv(1)
                use_acks = bool(int.from_bytes(use_acks, 'little'))
                transfer_size = int.from_bytes(conn.recv(4), 'little')
                print("msg_size: {}, transfer_size: {}, use_acks: {}".format(msg_size, transfer_size, use_acks))
                while( byte_count < transfer_size): # loop while we still receive data and we haven't gone over transfer size
                    msg = conn.recv(msg_size)
                    if len(msg) <= 0:
                        break
                    msgs += 1
                    byte_count += len(msg)
                    if use_acks:
                        conn.send(int(True).to_bytes(1, 'little'))
                conn.close()
            except socket.timeout as err:
                # print(err)
                pass

    def __run_udp__(self):
        '''Run the UDP loop'''
        self._usock = create_socket()
        self._usock.settimeout(1)
        self._usock.bind((self.listen_addr, self.port + 1))
        while not self.stop:
            msgs = 0
            byte_count = 0
            try:
                msg_size, addr = self._usock.recvfrom(4)
                msg_size = int.from_bytes(msg_size, 'little')
                use_acks, addr = self._usock.recvfrom(1)
                use_acks = bool(int.from_bytes(use_acks, 'little'))
                transfer_size, addr = self._usock.recvfrom(4)
                transfer_size = int.from_bytes(transfer_size, 'little')
                print("msg_size: {}, transfer_size: {}, use_acks: {}".format(msg_size, transfer_size, use_acks))
                while( byte_count < transfer_size): # loop while we still receive data and we haven't gone over transfer size
                    msg, addr = self._usock.recvfrom(msg_size)
                    if len(msg) <= 0 or len(msg) != msg_size:
                        print("Msg size was supposed to be {}. Got {}".format(msg_size, len(msg)))
                        break
                    msgs += 1
                    byte_count += len(msg) 
                    if use_acks: # If the protocol is using ACKS, send the true bit back. 
                        self._usock.sendto(int(True).to_bytes(1, 'little'), addr)
            except socket.timeout:
                # print(err)
                pass

    def run(self):
        '''Starts the UDP and TCP listening threads'''
        self.stop = False
        self.tcp_thread = threading.Thread(None, target=self.__run_tcp__)
        self.udp_thread = threading.Thread(None, target=self.__run_udp__)
        self.tcp_thread.start()
        self.udp_thread.start()

    def stop_server(self):
        self.stop = True
        self.wait_forever()
    
    def wait_forever(self):
        self.udp_thread.join()
        self.tcp_thread.join()

class TCPClient(object):
    def __init__(self, addr, port, acks=False):
        self.addr = addr
        self.port = port
        self.acks = acks

    def run(self, msg_size, total_transfer):
        '''Run the transfer test'''
        _sock = create_socket(udp=False) # Create TCP socket
        _sock.connect((self.addr, self.port)) # connect to the server
        ret = (None, None, None, None)
        try:
            # Set up the transfer parameters
            _sock.send(msg_size.to_bytes(4, 'little'))
            _sock.send(int(self.acks).to_bytes(1, 'little'))
            _sock.send(total_transfer.to_bytes(4, 'little'))
            msg = os.urandom(msg_size) # Random byte string to send for each msg
            num_msgs = 0
            num_bytes = 0
            start = time.time()
            while( num_bytes < total_transfer ):
                sent = _sock.send(msg) # Send the message
                num_bytes += sent
                num_msgs += 1
                if sent != msg_size: # Must send the whole message or we error
                    raise RuntimeError("Did not send the corrent number of bytes")
                if self.acks: # If we're looking for ACKS, make sure we get the ACK
                    ack = _sock.recv(1)
                    ack = bool(int.from_bytes(ack, 'little'))
                    if ack is not True:
                        raise RuntimeError("Did not get True ACK")
            # End and print results.
            end = time.time()
            print("Transfer finished: Messages Sent: {}, Bytes Sent: {}, Total Time: {} seconds, Transfer Rate {} MB/s".format(num_msgs, num_bytes, round(end-start, 4), num_bytes/(end-start)/M))
            ret = (num_msgs, num_bytes, round(end-start, 4), num_bytes/(end-start)/M)
        except Exception as err:
            print("client error")
            print(err)
        finally:
            _sock.close()
            return ret
        

class UDPClient(object):

    def __init__(self, addr, port, acks=False):
        self.addr = addr
        self.port = port
        self.acks = acks
    
    def run(self, msg_size, total_transfer):
        '''UDP Transfer test client

        Very similar to the TCP client except we use sendto/recvfrom
        '''
        _sock = create_socket()
        addr = (self.addr, self.port)
        try:
            _sock.sendto(msg_size.to_bytes(4, 'little'), addr)
            time.sleep(0.1)
            _sock.sendto(int(self.acks).to_bytes(1, 'little'), addr)
            time.sleep(0.1)
            _sock.sendto(total_transfer.to_bytes(4, 'little'), addr)
            time.sleep(0.1)
            msg = os.urandom(msg_size)
            num_msgs = 0
            num_bytes = 0
            start = time.time()
            while( num_bytes < total_transfer ):
                sent = _sock.sendto(msg, addr)
                num_bytes += sent
                num_msgs += 1
                if sent != msg_size:
                    raise RuntimeError("Did not send the correct number of bytes")
                if self.acks:
                    ack, addr = _sock.recvfrom(1)
                    ack = bool(int.from_bytes(ack, 'little'))
                    if ack is not True:
                        raise RuntimeError("Did not get True ACK")
            end = time.time()
            print("Transfer finished: Messages Sent: {}, Bytes Sent: {}, Total Time: {} seconds, Transfer Rate {} MB/s".format(num_msgs, num_bytes, round(end-start, 4), num_bytes/(end-start)/M))
            return (num_msgs, num_bytes, round(end-start, 4), num_bytes/(end-start)/M)
        except Exception as err:
            # print("client error")
            print(err)
        finally:
            _sock.close()
            return (None, None, None, None)
        


def test():
    s1 = Server('0.0.0.0', 8080)
    s1.run()
    # client = TCPClient('127.0.0.1', 8080)
    # for x in range (1, 15):
        # client.run(2 ** x, 5 * (2 ** 20))
    client2 = TCPClient('127.0.0.1', 8080)
    client3 = UDPClient('127.0.0.1', 8081, acks=False)
    time.sleep(1)
    for x in range(3, 15):
        client2.run(2 ** x, 1 * M)
        client3.run(2 ** x, 1 * M)

    time.sleep(.1)
    s1.stop_server()


if __name__ == "__main__":
    test()
