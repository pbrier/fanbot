'''
Created on Mar 25, 2013

@author: smeenka
'''
import wx

from fanbotcontrol import FanbotControl

# Change "oneMinutePython" with the name of your application

class FanbotControlApp(wx.App):
    def OnInit(self):
        self.mainFrame = FanbotControl(None)
        self.mainFrame.Show()
        self.SetTopWindow(self.mainFrame)
        return True

app = FanbotControlApp(0)
app.MainLoop()