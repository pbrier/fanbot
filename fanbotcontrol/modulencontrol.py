'''
Created on Mar 25, 2013

@author: smeenka
'''
from designcontrol import DesignControl
from fanbotframe import PanelModules
from proxycontrol import ProxyControl
from hubprotocol import HubProtocol
import wx

from fanbotconfig import FanbotConfig

from executecontrol import ExecuteControl

class ModulenControl (PanelModules) :
    
    
    def __init__( self, parent, remote ):
        """ remote is instance of HubProtocol for communication with remote"""
        PanelModules.__init__( self,parent)
        self.remote = remote
        self.scaleX = 1
        self.scaleY = 1



    
    def listModuleConfigOnListBox( self, event ):
        event.Skip()
    
    def buttonResetDiscOnButtonClick( self, event ):
        self.remote.sendCommand( HubProtocol.command_disc_reset)
    
    def buttonDiscoverOnButtonClick( self, event ):
        self.remote.sendCommand( HubProtocol.command_discovery)
    
    def buttonClearListOnButtonClick( self, event ):
        self.listModules.Clear()
    
    
    def panelModulesCanvasOnSize( self, event ):
        event.Skip()

    def panelModulesCanvasOnPaint( self, event ):
        dc = wx.PaintDC(self.panelModulesCanvas)
        dc.BeginDrawing()
        dc.SetUserScale(self.scaleX, self.scaleY)
        dc.Clear()
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
            if point.x < 0 or point.x >= 50 or point.y < 0 or point.y >= 20:
                #print 'x,y out of bounds: ',point.x ,':',point.y    
                return
            self.pointCurrent = point    
            #print "mouse event X : " ,point.x , " Y: ",point.y

        if event.LeftDown():
           #print "mouse down X : " ,point.x , " Y: ",point.y
           # Left mouse button down, change cursor to
           # something else to denote event capture
           self.pointStartDrag = point
           cur = wx.StockCursor(wx.CURSOR_CROSS)  
           canvas.SetCursor(cur)
           self.setPixel(point.x, point.y,self.brushColor)        
           # invalidate current canvas
           self.Refresh()

            
        if event.LeftIsDown():
            self.pointDragTo = point
            #print "Dragging to X : " ,point.x , " Y: ",point.y
            cur = wx.StockCursor(wx.CURSOR_CROSS)  
            canvas.SetCursor(cur)
            if self.drawMode == 0:
                self.setPixel(point.x, point.y,self.brushColor)
            if self.drawMode == 1:
                for x in range (self.pointStartDrag.x,point.x):
                    for y in range (self.pointStartDrag.y,point.y):
                        self.setPixel(x, y,self.brushColor)
                                    
            self.panelIconCanvas.Refresh()
                 
    
    def listModulesOnListBox( self, event ):
        fname = event.GetString()
        print 'selected: ', fname
        
        
    def handleCommand(self, opcode,payload):
        """ Handle incoming data from remote hubs or hubsimulator"""
        print "Received opcode: %04x -- %s" % (opcode,payload)      
        
        if opcode == HubProtocol.command_disc_ack:
            str = " " 
            for i in payload:
                str =  "%s %02x"%(str,i)
            wx.CallAfter(self.appendListmodulesInGuiThread,str)
            
            
    def appendListmodulesInGuiThread(self,str):  
        self.listModules.Append(str)            

            
