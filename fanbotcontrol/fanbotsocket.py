'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array
from  serial import Serial, SerialException
import socket
import threading


class FanbotSocket:
    listener = None

    """
        listener is the instance which will parse the received frames of data.
        Listener must implement function:  createProtocolParser() 
        This function will create and return an instance of a  parser which has at least function 
        handleCommand(opcode,data = None)   
    """
    def __init__(self,listener): 
        self.socket = None
        self.listener = listener
        self.thread = None
 

    def sendFrame(self,frame):
        if self.socket != None:
            try:
                self.socket.send(frame)
            except Exception as e:
                print "sendCommand Exception %s %s"  %(e.__class__.__name__, e.message)
                raise e


    def openSocket(self,host,port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            # Connect to server and send data
            self.socket.connect((host, port))
            print "Openened socket to %s:%d" %(host,port)
            self.thread = threading.Thread(target=self.receiverHandler)
            self.thread.setDaemon(1)
            self.alive = True
            self.thread.start()
        except Exception as e:
            print "openSocket Exception %s %s" % (e.__class__.__name__, e.message)
            self.socket = None
            raise e
 
  
    def closeSocket(self):
        if self.socket != None:
            self.socket.close()
            self.socket = None
        self.alive = False
        self.parser = None     
            
            
    def receiverHandler(self):
        print "Started receiving on socket" 
        if self.listener:
            self.parser = self.listener.createProtocolParser()
        while self.alive:
            frame = self.socket.recv(1024)
            if self.parser:
                self.parser.parseFrame(frame)
        print "Stopped receiving on socket" 
            
                
