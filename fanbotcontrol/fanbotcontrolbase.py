"""Subclass of HubSimulFrame, which is generated by wxFormBuilder."""

from fanbotconfig import FanbotConfig
from hubprotocol import HubProtocol

class ControlBase:
    messageHandler = None
    
    def __init__( self):
        print 'ControlBase constructor  '
        



        
    def sendMessage(self,message):
        if ControlBase.messageHandler:
            ControlBase.messageHandler(message)
        else:
            print 'Message: ', message    
                