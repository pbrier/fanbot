'''
Created on Mar 25, 2013

@author: smeenka
'''


import SocketServer
import threading
import array

class HubSimulServer:
    HOST, PORT = "localhost", 1234
    handler = None
    alive = True
    SEND_DATA_FRAME = 0x1010
    """
        Message format Meaning
    Byte 
    0 .. 1 Start of message: Allways '#' + '#'
    2 .. 3 Opcode 16 bit opcode. (LSB first)
    4 .. 5 16 bit Length (LSB first) of subsequent data (excluding CRC)
    6 .. [length + 6] 8 bit binary data (can be zero length)
    [Length + 6] .. [Length + 7] 16 bit CRC (LSB first)
    """
     
    def __init__(self,handler):
        HubSimulServer.handler = handler
        self.alive = True
        """Start the server  thread"""        
        self.thread = threading.Thread(target=self.serverThread)
        self.thread.setDaemon(1)
        self.thread.start()

        
    def serverThread(self):
        print "Starting socket server thread "
        self.server = SocketServer.TCPServer((HubSimulServer.HOST, HubSimulServer.PORT), MyTCPHandler)
        # Activate the server; this will keep running until you
        # interrupt the program with Ctrl-C
        self.server.serve_forever()
        
    def shutdown(self):
        HubSimulServer.alive = False
        #self.server.shutdown()

class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        print "Client connected: %s" % self.client_address[0]
        state = 0
        calcChecksum = 0
        checksum = 0 
        rcvchecksum = 0 
        opcode = 0
        len = 0
        idx = 0
        payload = []
        alive = True
        while HubSimulServer.alive and alive:
            try:
                data = self.request.recv(1024)
                print data
                
                for c in data:
                    b = ord(c)
                    if state == 0:
                        checksum = 0
                        if c == "#": 
                            state = 1 
                    elif state == 1:
                        if c == "#": 
                            state = 2
                    elif state == 2:
                        state = 3
                        opcode = b
                    elif state == 3:
                        opcode += b << 8
                        state = 4
                    elif state == 4:
                        state = 5
                        len = b
                    elif state == 5:
                        len += b << 8
                        state = 6
                    elif state == 6:
                        payload= array.array('B', [0] * len)
                        idx = 1
                        payload[0]= b
                        state = 7
                    elif state == 7:
                        payload[idx]= b
                        idx += 1
                        if idx == len:
                            state = 8
                    elif state == 8:
                        calcChecksum = checksum % 0x10000
                        state = 9
                        rcvchecksum = b
                    elif state == 9:
                        rcvchecksum += b << 8
                        if calcChecksum == rcvchecksum:
                            HubSimulServer.handler.handleClientRequest(opcode,payload)
                        else:    
                            print "checksum not correct  received: %d expected %d" %(rcvchecksum,calcChecksum)
                        state = 0
                    checksum += b
                # just send back the same data, but upper-cased
                self.request.send("ok")
            except Exception as e:
                print "server handle:exception %s %s"% (e.__class__.__name__, e.message)
                alive = False        
        print "Client disconnected: %s" % self.client_address[0]

if __name__ == "__main__":
    import time

    class test:
        def handleClientRequest(self,data):
            print "test: ", data

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server = HubSimulServer(test())
    