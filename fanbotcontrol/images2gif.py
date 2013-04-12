""" MODULE images2gif

Provides a function (writeGif) to write animated gif from a series
of PIL images or numpy arrays.

This code is provided as is, and is free to use for all.

Almar Klein (June 2009)

- based on gifmaker (in the scripts folder of the source distribution of PIL)
- based on gif file structure as provided by wikipedia

"""

import PIL
from PIL import Image
from PIL.GifImagePlugin import getheader, getdata

import array

# getheader gives a 87a header and a color palette (two elements in a list).
# getdata()[0] gives the Image Descriptor up to (including) "LZW min code size".
# getdatas()[1:] is the image data itself in chuncks of 256 bytes (well
# technically the first byte says how many bytes follow, after which that
# amount (max 255) follows).


def intToBin(i):
    """ Integer to string representation in little endian with two bytes """
    # devide in two parts (bytes)
    i1 = i % 256
    i2 = int( i/256)
    # make string (little endian)
    return chr(i1) + chr(i2)


def getheaderAnim(im):
    """ Animation header. To replace the getheader()[0] """
    bb = "GIF89a"
    bb += intToBin(im.size[0])
    bb += intToBin(im.size[1])
    bb += "\x87\x00\x00"
    return bb


def getAppExt(loops=0):
    """ Application extention. Part that secifies amount of loops. 
    if loops is 0, if goes on infinitely.
    """
    bb = "\x21\xFF\x0B"  # application extension
    bb += "NETSCAPE2.0"
    bb += "\x03\x01"
    if loops == 0:
        loops = 2**16-1
    bb += intToBin(loops)
    bb += '\x00'  # end
    return bb


def getGraphicsControlExt(duration=0.1):
    """ Graphics Control Extension. A sort of header at the start of
    each image. Specifies transparancy and duration. """
    bb = '\x21\xF9\x04'
    bb += '\x08'  # no transparancy
    bb += intToBin( int(duration*100) ) # in 100th of seconds
    bb += '\x00'  # no transparant color
    bb += '\x00'  # end
    return bb


def _writeGifToFile(fp, images, durations, loops):
    """ Given a set of images writes the bytes to the specified stream.
    """
    
    # init
    frames = 0
    previous = None
    
    for im in images:
        
        if not previous:
            # first image
            
            # gather data
            palette = getheader(im)[1]
            data = getdata(im)
            imdes, data = data[0], data[1:]            
            header = getheaderAnim(im)
            appext = getAppExt(loops)
            graphext = getGraphicsControlExt(durations[0])
            
            # write global header
            fp.write(header)
            fp.write(palette)
            fp.write(appext)
            
            # write image
            fp.write(graphext)
            fp.write(imdes)
            for d in data:
                fp.write(d)
            
        else:
            # gather info (compress difference)              
            data = getdata(im) 
            imdes, data = data[0], data[1:]       
            graphext = getGraphicsControlExt(durations[frames])
            
            # write image
            fp.write(graphext)
            fp.write(imdes)
            for d in data:
                fp.write(d)

        
        # prepare for next round
        previous = im.copy()        
        frames = frames + 1

    fp.write(";")  # end gif
    return frames


def writeGif(filename, images, duration=0.1, loops=0, dither=1):
    """ writeGif(filename, images, duration=0.1, loops=0, dither=1)
    Write an animated gif from the specified images. 
    images should be a list of numpy arrays of PIL images.
    Numpy images of type float should have pixels between 0 and 1.
    Numpy images of other types are expected to have values between 0 and 255.
    """
    
    if PIL is None:
        raise RuntimeError("Need PIL to write animated gif files.")
    
    images2 = []
    
    # convert to PIL
    for im in images:
        
        if isinstance(im,Image.Image):
            images2.append( im.convert('P',dither=dither) )
            
        else:
            raise ValueError("Unknown image type.")
    
    # check duration
    if hasattr(duration, '__len__'):
        if len(duration) == len(images2):
            durations = [d for d in duration]
        else:
            raise ValueError("len(duration) doesn't match amount of images.")
    else:
        durations = [duration for im in images2]
        
    
    # open file
    fp = open(filename, 'wb')
    
    # write
    try:
        n = _writeGifToFile(fp, images2, durations, loops)
        print n, 'frames written'
    finally:
        fp.close()
    
    
if __name__ == '__main__':
    images = []
    for i in range(10):
        buffer= array.array('B', [0xFF] * 200 * 100 * 4)
        for j in range(0,200 * 100 * 4,4):
            buffer[j] = i * 25
        im = Image.frombuffer("RGBA", (200,100), buffer) 
        images.append(im)
    
    writeGif('icons/testImage2gif.gif',images, duration=0.1, dither=0)
    
