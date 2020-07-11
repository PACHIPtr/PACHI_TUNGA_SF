import app
import ui
import grp
import wndMgr
import time

class TextBar(ui.Window):
	def __init__(self, width, height):
		ui.Window.__init__(self)
		self.handle = grp.CreateTextBar(width, height)

	def __del__(self):
		ui.Window.__del__(self)
		grp.DestroyTextBar(self.handle)

	def ClearBar(self):
		grp.ClearTextBar(self.handle)

	def SetClipRect(self, x1, y1, x2, y2):
		grp.SetTextBarClipRect(self.handle, x1, y1, x2, y2)

	def TextOut(self, x, y, text):
		grp.TextBarTextOut(self.handle, x, y, text)

	def OnRender(self):
		x, y = self.GetGlobalPosition()
		grp.RenderTextBar(self.handle, x, y)

	def SetTextColor(self, r, g, b):
		grp.TextBarSetTextColor(self.handle, r, g, b)

	def GetTextExtent(self, text):
		return grp.TextBarGetTextExtent(self.handle, text)

class TipBoard(ui.Bar):

	SCROLL_WAIT_TIME = 3.0
	TIP_DURATION = 5.0
	STEP_HEIGHT = 17

	def __init__(self):
		ui.Bar.__init__(self)

		self.AddFlag("not_pick")
		self.tipList = []
		self.curPos = 0
		self.dstPos = 0
		self.nextScrollTime = 0

		if app.ENABLE_OX_RENEWAL:
			self.width = 460
		else:
			self.width = 370		

		self.SetPosition(0, 70)
		if app.ENABLE_OX_RENEWAL:
			self.SetSize(self.width, 20)
		else:
			self.SetSize(370, 20)
		self.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.5))
		self.SetWindowHorizontalAlignCenter()

		self.__CreateTextBar()

	def __del__(self):
		ui.Bar.__del__(self)

	def __CreateTextBar(self):

		x, y = self.GetGlobalPosition()

		if app.ENABLE_OX_RENEWAL:
			self.textBar = TextBar(self.width, 300)
		else:
			self.textBar = TextBar(370, 300)
		self.textBar.SetParent(self)
		self.textBar.SetPosition(3, 5)		
		if app.ENABLE_OX_RENEWAL:
			self.textBar.SetClipRect(5, y, wndMgr.GetScreenWidth(), y+18)
		else:
			self.textBar.SetClipRect(0, y, wndMgr.GetScreenWidth(), y+18)
		self.textBar.Show()

	def __CleanOldTip(self):
		leaveList = []
		for tip in self.tipList:
			madeTime = tip[0]
			if app.GetTime() - madeTime > self.TIP_DURATION:
				pass
			else:
				leaveList.append(tip)

		self.tipList = leaveList

		if not leaveList:
			self.textBar.ClearBar()
			self.Hide()
			return

		self.__RefreshBoard()

	def __RefreshBoard(self):

		self.textBar.ClearBar()

		index = 0
		for tip in self.tipList:
			text = tip[1]
			self.textBar.TextOut(0, index*self.STEP_HEIGHT, text)
			index += 1

	def SetTip(self, text):

		if not app.IsVisibleNotice():
			return

		curTime = app.GetTime()
		self.tipList.append((curTime, text))
		self.__RefreshBoard()

		self.nextScrollTime = app.GetTime() + 1.0

		if not self.IsShow():
			self.curPos = -self.STEP_HEIGHT
			self.dstPos = -self.STEP_HEIGHT
			self.textBar.SetPosition(3, 5 - self.curPos)
			self.Show()

	def OnUpdate(self):

		if not self.tipList:
			self.Hide()
			return

		if app.GetTime() > self.nextScrollTime:
			self.nextScrollTime = app.GetTime() + self.SCROLL_WAIT_TIME

			self.dstPos = self.curPos + self.STEP_HEIGHT

		if self.dstPos > self.curPos:
			self.curPos += 1
			self.textBar.SetPosition(3, 5 - self.curPos)

			if self.curPos > len(self.tipList)*self.STEP_HEIGHT:
				self.curPos = -self.STEP_HEIGHT
				self.dstPos = -self.STEP_HEIGHT

				self.__CleanOldTip()

if app.ENABLE_OX_RENEWAL:
	class BigBoardControl(ui.Bar):

		FONT_WIDTH	= 18
		FONT_HEIGHT	= 18
		LINE_WIDTH  = 500
		LINE_HEIGHT	= FONT_HEIGHT + 5
		STEP_HEIGHT = LINE_HEIGHT * 2
		LINE_CHANGE_LIMIT_WIDTH = 450
		BIG_TEXTBAR_MAX_HIGHT = 1000

		FRAME_IMAGE_FILE_NAME_LIST = [
			"d:/ymir work/interface/oxevent/frame_new_0.sub", 
			"d:/ymir work/interface/oxevent/frame_new_1.sub", 
		]

		FRAME_IMAGE_STEP = 256

		FRAME_BASE_X = -20
		FRAME_BASE_Y = -12

		def __init__(self):
			ui.Bar.__init__(self)

			self.AddFlag("not_pick")
			self.tipList = []
			self.curPos = 0
			self.dstPos = 0
			self.nextScrollTime = 0
			self.scrollstop = 0
			self.pretexsize = 0
			self.nexttextsize = 0
			self.changeline = 0
			self.endsayi = 0
			self.addtipListcount = 0

			self.SetPosition(0, 150)
			self.SetSize(512, 55)
			self.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.5))
			self.SetWindowHorizontalAlignCenter()

			self.__CreateTextBar()
			self.__LoadFrameImages()
			 

		def __LoadFrameImages(self):
			x = self.FRAME_BASE_X
			y = self.FRAME_BASE_Y
			self.imgList = []
			for imgFileName in self.FRAME_IMAGE_FILE_NAME_LIST:
				self.imgList.append(self.__LoadImage(x, y, imgFileName))

		def __LoadImage(self, x, y, fileName):
			img = ui.ImageBox()
			img.SetParent(self)
			img.AddFlag("not_pick")
			img.LoadImage(fileName)
			img.SetPosition(x, y)
			img.Show()
			return img
			
		def __del__(self):
			self.tipList = []
			self.textBar.ClearBar()
			self.Hide()
			ui.Bar.__del__(self)

		def __CreateTextBar(self):

			x, y = self.GetGlobalPosition()

			self.textBar = BigTextBar(self.LINE_WIDTH, self.BIG_TEXTBAR_MAX_HIGHT, self.FONT_HEIGHT)
			self.textBar.SetParent(self)
			self.textBar.SetPosition(6, 8)
			self.textBar.SetTextColor(242, 231, 193)
			self.textBar.SetClipRect(0, y+8, wndMgr.GetScreenWidth(), y+8+self.STEP_HEIGHT)
			self.textBar.Show()

		def __CleanOldTip(self):
			self.tipList = []
			self.textBar.ClearBar()
			self.Hide()

		def __RefreshBoard(self):

			self.textBar.ClearBar()
			preaddtipListcount = self.addtipListcount
			self.addtipListcount = 0

			index = 0
			for text in self.tipList:
				(text_width, text_height) = self.textBar.GetTextExtent(text)
				if index == 0:
					self.textBar.TextOut((500-text_width)/2, index*self.LINE_HEIGHT, " ")
					index += 1
					self.addtipListcount += 1
				
				if index == (len(self.tipList) - (len(self.tipList) - self.pretexsize))+1 and not self.pretexsize == 0:

					self.textBar.TextOut((500-text_width)/2, index*self.LINE_HEIGHT, " ")
					index += 1
					self.textBar.TextOut((500-text_width)/2, index*self.LINE_HEIGHT, " ")
					index += 1
					self.addtipListcount += 2

				self.textBar.TextOut((500-text_width)/2, index*self.LINE_HEIGHT, text)
				index += 1
			
			if self.tipList:
				index += 1
				self.addtipListcount += 1
				self.textBar.TextOut((500-text_width)/2, index*self.LINE_HEIGHT, " ")

		def SetTip(self, text):

			if not app.IsVisibleNotice():
				return	
			
			if text == "#start" or text == "#end" or text == "#send":
				pass
			else:
				self.__AppendText(text)

			if text == "#start" or text == "#send":
				
				if text == "#send" and self.pretexsize == 0:
					text = "#end"
				
				self.__RefreshBoard()

				if text == "#start":
					self.STEP_HEIGHT = (self.LINE_HEIGHT * (len(self.tipList) + self.addtipListcount))
					self.pretexsize = len(self.tipList)

					x, y = self.GetGlobalPosition()
					self.textBar.SetClipRect(0, y+8, wndMgr.GetScreenWidth(), y+8+self.STEP_HEIGHT)
					self.SetSize(512, 10 + (self.LINE_HEIGHT * (len(self.tipList) + self.addtipListcount))+10)
					self.imgList[1].SetPosition(self.FRAME_BASE_X, self.STEP_HEIGHT + 10)

					self.scrollstop = 0
					self.changeline = 1
					
				if text == "#send":
					self.scrollstop = 1
					self.nexttextsize = len(self.tipList) - self.pretexsize

				if not self.IsShow():
					self.curPos = -self.STEP_HEIGHT
					self.dstPos = -self.STEP_HEIGHT
					self.textBar.SetPosition(3, 8 - self.curPos)
					self.Show()

			if text == "#end":
				self.curPos = -self.STEP_HEIGHT
				self.dstPos = -self.STEP_HEIGHT
				self.textBar.SetPosition(3, 8 - self.curPos)
				self.textcount = 0
				self.changeline = 0
				self.pretexsize = 0
				self.nexttextsize = 0
				self.addtipListcount = 0
				self.__CleanOldTip()

		def __AppendText(self, text):
			import dbg		
			prevPos = 0
			while 1:
				curPos = text.find(" ", prevPos)
				if curPos < 0:
					break
				
				(text_width, text_height) = self.textBar.GetTextExtent(text[:curPos])
				if text_width > self.LINE_CHANGE_LIMIT_WIDTH:
					self.tipList.append(text[:prevPos])
					
					(text_width, text_height) = self.textBar.GetTextExtent(text[prevPos:])
					if text_width > self.LINE_CHANGE_LIMIT_WIDTH:
						text = text[prevPos:]
						if text[:prevPos] == "":
							prevPos = curPos + 1
						else:
							prevPos = 0
					else:
						text = text[prevPos:]
						break
				else:
					prevPos = curPos + 1

			self.tipList.append(text)

		def OnUpdate(self):

			if not self.tipList:
				self.Hide()
				return

			if self.changeline:
				self.changeline = 0
				self.dstPos = self.curPos + self.STEP_HEIGHT

			if self.dstPos > self.curPos:
				self.curPos += 1
				self.textBar.SetPosition(3, 8 - self.curPos)

			if self.curPos > len(self.tipList)*self.LINE_HEIGHT:
				self.curPos = -self.STEP_HEIGHT
				self.dstPos = -self.STEP_HEIGHT

				self.__CleanOldTip()

class BigTextBar(TextBar):
	def __init__(self, width, height, fontSize):
		ui.Window.__init__(self)
		self.handle = grp.CreateBigTextBar(width, height, fontSize)
		
	def __del__(self):
		ui.Window.__del__(self)
		grp.DestroyTextBar(self.handle)

class BigBoard(ui.Bar):

	SCROLL_WAIT_TIME = 5.0
	TIP_DURATION = 10.0
	FONT_WIDTH	= 18
	FONT_HEIGHT	= 18
	LINE_WIDTH  = 500
	LINE_HEIGHT	= FONT_HEIGHT + 5
	STEP_HEIGHT = LINE_HEIGHT * 2
	LINE_CHANGE_LIMIT_WIDTH = 350

	FRAME_IMAGE_FILE_NAME_LIST = [
		"d:/ymir work/interface/oxevent/frame_0.sub", 
		"d:/ymir work/interface/oxevent/frame_1.sub", 
		"d:/ymir work/interface/oxevent/frame_2.sub",
	]

	FRAME_IMAGE_STEP = 256

	FRAME_BASE_X = -20
	FRAME_BASE_Y = -12

	def __init__(self):
		ui.Bar.__init__(self)

		self.AddFlag("not_pick")
		self.tipList = []
		self.curPos = 0
		self.dstPos = 0
		self.nextScrollTime = 0

		self.SetPosition(0, 150)
		self.SetSize(512, 55)
		self.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.5))
		self.SetWindowHorizontalAlignCenter()

		self.__CreateTextBar()
		self.__LoadFrameImages()
		 

	def __LoadFrameImages(self):
		x = self.FRAME_BASE_X
		y = self.FRAME_BASE_Y
		self.imgList = []
		for imgFileName in self.FRAME_IMAGE_FILE_NAME_LIST:
			self.imgList.append(self.__LoadImage(x, y, imgFileName))
			x += self.FRAME_IMAGE_STEP

	def __LoadImage(self, x, y, fileName):
		img = ui.ImageBox()
		img.SetParent(self)
		img.AddFlag("not_pick")
		img.LoadImage(fileName)
		img.SetPosition(x, y)
		img.Show()
		return img
		
	def __del__(self):
		ui.Bar.__del__(self)

	def __CreateTextBar(self):

		x, y = self.GetGlobalPosition()

		self.textBar = BigTextBar(self.LINE_WIDTH, 300, self.FONT_HEIGHT)
		self.textBar.SetParent(self)
		self.textBar.SetPosition(6, 8)
		self.textBar.SetTextColor(242, 231, 193)
		self.textBar.SetClipRect(0, y+8, wndMgr.GetScreenWidth(), y+8+self.STEP_HEIGHT)
		self.textBar.Show()

	def __CleanOldTip(self):
		curTime = app.GetTime()
		leaveList = []
		for madeTime, text in self.tipList:
			if curTime + self.TIP_DURATION <= madeTime:				
				leaveList.append(text)

		self.tipList = leaveList

		if not leaveList:
			self.textBar.ClearBar()
			self.Hide()
			return

		self.__RefreshBoard()

	def __RefreshBoard(self):

		self.textBar.ClearBar()

		if len(self.tipList) == 1:
			checkTime, text = self.tipList[0]
			(text_width, text_height) = self.textBar.GetTextExtent(text)
			self.textBar.TextOut((500-text_width)/2, (self.STEP_HEIGHT-8-text_height)/2, text)

		else:
			index = 0
			for checkTime, text in self.tipList:			
				(text_width, text_height) = self.textBar.GetTextExtent(text)	 
				self.textBar.TextOut((500-text_width)/2, index*self.LINE_HEIGHT, text)
				index += 1

	def SetTip(self, text):

		if not app.IsVisibleNotice():
			return		
		
		curTime = app.GetTime()
		self.__AppendText(curTime, text)
		self.__RefreshBoard()

		self.nextScrollTime = curTime + 1.0

		if not self.IsShow():
			self.curPos = -self.STEP_HEIGHT
			self.dstPos = -self.STEP_HEIGHT
			self.textBar.SetPosition(3, 8 - self.curPos)
			self.Show()

	def __AppendText(self, curTime, text):		
		import dbg		
		prevPos = 0
		while 1:
			curPos = text.find(" ", prevPos)
			if curPos < 0:
				break
			
			(text_width, text_height) = self.textBar.GetTextExtent(text[:curPos])
			if text_width > self.LINE_CHANGE_LIMIT_WIDTH:
				self.tipList.append((curTime, text[:prevPos]))
				self.tipList.append((curTime, text[prevPos:]))
				return

			prevPos = curPos + 1

		self.tipList.append((curTime, text))

	def OnUpdate(self):

		if not self.tipList:
			self.Hide()
			return

		if app.GetTime() > self.nextScrollTime:
			self.nextScrollTime = app.GetTime() + self.SCROLL_WAIT_TIME

			self.dstPos = self.curPos + self.STEP_HEIGHT

		if self.dstPos > self.curPos:
			self.curPos += 1
			self.textBar.SetPosition(3, 8 - self.curPos)

			if self.curPos > len(self.tipList)*self.LINE_HEIGHT:
				self.curPos = -self.STEP_HEIGHT
				self.dstPos = -self.STEP_HEIGHT

				self.__CleanOldTip()
				
class MissionBoard(ui.Bar):
	FONT_HEIGHT	= 15
	LINE_HEIGHT	= FONT_HEIGHT + 5
	STEP_HEIGHT	= LINE_HEIGHT + 5
	LONG_TEXT_START_X	= 300
	SCREEN_WIDTH	= wndMgr.GetScreenWidth()
	
	
	def __init__(self):
		ui.Bar.__init__(self)

		self.AddFlag("not_pick")
		self.missionText = None
		self.missionFullText = None
		self.curPos = 0
		self.dstPos = -5
		self.nextScrollTime = 0
		self.flowMode = False
		self.ScrollStartTime = 0.0

		self.SetPosition(0, 100)
		self.SetSize(self.SCREEN_WIDTH, 35)
		self.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 0.5))
		self.SetWindowHorizontalAlignCenter()

		self.__CreateTextBar()
		
	def __del__(self):
		ui.Bar.__del__(self)

	def __CreateTextBar(self):

		x, y = self.GetGlobalPosition()

		self.textBar = BigTextBar(self.SCREEN_WIDTH*2, 300, self.FONT_HEIGHT)
		self.textBar.SetParent(self)
		self.textBar.SetPosition(6, 8)
		self.textBar.SetTextColor(242, 231, 193)
		self.textBar.SetClipRect(0, y, self.SCREEN_WIDTH, y+8+self.STEP_HEIGHT)
		self.textBar.Show()

	def CleanMission(self):
		self.missionText = None
		self.missionFullText = None
		self.textBar.ClearBar()
		self.Hide()

	def __RefreshBoard(self):

		self.textBar.ClearBar()

		if self.missionFullText:
			(text_width, text_height) = self.textBar.GetTextExtent(self.missionFullText)
			
			if text_width>self.SCREEN_WIDTH:
				self.textBar.TextOut(0, (self.STEP_HEIGHT-8-text_height)/2, self.missionFullText)
				self.flowMode = True
			else:
				self.textBar.TextOut((wndMgr.GetScreenWidth()-text_width)/2, (self.STEP_HEIGHT-8-text_height)/2, self.missionFullText)
				self.flowMode = False

	def SetMission(self, text):
		self.__AppendText(text)
		self.__RefreshBoard()

		curTime = app.GetTime()
		
		(text_width, text_height) = self.textBar.GetTextExtent(self.missionFullText)
		
		if self.flowMode:
			self.dstPos = -text_width
			self.curPos = self.LONG_TEXT_START_X
			self.textBar.SetPosition(3 + self.curPos, 8)
		else:
			self.dstPos = 0
			self.curPos = self.STEP_HEIGHT
			self.textBar.SetPosition(3, 8 + self.curPos)
		
		if not self.IsShow():
			self.Show()
			
		self.Timer = Timer()
		self.Timer.Open(10.0)
		self.Timer.SAFE_SetTimeOverEvent(self.CleanMission)
			
	def SetSubMission(self, text):
		self.missionFullText = self.missionText + text
		
		preflowMode = self.flowMode
		
		self.__RefreshBoard()
		
		if preflowMode != self.flowMode:
			if self.flowMode:
				self.dstPos = -text_width
				self.curPos = self.LONG_TEXT_START_X
				self.textBar.SetPosition(3 + self.curPos, 8)
			else:
				self.dstPos = 0
				self.curPos = self.STEP_HEIGHT
				self.textBar.SetPosition(3, 8 + self.curPos)

	def __AppendText(self, text):
		if text == "":
			self.CleanMission()
			return
			
		self.missionText = text
		self.missionFullText = text
		
	def OnUpdate(self):			
		if self.missionFullText == None:
			self.Hide()
			return

		if self.dstPos < self.curPos:
			self.curPos -= 1
			if self.flowMode:
				self.textBar.SetPosition(3 + self.curPos, 8)
			else:
				self.textBar.SetPosition(3, 8 + self.curPos)
		else:
			if self.flowMode:
				self.curPos = self.SCREEN_WIDTH
				
class Timer(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.eventTimeOver = lambda *arg: None
		self.eventExit = lambda *arg: None
		self.OnUpdateTimer = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/pytimer.py")

		except:
			import exception as exception
			exception.Abort("WaitingDialog.LoadDialog.BindObject")

	def Open(self, waitTime):
		curTime = time.clock()
		self.endTime = curTime + waitTime
		self.OnUpdateTimer = 0
		self.Show()		

	def Close(self):
		self.Hide()

	def Destroy(self):
		self.Hide()

	def SAFE_SetTimeOverEvent(self, event):
		self.eventTimeOver = ui.__mem_func__(event)

	def SAFE_SetExitEvent(self, event):
		self.eventExit = ui.__mem_func__(event)
		
	def OnUpdate(self):
		self.OnUpdateTimer += 1
		if self.OnUpdateTimer == 60:
			self.OnUpdateTimer = 0
			lastTime = max(0, self.endTime - time.clock())
			if 0 == lastTime:
				self.Close()
				self.eventTimeOver()
			else:
				return
		
	def OnPressExitKey(self):
		self.Close()
		return True

