'''
Created on Mar 25, 2013

@author: smeenka
'''
from designcontrol import DesignControl
from modulencontrol import ModulenControl
from fanbotframe import FanbotFrame
from proxycontrol import ProxyControl
from  serial import Serial, SerialException
import wx
import wx.lib.evtmgr as em
from hubprotocol import ParserFactory, HubProtocol
from fanbotsocket import FanbotSocket

from fanbotconfig import FanbotConfig
from fanbotserial import FanbotSerial
import fanbotevent
from executecontrol import ExecuteControl
import time
from fanbotcontrolbase import  ControlBase

class FanbotControl (FanbotFrame,ControlBase) :
    
    
    def __init__( self, parent ):
        FanbotFrame.__init__( self,parent)

        FanbotConfig.load()

        self.remote = HubProtocol(None) # no transport layer yet
        self.serial = None
        self.socket = None
        panel = self.tabPanelDesign
        sizer = panel.GetSizer()    
        self.designControl = DesignControl(panel) 
        sizer.Add( self.designControl, 1, wx.EXPAND )
        
        panel = self.tabPanelExec
        sizer = panel.GetSizer()
        self.executor = ExecuteControl(panel,self.remote)
        sizer.Add(self.executor, 1, wx.EXPAND) 
 
        panel = self.tabPanelProxy
        sizer = panel.GetSizer()
        self.proxy = ProxyControl(panel,self.remote)
        sizer.Add(self.proxy, 1, wx.EXPAND) 

        panel = self.tabPanelModules
        sizer = panel.GetSizer()
        self.modulen = ModulenControl(panel,self.remote)
        sizer.Add(self.modulen, 1, wx.EXPAND) 

        self.remote.addListener(self.modulen)

        self.initComboSerialPorts()

        self.initCommunication()
        self.SetSizeWH(600,400)
        
        em.eventManager.Register(self.proxyOnChange, fanbotevent.EVT_PROXY_SERVER, self.proxy)
        #self.proxy.startProxyServer()
        ControlBase.messageHandler = self.displayMessage


    def proxyOnChange(self,event):
        print "proxyOnChange", event.getPayload() 
        enable = 'stopped' ==  event.getPayload()
        self.tabPanelDesign.Enable(enable)
        self.tabPanelModules.Enable(enable)
        self.tabPanelSettings.Enable(enable)
        self.executor.stopSendingFrames()
        self.tabPanelExec.Enable(enable)
        
    def displayMessage(self, message):
        self.labelStatus.SetLabel(message)          

    """ Panel Wave   """
    """ Panel Modules   """
        
    """ Panel Configuration   """
    def selectSerialPortOnCheckBox( self, event ):
        print "selectSerialPortOnCheckBox: "
        if self.socket:
            self.socket.closeSocket()
        self.buttonConnectSimul.SetLabel('connect')            
        self.handleSelection(False)

    def selectSimulationOnCheckBox( self, event ):
        if self.serial:
            self.serial.close()
        self.buttonConnectReal.SetLabel('connect')            
        self.handleSelection(True)
            
    
    def buttonConnectRealOnButtonClick( self, event ):
        serialname = self.comboSerialPorts.GetValue()
        if self.buttonConnectReal.GetLabel() == 'connect':
            FanbotConfig.setSerialport(serialname)
            try:
                factory = ParserFactory()             # Parser factory can create a parser object
                self.serial = FanbotSerial(factory);
                self.serial.open(serialname,FanbotConfig.baudrate)
                self.remote.setTransport(self.serial)
                self.buttonConnectReal.SetLabel('disconnect')
                FanbotConfig.setSerialport(serialname)
            except     SerialException as e:
                print "Unable to open serial port ...error:", e.message
                dlg = wx.MessageDialog(self, e.message +' Select another port. or exit program with <Cancel>!',
                                   'Unable to open serial port %s'%serialname ,
                                   wx.OK| wx.CANCEL | wx.ICON_QUESTION
                                   )
                result = dlg.ShowModal() 
                dlg.Destroy()
                if wx.ID_OK !=result:
                    exit(1)
        else:
            self.serial.close()
            self.serial = None
            self.remote.setTransport(None)
            self.buttonConnectReal.SetLabel('connect')
            print 'now disconnected'
    
    
    def buttonConnectSimulOnButtonClick( self, event ):
        host = self.textSimulation.GetValue()
        FanbotConfig.setSimulationIP(host)
        port = self.textPort.GetValue()
        FanbotConfig.setSimulationIPPort(port)

        if self.buttonConnectSimul.GetLabel() == 'connect':
            try:
                factory = ParserFactory()             # Parser factory can create a parser object
                self.socket = FanbotSocket(factory)
                self.socket.openSocket(host,int(port))
                self.remote.setTransport(self.socket)
                self.buttonConnectSimul.SetLabel('disconnect')
                print 'now connected'       
            except Exception as e:
                dlg = wx.MessageDialog(self, 'Select another port. or exit program with <Cancel>!',
                                   'Unable to open connection to simulator at %s:%s'%(host,port),
                                   wx.OK| wx.CANCEL | wx.ICON_QUESTION)
                
                result = dlg.ShowModal() 
                dlg.Destroy()
                if wx.ID_OK  !=result:
                    exit(1)
            
        else:
            self.socket.closeSocket()
            self.socket = None
            self.remote.setTransport(None)
            self.buttonConnectSimul.SetLabel('connect')
            print 'now disconnected'
                
    def buttonSaveOnButtonClick( self, event ):
        host = self.textSimulation.GetValue()
        FanbotConfig.setSimulationIP(host)

        port = self.textPort.GetValue()
        FanbotConfig.setSimulationIPPort(port)
 
        proxy = self.textProxyPort.GetValue()
        FanbotConfig.setProxyPort(proxy)

        serialname = self.comboSerialPorts.GetValue()
        FanbotConfig.setSerialport(serialname)
        FanbotConfig.save()
            
    def radioBoxFrameOnRadioBox( self, event ):
        val = self.radioBoxFrame.GetSelection()
        if val == 0:
             HubProtocol.frameCommand = HubProtocol.PLAY_FRAME
        if val == 1:
             HubProtocol.frameCommand = HubProtocol.LED_FRAME
        if val == 2:
             HubProtocol.frameCommand = HubProtocol.POS_FRAME
       
        
    def initComboSerialPorts( self ):
        self.comboSerialPorts.Clear()
        for i in range(16):
            try:
                s = Serial(i)
                self.comboSerialPorts.Append(s.portstr)
                s.close()   # explicit close 'cause of delayed GC in java
                s = None
            except SerialException:
                pass
            
        self.comboSerialPorts.SetValue(FanbotConfig.getSerialport())    


    """ local helper functions   """
    def initCommunication(self):
        host = FanbotConfig.getSimulationIP()
        port = FanbotConfig.getSimulationIPPort()
        self.textSimulation.SetValue(host)
        self.textPort.SetValue(str(port))
        
        self.proxyPort = FanbotConfig.getProxyPort() 
        self.textProxyPort.SetValue(str(self.proxyPort))
        simulation = FanbotConfig.getSimulation()
        self.handleSelection(simulation)
        if simulation:
            self.buttonConnectSimulOnButtonClick(None)
        else:
            self.buttonConnectRealOnButtonClick(None)

    
    def handleSelection(self, simul):
        FanbotConfig.setSimulation(simul)
        real = not simul
        print "selectSimulation: ", str(simul)
        self.selectSerialPort.SetValue(real)
        self.selectSimulation.SetValue(simul)
        self.comboSerialPorts.Enable(real)
        self.textSimulation.Enable(simul)
        self.textPort.Enable(simul)
        self.buttonConnectReal.Enable(real)
        self.buttonConnectSimul.Enable(simul)


    def __del__( self ):
        print "fanbotcontrol destructor ..."
                
                
