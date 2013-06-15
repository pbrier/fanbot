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
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( 873,470 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 700,470 ), wx.DefaultSize )
		
		bSizer2 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelMain = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer31 = wx.BoxSizer( wx.VERTICAL )
		
		self.notebook = wx.Notebook( self.panelMain, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.NB_BOTTOM )
		self.tabPanelProxy = wx.Panel( self.notebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer27 = wx.BoxSizer( wx.VERTICAL )
		
		self.tabPanelProxy.SetSizer( bSizer27 )
		self.tabPanelProxy.Layout()
		bSizer27.Fit( self.tabPanelProxy )
		self.notebook.AddPage( self.tabPanelProxy, u"proxy", False )
		self.tabPanelDesign = wx.Panel( self.notebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer4 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.tabPanelDesign.SetSizer( bSizer4 )
		self.tabPanelDesign.Layout()
		bSizer4.Fit( self.tabPanelDesign )
		self.notebook.AddPage( self.tabPanelDesign, u"tekenen", False )
		self.tabPanelExec = wx.Panel( self.notebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.tabPanelExec.SetSizer( bSizer12 )
		self.tabPanelExec.Layout()
		bSizer12.Fit( self.tabPanelExec )
		self.notebook.AddPage( self.tabPanelExec, u"uitvoeren", False )
		self.tabPanelModules = wx.Panel( self.notebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer22 = wx.BoxSizer( wx.VERTICAL )
		
		self.tabPanelModules.SetSizer( bSizer22 )
		self.tabPanelModules.Layout()
		bSizer22.Fit( self.tabPanelModules )
		self.notebook.AddPage( self.tabPanelModules, u"modules", False )
		self.tabPanelSettings = wx.Panel( self.notebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		gSizer1 = wx.GridSizer( 4, 4, 0, 0 )
		
		self.selectSerialPort = wx.CheckBox( self.tabPanelSettings, wx.ID_ANY, u"fanbotnet", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.selectSerialPort.SetValue(True) 
		gSizer1.Add( self.selectSerialPort, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		comboSerialPortsChoices = []
		self.comboSerialPorts = wx.ComboBox( self.tabPanelSettings, wx.ID_ANY, u"Combo!", wx.DefaultPosition, wx.DefaultSize, comboSerialPortsChoices, wx.CB_DROPDOWN )
		self.comboSerialPorts.SetMinSize( wx.Size( 200,20 ) )
		
		gSizer1.Add( self.comboSerialPorts, 1, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticText4 = wx.StaticText( self.tabPanelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		gSizer1.Add( self.m_staticText4, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.buttonConnectReal = wx.Button( self.tabPanelSettings, wx.ID_ANY, u"connect", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.buttonConnectReal, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.selectSimulation = wx.CheckBox( self.tabPanelSettings, wx.ID_ANY, u"simulation", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.selectSimulation, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.textSimulation = wx.TextCtrl( self.tabPanelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.textSimulation.SetMinSize( wx.Size( 200,30 ) )
		
		gSizer1.Add( self.textSimulation, 1, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 5 )
		
		self.textPort = wx.TextCtrl( self.tabPanelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.textPort.SetMinSize( wx.Size( -1,30 ) )
		
		gSizer1.Add( self.textPort, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.buttonConnectSimul = wx.Button( self.tabPanelSettings, wx.ID_ANY, u"connect", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.buttonConnectSimul, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticText8 = wx.StaticText( self.tabPanelSettings, wx.ID_ANY, u"proxy port", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		gSizer1.Add( self.m_staticText8, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.textProxyPort = wx.TextCtrl( self.tabPanelSettings, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer1.Add( self.textProxyPort, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticText9 = wx.StaticText( self.tabPanelSettings, wx.ID_ANY, u"MyLabel", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText9.Wrap( -1 )
		gSizer1.Add( self.m_staticText9, 0, wx.ALL, 5 )
		
		self.buttonSave = wx.Button( self.tabPanelSettings, wx.ID_ANY, u"Save", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.buttonSave.SetToolTipString( u"Save configuratie naar disk" )
		
		gSizer1.Add( self.buttonSave, 1, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.tabPanelSettings.SetSizer( gSizer1 )
		self.tabPanelSettings.Layout()
		gSizer1.Fit( self.tabPanelSettings )
		self.notebook.AddPage( self.tabPanelSettings, u"configuratie", True )
		
		bSizer31.Add( self.notebook, 1, wx.EXPAND |wx.ALL, 5 )
		
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
		self.notebook.Bind( wx.EVT_NOTEBOOK_PAGE_CHANGED, self.notebookOnNotebookPageChanged )
		self.selectSerialPort.Bind( wx.EVT_CHECKBOX, self.selectSerialPortOnCheckBox )
		self.buttonConnectReal.Bind( wx.EVT_BUTTON, self.buttonConnectRealOnButtonClick )
		self.selectSimulation.Bind( wx.EVT_CHECKBOX, self.selectSimulationOnCheckBox )
		self.buttonConnectSimul.Bind( wx.EVT_BUTTON, self.buttonConnectSimulOnButtonClick )
		self.textProxyPort.Bind( wx.EVT_LEAVE_WINDOW, self.textProxyPortOnLeaveWindow )
		self.buttonSave.Bind( wx.EVT_BUTTON, self.buttonSaveOnButtonClick )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def notebookOnNotebookPageChanged( self, event ):
		event.Skip()
	
	def selectSerialPortOnCheckBox( self, event ):
		event.Skip()
	
	def buttonConnectRealOnButtonClick( self, event ):
		event.Skip()
	
	def selectSimulationOnCheckBox( self, event ):
		event.Skip()
	
	def buttonConnectSimulOnButtonClick( self, event ):
		event.Skip()
	
	def textProxyPortOnLeaveWindow( self, event ):
		event.Skip()
	
	def buttonSaveOnButtonClick( self, event ):
		event.Skip()
	

###########################################################################
## Class PanelDrawIcon
###########################################################################

class PanelDrawIcon ( wx.Panel ):
	
	def __init__( self, parent ):
		wx.Panel.__init__ ( self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition, size = wx.Size( 526,300 ), style = wx.TAB_TRAVERSAL )
		
		self.SetToolTipString( u"dot1" )
		
		bSizer22 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer19 = wx.BoxSizer( wx.VERTICAL )
		
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
		
		bSizer19.Add( bSizer11, 1, wx.EXPAND, 5 )
		
		bSizer7 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.sliderAnimate = wx.Slider( self, wx.ID_ANY, 0, 0, 9, wx.DefaultPosition, wx.DefaultSize, wx.SL_HORIZONTAL )
		self.sliderAnimate.SetToolTipString( u"Animate" )
		
		bSizer7.Add( self.sliderAnimate, 1, wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer7.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.buttonDeleteFrame = wx.Button( self, wx.ID_ANY, u"-", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonDeleteFrame.SetToolTipString( u"verwijder frame" )
		
		bSizer7.Add( self.buttonDeleteFrame, 0, wx.ALL, 5 )
		
		self.buttonAddFrame = wx.Button( self, wx.ID_ANY, u"+", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonAddFrame.SetToolTipString( u"copy frame" )
		
		bSizer7.Add( self.buttonAddFrame, 0, wx.ALL, 5 )
		
		self.buttonPrev = wx.Button( self, wx.ID_ANY, u"<<", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonPrev.SetToolTipString( u"vorig frame" )
		
		bSizer7.Add( self.buttonPrev, 0, wx.ALL, 5 )
		
		self.buttonNext = wx.Button( self, wx.ID_ANY, u">>", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		self.buttonNext.SetToolTipString( u"volgende frame" )
		
		bSizer7.Add( self.buttonNext, 0, wx.ALL, 5 )
		
		self.labelFrame = wx.StaticText( self, wx.ID_ANY, u"0", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.labelFrame.Wrap( -1 )
		self.labelFrame.SetToolTipString( u"frame/nrframes" )
		self.labelFrame.SetMinSize( wx.Size( 50,-1 ) )
		
		bSizer7.Add( self.labelFrame, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		bSizer19.Add( bSizer7, 0, wx.EXPAND, 5 )
		
		bSizer22.Add( bSizer19, 7, wx.EXPAND, 5 )
		
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer9.SetMinSize( wx.Size( 200,-1 ) ) 
		self.m_scrolledWindow1 = wx.ScrolledWindow( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.VSCROLL )
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
		
		self.m_panel10 = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
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
		
		bSizer22.Add( bSizer9, 3, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer22 )
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
		self.sliderAnimate.Bind( wx.EVT_SCROLL, self.sliderAnimateOnScroll )
		self.buttonDeleteFrame.Bind( wx.EVT_BUTTON, self.buttonDeleteFrameOnButtonClick )
		self.buttonAddFrame.Bind( wx.EVT_BUTTON, self.buttonAddFrameOnButtonClick )
		self.buttonPrev.Bind( wx.EVT_BUTTON, self.buttonPrevOnButtonClick )
		self.buttonNext.Bind( wx.EVT_BUTTON, self.buttonNextOnButtonClick )
		self.listIcons.Bind( wx.EVT_LISTBOX, self.listIconsOnListBox )
		self.buttonDelete.Bind( wx.EVT_BUTTON, self.buttonDeleteOnButtonClick )
		self.buttonAdd.Bind( wx.EVT_BUTTON, self.buttonAddOnButtonClick )
		self.buttonSave.Bind( wx.EVT_BUTTON, self.buttonSaveOnButtonClick )
	
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
	

###########################################################################
## Class PanelExecute
###########################################################################

class PanelExecute ( wx.Panel ):
	
	def __init__( self, parent ):
		wx.Panel.__init__ ( self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition, size = wx.Size( 512,409 ), style = wx.TAB_TRAVERSAL )
		
		self.SetToolTipString( u"dot1" )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer16 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelCanvas = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.Size( -1,-1 ), wx.TAB_TRAVERSAL )
		bSizer16.Add( self.panelCanvas, 1, wx.EXPAND |wx.ALL, 5 )
		
		self.panelExecCommand = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.Size( 400,300 ), wx.TAB_TRAVERSAL )
		bSizer17 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_panel15 = wx.Panel( self.panelExecCommand, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer18 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.sliderSpeed = wx.Slider( self.m_panel15, wx.ID_ANY, 0, 0, 9, wx.DefaultPosition, wx.DefaultSize, wx.SL_HORIZONTAL|wx.SL_LABELS )
		bSizer18.Add( self.sliderSpeed, 2, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		bSizer18.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.checkRepeat = wx.CheckBox( self.m_panel15, wx.ID_ANY, u"Herhaal", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.checkRepeat.SetValue(True) 
		bSizer18.Add( self.checkRepeat, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_panel15.SetSizer( bSizer18 )
		self.m_panel15.Layout()
		bSizer18.Fit( self.m_panel15 )
		bSizer17.Add( self.m_panel15, 3, wx.ALL|wx.EXPAND, 5 )
		
		self.panelExecCommand.SetSizer( bSizer17 )
		self.panelExecCommand.Layout()
		bSizer16.Add( self.panelExecCommand, 0, wx.EXPAND |wx.ALL, 5 )
		
		bSizer11.Add( bSizer16, 7, wx.EXPAND, 5 )
		
		self.m_scrolledWindow2 = wx.ScrolledWindow( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.VSCROLL )
		self.m_scrolledWindow2.SetScrollRate( 5, 5 )
		bSizer19 = wx.BoxSizer( wx.VERTICAL )
		
		listExecChoices = []
		self.listExec = wx.ListBox( self.m_scrolledWindow2, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, listExecChoices, 0 )
		bSizer19.Add( self.listExec, 3, wx.ALL|wx.EXPAND, 5 )
		
		bSizer181 = wx.BoxSizer( wx.VERTICAL )
		
		radioFileTypeChoices = [ u"animations", u"banners" ]
		self.radioFileType = wx.RadioBox( self.m_scrolledWindow2, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, radioFileTypeChoices, 1, wx.RA_SPECIFY_ROWS )
		self.radioFileType.SetSelection( 0 )
		bSizer181.Add( self.radioFileType, 1, wx.ALL, 5 )
		
		bSizer19.Add( bSizer181, 0, wx.EXPAND, 5 )
		
		self.m_scrolledWindow2.SetSizer( bSizer19 )
		self.m_scrolledWindow2.Layout()
		bSizer19.Fit( self.m_scrolledWindow2 )
		bSizer11.Add( self.m_scrolledWindow2, 3, wx.EXPAND |wx.ALL, 5 )
		
		self.SetSizer( bSizer11 )
		self.Layout()
		
		# Connect Events
		self.panelCanvas.Bind( wx.EVT_PAINT, self.panelCanvasOnPaint )
		self.panelCanvas.Bind( wx.EVT_SIZE, self.panelCanvasOnSize )
		self.sliderSpeed.Bind( wx.EVT_SCROLL, self.sliderSpeedOnScroll )
		self.listExec.Bind( wx.EVT_LISTBOX, self.listExecOnListBox )
		self.radioFileType.Bind( wx.EVT_RADIOBOX, self.radioFileTypeOnRadioBox )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def panelCanvasOnPaint( self, event ):
		event.Skip()
	
	def panelCanvasOnSize( self, event ):
		event.Skip()
	
	def sliderSpeedOnScroll( self, event ):
		event.Skip()
	
	def listExecOnListBox( self, event ):
		event.Skip()
	
	def radioFileTypeOnRadioBox( self, event ):
		event.Skip()
	

###########################################################################
## Class PanelProxy
###########################################################################

class PanelProxy ( wx.Panel ):
	
	def __init__( self, parent ):
		wx.Panel.__init__ ( self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition, size = wx.Size( 500,300 ), style = wx.TAB_TRAVERSAL )
		
		bSizer25 = wx.BoxSizer( wx.VERTICAL )
		
		self.panelProxyCanvas = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer25.Add( self.panelProxyCanvas, 1, wx.EXPAND |wx.ALL, 5 )
		
		self.m_panel17 = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer26 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.buttonProxyStart = wx.Button( self.m_panel17, wx.ID_ANY, u"Start", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer26.Add( self.buttonProxyStart, 0, wx.ALL, 5 )
		
		self.m_staticText4 = wx.StaticText( self.m_panel17, wx.ID_ANY, u"proxy host:port", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		bSizer26.Add( self.m_staticText4, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.labelProxyHost = wx.StaticText( self.m_panel17, wx.ID_ANY, u"127.0.0.1 : 1234", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.labelProxyHost.Wrap( -1 )
		bSizer26.Add( self.labelProxyHost, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		bSizer26.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_staticText6 = wx.StaticText( self.m_panel17, wx.ID_ANY, u"connections:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		bSizer26.Add( self.m_staticText6, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.labelConnectionNr = wx.StaticText( self.m_panel17, wx.ID_ANY, u"0", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.labelConnectionNr.Wrap( -1 )
		bSizer26.Add( self.labelConnectionNr, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_panel17.SetSizer( bSizer26 )
		self.m_panel17.Layout()
		bSizer26.Fit( self.m_panel17 )
		bSizer25.Add( self.m_panel17, 0, wx.EXPAND |wx.ALL, 5 )
		
		self.SetSizer( bSizer25 )
		self.Layout()
		
		# Connect Events
		self.panelProxyCanvas.Bind( wx.EVT_PAINT, self.panelProxyCanvasOnPaint )
		self.panelProxyCanvas.Bind( wx.EVT_SIZE, self.panelProxyCanvasOnSize )
		self.buttonProxyStart.Bind( wx.EVT_BUTTON, self.buttonProxyStartOnButtonClick )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def panelProxyCanvasOnPaint( self, event ):
		event.Skip()
	
	def panelProxyCanvasOnSize( self, event ):
		event.Skip()
	
	def buttonProxyStartOnButtonClick( self, event ):
		event.Skip()
	

###########################################################################
## Class PanelModules
###########################################################################

class PanelModules ( wx.Panel ):
	
	def __init__( self, parent ):
		wx.Panel.__init__ ( self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition, size = wx.Size( 547,326 ), style = wx.TAB_TRAVERSAL )
		
		bSizer23 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer25 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer27 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.panelModulesCanvas = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.panelModulesCanvas.SetMinSize( wx.Size( 200,-1 ) )
		
		bSizer27.Add( self.panelModulesCanvas, 5, wx.EXPAND |wx.ALL, 5 )
		
		bSizer25.Add( bSizer27, 1, wx.EXPAND, 5 )
		
		self.m_panel16 = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer26 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.buttonResetDisc = wx.Button( self.m_panel16, wx.ID_ANY, u"reset discovery", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer26.Add( self.buttonResetDisc, 0, wx.ALL, 5 )
		
		self.buttonDiscover = wx.Button( self.m_panel16, wx.ID_ANY, u"discover", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer26.Add( self.buttonDiscover, 0, wx.ALL, 5 )
		
		
		bSizer26.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		
		bSizer26.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.checkBoxMatrix = wx.CheckBox( self.m_panel16, wx.ID_ANY, u"Matrix 6x4", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer26.Add( self.checkBoxMatrix, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.labelPosition = wx.StaticText( self.m_panel16, wx.ID_ANY, u"0:0", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.labelPosition.Wrap( -1 )
		bSizer26.Add( self.labelPosition, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_panel16.SetSizer( bSizer26 )
		self.m_panel16.Layout()
		bSizer26.Fit( self.m_panel16 )
		bSizer25.Add( self.m_panel16, 0, wx.EXPAND |wx.ALL, 5 )
		
		bSizer23.Add( bSizer25, 3, wx.EXPAND, 5 )
		
		bSizer27 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.AddGrowableCol( 0 )
		fgSizer1.AddGrowableRow( 1 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText11 = wx.StaticText( self, wx.ID_ANY, u"Hubs", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText11.Wrap( -1 )
		fgSizer1.Add( self.m_staticText11, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.labelHubId = wx.StaticText( self, wx.ID_ANY, u"Fanbots", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.labelHubId.Wrap( -1 )
		fgSizer1.Add( self.labelHubId, 1, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.scrolledWindowHubs = wx.ScrolledWindow( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.VSCROLL )
		self.scrolledWindowHubs.SetScrollRate( 5, 5 )
		self.scrolledWindowHubs.SetMinSize( wx.Size( 150,-1 ) )
		
		fgSizer1.Add( self.scrolledWindowHubs, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 2 )
		
		self.scrolledWindowFanbots = wx.ScrolledWindow( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.VSCROLL )
		self.scrolledWindowFanbots.SetScrollRate( 5, 5 )
		self.scrolledWindowFanbots.SetMinSize( wx.Size( 100,-1 ) )
		
		fgSizer1.Add( self.scrolledWindowFanbots, 1, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 2 )
		
		bSizer27.Add( fgSizer1, 1, wx.EXPAND, 1 )
		
		self.m_panel17 = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.ALWAYS_SHOW_SB|wx.TAB_TRAVERSAL )
		bSizer30 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.buttonResetConfig = wx.Button( self.m_panel17, wx.ID_ANY, u"Reset Config", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.buttonResetConfig.SetHelpText( u"Reset configuratie geselcteerde hub" )
		
		bSizer30.Add( self.buttonResetConfig, 0, wx.ALL, 5 )
		
		self.buttonSetConfig = wx.Button( self.m_panel17, wx.ID_ANY, u"Set Config", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.buttonSetConfig.SetHelpText( u"Set config geselecteerde hub" )
		
		bSizer30.Add( self.buttonSetConfig, 0, wx.ALL, 5 )
		
		self.buttonSetConfigAll = wx.Button( self.m_panel17, wx.ID_ANY, u"Set All", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.buttonSetConfigAll.SetToolTipString( u"Set config voor alle hubs" )
		
		bSizer30.Add( self.buttonSetConfigAll, 0, wx.ALL, 5 )
		
		self.m_panel17.SetSizer( bSizer30 )
		self.m_panel17.Layout()
		bSizer30.Fit( self.m_panel17 )
		bSizer27.Add( self.m_panel17, 0, wx.EXPAND |wx.ALL, 5 )
		
		bSizer23.Add( bSizer27, 2, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer23 )
		self.Layout()
		
		# Connect Events
		self.panelModulesCanvas.Bind( wx.EVT_LEFT_DOWN, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_LEFT_UP, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_MIDDLE_DOWN, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_MIDDLE_UP, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_RIGHT_DOWN, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_RIGHT_UP, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_MOTION, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_LEFT_DCLICK, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_MIDDLE_DCLICK, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_RIGHT_DCLICK, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_LEAVE_WINDOW, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_ENTER_WINDOW, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_MOUSEWHEEL, self.panelModulesCanvasOnMouseEvents )
		self.panelModulesCanvas.Bind( wx.EVT_PAINT, self.panelModulesCanvasOnPaint )
		self.panelModulesCanvas.Bind( wx.EVT_SIZE, self.panelModulesCanvasOnSize )
		self.buttonResetDisc.Bind( wx.EVT_BUTTON, self.buttonResetDiscOnButtonClick )
		self.buttonDiscover.Bind( wx.EVT_BUTTON, self.buttonDiscoverOnButtonClick )
		self.buttonResetConfig.Bind( wx.EVT_BUTTON, self.buttonResetConfigOnButtonClick )
		self.buttonSetConfig.Bind( wx.EVT_BUTTON, self.buttonSetConfigOnButtonClick )
		self.buttonSetConfigAll.Bind( wx.EVT_BUTTON, self.buttonSetConfigAllOnButtonClick )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def panelModulesCanvasOnMouseEvents( self, event ):
		event.Skip()
	
	def panelModulesCanvasOnPaint( self, event ):
		event.Skip()
	
	def panelModulesCanvasOnSize( self, event ):
		event.Skip()
	
	def buttonResetDiscOnButtonClick( self, event ):
		event.Skip()
	
	def buttonDiscoverOnButtonClick( self, event ):
		event.Skip()
	
	def buttonResetConfigOnButtonClick( self, event ):
		event.Skip()
	
	def buttonSetConfigOnButtonClick( self, event ):
		event.Skip()
	
	def buttonSetConfigAllOnButtonClick( self, event ):
		event.Skip()
	

