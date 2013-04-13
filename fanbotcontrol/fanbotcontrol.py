'''
Created on Mar 25, 2013

@author: smeenka
'''
from icondraw import IconDraw
from fanbotframe import FanbotFrame
from  serial import Serial, SerialException
import wx
import os
from fanbotconfig import FanbotConfig
from fanbotcomm import FanbotComm

class FanbotControl (FanbotFrame) :
    
    def __init__( self, parent ):
        FanbotFrame.__init__( self,parent)
        panel = self.panelDraw
        sizer = panel.GetSizer()    
        self.iconDraw = IconDraw(panel) 
        sizer.Add( self.iconDraw, 1, wx.EXPAND )
 
        FanbotConfig.load()
        self.comm = FanbotComm()
        
        self.serial = None
        self.initComboSerialPorts()
        self.initFileList()
        self.textSimulation.SetValue(FanbotConfig.getSimulationIP())
        self.textPort.SetValue(str(FanbotConfig.getSimulationIPPort()))
        self.Refresh()

    """ Panel drawDesign   """
    def buttonPrevOnButtonClick( self, event ):
        self.iconDraw.prevFrame()
        self.labelFrame.SetLabel(self.iconDraw.getFrameCountAsString())
    
    def buttonNextOnButtonClick( self, event ):
        self.iconDraw.nextFrame()
        self.labelFrame.SetLabel(self.iconDraw.getFrameCountAsString())
  
    def buttonAddFrameOnButtonClick( self, event ):
        self.iconDraw.addFrame()
        self.labelFrame.SetLabel(self.iconDraw.getFrameCountAsString())
        

    def buttonDeleteFrameOnButtonClick( self, event ):
        frnr = self.iconDraw.delFrame()
        self.labelFrame.SetLabel(self.iconDraw.getFrameCountAsString())

    def buttonAddOnButtonClick( self, event ):
        event.Skip()
         
    def buttonDeleteOnButtonClick( self, event ):
        print 'buttonDeleteOnButtonClick'
    
    def buttonSaveOnButtonClick( self, event ):
        self.iconDraw.saveCurrentFile()
    
    def listIconsOnListBox( self, event ):
        fname = event.GetString()
        print 'selected: ', fname
        self.iconDraw.loadFromFile("data/" + fname)
        self.labelFrame.SetLabel(self.iconDraw.getFrameCountAsString())
      
    
    def sliderAnimateOnScroll( self, event ):
        pos = event.GetPosition()
        self.iconDraw.animate(pos)
        if pos == 0:
            self.labelFrame.SetLabel(self.iconDraw.getFrameCountAsString())

        
        
        
        event.Skip()    
    def initFileList(self):
        for fname in os.listdir("data"):
            if ".gif" in fname:
                print fname
                self.listIcons.Append(fname)
    """ Panel Wave   """
    """ Panel Modules   """
        
    """ Panel Configuration   """
    def selectSerialPortOnCheckBox( self, event ):
        print "selectSerialPortOnCheckBox: "
        self.handleSelection(False)

            
    
    def buttonConnectRealOnButtonClick( self, event ):
        serialname = self.comboSerialPorts.GetValue()
        FanbotConfig.setSerialport(serialname)
        try:
            self.serial = Serial(serialname);
            self.serial.baudrate = 115200
            print "Opened serial port ", serialname
        except     SerialException:
            print "Unable to open serial port ..."
            dlg = wx.MessageDialog(self, 'Select another port. or exit program with <Cancel>!',
                               'Unable to open serial port  port 1' ,
                               wx.OK| wx.CANCEL | wx.ICON_QUESTION
                               )
            result = dlg.ShowModal() 
            dlg.Destroy()
            if wx.ID_OK ==result:
                None 
            else:
                exit(1)
    
    def selectSimulationOnCheckBox( self, event ):
        self.handleSelection(True)
    
    def buttonConnectSimulOnButtonClick( self, event ):
        host = self.textSimulation.GetValue()
        FanbotConfig.setSimulationIP(host)
        port = self.textPort.GetValue()
        FanbotConfig.setSimulationIPPort(port)
        FanbotComm.openSocket(host,int(port))
        
    def initComboSerialPorts( self ):
        self.comboSerialPorts.Clear()
        for i in range(16):
            try:
                s = Serial(i)
                self.comboSerialPorts.Append(s.portstr)
                s.close()   # explicit close 'cause of delayed GC in java
            except SerialException:
                pass
            
        self.comboSerialPorts.SetValue(FanbotConfig.getSerialport())    


    """ local helper functions   """
    def handleSelection(self, simul):
        FanbotConfig.setSimulation(simul)
        real = not simul
        print "selectSimulation: ", str(simul)
        self.selectSerialPort.SetValue(real)
        self.selectSimulation.SetValue(simul)
        self.comboSerialPorts.Enable(real)
        self.textSimulation.Enable(simul)
        self.buttonConnectReal.Enable(real)
        self.buttonConnectSimul.Enable(simul)


    def __del__( self ):
        print "destructor ..."
        FanbotConfig.save()# Save
                
                
