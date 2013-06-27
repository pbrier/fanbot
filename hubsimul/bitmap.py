'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array

import wx
from hubsimulframe import  BitmapCanvas
from hubprotocol import HubProtocol

class Bitmap(BitmapCanvas):
    bpp    = 4  # bytes per pixel
    width = 44
    height= 24
    
    def __init__( self, parent,w, h ):
        BitmapCanvas.__init__( self,parent)
        self.width = w
        self.height = h
        size = w * h * Bitmap.bpp
        self.buffer= array.array('B', [0] * size)
        self.scaleX = 1
        self.scaleY = 1
        val = 0
        for x in range(w):
            val = 0x10
            for y in range(h):
                self.setPixel(x, y, val,val,val)
                val += 30
                val %= 256
                if x % 10 ==0:
                    val = 10
                if y % 10 ==0:
                    val = 10
        self.BitmapCanvasOnSize(None)
    

    def setPixel(self,x,y,r,g,b):
        if y >= self.height:
            print "Row index too high: " , y
            exit(1)
        if x >= self.width:
            print "Col index too high: " , x
            exit(1)
        offset = (y * self.width + x) * Bitmap.bpp
        self.setPixelAt(offset,r,g,b)

    def setPixelAt(self,offset,r,g,b):
        if offset >= len(self.buffer) - 3:
            print "Offset too high: " , offset
            return
        self.buffer[offset + 0] = r
        self.buffer[offset + 1] = g
        self.buffer[offset + 2] = b
        self.buffer[offset + 3] = 0xFF  #alpha

 
    
    def BitmapCanvasOnPaint( self, event ):
        dc = wx.PaintDC(self)
        dc.BeginDrawing()
        dc.SetUserScale(self.scaleX, self.scaleY)
        dc.Clear()
#        dc.ComputeScaleAndOrigin()
#        dc.SetMapMode(wx.MM_POINTS)
        bitmap = wx.BitmapFromBufferRGBA(self.width,self.height,self.buffer) 
        dc.DrawBitmap(bitmap,0,0)
        dc.EndDrawing()
           
    def BitmapCanvasOnSize( self, event ):
        rect = self.GetRect()
        self.scaleX = rect.GetWidth() / self.width
        self.scaleY = rect.GetHeight() / self.height
        #print 'Scale x:y',self.scaleX,':',self.scaleY
        self.Refresh() 
        
        
    def fromCompressedBuffer(self,command,data):
          
        idx = -1
        val = 0
        inb = 0
        r = 0
        g = 0
        b = 0

        for j in range(0,self.width * self.height):
            if j % 8 == 0 and j < len(data) * 8: 
                idx += 1
                inb = data[idx]  
            if inb & 1 == 1: 
                val = 150 
            else: 
                val = 20
            inb = inb / 2
            offset = j * Bitmap.bpp
            if command == HubProtocol.PLAY_FRAME: 
                r = val    
            elif command == HubProtocol.LED_FRAME:      
                g= val    
            elif command == HubProtocol.POS_FRAME:      
                b = val
            self.setPixelAt(offset, r,g,b)
                    
        self.Refresh()       