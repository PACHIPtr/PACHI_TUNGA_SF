import net
import app
import ui
import dbg
import os
import time
import wndMgr

isLoaded = 0

class SvsideDialog(ui.Window):
	def __init__(self):
		ui.Window.__init__(self)
		self.isWorking = 0
		self.BuildWindow()

	def __del__(self):
		ui.Window.__del__(self)
		
	def BuildWindow(self):
		global isLoaded
		if isLoaded == 1:
			return
		isLoaded = 1
		self.verifyType = 1
		self.Board = ui.Board()
		self.Board.SetSize(155, 196)
		self.Board.SetCenterPosition()
		self.Board.SetPosition(wndMgr.GetScreenWidth()-325,(wndMgr.GetScreenHeight()-195)/2)
		#self.Board.AddFlag('movable')
		self.Board.AddFlag('float')
		self.Board.Show()
		self.__BuildKeyDict()
		self.comp = Component()

		self.img1 = self.comp.ExpandedImage(self.Board , 12, 12, 'lib/empty.jpg')
		self.imgo1 = self.comp.ExpandedImage(self.Board , 12, 12, 'lib/0.jpg')
		self.imgo2 = self.comp.ExpandedImage(self.Board , 32, 12, 'lib/0.jpg')
		self.imgo3 = self.comp.ExpandedImage(self.Board , 52, 12, 'lib/0.jpg')
		self.imgo4 = self.comp.ExpandedImage(self.Board , 72, 12, 'lib/0.jpg')
		self.nm1 = self.comp.Button(self.Board, '1', '', 10, 50, self.nm1_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm2 = self.comp.Button(self.Board, '2', '', 56, 50, self.nm2_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm3 = self.comp.Button(self.Board, '3', '', 102, 50, self.nm3_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm4 = self.comp.Button(self.Board, '4', '', 10, 74, self.nm4_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm5 = self.comp.Button(self.Board, '5', '', 56, 74, self.nm5_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm6 = self.comp.Button(self.Board, '6', '', 102, 74, self.nm6_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm7 = self.comp.Button(self.Board, '7', '', 10, 98, self.nm7_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm8 = self.comp.Button(self.Board, '8', '', 56, 98, self.nm8_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm9 = self.comp.Button(self.Board, '9', '', 102, 98, self.nm9_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nm0 = self.comp.Button(self.Board, '0', '', 56, 122, self.nm0_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nmdelete = self.comp.Button(self.Board, 'Sil', '', 102, 122, self.nmdelete_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.nmok = self.comp.Button(self.Board, 'Tamam', '', 33, 149, self.nmok_func, 'd:/ymir work/ui/public/large_button_01.sub', 'd:/ymir work/ui/public/large_button_02.sub', 'd:/ymir work/ui/public/large_button_03.sub')
		self.nmclear = self.comp.Button(self.Board, 'Temizle', '', 10, 122, self.nmclear_func, 'd:/ymir work/ui/public/small_button_01.sub', 'd:/ymir work/ui/public/small_button_02.sub', 'd:/ymir work/ui/public/small_button_03.sub')
		self.slotbar_nmcode, self.nmcode = self.comp.EditLine(self.Board, '', 102, 18, 37, 15, 4)
		self.svstxt = self.comp.TextLine(self.Board, '<svside> Bot Kontrol', 30, 173, self.comp.RGB(255,215,0))
	
	def nm1_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "1")
		pass
	
	def nm2_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "2")
		pass
	
	def nm3_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "3")
		pass
	
	def nm4_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "4")
		pass
	
	def nm5_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "5")
		pass
	
	def nm6_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "6")
		pass
	
	def nm7_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "7")
		pass
	
	def nm8_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "8")
		pass
	
	def nm9_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "9")
		pass
	
	def nm0_func(self):
		if len(self.nmcode.GetText()) < 4:
			self.nmcode.SetText(self.nmcode.GetText() + "0")
		pass
	
	def nmdelete_func(self):
		if len(self.nmcode.GetText()) > 0:
			self.nmcode.SetText(self.nmcode.GetText()[:-1])
		pass
	
	def nmclear_func(self):
		self.nmcode.SetText("")
			
	def nm_updateimgoffline(self, randcode):
		self.verifyType = 1
		self.Board.Show()
		self.img1.Hide()
		self.imgo1.Show()
		self.imgo2.Show()
		self.imgo3.Show()
		self.imgo4.Show()
		filepth = 'lib/'
		self.imgo1.LoadImage(filepth + randcode[0:1] + '.jpg')
		self.imgo2.LoadImage(filepth + randcode[1:2] + '.jpg')
		self.imgo3.LoadImage(filepth + randcode[2:3] + '.jpg')
		self.imgo4.LoadImage(filepth + randcode[3:4] + '.jpg')

	def nm_updateimgoffline2(self, randcode):
		self.verifyType = 2
		self.Board.Show()
		self.img1.Show()
		self.imgo1.Hide()
		self.imgo2.Hide()
		self.imgo3.Hide()
		self.imgo4.Hide()
		filepth = 'lib/' + randcode
		self.img1.LoadImage(filepth)
		
	def nmok_func(self):
		if self.verifyType == 2:
			net.SendChatPacket(" #ebvs:" + self.nmcode.GetText(), 0)
		else:
			net.SendWhisperPacket("<svside>", self.nmcode.GetText())
		self.nmclear_func()

	def __BuildKeyDict(self):
		onPressKeyDict = {}
		onPressKeyDict[app.DIK_F5]	= lambda : self.OpenWindow()
		self.onPressKeyDict = onPressKeyDict
	
	def OnKeyDown(self, key):
		try:
			self.onPressKeyDict[key]()
		except KeyError:
			pass
		except:
			raise
		return True
	
	def OpenWindow(self):
		if self.Board.IsShow():
			self.Board.Hide()
		else:
			self.Board.Show()
	
	def Close(self):
		self.Board.Hide()

class Component:
	def Button(self, parent, buttonName, tooltipText, x, y, func, UpVisual, OverVisual, DownVisual):
		button = ui.Button()
		if parent != None:
			button.SetParent(parent)
		button.SetPosition(x, y)
		button.SetUpVisual(UpVisual)
		button.SetOverVisual(OverVisual)
		button.SetDownVisual(DownVisual)
		button.SetText(buttonName)
		button.SetToolTipText(tooltipText)
		button.Show()
		button.SetEvent(func)
		return button

	def ToggleButton(self, parent, buttonName, tooltipText, x, y, funcUp, funcDown, UpVisual, OverVisual, DownVisual):
		button = ui.ToggleButton()
		if parent != None:
			button.SetParent(parent)
		button.SetPosition(x, y)
		button.SetUpVisual(UpVisual)
		button.SetOverVisual(OverVisual)
		button.SetDownVisual(DownVisual)
		button.SetText(buttonName)
		button.SetToolTipText(tooltipText)
		button.Show()
		button.SetToggleUpEvent(funcUp)
		button.SetToggleDownEvent(funcDown)
		return button

	def EditLine(self, parent, editlineText, x, y, width, heigh, max):
		SlotBar = ui.SlotBar()
		if parent != None:
			SlotBar.SetParent(parent)
		SlotBar.SetSize(width, heigh)
		SlotBar.SetPosition(x, y)
		SlotBar.Show()
		Value = ui.EditLine()
		Value.SetParent(SlotBar)
		Value.SetSize(width, heigh)
		Value.SetPosition(1, 1)
		Value.SetMax(max)
		Value.SetLimitWidth(width)
		Value.SetMultiLine()
		Value.SetText(editlineText)
		Value.Show()
		return SlotBar, Value

	def TextLine(self, parent, textlineText, x, y, color):
		textline = ui.TextLine()
		if parent != None:
			textline.SetParent(parent)
		textline.SetPosition(x, y)
		if color != None:
			textline.SetFontColor(color[0], color[1], color[2])
		textline.SetText(textlineText)
		textline.Show()
		return textline

	def RGB(self, r, g, b):
		return (r*255, g*255, b*255)

	def SliderBar(self, parent, sliderPos, func, x, y):
		Slider = ui.SliderBar()
		if parent != None:
			Slider.SetParent(parent)
		Slider.SetPosition(x, y)
		Slider.SetSliderPos(sliderPos / 100)
		Slider.Show()
		Slider.SetEvent(func)
		return Slider

	def ExpandedImage(self, parent, x, y, img):
		image = ui.ExpandedImageBox()
		if parent != None:
			image.SetParent(parent)
		image.SetPosition(x, y)
		image.LoadImage(img)
		image.Show()
		return image

	def ComboBox(self, parent, text, x, y, width):
		combo = ui.ComboBox()
		if parent != None:
			combo.SetParent(parent)
		combo.SetPosition(x, y)
		combo.SetSize(width, 15)
		combo.SetCurrentItem(text)
		combo.Show()
		return combo

	def ThinBoard(self, parent, moveable, x, y, width, heigh, center):
		thin = ui.ThinBoard()
		if parent != None:
			thin.SetParent(parent)
		if moveable == True:
			thin.AddFlag('movable')
			thin.AddFlag('float')
		thin.SetSize(width, heigh)
		thin.SetPosition(x, y)
		if center == True:
			thin.SetCenterPosition()
		thin.Show()
		return thin

	def Gauge(self, parent, width, color, x, y):
		gauge = ui.Gauge()
		if parent != None:
			gauge.SetParent(parent)
		gauge.SetPosition(x, y)
		gauge.MakeGauge(width, color)
		gauge.Show()
		return gauge

	def ListBoxEx(self, parent, x, y, width, heigh):
		bar = ui.Bar()
		if parent != None:
			bar.SetParent(parent)
		bar.SetPosition(x, y)
		bar.SetSize(width, heigh)
		bar.SetColor(0x77000000)
		bar.Show()
		ListBox=ui.ListBoxEx()
		ListBox.SetParent(bar)
		ListBox.SetPosition(0, 0)
		ListBox.SetSize(width, heigh)
		ListBox.Show()
		scroll = ui.ScrollBar()
		scroll.SetParent(ListBox)
		scroll.SetPosition(width-15, 0)
		scroll.SetScrollBarSize(heigh)
		scroll.Show()
		ListBox.SetScrollBar(scroll)
		return bar, ListBox

#SvsideDialog().Hide()
#SvsideDialog().Show()
