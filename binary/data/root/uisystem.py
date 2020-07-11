import net
import app
import player
import ui
import uiOption
import uiSystemOption
import uiGameOption
import uiScriptLocale
import networkModule
import constInfo
import localeInfo
import dbg
import introLogin
import serverInfo
import uiCommon
import constInfo
import uikeyboardsettings
import uiInventory

SYSTEM_MENU_FOR_PORTAL = False

###################################################################################################
## System
class SystemDialog(ui.ScriptWindow):

	def __init__(self, stream):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.stream = stream
		self.keyboardsettings = uikeyboardsettings.KeyChangeWindow()
		self.keyboardsettings.Hide()
		self.interface = None
	
	def __Initialize(self):
		self.eventOpenHelpWindow = None
		self.systemOptionDlg = None
		self.gameOptionDlg = None
		self.moveChannelDlg = None
		self.npcRange = None
		self.switchbot = None
		self.interface = None
		
	def LoadDialog(self):	
		if SYSTEM_MENU_FOR_PORTAL:
			self.__LoadSystemMenu_ForPortal()
		else:
			self.__LoadSystemMenu_Default()
			
	def __LoadSystemMenu_Default(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/systemdialog.py")

		self.GetChild("system_option_button").SAFE_SetEvent(self.__ClickSystemOptionButton)
		self.GetChild("game_option_button").SAFE_SetEvent(self.__ClickGameOptionButton)
		self.GetChild("change_button").SAFE_SetEvent(self.__ClickChangeCharacterButton)
		self.GetChild("logout_button").SAFE_SetEvent(self.__ClickLogOutButton)
		self.GetChild("exit_button").SAFE_SetEvent(self.__ClickExitButton)
		self.GetChild("cancel_button").SAFE_SetEvent(self.Close)
		self.GetChild("movechannel_button").SAFE_SetEvent(self.__ClickMoveChannelButton)
		self.GetChild("mall_button").SAFE_SetEvent(self.__ClickInGameShopButton)

	def __LoadSystemMenu_ForPortal(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/systemdialog_forportal.py")
		
		self.GetChild("system_option_button").SAFE_SetEvent(self.__ClickSystemOptionButton)
		self.GetChild("game_option_button").SAFE_SetEvent(self.__ClickGameOptionButton)
		self.GetChild("change_button").SAFE_SetEvent(self.__ClickChangeCharacterButton)
		self.GetChild("exit_button").SAFE_SetEvent(self.__ClickExitButton)
		self.GetChild("cancel_button").SAFE_SetEvent(self.Close)
		self.GetChild("movechannel_button").SAFE_SetEvent(self.__ClickMoveChannelButton)		

	def Destroy(self):
		self.ClearDictionary()
		
		if self.gameOptionDlg:
			self.gameOptionDlg.Destroy()
			
		if self.systemOptionDlg:
			self.systemOptionDlg.Destroy()
			
		self.__Initialize()

	def SetOpenHelpWindowEvent(self, event):
		self.eventOpenHelpWindow = event

	def OpenDialog(self):
		self.Show()
		
	def BindInterface(self, interface):
		self.interface = interface

	def __ClickChangeCharacterButton(self):
		self.Close()

		net.ExitGame()
		
	def __OnClosePopupDialog(self):
		self.popup = None		

	def __ClickLogOutButton(self):
		if SYSTEM_MENU_FOR_PORTAL: 
			if app.loggined:
				self.Close()
				net.ExitApplication()
			else:
				self.Close()
				net.LogOutGame()
		else:
			self.Close()
			net.LogOutGame()
			
	def __ClickExitButton(self):
		self.Close()
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.QUICK_GAME_END)
		questionDialog.SetAcceptEvent(lambda arg=True: self.ShutdownGame(arg))
		questionDialog.SetCancelEvent(lambda arg=False: self.ShutdownGame(arg))
		questionDialog.Open()
		self.questionDialog = questionDialog
			
	def ShutdownGame(self, answer):
		if not self.questionDialog:
			return
		
		if answer:
			app.Exit()
			
		self.questionDialog.Close()
		self.questionDialog = None

	def __ClickSystemOptionButton(self):
		self.Close()

		if not self.systemOptionDlg:
			self.systemOptionDlg = uiSystemOption.OptionDialog()

		self.systemOptionDlg.Show()

	def __ClickGameOptionButton(self):
		self.Close()

		if not self.gameOptionDlg:
			self.gameOptionDlg = uiGameOption.OptionDialog()
			self.gameOptionDlg.BindKeyBoardSettings(self.dlgKeyboardSettings)
		self.gameOptionDlg.Show()
	
	def __ClickHelpButton(self):
		self.Close()

		if None != self.eventOpenHelpWindow:
			self.eventOpenHelpWindow()

	def __ClickInGameShopButton(self):	 	
		self.Close()
		import uiItemShop
		self.itemshop = uiItemShop.ItemShopWindow()
		self.itemshop.Open()
	def Close(self):
		self.Hide()
		return True

	def RefreshMobile(self):
		if self.gameOptionDlg:
			self.gameOptionDlg.RefreshMobile()
		#self.optionDialog.RefreshMobile()

	def OnMobileAuthority(self):
		if self.gameOptionDlg:
			self.gameOptionDlg.OnMobileAuthority()
		#self.optionDialog.OnMobileAuthority()

	def OnBlockMode(self, mode):
		uiGameOption.blockMode = mode
		if self.gameOptionDlg:
			self.gameOptionDlg.OnBlockMode(mode)
		#self.optionDialog.OnBlockMode(mode)

	def OnChangePKMode(self):
		if self.gameOptionDlg:
			self.gameOptionDlg.OnChangePKMode()
		#self.optionDialog.OnChangePKMode()
		
	def __ClickMoveChannelButton(self):		
		self.Close()
		if self.moveChannelDlg:
			self.moveChannelDlg.Show()
		else:
			moveChannelDlg = MoveChannelDialog()
			moveChannelDlg.Show()
			self.moveChannelDlg = moveChannelDlg
	
	def OnPressExitKey(self):
		self.Close()
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def BindKeyboardWindow(self, dlgKeyboardSettings):
		self.dlgKeyboardSettings = dlgKeyboardSettings

class MoveChannelDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.StartChannelNumber = 0
		self.IsShow = False
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadDialog(self) :	
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/MoveChannelDialog.py")
		except:
			import exception as exception
			exception.Abort("MoveChannelDialog.__LoadDialog")
		

		self.ParentBoard = self.GetChild("MoveChannelBoard")
		self.ChildBoard = self.GetChild("BlackBoard")
		self.GetChild("MoveChannelTitle").SetCloseEvent(ui.__mem_func__(self.Close))
		
		self.ChannelList = []
		cnt = 5
		cnt = cnt - 1 # 99서버 제외
		
		self.DlgWidht = 190
		self.BlackBoardHeight = 23*cnt + 5*(cnt-1) + 13
		self.DlgHeight = self.BlackBoardHeight + 75
		
		self.AcceptBtn = ui.MakeButton(self.ParentBoard, 13, self.DlgHeight - 33, "", "d:/ymir work/ui/public/", "middle_button_01.sub", "middle_button_02.sub", "middle_button_03.sub")
		self.AcceptBtn.SetText( localeInfo.MOVE_CHANNEL_SELECT )
		self.AcceptBtn.SetEvent(ui.__mem_func__(self.AcceptButton))
		self.CloseBtn = ui.MakeButton(self.ParentBoard, self.DlgWidht - 73, self.DlgHeight - 33, "", "d:/ymir work/ui/public/", "middle_button_01.sub", "middle_button_02.sub", "middle_button_03.sub")
		self.CloseBtn.SetText( localeInfo.MOVE_CHANNEL_CANCEL )
		self.CloseBtn.SetEvent(ui.__mem_func__(self.Close))

		for i in xrange(cnt):
			btn = ui.MakeButton(self.ChildBoard, 8, 6 + i*28, "", "d:/ymir work/ui/game/myshop_deco/", "select_btn_01.sub", "select_btn_02.sub", "select_btn_03.sub")
			btn.SetText("Kanal {0}".format(int(i+1)))
			btn.SetEvent(ui.__mem_func__(self.__SelectChannel), i+1)
			self.ChannelList.append(btn)
		
		self.ParentBoard.SetSize(self.DlgWidht, self.DlgHeight)
		self.ChildBoard.SetSize(self.DlgWidht - 26, self.BlackBoardHeight)
		self.SetSize(self.DlgWidht, self.DlgHeight)		
		self.UpdateRect()
		
	def __SelectChannel(self, idx):
		self.ChangeChannelNumber = idx
		
		for btn in self.ChannelList:
			btn.SetUp()
			btn.Enable()
			
		self.ChannelList[idx-1].Down()
		self.ChannelList[idx-1].Disable()
		
	def AcceptButton(self):
		if self.ChangeChannelNumber == self.StartChannelNumber:
			return
		
		net.SendChatPacket("/channel " + str(self.ChangeChannelNumber))
		self.Close()
					
	def Show(self) :
		ui.ScriptWindow.Show(self)
		
		self.StartChannelNumber = constInfo.channel_idx
		self.__SelectChannel(self.StartChannelNumber)
		
		self.IsShow = True
	
	def Close(self):
		self.Hide()
		
		self.IsShow = False
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def IsShowWindow(self):
		return self.IsShow
		
