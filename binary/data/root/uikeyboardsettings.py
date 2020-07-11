import chat
import grp
import net
import app
import wndMgr
import uiCommon
import uiGuild
import uiToolTip
import ui
import constInfo
import uiScriptLocale
import localeInfo
import dbg
import player
import os
import grp

VALUE_KEY_CTRL = 1000
VALUE_KEY_ALT = 5000
VALUE_KEY_SHIFT = 9000
dirFile = "keysave.txt"

KEY_LIST_RESTRICTED = [4, 5, 7, 11, 12, 13, 14, 15, 16, 17, 18, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 55, 57, 58, 59, 60, 61, 66, 67]

tabButtonDefaultDict = {
	0 : ["W", app.DIK_W, uiScriptLocale.KEYCHANGE_UP1],
	1 : ["S", app.DIK_S, uiScriptLocale.KEYCHANGE_DOWN1],
	2 : ["A", app.DIK_A, uiScriptLocale.KEYCHANGE_LEFT1],
	3 : ["D", app.DIK_D, uiScriptLocale.KEYCHANGE_RIGHT1],
	4 : ["E", app.DIK_E, uiScriptLocale.KEYCHANGE_CAM_RIGHT1],
	5 : ["Q", app.DIK_Q, uiScriptLocale.KEYCHANGE_CAM_LEFT1],
	6 : ["R", app.DIK_R, uiScriptLocale.KEYCHANGE_CAM_ZOOM_IN1],
	7 : ["F", app.DIK_F, uiScriptLocale.KEYCHANGE_CAM_ZOOM_OUT1],
	8 : ["T", app.DIK_T, uiScriptLocale.KEYCHANGE_CAM_DOWN1],
	9 : ["G", app.DIK_G, uiScriptLocale.KEYCHANGE_CAM_UP1],
	10 : ["^", app.DIK_GRAVE, uiScriptLocale.KEYCHANGE_ROOT1],
	11 : ["SPACE", app.DIK_SPACE, uiScriptLocale.KEYCHANGE_ATTACK],
	12 : ["CTRL + H", app.DIK_H, uiScriptLocale.KEYCHANGE_RIDE_HORSE],
	13 : ["CTRL + F", app.DIK_F, uiScriptLocale.KEYCHANGE_RIDE_PEED],
	14 : ["CTRL + 1", app.DIK_1, uiScriptLocale.KEYCHANGE_EMOTION1],
	15 : ["CTRL + 2", app.DIK_2, uiScriptLocale.KEYCHANGE_EMOTION2],
	16 : ["CTRL + 3", app.DIK_3, uiScriptLocale.KEYCHANGE_EMOTION3],
	17 : ["CTRL + 4", app.DIK_4, uiScriptLocale.KEYCHANGE_EMOTION4],
	18 : ["CTRL + P", app.DIK_F9, uiScriptLocale.KEYCHANGE_SUPPORT_GUI],
	19 : ["UP", app.DIK_UP, uiScriptLocale.KEYCHANGE_UP2],
	20 : ["DOWN", app.DIK_DOWN, uiScriptLocale.KEYCHANGE_DOWN2],
	21 : ["LEFT", app.DIK_LEFT, uiScriptLocale.KEYCHANGE_LEFT2],
	22 : ["RIGHT", app.DIK_RIGHT, uiScriptLocale.KEYCHANGE_RIGHT2],
	23 : ["NUMPAD 6", app.DIK_NUMPAD6, uiScriptLocale.KEYCHANGE_CAM_RIGHT2],
	24 : ["NUMPAD 4", app.DIK_NUMPAD4, uiScriptLocale.KEYCHANGE_CAM_LEFT2],
	25 : ["PAGE UP", app.DIK_PGUP, uiScriptLocale.KEYCHANGE_CAM_ZOOM_IN2],
	26 : ["PAGE DOWN", app.DIK_PGDN, uiScriptLocale.KEYCHANGE_CAM_ZOOM_OUT2],
	27 : ["NUMPAD 8", app.DIK_NUMPAD8, uiScriptLocale.KEYCHANGE_CAM_DOWN2],
	28 : ["NUMPAD 2", app.DIK_NUMPAD2, uiScriptLocale.KEYCHANGE_CAM_UP2],
	29 : ["Y", app.DIK_Z, uiScriptLocale.KEYCHANGE_ROOT2],
	30 : ["CTRL + G", app.DIK_G, uiScriptLocale.KEYCHANGE_RIDE],
	31 : ["CTRL + B", app.DIK_B, uiScriptLocale.KEYCHANGE_RIDE_BYE],
	32 : ["CTRL + 5", app.DIK_5, uiScriptLocale.KEYCHANGE_EMOTION5],
	33 : ["CTRL + 6", app.DIK_6, uiScriptLocale.KEYCHANGE_EMOTION6],
	34 : ["CTRL + 7", app.DIK_7, uiScriptLocale.KEYCHANGE_EMOTION7],
	35 : ["CTRL + 8", app.DIK_8, uiScriptLocale.KEYCHANGE_EMOTION8],
	36 : ["CTRL + 9", app.DIK_9, uiScriptLocale.KEYCHANGE_EMOTION9],
	37 : ["K", app.DIK_K, uiScriptLocale.KEYCHANGE_SPECIAL_INVENTORY],
	38 : ["1", app.DIK_1, uiScriptLocale.KEYCHANGE_SLOT1],
	39 : ["2", app.DIK_2, uiScriptLocale.KEYCHANGE_SLOT2],
	40 : ["3", app.DIK_3, uiScriptLocale.KEYCHANGE_SLOT3],
	41 : ["4", app.DIK_4, uiScriptLocale.KEYCHANGE_SLOT4],
	42 : ["F1", app.DIK_F1, uiScriptLocale.KEYCHANGE_SLOT5],
	43 : ["F2", app.DIK_F2, uiScriptLocale.KEYCHANGE_SLOT6],
	44 : ["F3", app.DIK_F3, uiScriptLocale.KEYCHANGE_SLOT7],
	45 : ["F4", app.DIK_F4, uiScriptLocale.KEYCHANGE_SLOT8],
	46 : ["SHIFT + 1", app.DIK_1, uiScriptLocale.KEYCHANGE_QUICKSLOT1],
	47 : ["SHIFT + 2", app.DIK_2, uiScriptLocale.KEYCHANGE_QUICKSLOT2],
	48 : ["SHIFT + 3", app.DIK_3, uiScriptLocale.KEYCHANGE_QUICKSLOT3],
	49 : ["SHIFT + 4", app.DIK_4, uiScriptLocale.KEYCHANGE_QUICKSLOT4],
	50 : ["C", app.DIK_C, uiScriptLocale.KEYCHANGE_STATUS_WINDOW],
	51 : ["V", app.DIK_V, uiScriptLocale.KEYCHANGE_SKILL_WINDOW],
	52 : ["N", app.DIK_N, uiScriptLocale.KEYCHANGE_QUEST_WINDOW],
	53 : ["I", app.DIK_I, uiScriptLocale.KEYCHANGE_INVENTORY_WINDOW],
	54 : ["O", app.DIK_O, uiScriptLocale.KEYCHANGE_DDS_WINDOW],
	55 : ["M", app.DIK_M, uiScriptLocale.KEYCHANGE_ATLASMAP_WINDOW],
	56 : ["L", app.DIK_L, uiScriptLocale.KEYCHANGE_CHAT_WINDOW],
	57 : ["CTRL + Q", app.DIK_Q, uiScriptLocale.KEYCHANGE_QUEST_SCROLL_ONOFF],
	58 : ["ALT + G", app.DIK_G, uiScriptLocale.KEYCHANGE_GUILD_WINDOW],
	59 : ["CTRL + M", app.DIK_M, uiScriptLocale.KEYCHANGE_MESSENGER_WINDOW],
	60 : ["H", app.DIK_H, uiScriptLocale.KEYCHANGE_HELP_WINDOW],
	61 : ["B", app.DIK_B, uiScriptLocale.KEYCHANGE_ACTION_WINDOW],
	62 : ["+", app.DIK_ADD, uiScriptLocale.KEYCHANGE_MINIMAP_PLUS],
	63 : ["-", app.DIK_SUBTRACT, uiScriptLocale.KEYCHANGE_MINIMAP_MINER],
	64 : ["PRT SCN", app.DIK_SYSRQ, uiScriptLocale.KEYCHANGE_SCREENSHOT],
	65 : ["X", app.DIK_X, uiScriptLocale.KEYCHANGE_SHOW_NAME],
	66 : ["SHIFT + M", app.DIK_M, uiScriptLocale.KEYCHANGE_SHOW_MINIMAP],
	67 : ["ENTER", app.DIK_RETURN, uiScriptLocale.KEYCHANGE_SHOW_CHAT]
}

AVAIL_KEY_LIST = {
	app.DIK_A : "A",
	app.DIK_C : "C",
	app.DIK_D : "D",
	app.DIK_I : "I",
	app.DIK_J : "J",
	app.DIK_K : "K",
	app.DIK_L : "L",
	app.DIK_N : "N",
	app.DIK_O : "O",
	app.DIK_R : "R",
	app.DIK_S : "S",
	app.DIK_T : "T",
	app.DIK_U : "U",
	app.DIK_V : "V",
	app.DIK_W : "W",
	app.DIK_X : "X",
	app.DIK_Y : "Z",
	app.DIK_Z : "Y",
	app.DIK_GRAVE : "^",	
	app.DIK_UP : "UP",
	app.DIK_DOWN : "DOWN",
	app.DIK_LEFT : "LEFT",
	app.DIK_RIGHT : "RIGHT",
	app.DIK_ADD : "+",
	app.DIK_SUBTRACT : "-",
	app.DIK_END : "End",
	app.DIK_NUMLOCK : "Num Lock",	
	app.DIK_MINUS : "-",
	app.DIK_EQUALS : "=",
	app.DIK_DELETE : "Del",
	app.DIK_INSERT : "Insert",
	app.DIK_SCROLL : "Scroll Lock",
	app.DIK_NUMPAD0 : "NUMPAD 0",
	app.DIK_NUMPAD1 : "NUMPAD 1",
	app.DIK_NUMPAD2 : "NUMPAD 2",
	app.DIK_NUMPAD3 : "NUMPAD 3",
	app.DIK_NUMPAD4 : "NUMPAD 4",
	app.DIK_NUMPAD5 : "NUMPAD 5",
	app.DIK_NUMPAD6 : "NUMPAD 6",
	app.DIK_NUMPAD7 : "NUMPAD 7",
	app.DIK_NUMPAD8 : "NUMPAD 8",
	app.DIK_NUMPAD9 : "NUMPAD 9",
	app.DIK_PGUP : "PAGE UP",
	app.DIK_PGDN : "PAGE DOWN"
}

COMB_AVAIL_KEY_LIST = {
	app.DIK_LALT : "ALT",
	app.DIK_LSHIFT : "SHIFT",
	app.DIK_LCONTROL : "CTRL L",
	app.DIK_RCONTROL : "CTRL R"
}

def GetIsBlockedList(key):
	return (key in KEY_LIST_RESTRICTED)

class MouseReflector(ui.Window):
	def __init__(self, parent):
		ui.Window.__init__(self)
		self.SetParent(parent)
		self.AddFlag("not_pick")
		self.width = self.height = 0
		self.isDown = False
		
		self.DARK_COLOR = 0xFF004008 # Green
		self.BRIGHT_COLOR = 0xFF107a1c

	def Down(self):
		self.isDown = True

	def Up(self):
		self.isDown = False

	def OnRender(self):
		if self.isDown:
			grp.SetColor(self.DARK_COLOR)
		else:
			grp.SetColor(self.BRIGHT_COLOR)

		x, y = self.GetGlobalPosition()
		grp.RenderBar(x + 2, y + 2, self.GetWidth() - 4, self.GetHeight() - 4)

class AppendKeyboard(ui.ImageBox):
	def __init__(self, parent, key, x, y, event, filename = "d:/ymir work/ui/public/Parameter_Slot_03.sub"):
		ui.ImageBox.__init__(self)
		
		self.SetParent(parent)
		self.SetPosition(x, y)
		
		if GetIsBlockedList(key):
			self.AddFlag("not_pick")
			
			self.blockedSlot = ui.Bar()
			self.blockedSlot.SetParent(self)
			self.blockedSlot.SetSize(self.GetWidth() - 4, self.GetHeight() - 4)
			self.blockedSlot.SetColor(0xFF949291)
			self.blockedSlot.SetPosition(2, 2)		
			self.blockedSlot.Show()
			
		self.LoadImage(filename)

		self.STATE_SHOW = 0
		self.STATE_HIDE = 1
		self.STATE_UP = 2
		self.STATE_DOWN = 3
		self.STATE_EMPTY_SLOT = False

		self.mouseReflector = MouseReflector(self)
		self.mouseReflector.SetSize(self.GetWidth(), self.GetHeight())
		self.mouseReflector.UpdateRect()

		markSlot = ui.MakeImageBox(self, "d:/ymir work/ui/public/check_image.sub", 2560 * 1024, 0)
		markSlot.SetWindowHorizontalAlignCenter()
		markSlot.SetWindowVerticalAlignCenter()
		markSlot.Hide()
		
		emptySlot = ui.Bar()
		emptySlot.SetParent(self)
		emptySlot.SetSize(self.GetWidth() - 4, self.GetHeight() - 4)
		emptySlot.SetColor(0xFF741706) # Red
		emptySlot.SetPosition(2, 2)		
		emptySlot.Hide()
		
		if GetIsBlockedList(key):
			self.blockedSlot = ui.Bar()
			self.blockedSlot.SetParent(self)
			self.blockedSlot.SetSize(self.GetWidth() - 4, self.GetHeight() - 4)
			self.blockedSlot.SetColor(0xFF6e7870) # Grey
			self.blockedSlot.SetPosition(2, 2)
			self.blockedSlot.Show()

		self.markSlot = markSlot
		self.event = event
		self.emptySlot = emptySlot
		
		if not GetIsBlockedList(key):
			self.SetFullWindow(True)
		self.Show()

	def __del__(self):
		ui.ImageBox.__del__(self)

	def SetCheck(self, flag):
		if flag:
			self.SetMarkSlot(self.STATE_SHOW)
			self.SetMouseReflector(self.STATE_SHOW)
			
		else:
			if self.GetIsEmptySlot():
				self.SetColorEmptySlot(self.STATE_SHOW)
				
			self.SetMarkSlot(self.STATE_HIDE)
			self.SetMouseReflector(self.STATE_HIDE)
			
	def SetFullWindow(self, flag):
		if flag:
			self.SetPickAlways()
		else:
			self.DelPickAlways()
			
	def SetMouseReflector(self, state):
		if state == self.STATE_SHOW:
			self.mouseReflector.Show()
			
		elif state == self.STATE_HIDE:
			self.mouseReflector.Hide()

		elif state == self.STATE_UP:
			self.mouseReflector.Up()

		elif state == self.STATE_DOWN:
			self.mouseReflector.Down()	

	def SetColorEmptySlot(self, state):
		if state == self.STATE_SHOW:
			self.emptySlot.Show()
			
		elif state == self.STATE_HIDE:
			self.emptySlot.Hide()
			
	def SetMarkSlot(self, state):
		if state == self.STATE_SHOW:
			self.markSlot.Show()
			
		elif state == self.STATE_HIDE:
			self.markSlot.Hide()
			
	def SetEmptySlot(self, flag):
		if flag:
			self.STATE_EMPTY_SLOT = True
		else:
			self.STATE_EMPTY_SLOT = False
			
	def IsShowedEmptySlot(self):
		return self.emptySlot.IsShow()
			
	def Unbound(self):
		if self.IsShowedEmptySlot() and self.GetIsEmptySlot():
			self.SetColorEmptySlot(self.STATE_HIDE)
			self.SetEmptySlot(False)
			
	def GetIsEmptySlot(self):
		return self.STATE_EMPTY_SLOT
		
	def ShowEmptySlot(self):
		self.SetColorEmptySlot(self.STATE_SHOW)

	def HideEmptySlot(self):
		self.SetColorEmptySlot(self.STATE_HIDE)
		self.SetMarkSlot(self.STATE_HIDE)
		self.SetMouseReflector(self.STATE_HIDE)
		self.SetEmptySlot(False)
		
	def IsSelected(self):
		return self.markSlot.IsShow()

	def OnMouseOverIn(self):
		if self.IsSelected():
			return
			
		if self.GetIsEmptySlot():
			self.SetColorEmptySlot(self.STATE_HIDE)

		self.SetMouseReflector(self.STATE_SHOW)

	def OnMouseOverOut(self):
		if self.IsSelected():
			return
			
		if self.GetIsEmptySlot():
			self.SetColorEmptySlot(self.STATE_SHOW)

		self.SetMouseReflector(self.STATE_HIDE)

	def OnMouseLeftButtonDown(self):
		if self.IsSelected():
			return

		self.SetMouseReflector(self.STATE_DOWN)

	def OnMouseLeftButtonUp(self):
		if self.IsSelected():
			return

		self.SetMouseReflector(self.STATE_UP)
		self.event()

def getattr(row):
	attr = {4: "PROFILE_KEYBOARD_DICT=[\n", 2: ",\n|", 6: "/*key*/]"}
	if attr.has_key(row):
		return attr[row]	

class KeyChangeWindow(ui.ScriptWindow):
	def __init__(self, stream = None):
		ui.ScriptWindow.__init__(self)

		self.STATE_SAVE = 1
		self.STATE_CLOSE = 2
		self.EMPTY_ATTRIBUTE = 999
		self.EMPTY_TEXT = ""
		self.HOTKEY_LINE = "%s|%d"
		
		self.MAX_KEYBOARD_CATEGORY = 68
		
		self.TAB_POSITION_X = [120, 320, 490, 676]
		self.TAB_POSITION_Y = 20

		self.START_CATEGORY_STANDARD_A = 0
		self.END_CATEGORY_STANDARD_A = 19

		self.START_CATEGORY_STANDARD_B = 19
		self.END_CATEGORY_STANDARD_B = 38
		
		self.START_CATEGORY_QUICKSLOT = 38
		self.END_CATEGORY_QUICKSLOT = 50
		
		self.START_CATEGORY_WINDOW = 50
		self.END_CATEGORY_WINDOW = 68
		
		self.textLine = {}
		self.keyboardDict = {}
		self.wndQuizState = {}
		self.stream = stream
		self.main = {"profile_keyboard" : {}}
		self.Init()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Init(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/keychange_window.py")
		except:
			import exception
			exception.Abort("KeyChangeWindow.LoadObject")
		try:
			self.board = self.GetChild("board")
			self.titleBar = self.GetChild("TitleBar")
			self.btn_clear = self.GetChild("ClearButton")
			self.btn_save = self.GetChild("SaveButton")
			self.btn_cancel = self.GetChild("CancelButton")

		except:
			import exception
			exception.Abort("KeyChangeWindow.BindObject")		

		for row in xrange(self.MAX_KEYBOARD_CATEGORY):
			x, y = self.GetSplitPosition(row)
			self.main["profile_keyboard"].update({row : [AppendKeyboard(self, row, x, 75 + y, lambda arg = row: self.OnSelectType(arg)), self.SetText(row, x + 45, 77 + y)]})
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.btn_save.SAFE_SetEvent(self.AskQuestionPacket, self.STATE_SAVE)			
		self.btn_cancel.SAFE_SetEvent(self.AskQuestionPacket, self.STATE_CLOSE)
		self.btn_clear.SAFE_SetEvent(self.LoadDefaultKeyboard)
		self.SetCenterPosition()
		
	def SetText(self, row, x, y):
		self.textLine[row] = ui.TextLine()
		self.textLine[row].SetParent(self)
		self.textLine[row].SetPosition(x, y)
		self.textLine[row].SetText("")
		self.textLine[row].SetHorizontalAlignCenter()
		self.textLine[row].Show()
		
	def GetSplitPosition(self, row):
		if row in xrange(self.START_CATEGORY_STANDARD_A, self.END_CATEGORY_STANDARD_A):
			return self.TAB_POSITION_X[0], row * self.TAB_POSITION_Y
				
		elif row in xrange(self.START_CATEGORY_STANDARD_B, self.END_CATEGORY_STANDARD_B):
			return self.TAB_POSITION_X[1], (row - self.START_CATEGORY_STANDARD_B) * self.TAB_POSITION_Y

		elif row in xrange(self.START_CATEGORY_QUICKSLOT, self.END_CATEGORY_QUICKSLOT):
			return self.TAB_POSITION_X[2], (row - self.START_CATEGORY_QUICKSLOT) * self.TAB_POSITION_Y

		elif row in xrange(self.START_CATEGORY_WINDOW, self.END_CATEGORY_WINDOW):
			return self.TAB_POSITION_X[3], (row - self.START_CATEGORY_WINDOW) * self.TAB_POSITION_Y
			
	def LoadState(self, row):
		return self.main["profile_keyboard"][row][0]
		
	def OnSelectType(self, row):
		for key in xrange(self.MAX_KEYBOARD_CATEGORY):
			if row == key:
				self.LoadState(key).SetCheck(True)
			else:
				self.LoadState(key).SetCheck(False)
				
	def LoadDefaultKeyboard(self):
		for row, data in tabButtonDefaultDict.iteritems():
			self.LoadText(row, data[0])
			self.LoadState(row).HideEmptySlot()
			
			self.keyboardDict[row][0] = data[1]
			self.keyboardDict[row][1] = data[0]

	def GetIsSelected(self):
		for row in xrange(self.MAX_KEYBOARD_CATEGORY):
			if self.LoadState(row).IsSelected():
				return True
		return False
		
	def IsSelected(self, row):
		return self.LoadState(row).IsSelected()
		
	def LoadText(self, row, value):
		self.textLine[row].SetText(value)
		
	def SetKeyboard(self, row, value, text):
		if value != self.EMPTY_ATTRIBUTE:
			self.keyboardDict[row][1] = text

		self.keyboardDict[row][0] = value
		self.LoadState(row).SetCheck(False)
		
	def SaveSettings(self, flag):
		list = []
		dict = self.keyboardDict
		x, y = 1, 0
		
		if not flag:
			dict = tabButtonDefaultDict
			x, y = 0, 1

		for i in xrange(self.MAX_KEYBOARD_CATEGORY):
			symmetric_difference = self.HOTKEY_LINE % (dict[i][x], dict[i][y])
			list.append(symmetric_difference)

		f = open(dirFile, 'w+')
		for key in list:
			f.write(str(key) + "\n")

	def LoadSettings(self):
		data = open(dirFile, "r").readlines()

		for row in xrange(len(data)):
			lines = str(data[row])
				
			if lines == "\n" or lines == "":
				continue

			tokens = lines.split("|")
			rowKey, rowName = int(tokens[1]), str(tokens[0])

			self.keyboardDict.update({row : [rowKey, rowName]})
		
		for row, data in self.keyboardDict.iteritems():
			if data[0] == self.EMPTY_ATTRIBUTE:
				self.LoadState(row).ShowEmptySlot()
				self.LoadState(row).SetEmptySlot(True)
				self.LoadText(row, self.EMPTY_TEXT)
			else:
				self.LoadText(row, data[1])
				
			self.LoadState(row).SetCheck(False)
				
	def IsPressed_CTRL(self):
		return app.IsPressed(app.DIK_LCONTROL)
		
	def IsPressed_ALT(self):
		return app.IsPressed(app.DIK_LALT) 
		
	def IsPressed_SHIFT(self):
		return app.IsPressed(app.DIK_LSHIFT)
		
	def MarkBox(self, value):
		for row, data in self.keyboardDict.iteritems():
			if value == data[0]:
				self.LoadState(row).ShowEmptySlot()
				self.LoadState(row).SetEmptySlot(True)
				self.SetKeyboard(row, self.EMPTY_ATTRIBUTE, "")
				self.LoadText(row, self.EMPTY_TEXT)

	def WriteSettings(self, key):
		value, index = 0, 0
		text = ""
		
		if key in COMB_AVAIL_KEY_LIST:
			chat.AppendChat(chat.CHAT_TYPE_INFO, uiScriptLocale.KEYCHANGE_IMPOSSIBLE_CHANGE)
			return
		
		if not key in AVAIL_KEY_LIST:
			chat.AppendChat(chat.CHAT_TYPE_INFO, uiScriptLocale.KEYCHANGE_IMPOSSIBLE_CHANGE2)
			return

		if not self.GetIsSelected():
			chat.AppendChat(chat.CHAT_TYPE_INFO, uiScriptLocale.KEYCHANGE_IMPOSSIBLE_CHANGE3)
			return

		if key in AVAIL_KEY_LIST:
			text = AVAIL_KEY_LIST[key]
			value = key

		for row, data in self.keyboardDict.iteritems():
			if self.IsSelected(row):

				if value is data[0]:
					return

				index = row

		self.MarkBox(value)
		self.LoadText(index, text)
		self.SetKeyboard(index, value, text)
		self.LoadState(index).Unbound()
		
	def BuildKeyDict(self):
	
		if not os.path.isfile(dirFile):
			file(dirFile, 'w').close()

		if not os.stat(r"keysave.txt").st_size > 0:
			self.SaveSettings(False)

		def swapcase(index):

			def key(row):
				return self.keyboardDict[row][0]

			onListKeyDict = {
				"KEYCHANGE_UP_1" : key(0),
				"KEYCHANGE_DOWN_1" : key(1),		
				"KEYCHANGE_LEFT_1" : key(2),	
				"KEYCHANGE_RIGHT_1" : key(3),
				"KEYCHANGE_CAM_ZOOM_IN_1" : key(6),
				"KEYCHANGE_CAM_DOWN_1" : key(8),
				"KEYCHANGE_CAM_UP_1" : key(9),
				"KEYCHANGE_ROOT_1" : key(10),

				"KEYCHANGE_UP_2" : key(19),
				"KEYCHANGE_DOWN_2" : key(20),
				"KEYCHANGE_LEFT_2" : key(21),
				"KEYCHANGE_RIGHT_2" : key(22),
				"KEYCHANGE_CAM_RIGHT_2" : key(23),
				"KEYCHANGE_CAM_LEFT_2" : key(24),
				"KEYCHANGE_CAM_ZOOM_IN_2" : key(25),
				"KEYCHANGE_CAM_ZOOM_OUT_2" : key(26),
				"KEYCHANGE_CAM_DOWN_2" : key(27),
				"KEYCHANGE_CAM_UP_2" : key(28),
				"KEYCHANGE_ROOT_2" : key(29),

				"KEYCHANGE_STATUS_WINDOW" : key(50),
				"KEYCHANGE_SKILL_WINDOW" : key(51),
				"KEYCHANGE_QUEST_WINDOW" : key(52),
				"KEYCHANGE_INVENTORY_WINDOW" : key(53),
				"KEYCHANGE_DDS_WINDOW" : key(54),
				"KEYCHANGE_CHAT_WINDOW" : key(56),

				"KEYCHANGE_MINIMAP_PLUS" : key(62),
				"KEYCHANGE_MINIMAP_MINER" : key(63),
				"KEYCHANGE_SCREENSHOT" : key(64),
				"KEYCHANGE_SHOW_NAME" : key(65),
			}
			if onListKeyDict.has_key(index):
				return onListKeyDict[index]
	
		self.LoadSettings()
		
		##############################################################################################
		###	OnKeyDown
		##############################################################################################
		onPressKeyDict = {}
		onPressKeyDict[swapcase("KEYCHANGE_UP_1")]	= lambda : player.SetSingleDIKKeyState(app.DIK_UP, True)
		onPressKeyDict[swapcase("KEYCHANGE_DOWN_1")] = lambda : player.SetSingleDIKKeyState(app.DIK_DOWN, True)
		onPressKeyDict[swapcase("KEYCHANGE_LEFT_1")] = lambda : player.SetSingleDIKKeyState(app.DIK_LEFT, True)
		onPressKeyDict[swapcase("KEYCHANGE_RIGHT_1")] = lambda : player.SetSingleDIKKeyState(app.DIK_RIGHT, True)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_ZOOM_IN_1")]	= lambda : app.ZoomCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_F] = lambda : self.PressFKey() 
		onPressKeyDict[swapcase("KEYCHANGE_CAM_DOWN_1")] = lambda : app.PitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_UP_1")] = lambda : app.PitchCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[swapcase("KEYCHANGE_ROOT_1")] = lambda : player.PickCloseItemVector()
		onPressKeyDict[app.DIK_SPACE] = lambda : player.SetAttackKeyState(True)
		onPressKeyDict[swapcase("KEYCHANGE_UP_2")]	= lambda : player.SetSingleDIKKeyState(app.DIK_UP, True)
		onPressKeyDict[swapcase("KEYCHANGE_DOWN_2")] = lambda : player.SetSingleDIKKeyState(app.DIK_DOWN, True)
		onPressKeyDict[swapcase("KEYCHANGE_LEFT_2")] = lambda : player.SetSingleDIKKeyState(app.DIK_LEFT, True)
		onPressKeyDict[swapcase("KEYCHANGE_RIGHT_2")] = lambda : player.SetSingleDIKKeyState(app.DIK_RIGHT, True)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_RIGHT_2")] = lambda : app.RotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_LEFT_2")] = lambda : app.RotateCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_ZOOM_IN_2")]	= lambda : app.ZoomCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_ZOOM_OUT_2")] = lambda : app.ZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_DOWN_2")] = lambda : app.PitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[swapcase("KEYCHANGE_CAM_UP_2")] = lambda : app.PitchCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[swapcase("KEYCHANGE_ROOT_2")] = lambda : player.PickCloseItem()

		onPressKeyDict[app.DIK_1]	= lambda : self.stream.PressNumKey(1)
		onPressKeyDict[app.DIK_2]	= lambda : self.stream.PressNumKey(2)
		onPressKeyDict[app.DIK_3]	= lambda : self.stream.PressNumKey(3)
		onPressKeyDict[app.DIK_4]	= lambda : self.stream.PressNumKey(4)
		onPressKeyDict[app.DIK_5]	= lambda : self.stream.PressNumKey(5)
		onPressKeyDict[app.DIK_6]	= lambda : self.stream.PressNumKey(6)
		onPressKeyDict[app.DIK_7]	= lambda : self.stream.PressNumKey(7)
		onPressKeyDict[app.DIK_8]	= lambda : self.stream.PressNumKey(8)
		onPressKeyDict[app.DIK_9]	= lambda : self.stream.PressNumKey(9)
		onPressKeyDict[app.DIK_F1]	= lambda : self.stream.PressQuickSlot(4)
		onPressKeyDict[app.DIK_F2]	= lambda : self.stream.PressQuickSlot(5)
		onPressKeyDict[app.DIK_F3]	= lambda : self.stream.PressQuickSlot(6)
		onPressKeyDict[app.DIK_F4]	= lambda : self.stream.PressQuickSlot(7)
		onPressKeyDict[app.DIK_F5]	= lambda : self.stream.AverageSearchOpen()
		onPressKeyDict[app.DIK_F6]	= lambda : net.SendChatPacket("/get_guild_rank_list")
		
		onPressKeyDict[swapcase("KEYCHANGE_MESSENGER_WINDOW")] = lambda : self.stream.PressExtraKey("MESSENGER_WINDOW")
		onPressKeyDict[swapcase("KEYCHANGE_STATUS_WINDOW")]	= lambda : self.stream.interface.ToggleCharacterWindow("STATUS")
		onPressKeyDict[swapcase("KEYCHANGE_SKILL_WINDOW")] = lambda : self.stream.interface.ToggleCharacterWindow("SKILL")
		onPressKeyDict[swapcase("KEYCHANGE_QUEST_WINDOW")] = lambda : self.stream.interface.ToggleCharacterWindow("QUEST")
		onPressKeyDict[swapcase("KEYCHANGE_INVENTORY_WINDOW")] = lambda : self.stream.interface.ToggleInventoryWindow()
		onPressKeyDict[swapcase("KEYCHANGE_DDS_WINDOW")] = lambda : self.stream.interface.ToggleDragonSoulWindowWithNoInfo()
		onPressKeyDict[swapcase("KEYCHANGE_CHAT_WINDOW")] = lambda : self.stream.interface.ToggleChatLogWindow()
		onPressKeyDict[swapcase("KEYCHANGE_MINIMAP_PLUS")] = lambda : self.stream.interface.MiniMapScaleUp()
		onPressKeyDict[swapcase("KEYCHANGE_MINIMAP_MINER")]	= lambda : self.stream.interface.MiniMapScaleDown()
		onPressKeyDict[swapcase("KEYCHANGE_SCREENSHOT")] = lambda : self.stream.SaveScreen()
		onPressKeyDict[swapcase("KEYCHANGE_SHOW_NAME")] = lambda : self.stream.ShowNameNew()

		onPressKeyDict[app.DIK_LCONTROL] = lambda : self.stream.interface.ShowMouseImage()
		onPressKeyDict[app.DIK_P] = lambda: self.PressPKey()
		onPressKeyDict[app.DIK_M] = lambda : self.PressMKey()
		onPressKeyDict[app.DIK_G] = lambda : self.PressGKey()
		onPressKeyDict[app.DIK_H] = lambda : self.PressHKey()
		onPressKeyDict[app.DIK_B] = lambda : self.PressBKey()
		onPressKeyDict[app.DIK_Q] = lambda : self.PressQKey()
		onPressKeyDict[app.DIK_E] = lambda : app.RotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_LSHIFT] = lambda: self.stream.SetQuickPageMode()		
		onPressKeyDict[app.DIK_J] = lambda : self.stream.PressExtraKey("RIDE_J")
		onPressKeyDict[app.DIK_LALT] = lambda : self.stream.ShowName()
				
		onPressKeyDict[app.DIK_F9]	= lambda : self.stream.OpenSupportShamanGui()
		onPressKeyDict[app.DIK_K]	= lambda : self.stream.PressExtendedInventory()
		self.onPressKeyDict = onPressKeyDict
		
		##############################################################################################
		###	OnKeyUp
		##############################################################################################
		onClickKeyDict = {}

		onClickKeyDict[swapcase("KEYCHANGE_CAM_ZOOM_IN_1")]	= lambda : app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_DOWN_1")] = lambda : app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_UP_1")] = lambda : app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_RIGHT_2")] = lambda : app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_LEFT_2")] = lambda : app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_ZOOM_IN_2")]	= lambda : app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_ZOOM_OUT_2")] = lambda : app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_DOWN_2")] = lambda : app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_CAM_UP_2")] = lambda : app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[swapcase("KEYCHANGE_UP_1")]	= lambda : player.SetSingleDIKKeyState(app.DIK_UP, False)
		onClickKeyDict[swapcase("KEYCHANGE_DOWN_1")] = lambda : player.SetSingleDIKKeyState(app.DIK_DOWN, False)
		onClickKeyDict[swapcase("KEYCHANGE_LEFT_1")] = lambda : player.SetSingleDIKKeyState(app.DIK_LEFT, False)
		onClickKeyDict[swapcase("KEYCHANGE_RIGHT_1")] = lambda : player.SetSingleDIKKeyState(app.DIK_RIGHT, False)
		onClickKeyDict[swapcase("KEYCHANGE_UP_2")]	= lambda : player.SetSingleDIKKeyState(app.DIK_UP, False)
		onClickKeyDict[swapcase("KEYCHANGE_DOWN_2")] = lambda : player.SetSingleDIKKeyState(app.DIK_DOWN, False)
		onClickKeyDict[swapcase("KEYCHANGE_LEFT_2")] = lambda : player.SetSingleDIKKeyState(app.DIK_LEFT, False)
		onClickKeyDict[swapcase("KEYCHANGE_RIGHT_2")] = lambda : player.SetSingleDIKKeyState(app.DIK_RIGHT, False)
		onClickKeyDict[app.DIK_SPACE] = lambda : player.SetAttackKeyState(False)
		onClickKeyDict[swapcase("KEYCHANGE_SHOW_NAME")] = lambda : self.stream.HideNameNew()
		onClickKeyDict[app.DIK_LCONTROL] = lambda : self.stream.interface.HideMouseImage()
		onClickKeyDict[app.DIK_LALT] = lambda : self.stream.HideName()
		onClickKeyDict[app.DIK_Q] = lambda :app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_LSHIFT] = lambda: self.stream.SetQuickSlotMode()
		onClickKeyDict[app.DIK_F] = lambda : app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_E] = lambda : app.RotateCamera(app.CAMERA_STOP)
		self.onClickKeyDict = onClickKeyDict
		
	def PressQKey(self):
		if app.IsPressed(app.DIK_LCONTROL):
			self.stream.PressExtraKey("QUEST_SCROLL_ONOFF")
		else:
			app.RotateCamera(app.CAMERA_TO_NEGATIVE)
		
	def PressPKey(self):
		if app.IsPressed(app.DIK_LCONTROL):
			self.stream.PressExtraKey("HIDE_PM")
		else:
			self.stream.OpenPetMainGui()
			
	def PressHKey(self):
		if app.IsPressed(app.DIK_LCONTROL):
			self.stream.PressExtraKey("RIDE_HORSE")
		else:
			self.stream.interface.OpenHelpWindow()
			
	def PressBKey(self):
		if app.IsPressed(app.DIK_LCONTROL):
			self.stream.PressExtraKey("RIDE_BYE")
		else:
			self.stream.interface.ToggleCharacterWindow("EMOTICON")
			
	def PressGKey(self):
		self.stream.PressGKey()
			
	def PressFKey(self):
		if app.IsPressed(app.DIK_LCONTROL):
			self.stream.PressExtraKey("RIDE_PEED")
		else:
			app.ZoomCamera(app.CAMERA_TO_POSITIVE)
	
	def PressMKey(self):
		if app.IsPressed(app.DIK_LALT):
			self.stream.interface.ToggleMessenger()
		elif app.IsPressed(app.DIK_LCONTROL):
			self.stream.PressExtraKey("MESSENGER_WINDOW")
		elif app.IsPressed(app.DIK_LSHIFT):
			self.stream.PressExtraKey("SHOW_MINIMAP")
		else:
			self.stream.interface.wndMiniMap.ToggleAtlasWindow()
		
	def OnKeyUp(self, key):
		if not self.IsShow():
			try:
				self.onClickKeyDict[key]()
			except KeyError:
				pass
			except:
				raise

		return True
		
	def OnKeyDown(self, key):
		if not self.IsShow():
			self.LoadSettings()
			self.BuildKeyDict()

			try:
				self.onPressKeyDict[key]()
			except KeyError:
				pass
			except:
				raise

		if self.IsShow():
			self.WriteSettings(key)

		return True
		
	def AnswerWithKey(self, answer, key):
		if not self.wndQuizState[key]:
			return

		self.wndQuizState[key].Close()
		self.wndQuizState[key] = None
		
		if not answer:
			return
			
		if key == self.STATE_SAVE:
			self.SaveSettings(True)
			self.SetFullWindow(True)
			self.Hide()
			
		elif key == self.STATE_CLOSE:
			self.SetFullWindow(True)
			self.Hide()
			
	def SetFullWindow(self, flag):
		for row in xrange(self.MAX_KEYBOARD_CATEGORY):
			if not row in KEY_LIST_RESTRICTED:
				self.LoadState(row).SetFullWindow(flag)
		
	def AskQuestionPacket(self, key):
		self.SetFullWindow(False)

		self.QUESTION_DESCRIPTION = {
			1 : uiScriptLocale.KEYCHANGE_EXIT_AND_SAVE, 2 : uiScriptLocale.KEYCHANGE_EXIT_WITHOUT_SAVE,		
		}

		self.wndQuizState[key] = uiCommon.QuestionDialog()
		self.wndQuizState[key].SetText(self.QUESTION_DESCRIPTION[key])
		self.wndQuizState[key].SetWidth(400)
		self.wndQuizState[key].SetAcceptEvent(lambda arg = True, key = key: self.AnswerWithKey(arg, key))
		self.wndQuizState[key].SetCancelEvent(lambda arg = False, key = key: self.AnswerWithKey(arg, key))
		self.wndQuizState[key].Open()

	def Open(self):
		self.LoadSettings()
		self.Show()

	def Close(self):
		self.AskQuestionPacket(2)