'''
Created on Mar 25, 2013

@author: smeenka
'''


import SocketServer
import threading
import array

class HubSimulServer:
    HOST, PORT = "localhost", 1234
    alive = True
    handlers = []
    listener = None
    """
        listener is the instance which will parse the received frames of data.
        Listener must implement function:  createProtocolParser() 
        This function will create and return an instance of a  parser which has at least function 
        handleCommand(opcode,data = None)   
    """
    def __init__(self,listener):

        HubSimulServer.listener = listener
        self.alive = True
        self.thread = threading.Thread(target=self.serverThread)
        self.thread.setDaemon(1)
        self.thread.start()

    def __del__( self ):
        print "HubSimulServer destructor ..."
        self.shutdown()

        
    def serverThread(self):
        print "Starting socket server thread on port ", HubSimulServer.PORT
        server = SocketServer.ThreadingTCPServer((HubSimulServer.HOST, HubSimulServer.PORT), MyTCPHandler)
        server.serve_forever()
        self.server = server
        
    def shutdown(self):
        HubSimulServer.alive = False
        self.server.shutdown()
        self.server = None

    def sendFrame(self,frame):
        for handler in HubSimulServer.handlers:
            handler.sendFrame(frame)








class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, testand must
    override the handle() method to implement communication to the
    client.
    """
    def setup(self):
        print self.client_address, 'connected!'
        HubSimulServer.handlers.append(self)
        if HubSimulServer.listener:
            self.parser = HubSimulServer.listener.createProtocolParser()
            self.alive = True
        else:
            print 'No listener configured for this class'  

                
        
        
    def finish(self):
        print self.client_address, 'disconnected!'
        HubSimulServer.handlers.remove(self)
        self.parser = None
        self.alive = False

    def sendFrame(self,frame):
        try:
            self.request.send(frame)
        except Exception as e:
            print "server handle:exception %s %s"% (e.__class__.__name__, e.message)
            self.alive = False        
        
    def handle(self):
        print "Client connected: %s" % self.client_address[0]
        frame = None
        while HubSimulServer.alive and self.alive:
            try:
                frame = self.request.recv(1024)
            except Exception as e:
                print "server handle:exception %s %s"% (e.__class__.__name__, e.message)
                self.alive = False
            if self.alive and self.parser:    
                
                try:
                    self.parser.parseFrame(frame)
                except Exception as e:
                    print "parser :exception %s %s"% (e.__class__.__name__, e.message)
                    raise e
        print "MyTCPHandler forced to dye"


if __name__ == "__main__":
    import time

    class TestHubProtocol:
        def __init__(self,transport):
            self.transport = transport

    class TestParserFactory:
        def setServer(self,server):
            self.server = server
        def createProtocolParser(self):
            """ create and return instance of HubProtocol parser""" 
            return TestHubProtocolParser(self.server)     

        
    class TestHubProtocolParser:
        
        def __init__(self,server):
            self.server = server
            
        def parseFrame(self,frame):
            print "Received frame: %s" % frame
            self.server.sendFrame("Received frame: " + frame)


    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    factory = TestParserFactory()
    server = HubSimulServer(factory)
    protocol = TestHubProtocol(server)
    factory.setServer(server)
    
    while True:
        time.sleep(0.01)
    