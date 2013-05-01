""" Protocol of the data send between the controller and the hubs

command              direction  parameters            description
command_sendframe    pc->hub    1                    broadcast data frame to all hubs. 
                                                     No response expected from hub                     
                                frame                125 byte for 50x20 tribune. 
                                                     frame data for each fanbot 1 bit
                                                     first bit in first byte is top left
                                                     last bit in last byte is bottom right
command_disc_reset   pc->hub    0                    broadcast to all hubs that discovery flag must be reset
                                                     No response expected from hub
command_discovery    pc->hub    0                    broadcast to all hubs that they should response
                                                     Each hub with discovery flag reset 
                                                     respond with command_disc_ack, within 3 seconds
command_disc_ack     hub->pc    1                    Hub will send this command asynchrounous
                                hub address          8 bytes with hub unique hw address
command_config       pc->hub    2                    Addressed hub will take over the config, and set discovered flag
                                hub address          8 bytes with hub unique hw address
                                config data          2x24 byte: 24 words (little endian) 
command_ack          hub->pc    0                    hub acknowlidge
"""
import array

class HubProtocol:
    command_sendframe =  0x1030
    command_disc_reset = 0x2010
    command_discovery  = 0x2020
    command_disc_ack   = 0x6567 
    command_config     = 0x3010
    command_ack        = 0x1010
    listeners          = []

    def __init__(self,transport):
        self.transport          = transport 
      
    @classmethod
    def addListener(cls,listener):
        """ Listeners must implement method handleCommand(opcode,data = None) """
        cls.listeners.append(listener)
        
 
    def sendAck(self):
        self.sendCommand(HubProtocol.command_ack)

    def sendDiscoveryAck(self,myid):
        if not myid or len(myid) < 8:
            print "sendDiscoveryAck Incorect parameter"
            return
        self.sendCommand(HubProtocol.command_disc_ack,8,myid)
        
    def sendCommand(self,opcode,len = 0 ,data = None):
        print "send command %04x " % opcode
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
        if data:
            for b in data:
                frame[idx] = b
                idx += 1
        for b in frame:
            checksum += b
        frame[idx ]=  checksum  % 0x100
        idx += 1
        frame[idx ]= (checksum >> 8)  % 0x100
        if self.transport:
            self.transport.sendFrame(frame)
        else:
            print 'No transport layer is configured in hub protocol'    


class ParserFactory:
    """
    instance of this class can create a parser object
    The parser object must implement handleCommand(opcode,data = None) 
    """
    def createProtocolParser(cls):
        """ create and return instance of HubProtocol parser""" 
        return HubProtocolParser()     


class HubProtocolParser:
    """
    Incoming data will be parsed by this class
    If a valid frame is fully parsed, for each  HubProtocol listener the function
    handleCommand(opcode,data = None) is called

    """
    def __init__(self):
        self.state = 0
        self.calcChecksum = 0
        self.checksum = 0 
        self.rcvchecksum = 0 
        self.opcode = 0
        self.len = 0
        self.idx = 0
        self.payload = []

    def parseFrame(self,frame):
        for c in frame:
            b = ord(c)
            #print b, " # ", self.state
            if self.state == 0:
                self.checksum = 0
                if c == "#": 
                    self.state = 1 
            elif self.state == 1:
                if c == "#": 
                    self.state = 2
                else:    
                    print " frame error "
                    self.state = 0
            elif self.state == 2:
                self.state = 3
                self.opcode = b
            elif self.state == 3:
                self.opcode += b << 8
                #print " opcode %04x" % self.opcode
                self.state = 4
            elif self.state == 4:
                self.state = 5
                self.len = b
            elif self.state == 5:
                self.len += b << 8
                #print " len  %04x" % self.len
                self.payload= array.array('B', [0] * self.len)
                self.idx    = 0
                if self.len == 0:
                    self.state = 8
                else :
                    self.state = 7    
                    
            elif self.state == 7:
                self.payload[self.idx]= b
                self.idx += 1
                if self.idx == self.len:
                    self.state = 8
            elif self.state == 8:
                self.calcChecksum = self.checksum % 0x10000
                self.state = 9
                self.rcvchecksum = b
            elif self.state == 9:
                self.rcvchecksum += b << 8
                if self.calcChecksum == self.rcvchecksum:
                    for listener in HubProtocol.listeners:
                        listener.handleCommand(self.opcode,self.payload)
                else:    
                    print "checksum not correct  received: %d expected %d" %(self.rcvchecksum,self.calcChecksum)
                self.state = 0
            self.checksum += b   

    
if __name__ == "__main__":
    import time
    from hubsimulserver import HubSimulServer
        
    class TestHubProtocolListener:
        
        def __init__(self,server):
            self.protocol = server
            
        def handleCommand(self, opcode,payload):
            print "Received opcode: %4x -- %s" % (opcode,payload)
            self.protocol.sendCommand(opcode, len(payload),payload)


    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    factory = ParserFactory()
    server = HubSimulServer(factory)
    hubProtocol = HubProtocol(server)

    listener =TestHubProtocolListener(hubProtocol)
    hubProtocol.addListener(listener)

    
    while True:
        time.sleep(0.01)
    

