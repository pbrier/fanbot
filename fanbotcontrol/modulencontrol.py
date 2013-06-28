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
import array
from bitmap import Bitmap
from hubs import Hub

from fanbotcontrolbase import  ControlBase

from fanbotconfig import FanbotConfig

from executecontrol import ExecuteControl
import fanbotevent 

class ModulenControl (PanelModules,ControlBase) :
    
    
    def __init__( self, parent, remote ):
        """ remote is instance of HubProtocol for communication with remote"""
        PanelModules.__init__( self,parent)
        ControlBase.__init__(self)
        self.remote = remote
        self.scaleX = 1
        self.scaleY = 1
        self.drawMode = 0
        self.hubList = hubs.HubListModel(self.scrolledWindowHubs,self)
        listHubsSizer = wx.BoxSizer( wx.VERTICAL )
        listHubsSizer.Add( self.hubList, 1, wx.ALL|wx.EXPAND, 5)
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
        
        self.compressedFrame = array.array('B', [0] * 125)
        
        self.bitmap = Bitmap(FanbotConfig.width,FanbotConfig.height)
        self.bitmap.clear()
        
        for hub in self.hubList.hubs:
            self.showHub(hub);
            
        self.timerAnimate = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.timerAnimateHandler, self.timerAnimate)
        self.animateIdx = 0


        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.timerDiscover, self.timer)   
        self.timerDiscoverCount = 0;    
 
    
    def buttonResetDiscOnButtonClick( self, event ):
        self.remote.sendCommand( HubProtocol.RESET)
        self.hubList.resetDiscovery()
        
    
    def buttonDiscoverOnButtonClick( self, event ):
        self.remote.sendCommand( HubProtocol.REQUEST_ID)
        self.timer.Start(2000)
        self.timerDiscoverCount = 2
        self.sendMessage("Discover module. Even geduld aub... ")
        
    def timerDiscover(self,event):        
        if self.timerDiscoverCount == 0:
            self.timer.Stop()
            
        else:
            self.timerDiscoverCount -= 1 
            self.remote.sendCommand( HubProtocol.REQUEST_ID)
            self.hubList.refresh()
        
    
    def buttonClearListOnButtonClick( self, event ):
        self.listModules.Clear()
    
    def buttonResetConfigOnButtonClick( self, event ):
        if self.currenthub:
            self.resetHub(self.currenthub)
            self.remote.sendConfig(self.currenthub.idAsArray(),self.currenthub.config)

    def buttonSetConfigOnButtonClick( self, event ):
        if self.currenthub:
            hub =  self.currenthub
            print "buttonSetConfigOnButtonClick, currenthub: ", hub.id
            self.remote.sendConfig(hub.idAsArray(),hub.config)

    def buttonSetConfigAllOnButtonClick( self, event ):
        print "buttonSetConfigAllButtonClick"
        for hub in self.hubList.hubs:
            self.remote.sendConfig(hub.idAsArray(),hub.config)
            time.sleep(0.100)
            

            
    def panelModulesCanvasOnSize( self, event ):
        rect = self.panelModulesCanvas.GetRect()

        self.scaleX = rect.GetWidth() / self.bitmap.width
        self.scaleY = rect.GetHeight() / self.bitmap.height
        #print 'Scale x:y',self.scaleX,':',self.scaleY
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
            if self.checkBoxMatrix.IsChecked():
                """ set the snapto grid to horizontal 4 and vertical 6"""
                point.x /= 4
                point.x *= 4
                point.y /= 6
                point.y *= 6
                
            self.labelPosition.SetLabel("%s : %s" % (point.x,point.y))
            self.pointCurrent = point    
            #print "mouse event X : " ,point.x , " Y: ",point.y


        if event.LeftDown():
            self.panelModulesCanvas.Refresh()
            self.pointPrevious = self.pointCurrent
            if self.checkBoxMatrix.IsChecked():
                self.matrixHubConfig(point.x, point.y)
            else:        
                self.updateHubConfig(point.x, point.y)
                        
        elif event.LeftIsDown() and not self.checkBoxMatrix.IsChecked():
            if self.pointPrevious.x == self.pointCurrent.x and self.pointPrevious.y == self.pointCurrent.y:
                return;
            self.pointPrevious = self.pointCurrent
            #print "Dragging to X : " ,point.x , " Y: ",point.y
            cur = wx.StockCursor(wx.CURSOR_CROSS)  
            canvas.SetCursor(cur)
            self.updateHubConfig(point.x, point.y)
                                    
            
    def updateHubConfig(self,x,y):
        hub = self.currenthub
        if hub:
            if hub.canAddConfigItem():
                if self.bitmap.pixelSetWhite(x, y):
                    hub.setConfig(x, y)
                    self.panelModulesCanvas.Refresh()

    def matrixHubConfig(self,x,y):
        hub = self.currenthub
        if  hub:
            if hub.canAddConfigItem():
                hub.resetConfig()
                for y in range(self.pointCurrent.y,self.pointCurrent.y + 6): 
                    for x in range(self.pointCurrent.x,self.pointCurrent.x + 4):
                        self.bitmap.pixelSetWhite(x, y)
                        self.currenthub.setConfig(x, y)
                        self.panelModulesCanvas.Refresh()

                        
    def handleCommand(self, opcode,payload):
        """ Handle incoming data from remote hubs or hubsimulator"""
        print "Modulen: Received opcode: %04x -- %s" % (opcode,payload)      
        
        id = "" 
        if len(payload) >= 4:
            for i in range(4):
                id =  "%02x%s"%(payload[i],id)

        
        if opcode == HubProtocol.ID_REPORT:
            if len(payload) != 4:
                print  "Error: incorrect length from hub received"
                return
            self.sendMessage("Discovered hub: " +id)
            print "Modulen: Received ID_REPORT from %s" % (id)
            hub = self.hubList.addHub(id)
            hub.discoverd = True
            hub.responsive = True
            # self.remote.sendCommand( HubProtocol.TAG_ID,4,hub.idAsArray() ) # remove tagging, as it messes up frames
        elif opcode == HubProtocol.STATUS_REPORT:
            hub = self.currenthub
            if hub:
                hub.setFanbots(payload)
                self.fanbotList.setFanbots(hub.fanbots)
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
        self.fanbotList.setFanbots(None)  
        self.labelHubId.SetLabel("Hub: ") 
        self.panelModulesCanvas.Refresh()    
        self.remote.sendCommand( HubProtocol.REQUEST_STATUS,4,hub.idAsArray() )
        time.sleep(0.2)
        if 0 == self.sliderAnimateHub.GetValue():
            self.createHubBitmap()
            self.remote.sendFanbotFrame(self.compressedFrame)
 


    def fanbbotSelected(self,item):
        mappedindex = Hub.fanbotMapping[item] -1
        print "Selected fanbot index: %d mapped onto index %d" %( item,mappedindex)
        if 0 == self.sliderAnimateHub.GetValue():
            self.createFanbotBitmap(mappedindex)
            self.remote.sendFanbotFrame(self.compressedFrame)

    def showHub(self,hub):
        """for the given hub, show the pixels in the bitmap, in the color of he hub"""
        if hub:
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
       
       
    def sliderAnimateHubOnScroll( self, event ):
        speed = self.sliderAnimateHub.GetValue()
        if speed == 0:
            self.timerAnimate.Stop()
        else :
            self.timerAnimate.Start(250 -speed * 25)         
            
    def timerAnimateHandler(self,event):
        """  """
        hub         = self.currenthub
        if  hub:
            self.animateIdx += 1
            if self.animateIdx >= 24: 
                self.animateIdx = 0
            self.createFanbotBitmap(Hub.fanbotMapping[self.animateIdx] - 1)    
        self.remote.sendFanbotFrame(self.compressedFrame)
 
    def createHubBitmap(self):
        hub         = self.currenthub
        maxFrameLen = len(self.compressedFrame)
        if  hub:
            for i in range(maxFrameLen):
                self.compressedFrame[i]=0
            for i in range(24):
                pixelIdx = hub.config[i]
                if pixelIdx >= 0:
                    byteIdx = pixelIdx / 8
                    if byteIdx >= maxFrameLen:
                        return;
                    bitIdx = pixelIdx % 8
                    self.compressedFrame[byteIdx] |= 1 << bitIdx
    
                
        
    def createFanbotBitmap(self,idx):        
        hub         = self.currenthub
        maxFrameLen = len(self.compressedFrame)
        if  hub:
            for i in range(maxFrameLen):
                self.compressedFrame[i]=0
            if idx >= 24:
                return    
            pixelIdx = hub.config[idx]
            if pixelIdx >= 0:
                byteIdx = pixelIdx / 8
                if byteIdx >= maxFrameLen:
                    return;
                bitIdx = pixelIdx % 8
                self.compressedFrame[byteIdx] = 1 << bitIdx
