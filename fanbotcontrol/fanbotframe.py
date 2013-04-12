# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

brush1 = 1000
brush4 = 1001
brush9 = 1002
drawdot = 1003
drawline = 1004
toolcolor = 1005
toolblack = 1006
toolWhite = 1007

###########################################################################
## Class FanbotFrame
###########################################################################

class FanbotFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( -1,-1 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 700,470 ), wx.DefaultSize )
		
		bSizer2 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelMain = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer31 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_notebook1 = wx.Notebook( self.panelMain, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.NB_BOTTOM )
		self.panelDesign = wx.Panel( self.m_notebook1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer4 = wx.BoxSizer( wx.HORIZONTAL )
		
		sizerDraw = wx.BoxSizer( wx.VERTICAL )
		
		self.panelDraw = wx.Panel( self.panelDesign, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer8 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelDraw.SetSizer( bSizer8 )
		self.panelDraw.Layout()
		bSizer8.Fit( self.panelDraw )
		sizerDraw.Add( self.panelDraw, 7, wx.EXPAND |wx.ALL, 5 )
		
		self.m_panel9 = wx.Panel( self.panelDesign, wx.ID_ANY, wx.DefaultPosition, wx.Size( 30,-1 ), wx.TAB_TRAVERSAL )
		bSizer7 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.sliderAnimate = wx.Slider( self.m_panel9, wx.ID_ANY, 0, 0, 9, wx.DefaultPosition, wx.DefaultSize, wx.SL_HORIZONTAL )
		self.sliderAnimate.SetToolTipString( u"Animate" )
		
		bSizer7.Add( self.sliderAnimate, 1, wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer7.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.buttonDeleteFrame = wx.Button( self.m_panel9, wx.ID_ANY, u"-", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonDeleteFrame.SetToolTipString( u"verwijder frame" )
		
		bSizer7.Add( self.buttonDeleteFrame, 0, wx.ALL, 5 )
		
		self.buttonAddFrame = wx.Button( self.m_panel9, wx.ID_ANY, u"+", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonAddFrame.SetToolTipString( u"copy frame" )
		
		bSizer7.Add( self.buttonAddFrame, 0, wx.ALL, 5 )
		
		self.buttonPrev = wx.Button( self.m_panel9, wx.ID_ANY, u"<<", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonPrev.SetToolTipString( u"vorig frame" )
		
		bSizer7.Add( self.buttonPrev, 0, wx.ALL, 5 )
		
		self.buttonNext = wx.Button( self.m_panel9, wx.ID_ANY, u">>", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonNext.SetToolTipString( u"volgende frame" )
		
		bSizer7.Add( self.buttonNext, 0, wx.ALL, 5 )
		
		self.labelFrame = wx.StaticText( self.m_panel9, wx.ID_ANY, u"0", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.labelFrame.Wrap( -1 )
		self.labelFrame.SetToolTipString( u"frame/nrframes" )
		self.labelFrame.SetMinSize( wx.Size( 50,-1 ) )
		
		bSizer7.Add( self.labelFrame, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_panel9.SetSizer( bSizer7 )
		self.m_panel9.Layout()
		sizerDraw.Add( self.m_panel9, 0, wx.EXPAND |wx.ALL, 5 )
		
		bSizer4.Add( sizerDraw, 7, wx.EXPAND, 5 )
		
		self.m_panel91 = wx.Panel( self.panelDesign, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_scrolledWindow1 = wx.ScrolledWindow( self.m_panel91, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.VSCROLL )
		self.m_scrolledWindow1.SetScrollRate( 5, 5 )
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		listIconsChoices = []
		self.listIcons = wx.ListBox( self.m_scrolledWindow1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, listIconsChoices, 0 )
		self.listIcons.SetMinSize( wx.Size( 200,300 ) )
		
		bSizer5.Add( self.listIcons, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_scrolledWindow1.SetSizer( bSizer5 )
		self.m_scrolledWindow1.Layout()
		bSizer5.Fit( self.m_scrolledWindow1 )
		bSizer9.Add( self.m_scrolledWindow1, 3, wx.EXPAND |wx.ALL, 5 )
		
		self.m_panel10 = wx.Panel( self.m_panel91, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer10 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer10.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.buttonDelete = wx.Button( self.m_panel10, wx.ID_ANY, u"-", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		bSizer10.Add( self.buttonDelete, 0, wx.ALL, 5 )
		
		self.buttonAdd = wx.Button( self.m_panel10, wx.ID_ANY, u"+", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		bSizer10.Add( self.buttonAdd, 0, wx.ALL, 5 )
		
		self.buttonSave = wx.Button( self.m_panel10, wx.ID_ANY, u"save", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.buttonSave.SetDefault() 
		bSizer10.Add( self.buttonSave, 0, wx.ALL, 5 )
		
		self.m_panel10.SetSizer( bSizer10 )
		self.m_panel10.Layout()
		bSizer10.Fit( self.m_panel10 )
		bSizer9.Add( self.m_panel10, 0, wx.EXPAND |wx.ALL, 5 )
		
		self.m_panel91.SetSizer( bSizer9 )
		self.m_panel91.Layout()
		bSizer9.Fit( self.m_panel91 )
		bSizer4.Add( self.m_panel91, 3, wx.EXPAND |wx.ALL, 5 )
		
		self.panelDesign.SetSizer( bSizer4 )
		self.panelDesign.Layout()
		bSizer4.Fit( self.panelDesign )
		self.m_notebook1.AddPage( self.panelDesign, u"tekenen", True )
		self.panelWaves = wx.Panel( self.m_notebook1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_notebook1.AddPage( self.panelWaves, u"uitvoeren", False )
		self.panelModules = wx.Panel( self.m_notebook1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_notebook1.AddPage( self.panelModules, u"modules", False )
		self.panelSettings = wx.Panel( self.m_notebook1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		gSizer1 = wx.GridSizer( 2, 4, 0, 0 )
		
		self.selectSerialPort = wx.CheckBox( self.panelSettings, wx.ID_ANY, u"fanbotnet", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.selectSerialPort.SetValue(True) 
		gSizer1.Add( self.selectSerialPort, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		comboSerialPortsChoices = []
		self.comboSerialPorts = wx.ComboBox( self.panelSettings, wx.ID_ANY, u"Combo!", wx.DefaultPosition, wx.DefaultSize, comboSerialPortsChoices, wx.CB_DROPDOWN )
		self.comboSerialPorts.SetMinSize( wx.Size( 200,20 ) )
		
		gSizer1.Add( self.comboSerialPorts, 1, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticText4 = wx.StaticText( self.panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		gSizer1.Add( self.m_staticText4, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.buttonConnectReal = wx.Button( self.panelSettings, wx.ID_ANY, u"connect", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.buttonConnectReal, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.selectSimulation = wx.CheckBox( self.panelSettings, wx.ID_ANY, u"simulation", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.selectSimulation, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.textSimulation = wx.TextCtrl( self.panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.textSimulation.SetMinSize( wx.Size( 200,30 ) )
		
		gSizer1.Add( self.textSimulation, 1, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 5 )
		
		self.textPort = wx.TextCtrl( self.panelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.textPort.SetMinSize( wx.Size( -1,30 ) )
		
		gSizer1.Add( self.textPort, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.buttonConnectSimul = wx.Button( self.panelSettings, wx.ID_ANY, u"connect", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.buttonConnectSimul, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.panelSettings.SetSizer( gSizer1 )
		self.panelSettings.Layout()
		gSizer1.Fit( self.panelSettings )
		self.m_notebook1.AddPage( self.panelSettings, u"configuratie", False )
		
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
		bSizer2.Fit( self )
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.sliderAnimate.Bind( wx.EVT_SCROLL, self.sliderAnimateOnScroll )
		self.buttonDeleteFrame.Bind( wx.EVT_BUTTON, self.buttonDeleteFrameOnButtonClick )
		self.buttonAddFrame.Bind( wx.EVT_BUTTON, self.buttonAddFrameOnButtonClick )
		self.buttonPrev.Bind( wx.EVT_BUTTON, self.buttonPrevOnButtonClick )
		self.buttonNext.Bind( wx.EVT_BUTTON, self.buttonNextOnButtonClick )
		self.listIcons.Bind( wx.EVT_LISTBOX, self.listIconsOnListBox )
		self.buttonDelete.Bind( wx.EVT_BUTTON, self.buttonDeleteOnButtonClick )
		self.buttonAdd.Bind( wx.EVT_BUTTON, self.buttonAddOnButtonClick )
		self.buttonSave.Bind( wx.EVT_BUTTON, self.buttonSaveOnButtonClick )
		self.selectSerialPort.Bind( wx.EVT_CHECKBOX, self.selectSerialPortOnCheckBox )
		self.buttonConnectReal.Bind( wx.EVT_BUTTON, self.buttonConnectRealOnButtonClick )
		self.selectSimulation.Bind( wx.EVT_CHECKBOX, self.selectSimulationOnCheckBox )
		self.buttonConnectSimul.Bind( wx.EVT_BUTTON, self.buttonConnectSimulOnButtonClick )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def sliderAnimateOnScroll( self, event ):
		event.Skip()
	
	def buttonDeleteFrameOnButtonClick( self, event ):
		event.Skip()
	
	def buttonAddFrameOnButtonClick( self, event ):
		event.Skip()
	
	def buttonPrevOnButtonClick( self, event ):
		event.Skip()
	
	def buttonNextOnButtonClick( self, event ):
		event.Skip()
	
	def listIconsOnListBox( self, event ):
		event.Skip()
	
	def buttonDeleteOnButtonClick( self, event ):
		event.Skip()
	
	def buttonAddOnButtonClick( self, event ):
		event.Skip()
	
	def buttonSaveOnButtonClick( self, event ):
		event.Skip()
	
	def selectSerialPortOnCheckBox( self, event ):
		event.Skip()
	
	def buttonConnectRealOnButtonClick( self, event ):
		event.Skip()
	
	def selectSimulationOnCheckBox( self, event ):
		event.Skip()
	
	def buttonConnectSimulOnButtonClick( self, event ):
		event.Skip()
	

###########################################################################
## Class PanelDrawIcon
###########################################################################

class PanelDrawIcon ( wx.Panel ):
	
	def __init__( self, parent ):
		wx.Panel.__init__ ( self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition, size = wx.Size( 400,300 ), style = wx.TAB_TRAVERSAL )
		
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_3DDKSHADOW ) )
		self.SetToolTipString( u"dot1" )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.toolbarDraw = wx.ToolBar( self, wx.ID_ANY, wx.DefaultPosition, wx.Size( -1,-1 ), wx.TB_VERTICAL )
		self.toolbarDraw.SetToolBitmapSize( wx.Size( 24,24 ) )
		self.toolbarDraw.AddLabelTool( brush1, u"tool", wx.Bitmap( u"icons/dot1.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, u"dot1", wx.EmptyString ) 
		self.toolbarDraw.AddLabelTool( brush4, u"tool", wx.Bitmap( u"icons/dot4.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, u"dot4", wx.EmptyString ) 
		self.toolbarDraw.AddLabelTool( brush9, u"tool", wx.Bitmap( u"icons/dot9.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, u"dot9", wx.EmptyString ) 
		self.toolbarDraw.AddSeparator()
		self.toolbarDraw.AddLabelTool( drawdot, u"tool", wx.Bitmap( u"icons/freehand.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, u"vrij tekenen", wx.EmptyString ) 
		self.toolbarDraw.AddLabelTool( drawline, u"tool", wx.Bitmap( u"icons/line.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, u"teken vierkant", wx.EmptyString ) 
		self.toolbarDraw.AddSeparator()
		self.toolbarDraw.AddLabelTool( toolcolor, wx.EmptyString, wx.Bitmap( u"icons/rgb.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, wx.EmptyString, wx.EmptyString ) 
		self.toolbarDraw.AddLabelTool( toolblack, u"tool", wx.Bitmap( u"icons/paint-black.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, wx.EmptyString, wx.EmptyString ) 
		self.toolbarDraw.AddLabelTool( toolWhite, u"tool", wx.Bitmap( u"icons/paint-white.png", wx.BITMAP_TYPE_ANY ), wx.NullBitmap, wx.ITEM_RADIO, wx.EmptyString, wx.EmptyString ) 
		self.toolbarDraw.Realize()
		
		bSizer11.Add( self.toolbarDraw, 0, wx.EXPAND, 5 )
		
		self.panelIconCanvas = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer11.Add( self.panelIconCanvas, 7, wx.ALL|wx.EXPAND, 5 )
		
		self.SetSizer( bSizer11 )
		self.Layout()
		
		# Connect Events
		self.Bind( wx.EVT_TOOL, self.toolBrush1OnToolClicked, id = brush1 )
		self.Bind( wx.EVT_TOOL, self.toolBrush4OnToolClicked, id = brush4 )
		self.Bind( wx.EVT_TOOL, self.toolBrush9OnToolClicked, id = brush9 )
		self.Bind( wx.EVT_TOOL, self.toolDrawDotOnToolClicked, id = drawdot )
		self.Bind( wx.EVT_TOOL, self.toolDrawLineOnToolClicked, id = drawline )
		self.Bind( wx.EVT_TOOL, self.toolColorOnToolClicked, id = toolcolor )
		self.Bind( wx.EVT_TOOL, self.toolColorBlackOnToolClicked, id = toolblack )
		self.Bind( wx.EVT_TOOL, self.toolColorWhiteOnToolClicked, id = toolWhite )
		self.panelIconCanvas.Bind( wx.EVT_LEFT_DOWN, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_LEFT_UP, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_MIDDLE_DOWN, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_MIDDLE_UP, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_RIGHT_DOWN, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_RIGHT_UP, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_MOTION, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_LEFT_DCLICK, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_MIDDLE_DCLICK, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_RIGHT_DCLICK, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_LEAVE_WINDOW, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_ENTER_WINDOW, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_MOUSEWHEEL, self.panelIconCanvasOnMouseEvents )
		self.panelIconCanvas.Bind( wx.EVT_PAINT, self.panelIconCanvasOnPaint )
		self.panelIconCanvas.Bind( wx.EVT_SIZE, self.panelIconCanvasOnSize )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def toolBrush1OnToolClicked( self, event ):
		event.Skip()
	
	def toolBrush4OnToolClicked( self, event ):
		event.Skip()
	
	def toolBrush9OnToolClicked( self, event ):
		event.Skip()
	
	def toolDrawDotOnToolClicked( self, event ):
		event.Skip()
	
	def toolDrawLineOnToolClicked( self, event ):
		event.Skip()
	
	def toolColorOnToolClicked( self, event ):
		event.Skip()
	
	def toolColorBlackOnToolClicked( self, event ):
		event.Skip()
	
	def toolColorWhiteOnToolClicked( self, event ):
		event.Skip()
	
	def panelIconCanvasOnMouseEvents( self, event ):
		event.Skip()
	
	def panelIconCanvasOnPaint( self, event ):
		event.Skip()
	
	def panelIconCanvasOnSize( self, event ):
		event.Skip()
	

