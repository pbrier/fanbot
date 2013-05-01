'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array
import wx

class Banner:
    bpp    = 3  # bytes per pixel
    ppf     = 5 # pixels per frame
    
    def __init__( self, w, h, filename ):
        self.width = w
        self.height = h
        self.frameCount = 0 
        self.frameIndex = 0
        
        self.im = None
        self.frame = None
        self.loadFromFile(filename)      
        self.nextFrame()    
 
    def getBitmap(self):     
        return  wx.BitmapFromImage(self.frame)
    
    def loadFromFile(self, filename):
        """ filename must end on bmp.
            bmp files are intepreted as info files shifted from left to right to the viewport:
        """
        self.filename = filename
        self.frameIndex = 0
        if not filename.endswith('.bmp'):
            print 'Error: filenname must end with .bmp'
                    
        try: 
            im = wx.Image(self.filename,   type = wx.BITMAP_TYPE_BMP)

        except EOFError as e:
            print "loadFromFile EOFError ", e.message
        except IOError as eo:
            print "loadFromFile IOError ", eo.message
        except Exception as e1:   
            print "loadFromFile Exception " , e1.message
 
        w = im.GetWidth()
        h = im.GetHeight()
        factor = 1.0 *self.height / h
        neww = 1 +  int(w * factor)
        self.frameCount = int (  (w - self.width) / Banner.ppf )
        print 'factor is :' , factor, 'width :', neww, 'frame count: ', self.frameCount
        self.im = im
        self.nextFrame()

        #for key,value in self.im.info:
        #    print "Key: ", key ," value: ",value    
 
    def getCompressedArray(self):
        size = self.height * self.width / 8
        result = array.array('B', [0] * size)
        
        byteidx = 0
        bitidx  = 0
        b   = 0
        for y in range(self.height):
            for x  in range(self.width):
                """ if r > 10   and b > 10 and g > 10 set bit in compressed array"""
                red = self.frame.GetRed(x,y)
                if red > 10:
                    green = self.frame.GetGreen(x,y)
                    if green > 10:
                        blue = self.frame.GetBlue(x,y)
                        if blue > 10:
                            b |= 128
                bitidx +=1
                if bitidx < 8:
                    b = b >> 1
                else:    
                    bitidx = 0
                    if byteidx < size:
                        result[byteidx] = b
                    b = 0
                    byteidx += 1
    
        return result        
    
    def saveCurrentFile(self):
        print "saveCurrentFile not supported:" 
 

                

    def nextFrame(self):
        """If a GIF: (animate) Advance to next frame in animated GIF. If beyond the last, open a new frame
           If a BMP (banner) : Shift left a banner with a few pixels
           
        """

        x_offset = self.frameIndex * Banner.ppf

        self.frame = self.im.GetSubImage(wx.Rect(x_offset, 0, self.width,  self.height) )
        return self.frameIndex


    def getFrameNr(self):
        return self.frameIndex

    def getFrameCount(self):
        return self.frameCount;
    
    def setFrameNr(self,nr):
        if nr >= self.frameCount:
            print "Cannot set frame nr, is beyond frmaecount: ", self.frameCount
            return
        self.frameIndex = nr
        self.nextFrame()
            
