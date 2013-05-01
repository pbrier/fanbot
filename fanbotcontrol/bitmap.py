'''
Created on Mar 27, 2013

@author: SMEENKA
'''
import array
import wx
from PIL import Image
from images2gif import writeGif

class Bitmap:
    bpp    = 4  # bytes per pixel
    
    def __init__( self, w, h , filename = None):
        self.width = w
        self.height = h
        
        if filename == None:
            self.filename = 'data/animatedgif.gif'

            size = w * h / 8
            buffer= array.array('B', [0] * size)
            val = 0
            for i in range(len(buffer)):
                buffer[i] = val
                val += 1
                val %= 256
            self.loadFromCompressedArray(buffer)      
        else:
            self.loadFromFile(filename)    
        self.framenr = 0
        self.changed = False
 
    def togglePixel(self,x,y):
        if y >= self.height:
            print "Row index too high: " , y
            return
        if x >= self.width:
            print "Col index too high: " , x
            return
        offset = (y * self.width + x) * Bitmap.bpp
        if self.buffer[offset] > 0:
            val = 0
        else:
            val = 255       
        self.im.putpixel( (x,y),val)


    
    def setPixel(self,x,y,val):
        if y >= self.height:
            print "Row index too high: " , y
            return
        if x >= self.width:
            print "Col index too high: " , x
            return
        print 'Setting pixel at ',x,":",y   
        self.changed = True 
        self.im.putpixel( (x,y),val)

 
 
 
    def getBitmap(self):     
        #return wx.BitmapFromBufferRGBA(self.width,self.height,self.im.getdata())
        return wx.BitmapFromImage(self.pil2image(False))
    



    def loadFromFile(self,filename):
        """ filename must end on gif.
            gif files are interpreted as animated gif files with multiple frames
        """
        if not filename.endswith('.gif'):
            print 'Error: filenname must end with .gif'
 
        self.filename = filename
        self.framenr = 0
        self.frames = []
        moreframes = True
        idx = 0
        while moreframes:
            try: 
                im = Image.open(filename)
                for i in range(idx):
                    im.seek(i)
                idx += 1
                frame = im.resize((self.width,self.height))
                frame = frame.convert("RGBA")
                self.frames.append(frame)

            except EOFError as e:
                print "Last frame reached in image ", e.message
                moreframes = False
            except IOError as eo:
                print "Last frame reached in image ", eo.message
                moreframes = False
            except Exception as e1:   
                print "Last frame reached in image" , e1.message
                moreframes = False
                    
        self.im = self.frames[self.framenr]        

        print self.im.info
        self.changed=False

        #for key,value in self.im.info:
        #    print "Key: ", key ," value: ",value    
 

    def loadFromBufferStart(self):
        self.buffer= array.array('B', [0] * self.width * self.height * Bitmap.bpp)
        self.bufferidx = 0

    def loadFromBufferAppend(self,b):
        i = self.bufferidx
        if i > len(self.buffer) - 4:
            return
        self.buffer[i + 0]= b
        self.buffer[i + 1]= b
        self.buffer[i + 2]= b
        self.buffer[i + 3]= 0xFF  # alfa
        self.bufferidx = i + 4

    def loadFromBufferFinish(self):
        im = Image.frombuffer("RGBA", (self.width,self.height), self.buffer)
        self.im = im
        self.frames = [im]        

    def loadFromCompressedArray(self,compressed):
        buffer= array.array('B', [0] * self.width * self.height * Bitmap.bpp)
        idx = -1
        val = 0
        inb = 0

        for j in range(0,self.width * self.height):
            if j % 8 == 0 and j < len(compressed) * 8: 
                idx += 1
                inb = compressed[idx]  
            if inb & 1 == 1: 
                val = 150 
            else: 
                val = 20
            inb = inb / 2
            offset = j * Bitmap.bpp
            buffer[offset+0] = val  
            buffer[offset+1] = val  
            buffer[offset+2] = val  
            buffer[offset+3] = 0xFF
        im = Image.frombuffer("RGBA", (self.width,self.height), buffer)
        self.im = im
        self.frames = [im]        
        
        
        
    def getCompressedArray(self):
        """If a GIF: (animate) Advance to next frame in animated GIF. If beyond the last, open a new frame
           If a BMP (banner) : Shift left a banner with a few pixels
           
        """

        size = self.height * self.width / 8
        result = array.array('B', [0] * size)
        
        pixels = self.im.getdata()
        byteidx = 0
        bitidx  = 0
        b   = 0
        for pixel in pixels:
            if pixel[0] > 50 and pixel[1] > 50 and pixel[2] > 50: 
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
        try:
            writeGif(self.filename,self.frames, duration=0.3, dither=0)
            self.changed = False
        except Exception as e:
            print "Unable to save file error:" ,e.message
            raise e
 
    def delFrame(self):
        """delete frame at current index.  Do nothing if only one frame        """
        if len(self.frames) <= 1: return 0
        self.frames.pop(self.framenr)
        if self.framenr > len(self.frames) -1:
            self.framenr -=1
        self.im = self.frames[self.framenr]
        print 'deleted new frame at index' ,self.framenr
        self.changed = True
        return self.framenr

    def addFrame(self):
        """add frame after the current index.  
           return the frame number of the newly inserted  frame    
        """
        frame = self.im.copy() 
        self.framenr += 1
        self.frames.insert(self.framenr, frame)
        self.im = self.frames[self.framenr]
        print 'added new frame at index' ,self.framenr
        self.changed = True
        return self.framenr        

                
    def prevFrame(self):
        """Go back  to previous  frame in animated GIF. 
           return true if still more frames are available, false if this is the first frame    
        """
        if self.framenr == 0: return False
        self.framenr -=1
        self.im = self.frames[self.framenr]
        return True

    def nextFrame(self):
        """Advance to next frame in animated GIF. If beyond the last do nothing
           return true if still more frames are available, false if this is the last frame    
        """
        if self.framenr >= len(self.frames) -1:return False
        self.framenr +=1
        self.im = self.frames[self.framenr]
        return True

    def setFrameNr(self,nr):
        if nr < 0 or nr >= len(self.frames) -1:return
        self.framenr = nr
        self.im = self.frames[self.framenr]

    def getFrameCount(self):
        return len(self.frames)    

    def getFrameNr(self):
        return self.framenr    
            
    def pil2image(self,alpha=True):
        """Convert PIL Image to wx.Image."""
        if alpha:
            image = apply( wx.EmptyImage, self.im.size )
            image.SetData( self.im.convert( "RGB").tostring() )
            image.SetAlphaData(self.im.convert("RGBA").tostring()[3::4])
        else:
            image = wx.EmptyImage(self.im.size[0], self.im.size[1])
            new_image = self.im.convert('RGB')
            data = new_image.tostring()
            image.SetData(data)
        return image
    
                    
        