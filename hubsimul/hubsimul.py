'''
Created on Mar 25, 2013

@author: smeenka
'''
import array
from hubprotocol import HubProtocol

class HubSimul :
    
    def __init__( self, startindex,protocol ):
        self.protocol = protocol
        self.discovered = False
        self.id = array.array('B', [0] * 8)
        for i in range(7):
            self.id[i] =  65  + i
        self.id[7] = startindex + 65 
        self.config = []


    def checkId(self,data):
        if not data or len(data) < 8:
            return False
         
        for i in range(8):
            if self.id[i] !=  data[i]:
                return False
        return True                
    
    def __del__( self ):
        pass
    
    
    def handleCommand(self,opcode,address = None,data = None):
        print "received command: " , opcode

        if opcode == HubProtocol.command_sendframe:
            None

        elif opcode == HubProtocol.command_disc_reset:
            print 'discovery: '  
            self.discovered = False

        elif opcode == HubProtocol.command_discovery:
            print "received command: " , opcode
            if not self.discovered :
                self.protocol.sendDiscoveryAck(self.id)
            
            
        elif opcode == HubProtocol.command_config:
            if self.checkId(data):
                print 'Ok I am addressed!: ', address 
            
                
    
 
            
            

