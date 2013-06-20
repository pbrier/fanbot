



import array


class HubProtocol:
    """
    Opcodes
    NR      Name          Direction    Data
    1       REQUEST_ID     PC --> HUB    All nodes that are not tagged will send their ID on the bus 
                                        after a random time [0 to 1 sec]
    2       TAG_ID         PC --> HUB    4 bytes (UID). The hub that is tagged (its UID corresponds to the UID data) 
                                        wil not respond to any REQUEST_ID commands.
    3       PLAY_FRAME     PC --> HUB    128 bytes (1024 bits) with play status
                                        Send a bitmapped block of data to all hubs. Each hub will decode the appropriate bits and send their state to the robots via the hub outputs
    4       LED_FRAME      PC --> HUB    128 bytes (1024 bits) with led status (ON/OFF)
    5       POS_FRAME      PC --> HUB    256 bytes (2048 bits) with servo position status (2 bits / port, 0=OFF, 1=LEFT,2=MIDDLE,3=RIGHT)
    6       REQUEST_STATUS PC --> HUB    4 bytes (UID).
                                        The addressed slave will respond with their unique ID and status information in the HUB_STATUS_REPORT
    7       CONFIG_FRAME   PC --> HUB    4 + 48 bytes (4 bytes UID and 24 x 16bits indexes for all outputs)
    128     ID_REPORT      HUB --> PC    4 bytes (UID of HUB)
    129     STATUS_REPORT  HUB --> PC    Total 104 bytes: 4 bytes (UID of HUB) + 24* 4 bytes UID of the connected robots. Followed by the 4 byte UID of the hub, and 4 byte software revision code of the hub
    0xDEAD  RESET          PC --> HUB    Cause a reset of the hubs
    """
    REQUEST_ID     = 1      
    TAG_ID         = 2      
    PLAY_FRAME     = 3     
    LED_FRAME      = 4     
    POS_FRAME      = 5     
    REQUEST_STATUS = 6     
    CONFIG_FRAME   = 7     
    ID_REPORT      = 128   
    STATUS_REPORT  = 129   
    """Next command should be added to the protocol"""
    DISC_RESET     = 0xde01       
    RESET          = 0xdead    
    
    HUB_ID_LEN         = 4
    HUB_STATUS_LEN     = 104    
    HUB_TIMEOUT_MS     = 100
    
    
    listeners          = []

    def __init__(self,transport):
        self.transport          = transport 
      
    def setTransport(self,transport):
        self.transport = transport
          
    @classmethod
    def addListener(cls,listener):
        """ Listeners must implement method handleCommand(opcode,data = None) """
        cls.listeners.append(listener)
        
    @classmethod
    def clearListeners(cls):
        """ clear list of listeners """
        cls.listeners =[]
 


    def sendConfig(self,id,config):
        data = []
        for i in range(0,len(id)):
            data.append(id[i])
        for i in range(0,len(config)):
            word = config[i]
            if word < 0: word = 0xFFFF
            data.append(word % 0x100)
            data.append((word / 0x100) % 0x100)
        self.sendCommand(HubProtocol.CONFIG_FRAME,len(data), data)

        
    def sendFanbotFrame(self,data):
        self.sendCommand( HubProtocol.POS_FRAME,len(data),data)    
        
    def sendCommand(self,opcode,len = 0 ,data = None):
        print "send command %04x " % (opcode)
 
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
        """"  Parse incoming frame byte for byte. If a frame has been received return true else false """
        for c in frame:
            b = ord(c)
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
                else:
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
                self.state = 0
                self.rcvchecksum += b << 8
                if self.calcChecksum == self.rcvchecksum:
                    for listener in HubProtocol.listeners:
                        listener.handleCommand(self.opcode,self.payload)
                else:    
                    print "checksum not correct  received: %d expected %d" %(self.rcvchecksum,self.calcChecksum)

            self.checksum += b   
    
if __name__ == "__main__":
    import time
    from fanbotsocket import FanbotSocket
        
    class TestHubProtocolListener:
            
        def handleCommand(self, opcode,payload):
            print "Received opcode: %04x -- %s" % (opcode,payload)


    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    factory = ParserFactory()
    socket = FanbotSocket(factory)
    listener =TestHubProtocolListener()
    hubProtocol = HubProtocol(socket)
    hubProtocol.addListener(listener)
    
    socket.openSocket("127.0.0.1",1234)
    time.sleep(1)
    
    hubProtocol.sendCommand(HubProtocol.REQUEST_ID)

    
    while True:
        time.sleep(0.01)
    

