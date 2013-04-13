'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array
from  serial import Serial, SerialException
import socket


class FanbotComm:
    """
    Message format Meaning
    Byte 
    0 .. 1 Start of message: Allways '#' + '#'
    2 .. 3 Opcode 16 bit opcode. (LSB first)
    4 .. 5 16 bit Length (LSB first) of subsequent data (excluding CRC)
    6 .. [length + 6] 8 bit binary data (can be zero length)
    [Length + 6] .. [Length + 7] 16 bit CRC (LSB first)
    """


    
 
    socket = None
    serial = None
 
    @classmethod
    def sendCommand(cls,opcode,len,data):
        framelen = len + 8
        frame = array.array('B', [0] * framelen )
        checksum = 0
        frame[0]= ord('#')
        frame[1]= ord('#')
        frame[2]= opcode % 0x100
        frame[3]= (opcode >> 8) % 0x100
        frame[4]= len  % 0x100
        frame[5]= (len >> 8)  % 0x100
        idx = 6
        for b in data:
            frame[idx] = b
            idx += 1
        for b in frame:
            checksum += b
        frame[idx ]=  checksum  % 0x100
        idx += 1
        frame[idx ]= (checksum >> 8)  % 0x100
        if cls.socket != None:
            try:
                cls.socket.send(frame)
                received = cls.socket.recv(1024)
                print received
            except Exception as e:
                print "sendCommand Exception %s %s"  %(e.__class__.__name__, e.message)

    @classmethod
    def openSocket(cls,host,port):
        cls.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            # Connect to server and send data
            cls.socket.connect((host, port))
            print "Openened socket to %d:%d" %(host,port)
        except Exception as e:
            print "openSocket Exception %s %s" % (e.__class__.__name__, e.message)
 
    @classmethod
    def closeSocket(cls):
        if cls.socket != None:
            cls.socket.close()
            cls.socket = None
