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
import localeinfo
import constInfo
import datetime
import uiCommon
import time
import serverCommandParser
import uiScriptLocale
import os
import linecache

def SecondToHM(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24

	if hour <= 0:
		return "%d Dakika %02d Saniye" % (minute, second)
	else:
		return "%d Saat %02d Dakika %02d Saniye" % (hour, minute, second)

LOGIN_DELAY_SEC = 0.0
SKIP_LOGIN_PHASE = False
SKIP_LOGIN_PHASE_SUPPORT_CHANNEL = False
FULL_BACK_IMAGE = False

VIRTUAL_KEYBOARD_NUM_KEYS = 46
VIRTUAL_KEYBOARD_RAND_KEY = True

SERVER_IP = serverInfo.SERVER_IP

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


FAIL_LOGIN_COUNTER = 0
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
		self.channelButtonList = []

		self.loadingImage = None

		self.virtualKeyboard = None
		self.virtualKeyboardMode = "ALPHABET"
		self.virtualKeyboardIsUpper = False
		# self.FlagButtonList = []
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
			"SECUREPC"	: localeInfo.LOGIN_FAILURE_SECUREPC,
			"HDD_BAN"	: localeInfo.LOGIN_FAILURE_HDD_BAN,
			"NOCLIENT"	: localeInfo.LOGIN_FAILURE_CLIENT_VERSION,
			"NOPIN"		: localeInfo.LOGIN_FAILURE_PIN,
			"NOSERVER"		: localeInfo.LOGIN_FAILURE_SERVER,
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
		connectingIP = self.stream.GetConnectAddr()
		if connectingIP:
			self.__OpenConnectBoard()

		self.Show()
	#	self.PopupNotifyMessage(localeInfo.LOGIN_SUCCES_NO + str(constInfo.LOGIN_SUCCES_STATUS))

		#self.__RefreshServerList()
		self.__OpenServerBoard()
		self.RefreshChannelButtonList()
		self.__SetChannel(1)
		self.idEditLine.SetFocus()
		app.ShowCursor()

	def Close(self):

		if self.connectingDialog:
			self.connectingDialog.Close()
		self.connectingDialog = None

		ServerStateChecker.Initialize(self)

		print "---------------------------------------------------------------------------- CLOSE LOGIN WINDOW "
		#
		# selectMusicÀÌ ¾øÀ¸¸é BGMÀÌ ²÷±â¹Ç·Î µÎ°³ ´Ù Ã¼Å©ÇÑ´Ù.
		#
		if musicInfo.loginMusic != "" and musicInfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.loginMusic)

		## NOTE : idEditLine¿Í pwdEditLineÀº ÀÌº¥Æ®°¡ ¼­·Î ¿¬°á µÇ¾îÀÖ¾î¼­
		##        Event¸¦ °­Á¦·Î ÃÊ±âÈ­ ÇØÁÖ¾î¾ß¸¸ ÇÕ´Ï´Ù - [levites]
		self.idEditLine.SetTabEvent(0)
		self.idEditLine.SetReturnEvent(0)
		self.pwdEditLine.SetReturnEvent(0)
		self.pwdEditLine.SetTabEvent(0)
		
		self.pinEditLine.SetReturnEvent(0)
		self.pinEditLine.SetTabEvent(0)

		self.connectBoard = None
		#self.kayitBoard = None
		#self.loginBoard = None
		# self.languageBoard = None
		self.idEditLine = None
		self.pwdEditLine = None
		self.pinEditLine = None
		self.inputDialog = None
		self.connectingDialog = None
		self.loadingImage = None

		# self.serverBoard				= None
		# self.serverList					= None
		#self.channelList				= None
		self.kaydetButton = None
		self.k1Button = None
		self.k2Button = None
		self.k3Button = None
		self.k4Button = None
		# self.s1Button = None
		# self.s2Button = None
		# self.s3Button = None
		# self.s4Button = None
		#self.kayitBoard = None
		self.WebLink = None
		# self.logo = None

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

			#self.GetChild("key_space").SetEvent(None)
			#self.GetChild("key_backspace").SetEvent(None)
			#self.GetChild("key_enter").SetEvent(None)
			#self.GetChild("key_shift").SetToggleDownEvent(None)
			#self.GetChild("key_shift").SetToggleUpEvent(None)
			#self.GetChild("key_at").SetToggleDownEvent(None)
			#self.GetChild("key_at").SetToggleUpEvent(None)

			self.virtualKeyboard = None

		self.KillFocus()
		self.Hide()

		self.stream.popupWindow.Close()
		self.loginFailureFuncDict=None

		ime.ClearExceptKey()

		app.HideCursor()

	def __SaveChannelInfo(self):
		try:
			file=old_open("channel.inf", "w")
			file.write("%d %d %d" % (self.__GetServerID(), self.__GetChannelID(), self.__GetRegionID()))
		except:
			print "LoginWindow.__SaveChannelInfo - SaveError"

	def __LoadChannelInfo(self):
		try:
			file=old_open("channel.inf")
			lines=file.readlines()

			if len(lines)>0:
				tokens=lines[0].split()

				selServerID=int(tokens[0])
				selChannelID=int(tokens[1])

				if len(tokens) == 3:
					regionID = int(tokens[2])

				return regionID, selServerID, selChannelID

		except:
			print "LoginWindow.__LoadChannelInfo - OpenError"
			return -1, -1, -1

	def __ExitGame(self):
		app.Exit()

	def SetIDEditLineFocus(self):
		if self.idEditLine != None:
			self.idEditLine.SetFocus()

	def SetPasswordEditLineFocus(self):
		if self.pwdEditLine != None:
			self.pwdEditLine.SetFocus()
			
	def SetPinEditLineFocus(self):
		if self.pinEditLine != None:
			self.pinEditLine.SetFocus()

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
			self.PopupNotifyMessage(localeInfo.LOGIN_FAILURE_TIMEOUT, self.SetPinEditLineFocus)
		else:
			self.PopupNotifyMessage(localeInfo.LOGIN_CONNECT_FAILURE, self.SetPinEditLineFocus)

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

		loginFailureFunc=self.loginFailureFuncDict.get(error, self.SetPinEditLineFocus)
		self.PopupNotifyMessage(loginFailureMsg, loginFailureFunc)

		if str(loginFailureMsg) == str(localeInfo.LOGIN_FAILURE_BLOCK_LOGIN) or str(loginFailureMsg) == str(localeInfo.LOGIN_FAILURE_HDD_BAN):
			snd.PlaySound('sound/ui/ban.wav')
			x, y = self.GetGlobalPosition()
			sx, sy = x + 10, y + 30
	
			app.ShowWebPage("https://i.hizliresim.com/jgZ65L.gif", (sx, sy, 500, 320))
		else:
			snd.PlaySound("sound/ui/loginfail.wav")
		
		global FAIL_LOGIN_COUNTER
		FAIL_LOGIN_COUNTER += 1
		if (FAIL_LOGIN_COUNTER >= 7):
			xxx = app.GetGlobalTimeStamp()+600
			fail = open("lib/login_time", "w")
			fail.write(str(xxx))
			FAIL_LOGIN_COUNTER = 0

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

		self.SetPinEditLineFocus()
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
			# self.serverBoard			= GetObject("ServerBoard")
			# self.serverList				= GetObject("ServerList")
			# self.channelList			= GetObject("ChannelList")
			# self.serverSelectButton		= GetObject("ServerSelectButton")
			# self.serverExitButton		= GetObject("ServerExitButton")
			# self.loginBoard				= GetObject("LoginBoard")
			# self.languageBoard			= GetObject("LanguageBoard")


			self.connectBoard			= GetObject("ConnectBoard")
			self.idEditLine				= GetObject("ID_EditLine")
			self.pwdEditLine			= GetObject("Password_EditLine")
			self.pinEditLine			= GetObject("Pin_EditLine")
			
			self.channelButtonList.append(GetObject("Channel1Button"))
			self.channelButtonList.append(GetObject("Channel2Button"))
			self.channelButtonList.append(GetObject("Channel3Button"))
			self.channelButtonList.append(GetObject("Channel4Button"))
			self.channelButtonList.append(GetObject("Channel5Button"))
			self.channelButtonList.append(GetObject("Channel6Button"))
			self.Channel1Status = GetObject("Channel1Status")
			self.Channel2Status = GetObject("Channel2Status")
			self.Channel3Status = GetObject("Channel3Status")
			self.Channel4Status = GetObject("Channel4Status")
			self.Channel5Status = GetObject("Channel5Status")
			self.Channel6Status = GetObject("Channel6Status")
			
			# self.FlagButtonList.append(GetObject("TR"))
			# self.FlagButtonList.append(GetObject("DE"))
			# self.FlagButtonList.append(GetObject("EN"))
			
			# self.serverInfo				= GetObject("ConnectName")
			# self.selectConnectButton	= GetObject("SelectConnectButton")
			self.loginButton			= GetObject("LoginButton")
			self.loginExitButton		= GetObject("LoginExitButton")
			self.kaydetButton		= GetObject("KaydetConnectButton")
			self.k1Button		= GetObject("K1Button")
			self.k2Button		= GetObject("K2Button")
			self.k3Button		= GetObject("K3Button")
			self.k4Button		= GetObject("K4Button")
			self.s1Button		= GetObject("S1Button")
			self.s2Button		= GetObject("S2Button")
			self.s3Button		= GetObject("S3Button")
			self.s4Button		= GetObject("S4Button")
			# self.kayitBoard			= GetObject("KayitBoard")
			self.WebLink		= GetObject("WebLink")
			# self.logo			= GetObject("logo")
			
			self.butonliste = {}
			self.butonliste["slot1"] = self.k1Button
			self.butonliste["slot2"] = self.k2Button
			self.butonliste["slot3"] = self.k3Button
			self.butonliste["slot4"] = self.k4Button

			#self.virtualKeyboard		= self.GetChild2("VirtualKeyboard")

			#if self.virtualKeyboard:
			#	self.VIRTUAL_KEY_ALPHABET_UPPERS = Suffle(localeInfo.VIRTUAL_KEY_ALPHABET_UPPERS)
			#	self.VIRTUAL_KEY_ALPHABET_LOWERS = "".join([localeInfo.VIRTUAL_KEY_ALPHABET_LOWERS[localeInfo.VIRTUAL_KEY_ALPHABET_UPPERS.index(e)] for e in self.VIRTUAL_KEY_ALPHABET_UPPERS])
			#	if localeInfo.IsBRAZIL():
			#		self.VIRTUAL_KEY_SYMBOLS_BR = Suffle(localeInfo.VIRTUAL_KEY_SYMBOLS_BR)
			#	else:
			#		self.VIRTUAL_KEY_SYMBOLS = Suffle(localeInfo.VIRTUAL_KEY_SYMBOLS)
				#self.VIRTUAL_KEY_NUMBERS = Suffle(localeInfo.VIRTUAL_KEY_NUMBERS)
				#self.__VirtualKeyboard_SetAlphabetMode()

				#self.GetChild("key_space").SetEvent(lambda : self.__VirtualKeyboard_PressKey(' '))
				#self.GetChild("key_backspace").SetEvent(lambda : self.__VirtualKeyboard_PressBackspace())
				#self.GetChild("key_enter").SetEvent(lambda : self.__VirtualKeyboard_PressReturn())
				#self.GetChild("key_shift").SetToggleDownEvent(lambda : self.__VirtualKeyboard_SetUpperMode())
				#self.GetChild("key_shift").SetToggleUpEvent(lambda : self.__VirtualKeyboard_SetLowerMode())
				#self.GetChild("key_at").SetToggleDownEvent(lambda : self.__VirtualKeyboard_SetSymbolMode())
				#self.GetChild("key_at").SetToggleUpEvent(lambda : self.__VirtualKeyboard_SetAlphabetMode())
				
			##if wndMgr.GetScreenHeight() < 1000:
			# self.logo.Hide()
			self.WebLink.Hide()

		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.BindObject")

		# if self.IS_TEST:
			# self.selectConnectButton.Hide()
		# else:
			# self.selectConnectButton.SetEvent(ui.__mem_func__(self.__OnClickSelectConnectButton))

		#self.serverBoard.OnKeyUp = ui.__mem_func__(self.__ServerBoard_OnKeyUp)
		# self.xServerBoard, self.yServerBoard = self.serverBoard.GetLocalPosition()

		#self.serverSelectButton.SetEvent(ui.__mem_func__(self.__OnClickSelectServerButton))
		#self.serverExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitButton))

		self.loginButton.SetEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.loginExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitButton))

		# self.serverList.SetEvent(ui.__mem_func__(self.__OnSelectServer))

		self.idEditLine.SetReturnEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))
		self.idEditLine.SetTabEvent(ui.__mem_func__(self.pwdEditLine.SetFocus))

		self.pwdEditLine.SetReturnEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.pwdEditLine.SetTabEvent(ui.__mem_func__(self.pinEditLine.SetFocus))
		
		self.pinEditLine.SetReturnEvent(ui.__mem_func__(self.__OnClickLoginButton))
		self.pinEditLine.SetTabEvent(ui.__mem_func__(self.idEditLine.SetFocus))
		
		self.kaydetButton.SetEvent(ui.__mem_func__(self.__OnClickKaydetButton))
		
		self.channelButtonList[0].SAFE_SetEvent(self.__SetChannel, 1)
		self.channelButtonList[1].SAFE_SetEvent(self.__SetChannel, 2)
		self.channelButtonList[2].SAFE_SetEvent(self.__SetChannel, 3)
		self.channelButtonList[3].SAFE_SetEvent(self.__SetChannel, 4)
		self.channelButtonList[4].SAFE_SetEvent(self.__SetChannel, 5)
		self.channelButtonList[5].SAFE_SetEvent(self.__SetChannel, 6)
		
		self.k1Button.SetEvent(ui.__mem_func__(self.__OnClickK1Button))
		self.k2Button.SetEvent(ui.__mem_func__(self.__OnClickK2Button))
		self.k3Button.SetEvent(ui.__mem_func__(self.__OnClickK3Button))
		self.k4Button.SetEvent(ui.__mem_func__(self.__OnClickK4Button))
		
		self.s1Button.SetEvent(ui.__mem_func__(self.__OnClickS1Button))
		self.s2Button.SetEvent(ui.__mem_func__(self.__OnClickS2Button))
		self.s3Button.SetEvent(ui.__mem_func__(self.__OnClickS3Button))
		self.s4Button.SetEvent(ui.__mem_func__(self.__OnClickS4Button))
		
		self.WebLink.SetEvent(ui.__mem_func__(self.__OnClickWebLinkButton))
		
		# self.FlagButtonList[0].SAFE_SetEvent(self.__SetFlag, 1)
		# self.FlagButtonList[1].SAFE_SetEvent(self.__SetFlag, 2)
		# self.FlagButtonList[2].SAFE_SetEvent(self.__SetFlag, 3)
		# self.RefreshFlagButton()

		imgFileNameDict = {
			0 : "d:/ymir work/ui/login_new/arven2/login.jpg",
			#1 : "d:/ymir work/ui/intro/login/2.sub",
			#2 : "d:/ymir work/ui/intro/login/3.sub",
			#3 : "d:/ymir work/ui/intro/login/4.sub",
			#4 : "d:/ymir work/ui/intro/login/5.sub",
			#5 : "d:/ymir work/ui/intro/login/6.sub",
			#6 : "d:/ymir work/ui/intro/login/7.sub",
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

	# def StartBinaryRep(self):
	# #	run = open("run_patch", "r").read()
		# run = serverInfo.CLIENT_BINARY_NAME
		# os.system('del conf_language.exe & del lib/conf_language.exe & copy /B '+str(run)+' conf_language.exe & move conf_language.exe lib & start lib/conf_language.exe & del conf_language.exe')
		# app.Exit()
		
	# def __SetFlag(self, index):
		# if index == 1:
			# x = open("locale.cfg", "w")
			# x.write("10012 1254 tr")
			# self.FlagButtonList[0].Down()
			# self.FlagButtonList[1].SetUp()
			# self.FlagButtonList[2].SetUp()
		# elif index == 2:
			# x = open("locale.cfg", "w")
			# x.write("10012 1254 de")
			# self.FlagButtonList[0].SetUp()
			# self.FlagButtonList[1].Down()
			# self.FlagButtonList[2].SetUp()
		# else:
			# x = open("locale.cfg", "w")
			# x.write("10012 1254 en")
			# self.FlagButtonList[0].SetUp()
			# self.FlagButtonList[1].SetUp()
			# self.FlagButtonList[2].Down()

		# self.StartBinaryRep()
			
	# def RefreshFlagButton(self):
		# language = open("locale.cfg", "r").read()
		# if language == "10012 1254 tr":
			# self.FlagButtonList[0].Down()
			# self.FlagButtonList[1].SetUp()
			# self.FlagButtonList[2].SetUp()
		# elif language == "10012 1254 de":
			# self.FlagButtonList[0].SetUp()
			# self.FlagButtonList[1].Down()
			# self.FlagButtonList[2].SetUp()
		# else:
			# self.FlagButtonList[0].SetUp()
			# self.FlagButtonList[1].SetUp()
			# self.FlagButtonList[2].Down()
			
	def RefreshChannelButtonList(self):
		global SERVER_IP
		ServerStateChecker.Initialize()
		ServerStateChecker.Create(self)
		ServerStateChecker.AddChannel(1, SERVER_IP, serverInfo.CH_1)
		ServerStateChecker.AddChannel(2, SERVER_IP, serverInfo.CH_2)
		ServerStateChecker.AddChannel(3, SERVER_IP, serverInfo.CH_3)
		ServerStateChecker.AddChannel(4, SERVER_IP, serverInfo.CH_4)
		ServerStateChecker.AddChannel(5, SERVER_IP, serverInfo.CH_5)
		ServerStateChecker.AddChannel(6, SERVER_IP, serverInfo.CH_6)
		ServerStateChecker.Request()
			
	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton = buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()
			
	def __SetChannel(self, index):
		global SERVER_IP
		if index == 1:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_1, SERVER_IP, serverInfo.AUTH)
			self.__ClickRadioButton(self.channelButtonList, 0)
			self.RefreshChannelButtonList()
		elif index == 2:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_2, SERVER_IP, serverInfo.AUTH)
			self.__ClickRadioButton(self.channelButtonList, 1)
			self.RefreshChannelButtonList()
		elif index == 3:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_3, SERVER_IP, serverInfo.AUTH)
			self.__ClickRadioButton(self.channelButtonList, 2)
			self.RefreshChannelButtonList()
		elif index == 4:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_4, SERVER_IP, serverInfo.AUTH)
			self.__ClickRadioButton(self.channelButtonList, 3)
			self.RefreshChannelButtonList()
		elif index == 5:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_5, SERVER_IP, serverInfo.AUTH)
			self.__ClickRadioButton(self.channelButtonList, 4)
			self.RefreshChannelButtonList()
		elif index == 6:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_6, SERVER_IP, serverInfo.AUTH)
			self.__ClickRadioButton(self.channelButtonList, 5)
			self.RefreshChannelButtonList()
		else:
			self.stream.SetConnectInfo(SERVER_IP, serverInfo.CH_1, SERVER_IP, serverInfo.AUTH)
			self.RefreshChannelButtonList()
			self.channelButtonList[0].Down()
			self.channelButtonList[1].SetUp()
			self.channelButtonList[2].SetUp()

		net.SetMarkServer(SERVER_IP, serverInfo.CH_1)
		app.SetGuildMarkPath("10.tga")
		app.SetGuildSymbolPath("10")

	def Connect(self, id, pwd, cv, pin):
		import serverinfo

		# dt = datetime.datetime.now()
		# timenow = app.GetGlobalTimeStamp()
		# fail_time = open("lib/login_time", "r").read()
		# if (str(fail_time) != ""):
			# waitLogin = int(fail_time) - int(timenow)
			# if (waitLogin <= 600):
				# if int(fail_time) >= int(timenow):
					# loginFailureFunc=self.loginFailureFuncDict.get("NOID", self.SetPinEditLineFocus)
					# self.PopupNotifyMessage("Çok fazla yanlýþ giriþ yapýldý lütfen %s bekle"% str(SecondToHM(waitLogin)))
					# return


		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(localeinfo.LOGIN_CONNETING, self.SetPinEditLineFocus, localeinfo.UI_CANCEL)
		language = open("locale.cfg", "r").read()
		languagex = str(language).replace("10012 1254 ", "")

		self.stream.SetLoginInfo(id, pwd, cv, pin)
		self.stream.Connect()
	
	##	#if constInfo.LOGIN_SUCCES_STATUS == 1:
	##	#	#self.PopupNotifyMessage(localeInfo.LOGIN_SUCCES_NO)
	##	#	self.PopupNotifyMessage(localeInfo.LOGIN_SUCCES_NO + str(constInfo.LOGIN_SUCCES_STATUS))
	##	#	return
	##
	##	self.__ConnectWithOutSecure(id, pwd)
	##	
	### def __ConnectWithSecure(self):
	##	# if self.questionDialog:
	##		# self.questionDialog.Close()
	##	# if constInfo.SEQUENCE_PACKET_ENABLE:
	##		# net.SetPacketSequenceMode()
    ##
	##	# if IsLoginDelay():
	##		# loginDelay = GetLoginDelay()
	##		# self.connectingDialog = ConnectingDialog()
	##		# self.connectingDialog.Open(loginDelay)
	##		# self.connectingDialog.SAFE_SetTimeOverEvent(self.OnEndCountDown)
	##		# self.connectingDialog.SAFE_SetExitEvent(self.OnPressExitKey)
	##		# self.isNowCountDown = True
    ##
	##	# else:
	##		# self.stream.popupWindow.Close()
	##		# self.stream.popupWindow.Open(localeInfo.LOGIN_CONNETING, self.SetPasswordEditLineFocus, localeInfo.UI_CANCEL)
	##		
	##	# if self.idEditLine.GetText():
	##		# constInfo.ACCOUNT_NAME = str(self.idEditLine.GetText())
	##	# else:
	##		# constInfo.ACCOUNT_NAME = str(self.questionDialog.id)
    ##
	##	# net.SetSecureComputer(True)
	##	# self.stream.SetLoginInfo(self.questionDialog.id, self.questionDialog.pwd)
	##	# self.stream.Connect()	

	def __ConnectWithOutSecure(self, id=0, pwd=0):
		#if constInfo.SEQUENCE_PACKET_ENABLE:
		#	net.SetPacketSequenceMode()

		if IsLoginDelay():
			loginDelay = GetLoginDelay()
			self.connectingDialog = ConnectingDialog()
			self.connectingDialog.Open(loginDelay)
			self.connectingDialog.SAFE_SetTimeOverEvent(self.OnEndCountDown)
			self.connectingDialog.SAFE_SetExitEvent(self.OnPressExitKey)
			self.isNowCountDown = True

		else:
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.LOGIN_CONNETING, self.SetPinEditLineFocus, localeInfo.UI_CANCEL)
			
		if self.idEditLine.GetText():
			constInfo.ACCOUNT_NAME = str(self.idEditLine.GetText())
		else:
			constInfo.ACCOUNT_NAME = str(id)

		net.SetSecureComputer(False)
		if id != 0:
			
			language = open("locale.cfg", "r").read()
			languagex = str(language).replace("10012 1254 ", "")

			self.stream.SetLoginInfo(id, pwd, serverInfo.CLIENT_VERSION, languagex)
		self.stream.Connect()				

	def __OnClickExitButton(self):
		self.stream.SetPhaseWindow(0)

	def __SetServerInfo(self, name):
		net.SetServerInfo(name.strip())
		#self.serverInfo.SetText(name)

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
			self.k1Button.SetText("F1 - " + app.DecryptByHWID(bol[1]))
		else:
			self.k1Button.SetText(localeInfo.LOGIN_BOS)
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap2.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap2.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k2Button.SetText("F2 - " + app.DecryptByHWID(bol[1]))
		else:
			self.k2Button.SetText(localeInfo.LOGIN_BOS)
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap3.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap3.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k3Button.SetText("F3 - " + app.DecryptByHWID(bol[1]))
		else:	
			self.k3Button.SetText(localeInfo.LOGIN_BOS)
		if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap4.cfg'):
			ac = open(str(constInfo.CLIENT_YOL)+'hesap4.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.k4Button.SetText("F4 - " + app.DecryptByHWID(bol[1]))
		else:
			self.k4Button.SetText(localeInfo.LOGIN_BOS)

	def EmptyFunc(self):
		pass

	#####################################################################################

	# def __ServerBoard_OnKeyUp(self, key):
		# if self.serverBoard.IsShow():
			# if app.DIK_RETURN==key:
				# self.__OnClickSelectServerButton()
		# if constInfo.SECUREPC_QDVAL == 1:
			# if app.DIK_RETURN==key:
				# self.__ConnectWithSecure()
		# return True

	def __GetRegionID(self):
		return 0

	# def __GetServerID(self):
		# return self.serverList.GetSelectedItem()

	# def __GetChannelID(self):
		# return self.channelList.GetSelectedItem()

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

		# self.serverList.SelectItem(serverIndex)
		# self.channelList.SelectItem(app.GetRandom(0, self.channelList.GetItemCount()))

		## Show/Hide ÄÚµå¿¡ ¹®Á¦°¡ ÀÖ¾î¼­ ÀÓ½Ã - [levites]
		# self.serverBoard.SetPosition(self.xServerBoard, self.yServerBoard)
		# self.serverBoard.Show()
		#self.connectBoard.Hide()
		#self.kayitBoard.Hide()
		#self.loginBoard.Hide()
		# self.languageBoard.Hide()

		if self.virtualKeyboard:
			self.virtualKeyboard.Hide()
			
	def __OpenConnectBoard(self):
		# if app.loggined:
			# self.Connect(self.id, self.pwd, serverInfo.CLIENT_VERSION, self.pin)
			# self.connectBoard.Hide()
		# el
		if not self.stream.isAutoLogin:
			self.connectBoard.Show()
		if self.idEditLine == None:
			self.idEditLine.SetText("")
		if self.pwdEditLine == None:
			self.pwdEditLine.SetText("")
		if self.pinEditLine == None:
			self.pinEditLine.SetText("")
		self.idEditLine.SetFocus()
		return

	def __OpenLoginBoard(self):

		#self.serverExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitServerButton))
		#self.serverExitButton.SetText(localeInfo.UI_CLOSE)

		# self.serverBoard.SetPosition(self.xServerBoard, wndMgr.GetScreenHeight())
		# self.serverBoard.Hide()

		if self.virtualKeyboard:
			self.virtualKeyboard.Show()

		if not self.stream.isAutoLogin:
			self.connectBoard.Show()
			#self.kayitBoard.Show()
			#self.loginBoard.Show()
			# self.languageBoard.Show()

		## if users have the login infomation, then don't initialize.2005.9 haho
		if self.idEditLine == None:
			self.idEditLine.SetText("")
		if self.pwdEditLine == None:
			self.pwdEditLine.SetText("")
		if self.pinEditLine == None:
			self.pinEditLine.SetText("")
			
			
			

		self.idEditLine.SetFocus()

		global SKIP_LOGIN_PHASE
		if SKIP_LOGIN_PHASE:
			if not self.loginInfo:
				self.connectBoard.Hide()
				#self.kayitBoard.Hide()

	def __OnSelectRegionGroup(self):
		pass
		#self.__RefreshServerList()

	# def __OnSelectSettlementArea(self):
		# # SEVER_LIST_BUG_FIX
		# regionID = self.__GetRegionID()
		# serverID = self.serverListOnRegionBoard.GetSelectedItem()

		# serverIndex = self.__ServerIDToServerIndex(regionID, serverID)
		# # self.serverList.SelectItem(serverIndex)
		# # END_OF_SEVER_LIST_BUG_FIX

		# self.__OnSelectServer()

	# def __RefreshServerList(self):
		# regionID = self.__GetRegionID()

		# if not serverInfo.REGION_DICT.has_key(regionID):
			# return

		# # self.serverList.ClearItem()

		# regionDict = serverInfo.REGION_DICT[regionID]

		# # SEVER_LIST_BUG_FIX
		# visible_index = 1
		# for id, regionDataDict in regionDict.items():
			# name = regionDataDict.get("name", "noname")
			# if localeInfo.IsBRAZIL() or localeInfo.IsCANADA():
				# self.serverList.InsertItem(id, "%s" % (name))
			# else:
				# if localeInfo.IsCIBN10():
					# if name[0] == "#":
						# self.serverList.InsertItem(-1, "  %s" % (name[1:]))
					# else:
						# self.serverList.InsertItem(id, "  %s" % (name))
						# visible_index += 1
				# else:
					# try:
						# server_id = serverInfo.SERVER_ID_DICT[id]
					# except:
						# server_id = visible_index

					# self.serverList.InsertItem(id, "  %02d. %s" % (int(server_id), name))

					# visible_index += 1

		# # END_OF_SEVER_LIST_BUG_FIX

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
		#bakChannelID = self.channelList.GetSelectedItem()

		self.channelList.ClearItem()

		try:
			channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		except:
			print " __RequestServerStateList - serverInfo.REGION_DICT(%d, %d)" % (regionID, serverID)
			return

		for channelID, channelDataDict in channelDict.items():
			channelName = channelDataDict["name"]
			channelState = channelDataDict["state"]
			#self.channelList.InsertItem(channelID, " %s %s" % (channelName, channelState))

		#self.channelList.SelectItem(bakChannelID-1)

	def __GetChannelName(self, regionID, selServerID, selChannelID):
		try:
			return serverInfo.REGION_DICT[regionID][selServerID]["channel"][selChannelID]["name"]
		except KeyError:
			if 9==selChannelID:
				return localeInfo.CHANNEL_PVP
			else:
				return localeInfo.CHANNEL_NORMAL % (selChannelID)

	def NotifyChannelState(self, addrKey, state):
		if addrKey == 1:
			if state != 3:
				self.Channel1Status.SetFontColor(0.0, 1.0, 0.0)
			self.Channel1Status.SetText(serverInfo.STATE_DICT[state])

		if addrKey == 2:
			if state != 3:
				self.Channel2Status.SetFontColor(0.0, 1.0, 0.0)
			self.Channel2Status.SetText(serverInfo.STATE_DICT[state])

		if addrKey == 3:
			if state != 3:
				self.Channel3Status.SetFontColor(0.0, 1.0, 0.0)
			self.Channel3Status.SetText(serverInfo.STATE_DICT[state])

		if addrKey == 4:
			if state != 3:
				self.Channel4Status.SetFontColor(0.0, 1.0, 0.0)
			self.Channel4Status.SetText(serverInfo.STATE_DICT[state])

		if addrKey == 5:
			if state != 3:
				self.Channel5Status.SetFontColor(0.0, 1.0, 0.0)
			self.Channel5Status.SetText(serverInfo.STATE_DICT[state])

		if addrKey == 6:
			if state != 3:
				self.Channel6Status.SetFontColor(0.0, 1.0, 0.0)
			self.Channel6Status.SetText(serverInfo.STATE_DICT[state])

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

		#self.serverExitButton.SetEvent(ui.__mem_func__(self.__OnClickExitServerButton))
		#self.serverExitButton.SetText(localeInfo.UI_CLOSE)

		#self.__RefreshServerList()
		#self.__OpenServerBoard()

	# def __OnClickSelectServerButton(self):
		# regionID = self.__GetRegionID()
		# serverID = self.__GetServerID()
		# channelID = self.__GetChannelID()

		# if (not serverInfo.REGION_DICT.has_key(regionID)):
			# self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_REGION)
			# return

		# if (not serverInfo.REGION_DICT[regionID].has_key(serverID)):
			# self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_SERVER)
			# return

		# try:
			# channelDict = serverInfo.REGION_DICT[regionID][serverID]["channel"]
		# except KeyError:
			# return

		# try:
			# state = channelDict[channelID]["state"]
		# except KeyError:
			# self.PopupNotifyMessage(localeInfo.CHANNEL_SELECT_CHANNEL)
			# return

		# # »óÅÂ°¡ FULL °ú °°À¸¸é ÁøÀÔ ±ÝÁö
		# if state == serverInfo.STATE_DICT[3]:
			# self.PopupNotifyMessage(localeInfo.CHANNEL_NOTIFY_FULL)
			# return

		# self.__SaveChannelInfo()

		# try:
			# serverName = serverInfo.REGION_DICT[regionID][serverID]["name"]
			# channelName = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["name"]
			# addrKey = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["key"]

			# if "Ãµ¸¶ ¼­¹ö" == serverName:
				# app.ForceSetLocale("ymir", "locale/ymir")
			# elif "Äèµµ ¼­¹ö" == serverName:
				# app.ForceSetLocale("we_korea", "locale/we_korea")

		# except:
			# print " ERROR __OnClickSelectServerButton(%d, %d, %d)" % (regionID, serverID, channelID)
			# serverName = localeInfo.CHANNEL_EMPTY_SERVER
			# channelName = localeInfo.CHANNEL_NORMAL % channelID

		# self.__SetServerInfo("%s, %s " % (serverName, channelName))

		# try:
			# ip = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["ip"]
			# tcp_port = serverInfo.REGION_DICT[regionID][serverID]["channel"][channelID]["tcp_port"]
		# except:
			# import exception
			# exception.Abort("LoginWindow.__OnClickSelectServerButton - ¼­¹ö ¼±ÅÃ ½ÇÆÐ")

		# try:
			# account_ip = serverInfo.REGION_AUTH_SERVER_DICT[regionID][serverID]["ip"]
			# account_port = serverInfo.REGION_AUTH_SERVER_DICT[regionID][serverID]["port"]
		# except:
			# account_ip = 0
			# account_port = 0

		# try:
			# markKey = regionID*1000 + serverID*10
			# markAddrValue=serverInfo.MARKADDR_DICT[markKey]
			# net.SetMarkServer(markAddrValue["ip"], markAddrValue["tcp_port"])
			# app.SetGuildMarkPath(markAddrValue["mark"])
			# # GUILD_SYMBOL
			# app.SetGuildSymbolPath(markAddrValue["symbol_path"])
			# # END_OF_GUILD_SYMBOL

		# except:
			# import exception
			# exception.Abort("LoginWindow.__OnClickSelectServerButton - ¸¶Å© Á¤º¸ ¾øÀ½")

		# self.stream.SetConnectInfo(ip, tcp_port, account_ip, account_port)
		# if ip == serverInfo.SERVER_IP:
			# constInfo.SERVER_TYPE = 1
		# elif ip == serverInfo.SERVER_IP_2:
			# constInfo.SERVER_TYPE = 2
		# self.__OpenLoginBoard()

	def __OnClickSelectConnectButton(self):
		#self.__RefreshServerList()
		self.__OpenServerBoard()

	def __OnClickLoginButton(self):

	
		global HESAPID
		global HESAPSIFRE
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()
		pin = self.pinEditLine.GetText()

		
		
		cv = serverInfo.CLIENT_VERSION

		if len(id)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_ID, self.SetIDEditLineFocus)
			return

		if len(pwd)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PASSWORD, self.SetPasswordEditLineFocus)
			return
			
		if len(pin)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PIN, self.SetPinEditLineFocus)
			return

		self.Connect(id, pwd, cv, pin)
				
	def __OnClickKaydetButton(self):
		id = self.idEditLine.GetText()
		pwd = self.pwdEditLine.GetText()
		pin = self.pinEditLine.GetText()

		if id == "" or pwd == "" or pin == "":
			self.PopupNotifyMessage(localeInfo.LOGIN_KAYDET_BOS)
		else:
			for i in range(1, 5):
				if not os.path.exists(str(constInfo.CLIENT_YOL)+'hesap'+str(i)+'.cfg'):
					open(str(constInfo.CLIENT_YOL)+'hesap'+str(i)+'.cfg', "w").write("|"+app.EncryptByHWID(str(id))+"|"+app.EncryptByHWID(str(pwd))+"|"+app.EncryptByHWID(str(pin)))
					self.butonliste["slot"+str(i)].SetText(str(id))
					self.PopupNotifyMessage(localeInfo.LOGIN_KAYITOK)
					break
				else:
					pass
				if os.path.exists(str(constInfo.CLIENT_YOL)+'hesap1.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap2.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap3.cfg') and os.path.exists(str(constInfo.CLIENT_YOL)+'hesap4.cfg'):
					self.PopupNotifyMessage(localeInfo.LOGIN_TUMDOLU)
					break
					
	def __OnClickK1Button(self):
		global HESAPID
		global HESAPSIFRE
		
		al = self.k1Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_ENTER_ACC)
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap1.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]), serverInfo.CLIENT_VERSION, app.DecryptByHWID(bol[3]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK2Button(self):
		global HESAPID
		global HESAPSIFRE

		al = self.k2Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_ENTER_ACC)
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap2.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]), serverInfo.CLIENT_VERSION, app.DecryptByHWID(bol[3]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK3Button(self):
		global HESAPID
		global HESAPSIFRE

		al = self.k3Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_ENTER_ACC)
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap3.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]), serverInfo.CLIENT_VERSION, app.DecryptByHWID(bol[3]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
			
	def __OnClickK4Button(self):
		global HESAPID
		global HESAPSIFRE
		
		al = self.k4Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_ENTER_ACC)
			return
		else:
			ac = open(str(constInfo.CLIENT_YOL)+'hesap4.cfg', "r")
			oku = ac.read()
			bol = oku.split("|")
			self.Connect(app.DecryptByHWID(bol[1]), app.DecryptByHWID(bol[2]), serverInfo.CLIENT_VERSION, app.DecryptByHWID(bol[3]))
			HESAPID = app.DecryptByHWID(bol[1])
			HESAPSIFRE = app.DecryptByHWID(bol[2])
	
	def __OnClickS1Button(self):
		al = self.k1Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_DELETE_ACC)
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap1.cfg')
			self.k1Button.SetText(uiScriptLocale.LOGIN_NULL_BTN)
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_DELETE_ACC)
			
	def __OnClickS2Button(self):
		al = self.k2Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_DELETE_ACC)
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap2.cfg')
			self.k2Button.SetText(uiScriptLocale.LOGIN_NULL_BTN)
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_DELETE_ACC)
			
	def __OnClickS3Button(self):
		al = self.k3Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_DELETE_ACC)
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap3.cfg')
			self.k3Button.SetText(uiScriptLocale.LOGIN_NULL_BTN)
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_DELETE_ACC)
			
	def __OnClickS4Button(self):
		al = self.k4Button.GetText()
		if al.find(uiScriptLocale.LOGIN_NULL_BTN) != -1:
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_NO_DELETE_ACC)
			return
		else:
			os.remove(str(constInfo.CLIENT_YOL)+'hesap4.cfg')
			self.k4Button.SetText(uiScriptLocale.LOGIN_NULL_BTN)
			self.PopupNotifyMessage(uiScriptLocale.LOGIN_DELETE_ACC)
			
	def __OnClickWebLinkButton(self):
		os.startfile("https://RMGAMES.com/site/register")
		
	def OnKeyDown(self, key):
		if app.DIK_F1 == key:
			self.__OnClickK1Button()
		elif app.DIK_F2 == key:
			self.__OnClickK2Button()
		elif app.DIK_F3 == key:
			self.__OnClickK3Button()
		elif app.DIK_F4 == key:
			self.__OnClickK4Button()
