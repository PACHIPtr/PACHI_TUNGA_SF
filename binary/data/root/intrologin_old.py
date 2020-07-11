import app
import net
import ime
import systemSetting
import dbg
import ui
import snd
import wndMgr
import musicInfo
import serverInfo
import ServerStateChecker
import localeInfo
import constInfo
import uiCommon
import time
import serverCommandParser
import uiScriptLocale
import os
import linecache

LOGIN_DELAY_SEC = 0.0
SKIP_LOGIN_PHASE = False
SKIP_LOGIN_PHASE_SUPPORT_CHANNEL = False
FULL_BACK_IMAGE = False

VIRTUAL_KEYBOARD_NUM_KEYS = 46
VIRTUAL_KEYBOARD_RAND_KEY = True

def Suffle(src):
	if VIRTUAL_KEYBOARD_RAND_KEY:
		items = [item for item in src]

		itemCount = len(items)
		for oldPos in xrange(itemCount):
			newPos = app.GetRandom(0, itemCount-1)
			items[newPos], items[oldPos] = items[oldPos], items[newPos]

		return "".join(items)
	else:
		return src

def IsFullBackImage():
	global FULL_BACK_IMAGE
	return FULL_BACK_IMAGE

def IsLoginDelay():
	global LOGIN_DELAY_SEC
	if LOGIN_DELAY_SEC > 0.0:
		return True
	else:
		return False

def GetLoginDelay():
	global LOGIN_DELAY_SEC
	return LOGIN_DELAY_SEC

app.SetGuildMarkPath("test")

class ConnectingDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.eventTimeOver = lambda *arg: None
		self.eventExit = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/ConnectingDialog.py")

			self.board = self.GetChild("board")
			self.message = self.GetChild("message")
			self.countdownMessage = self.GetChild("countdown_message")

		except:
			import exception
			exception.Abort("ConnectingDialog.LoadDialog.BindObject")

	def Open(self, waitTime):
		curTime = time.clock()
		self.endTime = curTime + waitTime

		self.Lock()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Unlock()
		self.Hide()

	def Destroy(self):
		self.Hide()
		self.ClearDictionary()

	def SetText(self, text):
		self.message.SetText(text)

	def SetCountDownMessage(self, waitTime):
		self.countdownMessage.SetText("%.0f%s" % (waitTime, localeInfo.SECOND))

	def SAFE_SetTimeOverEvent(self, event):
		self.eventTimeOver = ui.__mem_func__(event)

	def SAFE_SetExitEvent(self, event):
		self.eventExit = ui.__mem_func__(event)

	def OnUpdate(self):
		lastTime = max(0, self.endTime - time.clock())
		if 0 == lastTime:
			self.Close()
			self.eventTimeOver()
		else:
			self.SetCountDownMessage(self.endTime - time.clock())

	def OnPressExitKey(self):
		#self.eventExit()
		return True

class LoginWindow(ui.ScriptWindow):



	def __init__(self, stream):
		print "NEW LOGIN WINDOW  ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(self)

		self.lastLoginTime = 0
		self.inputDialog = None
		self.connectingDialog = None
		self.stream=stream
		self.isNowCountDown=False
		self.isStartError=False
		#self.questionDialog = None

		self.xServerBoard = 0
		self.yServerBoard = 0

		self.loadingImage = None

		self.virtualKeyboard = None
		self.virtualKeyboardMode = "ALPHABET"
		self.virtualKeyboardIsUpper = False

		# @fixme001 BEGIN (timeOutMsg and timeOutOk undefined)
		self.timeOutMsg = False
		self.timeOutOk = False
		# @fixme001 END

	def __del__(self):
		net.ClearPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(0)
		ui.ScriptWindow.__del__(self)
		print "---------------------------------------------------------------------------- DELETE LOGIN WINDOW"

	def Open(self):
		ServerStateChecker.Create(self)

		print "LOGIN WINDOW OPEN ----------------------------------------------------------------------------"

		self.loginFailureMsgDict={
			#"DEFAULT" : localeInfo.LOGIN_FAILURE_UNKNOWN,

			"ALREADY"	: localeInfo.LOGIN_FAILURE_ALREAY,
			"NOID"		: localeInfo.LOGIN_FAILURE_NOT_EXIST_ID,
			"WRONGPWD"	: localeInfo.LOGIN_FAILURE_WRONG_PASSWORD,
			"FULL"		: localeInfo.LOGIN_FAILURE_TOO_MANY_USER,
			"SHUTDOWN"	: localeInfo.LOGIN_FAILURE_SHUTDOWN,
			"REPAIR"	: localeInfo.LOGIN_FAILURE_REPAIR_ID,
			"BLOCK"		: localeInfo.LOGIN_FAILURE_BLOCK_ID,
			"BESAMEKEY"	: localeInfo.LOGIN_FAILURE_BE_SAME_KEY,
			"NOTAVAIL"	: localeInfo.LOGIN_FAILURE_NOT_AVAIL,
			"NOBILL"	: localeInfo.LOGIN_FAILURE_NOBILL,
			"BLKLOGIN"	: localeInfo.LOGIN_FAILURE_BLOCK_LOGIN,
			"WEBBLK"	: localeInfo.LOGIN_FAILURE_WEB_BLOCK,
			"BADSCLID"	: localeInfo.LOGIN_FAILURE_WRONG_SOCIALID,
			"AGELIMIT"	: localeInfo.LOGIN_FAILURE_SHUTDOWN_TIME,
			"HWBANNED"	: localeInfo.LOGIN_FAILURE_HW_BANNED,
			"HDD_BAN"	: localeInfo.LOGIN_FAILURE_HDD_BAN,
		}

		self.loginFailureFuncDict = {
			"WRONGPWD"	: self.__DisconnectAndInputPassword,
			"QUIT"		: app.Exit,
		}

		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		self.SetWindowName("LoginWindow")

		if not self.__LoadScript("UIScript/LoginWindow.py"):
			dbg.TraceError("LoginWindow.Open - __LoadScript Error")
			return

		if musicInfo.loginMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/"+musicInfo.loginMusic)

		snd.SetSoundVolume(systemSetting.GetSoundVolume())

		# pevent key "[" "]"
		ime.AddExceptKey(91)
		ime.AddExceptKey(93)

		self.Show()
		self.__RefreshServerList()
		self.__OpenServerBoard()

		app.ShowCursor()

	def Close(self):

		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		ServerStateChecker.Initialize(self)

		print "---------------------------------------------------------------------------- CLOSE LOGIN WINDOW "
		#
		# selectMusic¿Ã æ¯¿∏∏È BGM¿Ã ≤˜±‚π«∑Œ µŒ∞≥ ¥Ÿ √º≈©«—¥Ÿ.
		#
		if musicInfo.loginMusic != "" and musicInfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.loginMusic)

		## NOTE : idEditLineøÕ pwdEditLine¿∫ ¿Ã∫•∆Æ∞° º≠∑Œ ø¨∞· µ«æÓ¿÷æÓº≠
		##        Event∏¶ ∞≠¡¶∑Œ √ ±‚»≠ «ÿ¡÷æÓæﬂ∏∏ «’¥œ¥Ÿ - [levites]
		self.idEditLine.SetTabEvent(0)
		self.idEditLine.SetReturnEvent(0)
		self.pwdEditLine.SetReturnEvent(0)
		self.pwdEditLine.SetTabEvent(0)

		self.connectBoard = None
		self.kayitBoard = None
		self.loginBoard = None
		self.idEditLine = None
		self.pwdEditLine = None
		self.inputDialog = None
		self.connectingDialog = None
		self.loadingImage = None

		self.serverBoard				= None
		self.serverList					= None
		self.channelList				= None
		
		self.kaydetButton = None
		self.k1Button = None
		self.k2Button = None
		self.k3Button = None
		self.k4Button = None
		self.k5Button = None
		self.k6Button = None
		self.k7Button = None
		self.k8Button = None
		self.s1Button = None
		self.s2Button = None
		self.s3Button = None
		self.s4Button = None
		self.s5Button = None
		self.s6Button = None
		self.s7Button = None
		self.s8Button = None
		self.kayitBoard = None
		self.WebLink = None
		self.logo = None

		self.VIRTUAL_KEY_ALPHABET_LOWERS = None
		self.VIRTUAL_KEY_ALPHABET_UPPERS = None
		self.VIRTUAL_KEY_SYMBOLS = None
		self.VIRTUAL_KEY_NUMBERS = None

		# VIRTUAL_KEYBOARD_BUG_FIX
		if self.virtualKeyboard:
			for keyIndex in xrange(0, VIRTUAL_KEYBOARD_NUM_KEYS+1):
				key = self.GetChild2("key_%d" % keyIndex)
				if key:
					key.SetEvent(None)

			self.GetChild("key_space").SetEvent(None)
			self.GetChild("key_backspace").SetEvent(None)
			self.GetChild("key_enter").SetEvent(None)
			self.GetChild("key_shift").SetToggleDownEvent(None)
			self.GetChild("key_shift").SetToggleUpEvent(None)
			self.GetChild("key_at").SetToggleDownEvent(None)
			self.GetChild("key_at").SetToggleUpEvent(None)

			self.virtualKeyboard = None

		self.KillFocus()
		self.Hide()

		self.stream.popupWindow.Close()
		self.loginFailureFuncDict=None

		ime.ClearExceptKey()

		app.HideCursor()

	def __SaveChannelInfo(self):
		pass

	def __LoadChannelInfo(self):
		pass

	def __ExitGame(self):
		app.Exit()

	def SetIDEditLineFocus(self):
		if self.idEditLine != None:
			self.idEditLine.SetFocus()

	def SetPasswordEditLineFocus(self):
		if self.pwdEditLine != None:
			self.pwdEditLine.SetFocus()

	def OnEndCountDown(self):
		self.isNowCountDown = False
		self.timeOutMsg = False
		self.OnConnectFailure()

	def OnConnectFailure(self):

		if self.isNowCountDown:
			return

		snd.PlaySound("sound/ui/loginfail.wav")

		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		if self.timeOutMsg:
			self.PopupNotifyMessage(localeInfo.LOGIN_FAILURE_TIMEOUT, self.SetPasswordEditLineFocus)
		else:
			self.PopupNotifyMessage(localeInfo.LOGIN_CONNECT_FAILURE, self.SetPasswordEditLineFocus)

	def OnHandShake(self):
		if not IsLoginDelay():
			snd.PlaySound("sound/ui/loginok.wav")
			self.PopupDisplayMessage(localeInfo.LOGIN_CONNECT_SUCCESS)

	def OnLoginStart(self):
		if not IsLoginDelay():
			self.PopupDisplayMessage(localeInfo.LOGIN_PROCESSING)

	def OnLoginFailure(self, error):
		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		try:
			loginFailureMsg = self.loginFailureMsgDict[error]
		except KeyError:
			loginFailureMsg = localeInfo.LOGIN_FAILURE_UNKNOWN  + error

		loginFailureFunc=self.loginFailureFuncDict.get(error, self.SetPasswordEditLineFocus)
		self.PopupNotifyMessage(loginFailureMsg, loginFailureFunc)

		snd.PlaySound("sound/ui/loginfail.wav")

	def __DisconnectAndInputID(self):
		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		self.SetIDEditLineFocus()
		net.Disconnect()
		
	def __DisconnectAndInputPassword(self):
		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		self.SetPasswordEditLineFocus()
		net.Disconnect()

	def __LoadScript(self, fileName):
		import dbg
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.LoadObject")
		try:
			GetObject=self.GetChild
			self.backgroundImage = GetObject("BackGround")
			self.serverBoard			= GetObject("ServerBoard")
			self.serverList				= GetObject("ServerList")
			self.channelList			= GetObject("ChannelList")
			self.serverSelectButton		= GetObject("ServerSelectButton")
			self.serverExitButton		= GetObject("ServerExitButton")
			self.connectBoard			= GetObject("ConnectBoard")
			self.loginBoard				= GetObject("LoginBoard")
			self.idEditLine				= GetObject("ID_EditLine")
			self.pwdEditLine			= GetObject("Password_EditLine")
			self.serverInfo				= GetObject("ConnectName")
			self.selectConnectButton	= GetObject("SelectConnectButton")
			self.loginButton			= GetObject("LoginButton")
			self.loginExitButton		= GetObject("LoginExitButton")
			self.kaydetButton		= GetObject("KaydetConnectButton")
			self.k1Button		= GetObject("K1Button")
			self.k2Button		= GetObject("K2Button")
			self.k3Button		= GetObject("K3Button")
			self.k4Button		= GetObject("K4Button")
			self.k5Button		= GetObject("K5Button")
			self.k6Button		= GetObject("K6Button")
			self.k7Button		= GetObject("K7Button")
			self.k8Button		= GetObject("K8Button")
			self.s1Button		= GetObject("S1Button")
			self.s2Button		= GetObject("S2Button")
			self.s3Button		= GetObject("S3Button")
			self.s4Button		= GetObject("S4Button")
			self.s5Button		= GetObject("S5Button")
			self.s6Button		= GetObject("S6Button")
			self.s7Button		= GetObject("S7Button")
			self.s8Button		= GetObject("S8Button")
			self.kayitBoard			= GetObject("KayitBoard")
			self.WebLink		= GetObject("WebLink")
			self.logo			= GetObject("logo")
			
			self.butonliste = {}
			self.butonliste["slot1"] = self.k1Button
			self.butonliste["slot2"] = self.k2Button
			self.butonliste["slot3"] = self.k3Button
			self.butonliste["slot4"] = self.k4Button
			self.butonliste["slot5"] = self.k5Button
			self.butonliste["slot6"] = self.k6Button
			self.butonliste["slot7"] = self.k7Button
			self.butonliste["slot8"] = self.k8Button


			self.virtualKeyboard		= self.GetChild2("VirtualKeyboard")

			if self.virtualKeyboard:
				self.VIRTUAL_KEY_ALPHABET_UPPERS = Suffle(localeInfo.VIRTUAL_KEY_ALPHABET_UPPERS)
				self.VIRTUAL_KEY_ALPHABET_LOWERS = "".join([localeInfo.VIRTUAL_KEY_ALPHABET_LOWERS[localeInfo.VIRTUAL_KEY_ALPHABET_UPPERS.index(e)] for e in self.VIRTUAL_KEY_ALPHABET_UPPERS])
				self.VIRTUAL_KEY_SYMBOLS = Suffle(localeInfo.VIRTUAL_KEY_SYMBOLS)
				self.VIRTUAL_KEY_NUMBERS = Suffle(localeInfo.VIRTUAL_KEY_NUMBERS)
				self.__VirtualKeyboard_SetAlphabetMode()

				self.GetChild("key_space").SetEvent(lambda : self.__VirtualKeyboard_PressKey(' '))
				self.GetChild("key_backspace").SetEvent(lambda : self.__VirtualKeyboard_PressBackspace())
				self.GetChild("key_enter").SetEvent(lambda : self.__VirtualKeyboard_PressReturn())
				self.GetChild("key_shift").SetToggleDownEvent(lambda : self.__VirtualKeyboard_SetUpperMode())
				self.GetChild("key_shift").SetToggleUpEvent(lambda : self.__VirtualKeyboard_SetLowerMode())
				self.GetChild("key_at").SetToggleDownEvent(lambda : self.__VirtualKeyboard_SetSymbolMode())
				self.GetChild("key_at").SetToggleUpEvent(lambda : self.__VirtualKeyboard_SetAlphabetMode())
				
			if wndMgr.GetScreenHeight() < 1000:
				self.logo.Hide()
				self.WebLink.Hide()

		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.BindObject")


		self.selectConnectButton.SetEvent(ui.__mem_func__(self.__OnClickSelectConnectButton))

		self.serverBoard.OnKeyUp = ui.__mem_func__(self.__ServerBoard_OnKeyUp)
		self.xServerBoard, self.yServerBoard = self.serverBoard.GetLocalPosition()

		self.serverSelectButton.SetEvent(ui.__mem_func__(self.__OnClickSelectServerButton))
		self.serverExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitButton))

		self.loginButton.SetEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.loginExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitButton))

		self.serverList.SetEvent(ui.__mem_func__(self.__OnSelectServer))

		self.idEditLine.SetReturnEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
		self.idEditLine.SetTabEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))

		self.pwdEditLine.SetReturnEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.pwdEditLine.SetTabEvent(ui.__mem_func__(self.idEditLine.SetFocus))
		
		self.kaydetButton.SetEvent(ui.__mem_func__(self.__OnClickKaydetButton))
		
		self.k1Button.SetEvent(ui.__mem_func__(self.__OnClickK1Button))
		self.k2Button.SetEvent(ui.__mem_func__(self.__OnClickK2Button))
		self.k3Button.SetEvent(ui.__mem_func__(self.__OnClickK3Button))
		self.k4Button.SetEvent(ui.__mem_func__(self.__OnClickK4Button))
		self.k5Button.SetEvent(ui.__mem_func__(self.__OnClickK5Button))
		self.k6Button.SetEvent(ui.__mem_func__(self.__OnClickK6Button))
		self.k7Button.SetEvent(ui.__mem_func__(self.__OnClickK7Button))
		self.k8Button.SetEvent(ui.__mem_func__(self.__OnClickK8Button))
		
		self.s1Button.SetEvent(ui.__mem_func__(self.__OnClickS1Button))
		self.s2Button.SetEvent(ui.__mem_func__(self.__OnClickS2Button))
		self.s3Button.SetEvent(ui.__mem_func__(self.__OnClickS3Button))
		self.s4Button.SetEvent(ui.__mem_func__(self.__OnClickS4Button))
		self.s5Button.SetEvent(ui.__mem_func__(self.__OnClickS5Button))
		self.s6Button.SetEvent(ui.__mem_func__(self.__OnClickS6Button))
		self.s7Button.SetEvent(ui.__mem_func__(self.__OnClickS7Button))
		self.s8Button.SetEvent(ui.__mem_func__(self.__OnClickS8Button))
		
		self.WebLink.SetEvent(ui.__mem_func__(self.__OnClickWebLinkButton))
		
		imgFileNameDict = {
			0 : "d:/ymir work/ui/intro/login/login.sub",
			#1 : "d:/ymir work/ui/intro/login/2.sub",
			#2 : "d:/ymir work/ui/intro/login/3.sub",
			#3 : "d:/ymir work/ui/intro/login/4.sub",
		}
		try:
			imgFileName = imgFileNameDict[app.GetRandom(0, len(imgFileNameDict) - 1)]
			self.backgroundImage.LoadImage(imgFileName)
		except:
			print "LoadingWindow.Open.LoadImage - %s File Load Error" % imgFileName
			self.backgroundImage.Hide()
		width = float(wndMgr.GetScreenWidth()) / float(self.backgroundImage.GetWidth())
		height = float(wndMgr.GetScreenHeight()) / float(self.backgroundImage.GetHeight())
		self.backgroundImage.SetScale(width, height)
		return 1

	def __VirtualKeyboard_SetKeys(self, keyCodes):
		uiDefFontBackup = localeInfo.UI_DEF_FONT
		localeInfo.UI_DEF_FONT = localeInfo.UI_DEF_FONT_LARGE

		keyIndex = 1
		for keyCode in keyCodes:
			key = self.GetChild2("key_%d" % keyIndex)
			if key:
				key.SetEvent(lambda x=keyCode: self.__VirtualKeyboard_PressKey(x))
				key.SetText(keyCode)
				key.ButtonText.SetFontColor(0, 0, 0)
				keyIndex += 1

		for keyIndex in xrange(keyIndex, VIRTUAL_KEYBOARD_NUM_KEYS+1):
			key = self.GetChild2("key_%d" % keyIndex)
			if key:
				key.SetEvent(lambda x=' ': self.__VirtualKeyboard_PressKey(x))
				key.SetText(' ')

		localeInfo.UI_DEF_FONT = uiDefFontBackup

	def __VirtualKeyboard_PressKey(self, code):
		ime.PasteString(code)

		#if self.virtualKeyboardMode == "ALPHABET" and self.virtualKeyboardIsUpper:
		#	self.__VirtualKeyboard_SetLowerMode()

	def __VirtualKeyboard_PressBackspace(self):
		ime.PasteBackspace()

	def __VirtualKeyboard_PressReturn(self):
		ime.PasteReturn()

	def __VirtualKeyboard_SetUpperMode(self):
		self.virtualKeyboardIsUpper = True

		if self.virtualKeyboardMode == "ALPHABET":
			self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_ALPHABET_UPPERS)
		elif self.virtualKeyboardMode == "NUMBER":
			self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_SYMBOLS)
		else:
			self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_NUMBERS)

	def __VirtualKeyboard_SetLowerMode(self):
		self.virtualKeyboardIsUpper = False

		if self.virtualKeyboardMode == "ALPHABET":
			self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_ALPHABET_LOWERS)
		elif self.virtualKeyboardMode == "NUMBER":
			self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_NUMBERS)
		else:
			self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_SYMBOLS)

	def __VirtualKeyboard_SetAlphabetMode(self):
		self.virtualKeyboardIsUpper = False
		self.virtualKeyboardMode = "ALPHABET"
		self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_ALPHABET_LOWERS)

	def __VirtualKeyboard_SetNumberMode(self):
		self.virtualKeyboardIsUpper = False
		self.virtualKeyboardMode = "NUMBER"
		self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_NUMBERS)

	def __VirtualKeyboard_SetSymbolMode(self):
		self.virtualKeyboardIsUpper = False
		self.virtualKeyboardMode = "SYMBOL"
		self.__VirtualKeyboard_SetKeys(self.VIRTUAL_KEY_SYMBOLS)

	def Connect(self, id, pwd):
		if IsLoginDelay():
			loginDelay = GetLoginDelay()
			self.connectingDialog = ConnectingDialog()
			self.connectingDialog.Open(loginDelay)
			self.connectingDialog.SAFE_SetTimeOverEvent(self.OnEndCountDown)
			self.connectingDialog.SAFE_SetExitEvent(self.OnPressExitKey)
			self.isNowCountDown = True

		else:
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.LOGIN_CONNETING, self.SetPasswordEditLineFocus, localeInfo.UI_CANCEL)
			
		if self.idEditLine.GetText():
			constInfo.ACCOUNT_NAME = str(self.idEditLine.GetText())
		else:
			constInfo.ACCOUNT_NAME = str(id)

		if id != 0:
			self.stream.SetLoginInfo(id, pwd)
		self.stream.Connect()				

	def __OnClickExitButton(self):
		self.stream.SetPhaseWindow(0)

	def __SetServerInfo(self, name):
		net.SetServerInfo(name.strip())
		self.serverInfo.SetText(name)

	def PopupDisplayMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg)

	def PopupNotifyMessage(self, msg, func=0):
		if not func:
			func=self.EmptyFunc

		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

	def __OnCloseInputDialog(self):
		if self.inputDialog:
			self.inputDialog.Close()
		self.inputDialog = None
		return True

	def OnPressExitKey(self):
		self.stream.popupWindow.Close()
		self.stream.SetPhaseWindow(0)
		return True

	def OnUpdate(self):
		ServerStateChecker.Update()
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap1.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap1.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k1Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k1Button.SetText(localeInfo.LOGIN_BOS)
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap2.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap2.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k2Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k2Button.SetText(localeInfo.LOGIN_BOS)
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap3.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap3.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k3Button.SetText(app.DecryptByHWID(bol[1]))
		else:	
			self.k3Button.SetText(localeInfo.LOGIN_BOS)
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap4.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap4.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k4Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k4Button.SetText(localeInfo.LOGIN_BOS)
			
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap5.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap5.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k5Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k5Button.SetText(localeInfo.LOGIN_BOS)
			
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap6.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap6.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k6Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k6Button.SetText(localeInfo.LOGIN_BOS)
			
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap7.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap7.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k7Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k7Button.SetText(localeInfo.LOGIN_BOS)
			
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap8.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap8.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k8Button.SetText(app.DecryptByHWID(bol[1]))
		else:
			self.k8Button.SetText(localeInfo.LOGIN_BOS)

	def EmptyFunc(self):
		pass

	#####################################################################################

	def __ServerBoard_OnKeyUp(self, key):
		if self.serverBoard.IsShow():
			if app.DIK_RETURN==key:
				self.__OnClickSelectServerButton()
		return True

	def __GetRegionID(self):
		return 0

	def __GetServerID(self):
		return self.serverList.GetSelectedItem()

	def __GetChannelID(self):
		return self.channelList.GetSelectedItem()

	# SEVER_LIST_BUG_FIX
	def __ServerIDToServerIndex(self, regionID, targetServerID):
		try:
			regionDict = serverInfo.REGION_DICT[regionID]
		except KeyError:
			return -1

		retServerIndex = 0
		for eachServerID, regionDataDict in regionDict.items():
			if eachServerID == targetServerID:
				return retServerIndex

			retServerIndex += 1

		return -1

	def __ChannelIDToChannelIndex(self, channelID):
		return channelID - 1
	# END_OF_SEVER_LIST_BUG_FIX

	def __OpenServerBoard(self):

		loadRegionID, loadServerID, loadChannelID = self.__LoadChannelInfo()

		serverIndex = self.__ServerIDToServerIndex(loadRegionID, loadServerID)
		channelIndex = self.__ChannelIDToChannelIndex(loadChannelID)

		self.serverList.SelectItem(serverIndex)
		self.channelList.SelectItem(app.GetRandom(0, self.channelList.GetItemCount()))

		## Show/Hide ƒ⁄µÂø° πÆ¡¶∞° ¿÷æÓº≠ ¿”Ω√ - [levites]
		self.serverBoard.SetPosition(self.xServerBoard, self.yServerBoard)
		self.serverBoard.Show()
		self.connectBoard.Hide()
		self.kayitBoard.Hide()
		self.loginBoard.Hide()

		if self.virtualKeyboard:
			self.virtualKeyboard.Hide()

	def __OpenLoginBoard(self):

		self.serverExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitServerButton))
		self.serverExitButton.SetText(localeInfo.UI_CLOSE)

		self.serverBoard.SetPosition(self.xServerBoard, wndMgr.GetScreenHeight())
		self.serverBoard.Hide()

		if self.virtualKeyboard:
			self.virtualKeyboard.Show()

		if not self.stream.isAutoLogin:
			self.connectBoard.Show()
			self.kayitBoard.Show()
			self.loginBoard.Show()

		## if users have the login infomation, then don't initialize.2005.9 haho
		if self.idEditLine == None:
			self.idEditLine.SetText("")
		if self.pwdEditLine == None:
			self.pwdEditLine.SetText("")

		self.idEditLine.SetFocus()

		global SKIP_LOGIN_PHASE
		if SKIP_LOGIN_PHASE:
			if not self.loginInfo:
				self.connectBoard.Hide()
				self.kayitBoard.Hide()

	def __OnSelectRegionGroup(self):
		self.__RefreshServerList()

	def __OnSelectSettlementArea(self):
		# SEVER_LIST_BUG_FIX
		regionID = self.__GetRegionID()
		serverID = self.serverListOnRegionBoard.GetSelectedItem()

		serverIndex = self.__ServerIDToServerIndex(regionID, serverID)
		self.serverList.SelectItem(serverIndex)
		# END_OF_SEVER_LIST_BUG_FIX

		self.__OnSelectServer()

	def __RefreshServerList(self):
		regionID = self.__GetRegionID()

		if not serverInfo.REGION_DICT.has_key(regionID):
			return

		self.serverList.ClearItem()

		regionDict = serverInfo.REGION_DICT[regionID]

		# SEVER_LIST_BUG_FIX
		visible_index = 1
		for id, regionDataDict in regionDict.items():
			name = regionDataDict.get("name", "noname")
			try:
				server_id = serverInfo.SERVER_ID_DICT[id]
			except:
				server_id = visible_index

			self.serverList.InsertItem(id, "  %02d. %s" % (int(server_id), name))					
			visible_index += 1

	def __OnSelectServer(self):
		self.__OnCloseInputDialog()
		self.__RequestServerStateList()
		self.__RefreshServerStateList()

	def __RequestServerStateList(self):
		regionID = self.__GetRegionID()
		serverID = self.__GetServerID()

		try:
			channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		except:
			print " __RequestServerStateList - serverInfo.REGION_DICT(%d, %d)" % (regionID, serverID)
			return

		ServerStateChecker.Initialize();
		for id, channelDataDict in channelDict.items():
			key=channelDataDict["key"]
			ip=channelDataDict["ip"]
			udp_port=channelDataDict["udp_port"]
			ServerStateChecker.AddChannel(key, ip, udp_port)

		ServerStateChecker.Request()

	def __RefreshServerStateList(self):

		regionID = self.__GetRegionID()
		serverID = self.__GetServerID()
		bakChannelID = self.channelList.GetSelectedItem()

		self.channelList.ClearItem()

		try:
			channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		except:
			print " __RequestServerStateList - serverInfo.REGION_DICT(%d, %d)" % (regionID, serverID)
			return

		for channelID, channelDataDict in channelDict.items():
			channelName = channelDataDict["name"]
			channelState = channelDataDict["state"]
			self.channelList.InsertItem(channelID, " %s %s" % (channelName, channelState))

		self.channelList.SelectItem(bakChannelID-1)

	def __GetChannelName(self, regionID, selServerID, selChannelID):
		try:
			return serverInfo.REGION_DICT[regionID][selServerID]["channel"][selChannelID]["name"]
		except KeyError:
			if 9==selChannelID:
				return localeInfo.CHANNEL_PVP
			else:
				return localeInfo.CHANNEL_NORMAL % (selChannelID)

	def NotifyChannelState(self, addrKey, state):
		try:
			stateName=serverInfo.STATE_DICT[state]
		except:
			stateName=serverInfo.STATE_NONE

		regionID=int(addrKey/1000)
		serverID=int(addrKey/10) % 100
		channelID=addrKey%10

		try:
			serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["state"] = stateName
			self.__RefreshServerStateList()

		except:
			import exception
			exception.Abort(localeInfo.CHANNEL_NOT_FIND_INFO)

	def __OnClickExitServerButton(self):
		print "exit server"
		self.__OpenLoginBoard()

	def __OnClickSelectRegionButton(self):
		regionID = self.__GetRegionID()
		serverID = self.__GetServerID()

		if (not serverInfo.REGION_DICT.has_key(regionID)):
			self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_REGION)
			return

		if (not serverInfo.REGION_DICT[regionID].has_key(serverID)):
			self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_SERVER)
			return

		self.__SaveChannelInfo()

		self.serverExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitServerButton))
		self.serverExitButton.SetText(localeInfo.UI_CLOSE)

		self.__RefreshServerList()
		self.__OpenServerBoard()

	def __OnClickSelectServerButton(self):
		regionID = self.__GetRegionID()
		serverID = self.__GetServerID()
		channelID = self.__GetChannelID()

		if (not serverInfo.REGION_DICT.has_key(regionID)):
			self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_REGION)
			return

		if (not serverInfo.REGION_DICT[regionID].has_key(serverID)):
			self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_SERVER)
			return

		try:
			channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		except KeyError:
			return

		try:
			state = channelDict[channelID]["state"]
		except KeyError:
			self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_CHANNEL)
			return

		# ªÛ≈¬∞° FULL ∞˙ ∞∞¿∏∏È ¡¯¿‘ ±›¡ˆ
		if state == serverInfo.STATE_DICT[3]:
			self.PopupNotifyMessage(localeInfo.CHANNEL_NOTIFY_FULL)
			return

		self.__SaveChannelInfo()

		try:
			serverName = serverInfo.REGION_DICT[regionID][serverID]["name"]
			channelName = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["name"]
			addrKey = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["key"]

			if "√µ∏∂ º≠πˆ" == serverName:
				app.ForceSetLocale("ymir", "locale/ymir")
			elif "ƒËµµ º≠πˆ" == serverName:
				app.ForceSetLocale("we_korea", "locale/we_korea")

		except:
			print " ERROR __OnClickSelectServerButton(%d, %d, %d)" % (regionID, serverID, channelID)
			serverName = localeInfo.CHANNEL_EMPTY_SERVER
			channelName = localeInfo.CHANNEL_NORMAL % channelID

		self.__SetServerInfo("%s, %s " % (serverName, channelName))

		try:
			ip = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["ip"]
			tcp_port = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["tcp_port"]
		except:
			import exception
			exception.Abort("LoginWindow.__OnClickSelectServerButton - º≠πˆ º±≈√ Ω«∆–")

		try:
			account_ip = serverInfo.REGION_AUTH_SERVER_DICT[regionID][serverID]["ip"]
			account_port = serverInfo.REGION_AUTH_SERVER_DICT[regionID][serverID]["port"]
		except:
			account_ip = 0
			account_port = 0

		try:
			markKey = regionID*1000 + serverID*10
			markAddrValue=serverInfo.MARKADDR_DICT[markKey]
			net.SetMarkServer(markAddrValue["ip"], markAddrValue["tcp_port"])
			app.SetGuildMarkPath(markAddrValue["mark"])
			# GUILD_SYMBOL
			app.SetGuildSymbolPath(markAddrValue["symbol_path"])
			# END_OF_GUILD_SYMBOL

		except:
			import exception
			exception.Abort("LoginWindow.__OnClickSelectServerButton - ∏∂≈© ¡§∫∏ æ¯¿Ω")

		self.stream.SetConnectInfo(ip, tcp_port, account_ip, account_port)
		if ip == serverInfo.SERVER_IP:
			constInfo.SERVER_TYPE = 1
		elif ip == serverInfo.SERVER_IP_2:
			constInfo.SERVER_TYPE = 2
		elif ip == serverInfo.SERVER_IP_3:
			constInfo.SERVER_TYPE = 3
		self.__OpenLoginBoard()

	def __OnClickSelectConnectButton(self):
		self.__RefreshServerList()
		self.__OpenServerBoard()

	def __OnClickLoginButton(self):
		global HESAPID
		global HESAPSIFRE
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()

		if len(id)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_ID, self.SetIDEditLineFocus)
			return

		if len(pwd)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PASSWORD, self.SetPasswordEditLineFocus)
			return

		self.Connect(id, pwd)
		
	def __OnClickKaydetButton(self):
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()
		if id == "" or pwd == "":
			self.PopupNotifyMessage(localeInfo.LOGIN_KAYDET_BOS)
		else:
			for i in range(1, 9):
				if not os.path.exists(str(constInfo.CLIENT_YOL)+'hesap'+str(i)+'.cfg'):
					open(str(constInfo.CLIENT_YOL)+'hesap'+str(i)+'.cfg', "w").write("|"+app.EncryptByHWID(str(id))+"|"+app.EncryptByHWID(str(pwd))+"|")
					self.butonliste["slot"+str(i)].SetText(str(id))
					self.PopupNotifyMessage(localeInfo.LOGIN_KAYITOK)
					break
				else:
					pass
				if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap1.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap2.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap3.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap4.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap5.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap6.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap7.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap8.cfg'):
					self.PopupNotifyMessage(localeInfo.LOGIN_TUMDOLU)
					break
					
	def __OnClickK1Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k1Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap1.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK2Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k2Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap2.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK3Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k3Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap3.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK4Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k4Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap4.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK5Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k5Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap5.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK6Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k6Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap6.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK7Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k7Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap7.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK8Button(self):
		global HESAPID
		global HESAPSIFRE
		al = self.k8Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Girilicek bir hesap yok!')
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap8.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
	
	def __OnClickS1Button(self):
		al = self.k1Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap1.cfg')
			self.k1Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS2Button(self):
		al = self.k2Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap2.cfg')
			self.k2Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS3Button(self):
		al = self.k3Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap3.cfg')
			self.k3Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS4Button(self):
		al = self.k4Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap4.cfg')
			self.k4Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS5Button(self):
		al = self.k5Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap5.cfg')
			self.k5Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS6Button(self):
		al = self.k6Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap6.cfg')
			self.k6Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS7Button(self):
		al = self.k7Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap7.cfg')
			self.k7Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickS8Button(self):
		al = self.k8Button.GetText()
		if al.find("Bo˛") != -1:
			self.PopupNotifyMessage('Silinecek bir hesap yok!')
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap8.cfg')
			self.k8Button.SetText("Bo˛")
			self.PopupNotifyMessage('Hesap silindi!')
			
	def __OnClickWebLinkButton(self):
		os.startfile("https://www.myte2.com/site/register")