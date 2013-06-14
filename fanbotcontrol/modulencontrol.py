'''
Created on Mar 25, 2013

@author: smeenka
'''
from designcontrol import DesignControl
from fanbotframe import PanelModules
from proxycontrol import ProxyControl
from hubprotocol import HubProtocol
import wx
import hubs
import time
from bitmap import Bitmap

from fanbotconfig import FanbotConfig

from executecontrol import ExecuteControl

class ModulenControl (PanelModules) :
    
    
    def __init__( self, parent, remote ):
        """ remote is instance of HubProtocol for communication with remote"""
        PanelModules.__init__( self,parent)
        self.remote = remote
        self.scaleX = 1
        self.scaleY = 1
        self.drawMode = 0
        self.hubList = hubs.HubListModel(self.scrolledWindowHubs,self)
        listHubsSizer = wx.BoxSizer( wx.VERTICAL )
        listHubsSizer.Add( self.hubList, 1, wx.ALL|wx.EXPAND, 1)
        self.scrolledWindowHubs.SetSizer( listHubsSizer )
        listHubsSizer.Fit( self.scrolledWindowHubs )
        

        self.fanbotList = hubs.FanbotListModel(self.scrolledWindowFanbots,self)
        listFanbotsSizer = wx.BoxSizer( wx.VERTICAL )
        listFanbotsSizer.Add( self.fanbotList, 1, wx.ALL|wx.EXPAND, 5 )
        self.scrolledWindowFanbots.SetSizer( listFanbotsSizer )
        listFanbotsSizer.Fit( self.scrolledWindowFanbots )

        self.pointCurrent = wx.Point(0,0)
        self.pointPrevious = wx.Point(0,0)
        self.currenthub = None
        
        self.bitmap = Bitmap(FanbotConfig.width,FanbotConfig.height)
        self.bitmap.clear()
        
        for hub in self.hubList.hubs:
            self.showHub(hub);
            
 
    
    def buttonResetDiscOnButtonClick( self, event ):
        self.remote.sendCommand( HubProtocol.DISC_RESET)
        self.hubList.resetDiscovery()
        
    
    def buttonDiscoverOnButtonClick( self, event ):
        self.remote.sendCommand( HubProtocol.REQUEST_ID)
        time.sleep(2)
        self.remote.sendCommand( HubProtocol.REQUEST_ID)
        time.sleep(2)
        self.hubList.refresh()
    
    def buttonClearListOnButtonClick( self, event ):
        self.listModules.Clear()
    
    def buttonResetConfigOnButtonClick( self, event ):
        if self.currenthub:
            self.resetHub(self.currenthub)
            self.remote.sendConfig(self.currenthub.idAsArray(),self.currenthub.config)

    def buttonSetConfigOnButtonClick( self, event ):
        if self.currenthub:
            print "buttonSetConfigOnButtonClick"
            self.remote.sendConfig(self.currenthub.idAsArray(),self.currenthub.config)

    def buttonSetConfigAllOnButtonClick( self, event ):
        print "buttonSetConfigAllButtonClick"
        for hub in self.hubList.hubs:
            self.remote.sendConfig(self.currenthub.idAsArray(),self.currenthub.config)
            time.sleep(0.100)
            

            
    def panelModulesCanvasOnSize( self, event ):
        rect = self.panelModulesCanvas.GetRect()

        self.scaleX = rect.GetWidth() / self.bitmap.width
        self.scaleY = rect.GetHeight() / self.bitmap.height
        print 'Scale x:y',self.scaleX,':',self.scaleY
        self.Refresh()

    def panelModulesCanvasOnPaint( self, event ):
        dc = wx.PaintDC(self.panelModulesCanvas)
        dc.BeginDrawing()
        dc.SetUserScale(self.scaleX, self.scaleY)
        dc.Clear()
        dc.DrawBitmap(self.bitmap.getBitmap(),0,0)
        dc.EndDrawing()
        
     
    def panelModulesCanvasOnMouseEvents( self, event ):
        point = None
        canvas = self.panelModulesCanvas
        if event:
            # set mouse cursor
            canvas.SetCursor(wx.StockCursor(wx.CURSOR_ARROW))
            # get device context of canvas
            dc= wx.ClientDC(canvas)
            dc.SetUserScale(self.scaleX, self.scaleY)

            point = event.GetLogicalPosition(dc)
            if point.x < 0 or point.x >= FanbotConfig.width or point.y < 0 or point.y >= FanbotConfig.height:
                #print 'x,y out of bounds: ',point.x ,':',point.y    
                return
            self.labelPosition.SetLabel("%s : %s" % (point.x,point.y))
            self.pointCurrent = point    
            #print "mouse event X : " ,point.x , " Y: ",point.y


        if event.LeftDown():
            self.panelModulesCanvas.Refresh()
            self.pointPrevious = self.pointCurrent
            self.updateHubConfig(point.x, point.y)
                        
        elif event.LeftIsDown():
            if self.pointPrevious.x == self.pointCurrent.x and self.pointPrevious.y == self.pointCurrent.y:
                return;
            self.pointPrevious = self.pointCurrent
            #print "Dragging to X : " ,point.x , " Y: ",point.y
            cur = wx.StockCursor(wx.CURSOR_CROSS)  
            canvas.SetCursor(cur)
            self.updateHubConfig(point.x, point.y)
                                    
            
    def updateHubConfig(self,x,y):
        if self.currenthub:
            if self.currenthub.canAddConfigItem():
                if self.bitmap.pixelSetWhite(x, y):
                    self.currenthub.setConfig(x, y)
                    self.panelModulesCanvas.Refresh()
                        
    def handleCommand(self, opcode,payload):
        """ Handle incoming data from remote hubs or hubsimulator"""
        print "Modulen: Received opcode: %04x -- %s" % (opcode,payload)      
        
        id = "" 
        if len(payload) >= 4:
            for i in range(4):
                id =  "%s%02x"%(id,payload[i])

        
        if opcode == HubProtocol.ID_REPORT:
            if len(payload) != 4:
                print  "Error: incorrect length from hub received"
                return
            print "Modulen: Received ID_REPORT from %s" % (id)
            hub = self.hubList.addHub(id)
            hub.discovered = True
            hub.responsive = True
            self.remote.sendCommand( HubProtocol.TAG_ID,4,hub.idAsArray() )
        elif opcode == HubProtocol.STATUS_REPORT:
            self.fanbotList.setFanbots(payload)
            self.labelHubId.SetLabel("Hub: " + id)
            
 
            
                      
    def hubSelected(self,hub):
        """Callback function called from the hubs list control"""
        self.showHub(self.currenthub)
        print 'selected hub:' ,hub.id
        self.currenthub = hub

        config = hub.config
        for i in range (0,len(config)):
            tuple = hub.getConfigColor(i)
            if tuple[0] >= 0:
                x = tuple[0]
                y = tuple[1]
                self.bitmap.setPixel(x,y, 0xFFFFFF)
        self.fanbotList.reset()    
        self.panelModulesCanvas.Refresh()    
        self.remote.sendCommand( HubProtocol.REQUEST_STATUS,4,hub.idAsArray() )

    def fanbbotSelected(item):
        print "Selected fanbot index: " , item

    def showHub(self,hub):
        """for the given hub, show the pixels in the bitmap, in the color of he hub"""
        if hub:
            print 'show hub:' ,hub.id
            config = hub.config
            for i in range (0,len(config)):
                tuple = hub.getConfigColor(i)
                if tuple[0] >= 0:
                    x = tuple[0]
                    y = tuple[1]
                    self.bitmap.setPixel(x,y, tuple[2])
                
    def resetHub(self, hub):
        """for the given hub, make all its pixels gray, and then reset config"""
        if hub:
            print 'reset hub:' ,hub.id
            config = hub.config
            for i in range (0,len(config)):
                tuple = hub.getConfigColor(i)
                if tuple[0] >= 0:
                    x = tuple[0]
                    y = tuple[1]
                    self.bitmap.pixelSetGray(x,y)
            hub.resetConfig() 
            self.panelModulesCanvas.Refresh()       
         
            

            
