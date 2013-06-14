'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array
from  serial import Serial, SerialException
import socket
import threading
import time

class FanbotSerial:
    serial = None
 
    listener = None

    """
        listener is the instance which will parse the received frames of data.
        Listener must implement function:  createProtocolParser() 
        This function will create and return an instance of a  parser which has at least function 
        handleCommand(opcode,data = None)   
    """
    def __init__(self,listener): 
        self.serial = None
        self.serialname = None
        self.listener = listener
        self.thread = None


    def sendFrame(self,frame):
        print "Serial: sendframe"
        if self.serial != None:
            try:
                self.serial.write(frame)
            except Exception as e:
                print "sendCommand Exception %s %s"  %(e.__class__.__name__, e.message)
                raise e

    def open(self,serialname,baud):
        print "Serial: Open serial: ", serialname
        self.serial = Serial(port=serialname, baudrate=baud);
        self.serialname = serialname
        self.thread = threading.Thread(target=self.receiverHandler)
        self.thread.setDaemon(1)
        self.alive = True
        self.thread.start()
 
    def close(self):
        if self.thread:
            self.alive = False
            self.thread = None
        if self.serial != None:
            self.serial.close()
            self.serial = None


    def receiverHandler(self):
        print "Started receiving on serial port", self.serialname
        frame = []
         
        if self.listener:
            self.parser = self.listener.createProtocolParser()
        while self.alive and self.serial:
            data = 0
            try:
                data = self.serial.read(1)
            except Exception  as e:
                print "Exception ",e
            print data            
            frame.append(data)
            if data == 13 or data == 10 and self.parser:
                self.parser.parseFrame(frame)
                print "Received frame: ", frame
                frame = []
                
        print "Stopped receiving on serial port " , self.serialname 
