'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array
from  serial import Serial, SerialException
import socket


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
        self.listener = listener
        self.thread = None
 

    def sendFrame(self,frame):
        if self.serial != None:
            try:
                self.serial.write(frame)
            except Exception as e:
                print "sendCommand Exception %s %s"  %(e.__class__.__name__, e.message)
                raise e

    def open(self,serialname,baud):
        self.serial = Serial(port=serialname, baudrate=baud);
        self.serial.open()
 
    def close(self):
        if self.serial != None:
            self.serial.close()
            self.serial = None
