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
        self.id = array.array('B', [0] * HubProtocol.HUB_ID_LEN)
        self.status = array.array('B', [0] * HubProtocol.HUB_STATUS_LEN)
        for i in range(HubProtocol.HUB_ID_LEN):
            self.id[i] =  65  + i
            
        """status: start with hub ID"""    
        self.id[3] = startindex + 65 
        for i in range(HubProtocol.HUB_ID_LEN):
            self.status[i] =  self.id[i]

        """all ids of connected fnabots, leave one disconnected for test"""    
        idx = 4
        for i in range(24):
            self.status[idx] =  self.id[3] 
            idx += 1
            self.status[idx] =  self.id[3]
            idx += 1
            self.status[idx] =  self.id[3]
            idx += 1
            self.status[idx] =  i
            idx += 1
        """software revisision"""    
        self.status[idx] =  1
        idx += 1
        self.status[idx] =  2
        idx += 1
        self.status[idx] =  3
        idx += 1
        self.status[idx] =  4

        """one fanbot is missing ...."""
        if startindex < 23:
            missing = 4 + 4* startindex
        else:
            missing = 4    
        self.status[missing + 0] = 0     
        self.status[missing + 1] = 0     
        self.status[missing + 2] = 0     
        self.status[missing + 3] = 0     




    def checkId(self,data):
        if not data or len(data) < HubProtocol.HUB_ID_LEN:
            return False
         
        for i in range(HubProtocol.HUB_ID_LEN):
            if self.id[i] !=  data[i]:
                return False
        return True                
    
    def __del__( self ):
        pass
    
    
    def handleCommand(self,opcode,data = None):
        #print "HubSimul: Received opcode: ", opcode, " data: ",data  

        if opcode == HubProtocol.LED_FRAME:
            None

        elif opcode == HubProtocol.DISC_RESET:
            print 'HubSimul:  received command: DISC_RESET: '  
            self.discovered = False

        elif opcode == HubProtocol.REQUEST_ID:
            print "HubSimul: received command: REQUEST_ID"
            if not self.discovered :
                self.protocol.sendDiscoveryAck(self.id)
                self.discovered = True
            
            
        elif opcode == HubProtocol.CONFIG_FRAME:
            if self.checkId(data):
                print "HubSimul: received command: CONFIG_FRAME "  , data
            
        elif opcode == HubProtocol.REQUEST_STATUS:
            if self.checkId(data):
                print "HubSimul: received command: REQUEST_STATUS "  , data
                self.protocol.sendCommand(HubProtocol.STATUS_REPORT,len(self.status) ,self.status)
   
 
            
            

