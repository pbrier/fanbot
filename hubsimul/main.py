'''
Created on Mar 25, 2013

@author: smeenka
'''
import wxversion
wxversion.select('2.8')
import wx

from hubsimulcontrol import HubSimulControl

# Change "oneMinutePython" with the name of your application

class FanbotHubSimul(wx.App):
    def OnInit(self):
        self.mainFrame = HubSimulControl(None)
        self.mainFrame.Show()
        self.SetTopWindow(self.mainFrame)
        return True

app = FanbotHubSimul(0)
app.MainLoop()