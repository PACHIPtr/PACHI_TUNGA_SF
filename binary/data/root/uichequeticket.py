import ui
import uiCommon
import localeInfo

# Import player, net and chat module
import player
import net
import chat

class ChequeTicketWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.itemPos 			= -1
		self.questionDlg 		= None
		self.__LoadWindow()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/ChequeTicketWindow.py")
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.LoadScriptFile")
			
		try:
			self.mainBoard		= self.GetChild("main_board")
			self.inputValue		= self.GetChild("InputValue")
			self.acceptBtn		= self.GetChild("Lbutton")
			self.cancelBtn		= self.GetChild("Rbutton")
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.BindObject")
			
		try:
			self.inputValue.OnIMEReturn = self.__OnClickAcceptButton
			self.inputValue.OnPressEscapeKey = self.Close
			self.mainBoard.SetCloseEvent(ui.__mem_func__(self.Close))
			self.acceptBtn.SetEvent(ui.__mem_func__(self.__OnClickAcceptButton))
			self.cancelBtn.SetEvent(ui.__mem_func__(self.Close))
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.SetEvent")
		
	def __OnCloseQuestionDialog(self):
		self.questionDlg.Close()
		self.questionDlg = None
		
	def Close(self):
		self.Hide()
		
		if (self.inputValue is not None):
			self.inputValue.SetText("0")
		
		if (self.inputValue is not None and self.inputValue.IsFocus()):
			self.inputValue.KillFocus()
		
		if (self.questionDlg is not None):
			self.questionDlg.Close()
			self.questionDlg = None
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def __OnClickAcceptButton(self):
		
		if (len(self.inputValue.GetText()) == 0):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_WON_TICKET_EMPTY)
			return
		
		value = int(self.inputValue.GetText())
		if (value > player.GetCheque()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_WON_TICKET_NOT_ENOUGH)
			return
			
		self.questionDlg = uiCommon.QuestionDialog2()
		self.questionDlg.SetText1(localeInfo.USE_WON_TICKET_QUESTION_DLG1)
		self.questionDlg.SetText2(localeInfo.USE_WON_TICKET_QUESTION_DLG2)
		self.questionDlg.SetAcceptEvent(ui.__mem_func__(self.__OnClickConfirmButton))
		self.questionDlg.SetCancelEvent(ui.__mem_func__(self.__OnCloseQuestionDialog))
		self.questionDlg.Open()
		
	def Open(self, itemPos):
		self.itemPos = itemPos
		self.SetCenterPosition()
		self.Show()
		
	def __OnClickConfirmButton(self):
		self.__OnCloseQuestionDialog()
		net.SendChatPacket("/use_won_ticket {0} {1}".format(self.itemPos, int(self.inputValue.GetText())))
		self.Close()
		
