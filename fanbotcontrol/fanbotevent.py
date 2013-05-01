'''
Created on Nov 28, 2011

@author: anton
'''
import sys
import wx


EVT_PROXY_PORT_CHANGED_ID = wx.NewEventType()
EVT_PROXY_PORT_CHANGED    = wx.PyEventBinder(EVT_PROXY_PORT_CHANGED_ID,1)

EVT_PROXY_SERVER_ID       = wx.NewEventType()
EVT_PROXY_SERVER          = wx.PyEventBinder(EVT_PROXY_SERVER_ID,1)


class FanbotEvent(wx.PyCommandEvent):
    def __init__(self,evtType,id,payload = None):
 
        wx.PyCommandEvent.__init__(self,evtType,id)
        self.payload = payload
        
    def setPayload(self,pl):
        self.payload = pl
            
    def getPayload(self):
        return self.payload    
        

    