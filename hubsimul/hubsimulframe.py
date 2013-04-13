# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class HubSimulFrame
###########################################################################

class HubSimulFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( 600,300 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 600,-400 ), wx.DefaultSize )
		
		bSizer2 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelMain = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer31 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_notebook1 = wx.Notebook( self.panelMain, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.NB_BOTTOM )
		self.panelRawBitmap = wx.Panel( self.m_notebook1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer4 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelRawBitmap.SetSizer( bSizer4 )
		self.panelRawBitmap.Layout()
		bSizer4.Fit( self.panelRawBitmap )
		self.m_notebook1.AddPage( self.panelRawBitmap, u"bitmap", True )
		self.panelModule = wx.Panel( self.m_notebook1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.panelModule.SetSizer( bSizer5 )
		self.panelModule.Layout()
		bSizer5.Fit( self.panelModule )
		self.m_notebook1.AddPage( self.panelModule, u"modulen", False )
		
		bSizer31.Add( self.m_notebook1, 1, wx.EXPAND |wx.ALL, 5 )
		
		self.panelMain.SetSizer( bSizer31 )
		self.panelMain.Layout()
		bSizer31.Fit( self.panelMain )
		bSizer2.Add( self.panelMain, 1, wx.EXPAND |wx.ALL, 5 )
		
		self.panelStatus = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SUNKEN_BORDER|wx.TAB_TRAVERSAL )
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.labelStatus = wx.StaticText( self.panelStatus, wx.ID_ANY, u"status", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.labelStatus.Wrap( -1 )
		self.labelStatus.SetMinSize( wx.Size( 200,15 ) )
		
		bSizer3.Add( self.labelStatus, 0, wx.ALL, 5 )
		
		self.panelStatus.SetSizer( bSizer3 )
		self.panelStatus.Layout()
		bSizer3.Fit( self.panelStatus )
		bSizer2.Add( self.panelStatus, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.SetSizer( bSizer2 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.panelMain.Bind( wx.EVT_LEFT_DOWN, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_LEFT_UP, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_MIDDLE_DOWN, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_MIDDLE_UP, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_RIGHT_DOWN, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_RIGHT_UP, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_MOTION, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_LEFT_DCLICK, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_MIDDLE_DCLICK, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_RIGHT_DCLICK, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_LEAVE_WINDOW, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_ENTER_WINDOW, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_MOUSEWHEEL, self.panelTribuneOnMouseEvents )
		self.panelMain.Bind( wx.EVT_PAINT, self.panelTribuneOnPaint )
		self.panelMain.Bind( wx.EVT_SIZE, self.panelTribuneOnSize )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def panelTribuneOnMouseEvents( self, event ):
		event.Skip()
	
	def panelTribuneOnPaint( self, event ):
		event.Skip()
	
	def panelTribuneOnSize( self, event ):
		event.Skip()
	

###########################################################################
## Class BitmapCanvas
###########################################################################

class BitmapCanvas ( wx.Panel ):
	
	def __init__( self, parent ):
		wx.Panel.__init__ ( self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition, size = wx.Size( 500,300 ), style = wx.TAB_TRAVERSAL )
		
		
		# Connect Events
		self.Bind( wx.EVT_PAINT, self.BitmapCanvasOnPaint )
		self.Bind( wx.EVT_SIZE, self.BitmapCanvasOnSize )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def BitmapCanvasOnPaint( self, event ):
		event.Skip()
	
	def BitmapCanvasOnSize( self, event ):
		event.Skip()
	

