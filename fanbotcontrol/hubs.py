'''
Created on Mar 25, 2013

@author: smeenka
'''
import wx
import random
from fanbotconfig import FanbotConfig
import array



class HubListModel(wx.ListCtrl):
    def __init__(self, parent, listener):
        wx.ListCtrl.__init__(
            self, parent, -1, 
            style=wx.LC_REPORT|wx.LC_VIRTUAL|wx.LC_HRULES|wx.LC_VRULES
            )
        self.parent = parent
        self.InsertColumn(0, "ID (hex)")
        self.InsertColumn(1, "Disc")
        self.InsertColumn(2, "Resp")
        self.listener = listener
        
        self.SetColumnWidth(0, 60)
        self.SetColumnWidth(1, 35)
        self.SetColumnWidth(2, 35)

        self.hubs = []
        ''' self.hubs.append(Hub('0x1234'))
        self.hubs.append(Hub('0x3456'))
        self.hubs.append(Hub('0x789A'))
        '''
        self.load()
        self.SetItemCount(len(self.hubs))
        
        self.attr = wx.ListItemAttr()
        self.attr.SetBackgroundColour("light blue")

        self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnItemSelected)
        self.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.OnItemActivated)
        self.Bind(wx.EVT_LIST_ITEM_DESELECTED, self.OnItemDeselected)


    def OnItemSelected(self, event):
        item = event.m_itemIndex
        print ('OnItemSelected: "%s", "%s", "%s", "%s"\n' %
                           (item,
                            self.GetItemText(item),
                            self.getColumnText(item, 1),
                            self.getColumnText(item, 2)))
        if item >= len(self.hubs):
            print 'Error row index too high: ', item
            return ""
        hub = self.hubs[item]
        self.listener.hubSelected(hub)

    def OnItemActivated(self, event):
        self.currentItem = event.m_itemIndex
        print("OnItemActivated: %s\nTopItem: %s\n" %
                           (self.GetItemText(self.currentItem), self.GetTopItem()))

    def getColumnText(self, index, col):
        item = self.GetItem(index, col)
        return item.GetText()

    def OnItemDeselected(self, evt):
        print("OnItemDeselected: %s" % evt.m_itemIndex)


    #---------------------------------------------------
    # These methods are callbacks for implementing the
    # "virtualness" of the list...  Normally you would
    # determine the text, attributes and/or image based
    # on values from some external data source, but for
    # this demo we'll just calculate them
    def OnGetItemText(self, item, col):
        if item >= (self.hubs):
            print 'Error row index too high: ', items
            return ""
        hub = self.hubs[item]
        if col == 0: return hub.id
        elif col == 1: return self.boolean2String(hub.discoverd)
        elif col == 2: return self.boolean2String(hub.responsive)
        else: return ""

    def boolean2String(self,bool):
        if bool:
            return "1"
        else:
            return "0"

    def OnGetItemImage(self, item):
            return -1

    def OnGetItemAttr(self, item):
        if item >= (self.hubs):
            print 'Error row index too high: ', items
            return ""
        hub = self.hubs[item]
        if not hub.discoverd or not hub.responsive:
            return self.attr
        else:   
            return None

    def resetDiscovery(self):
        for hub in self.hubs:
            hub.discoverd = False
            hub.responsive = False
        self.parent.Refresh()    


    def load(self):
        for id in FanbotConfig.getHubIDs():
            self.addHub(id)
        self.SetItemCount(len(self.hubs))
        self.parent.Refresh();  

    def refresh(self):
        self.SetItemCount(len(self.hubs))
        self.parent.Refresh()
        
    def addHub(self,id):
        """check if hub is already defined, and if not create one.
        Returns the found hub"""
        found = None
        for hub in self.hubs:
            if hub.id == id:
                found = hub
                break
        if found == None:
            found = Hub(id)
            self.hubs.append(found)
        return found    
            
class FanbotListModel(wx.ListCtrl):
    def __init__(self, parent, listener):
        wx.ListCtrl.__init__(
            self, parent, -1, 
            style=wx.LC_REPORT|wx.LC_VIRTUAL|wx.LC_HRULES|wx.LC_VRULES
            )
        self.parent = parent
        self.InsertColumn(0, "#")
        self.InsertColumn(1, "ID")
        self.listener = listener
        
        self.SetColumnWidth(0, 20)


        self.SetItemCount(24)
        
        self.attr = wx.ListItemAttr()
        self.attr.SetBackgroundColour("light blue")

        self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnItemSelected)
        self.fanbots = None 

    def setFanbots(self,fanbots):
        self.fanbots = fanbots   
        self.parent.Refresh();





            
    def OnItemSelected(self, event):
        item = event.m_itemIndex
        print 'OnItemSelected: "%s", "%s"\n' % (item,self.getColumnText(item, 1) )
        if item >= 24:
            print 'Error row index too high: ', item
            return ""
        self.listener.fanbbotSelected(item)


    def getColumnText(self, index, col):
        item = self.GetItem(index, col)
        return item.GetText()



    #---------------------------------------------------
    # These methods are callbacks for implementing the
    # "virtualness" of the list...  Normally you would
    # determine the text, attributes and/or image based
    # on values from some external data source, but for
    # this demo we'll just calculate them
    def OnGetItemText(self, item, col):
        result = ''
        if item >= 24:
            print 'Error row index too high: ', items
            return result
        if col == 0: return item
        elif col == 1 and self.fanbots: result = self.fanbots[item]
        return result

    def OnGetItemImage(self, item):
        return -1

    def OnGetItemAttr(self, item):
        return None

            
class Hub :
    def __init__( self,id):
        """id must be a string with hexadecimal representaiton of ID example: '0x1234' """
        self.id = id
        self.discoverd = False
        self.responsive = False
        self.configidx = 0
        self.config = FanbotConfig.getHubConfig(self.id)
        self.fanbots = FanbotConfig.getHubFanbots(self.id)
        
        for i in range(24):
            if len(self.config) < 24:
                self.config.append(-1)
            if len(self.fanbots) < 24:
                self.fanbots.append(-1)
                 
        self.color = random.randint(20, 250) * 0x10000 + random.randint(20, 250) * 0x100 + random.randint(20, 250) 
        
    def save(self):
        FanbotConfig.setHubConfig(id,self.config)

    def resetConfig(self):
        self.configidx = 0
        for  i in range (0,24):
            self.config[i] = -1

    def canAddConfigItem(self):
        return self.configidx < 24        
            
    def setConfig(self,x,y):
        """"Set the config value on the current index and increment index
            If the index < max return true else false"""
        if self.configidx > 23:
            print 'Maximum amount of pixels reacched (24) '
            return False
        
        val = x + y * FanbotConfig.width
        #print 'setting config index %d to %d'%(self.configidx,val)
            
        if len(self.config) <= self.configidx:
            self.config.append(val)
        else:
            self.config[self.configidx] = val
        self.configidx += 1

        return True            

    def getConfigColor(self,index):
         if index > 23:
            print 'index too high: ', index
            return
         val = self.config[index]
         if val < 0:
             return (-1,0,0)
         
         x = val % FanbotConfig.width
         y = val / FanbotConfig.width            
         y = y % FanbotConfig.height      
         return (x,y,self.color)

    def setFanbots(self,rawdata):
        """From the raw data as received from the hub, extract the fanbot ID's"""
        if len(rawdata) < 100:
            print "Received incoorect data size. Expected 100 got: ",len(rawdata)
            return
        rawIndex = 4
        for i in range(24):
            id = ""
            for j in range(4):
                id =  "%02x%s"%(rawdata[rawIndex],id)
                rawIndex +=1
                if id == '00000000':
                    id = "--------"
            self.fanbots[i] = id                  

    def resetFanbots(self):
        """ reset the list of  fanbot ID's"""
        for i in range(24):
            self.fanbots[i] = "----"    
 
    def idAsArray(self):
         idAsLong = int(self.id,16)
         idArray = []
         for i in range (4):
             byte = idAsLong >> (i * 8)
             byte %= 0x100
             idArray.append(byte )
         return idArray    
                      
