import app
import item
import ime
import shop
import ui
import localeInfo
import uiScriptLocale
import dbg
import uiToolTip
import constInfo
import chat
import net

class PopupDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.acceptEvent = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/PopupDialog.py")

			self.board = self.GetChild("board")
			self.message = self.GetChild("message")
			self.accceptButton = self.GetChild("accept")
			self.accceptButton.SetEvent(ui.__mem_func__(self.Close))

		except:
			import exception
			exception.Abort("PopupDialog.LoadDialog.BindObject")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()
		self.acceptEvent()

	def Destroy(self):
		self.Close()
		self.ClearDictionary()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SetText(self, text):
		self.message.SetText(text)

	def SetAcceptEvent(self, event):
		self.acceptEvent = event

	def SetButtonName(self, name):
		self.accceptButton.SetText(name)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnIMEReturn(self):
		self.Close()
		return True
		
class PopupDialogNew(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.acceptEvent = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/PopupDialogNew.py")

			self.board = self.GetChild("board")
			self.message = self.GetChild("message")
			self.message2 = self.GetChild("message2")
			self.accceptButton = self.GetChild("accept")
			self.accceptButton.SetEvent(ui.__mem_func__(self.Close))

		except:
			import exception
			exception.Abort("PopupDialog.LoadDialog.BindObject")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()
		self.acceptEvent()

	def Destroy(self):
		self.Close()
		self.ClearDictionary()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SetText1(self, text, arg=None):
		self.message.SetText(text)
		
	def SetText2(self, text, arg=None):
		self.message2.SetText(text)

	def SetAcceptEvent(self, event):
		self.acceptEvent = event

	def SetButtonName(self, name):
		self.accceptButton.SetText(ButtonName)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnIMEReturn(self):
		self.Close()
		return True
		
class PopupDialog2(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.acceptEvent = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/PopupDialog2.py")

			self.board = self.GetChild("board")
			self.message1 = self.GetChild("message1")
			self.message2 = self.GetChild("message2")
			self.accceptButton = self.GetChild("accept")
			self.accceptButton.SetEvent(ui.__mem_func__(self.Close))

		except:
			import exception
			exception.Abort("PopupDialog.LoadDialog.BindObject")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()
		self.acceptEvent()

	def Destroy(self):
		self.Close()
		self.ClearDictionary()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SetText1(self, text):
		self.message1.SetText(text)
		
	def SetText2(self, text):
		self.message2.SetText(text)		

	def SetAcceptEvent(self, event):
		self.acceptEvent = event

	def SetButtonName(self, name):
		self.accceptButton.SetText(ButtonName)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnIMEReturn(self):
		self.Close()
		return True

class InputDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/inputdialog.py")

		getObject = self.GetChild
		self.board = getObject("Board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputSlot = getObject("InputSlot")
		self.inputValue = getObject("InputValue")

	def Open(self):
		self.inputValue.SetFocus()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputSlot = None
		self.inputValue = None
		self.Hide()

	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetNumberMode(self):
		self.inputValue.SetNumberMode()

	def SetSecretMode(self):
		self.inputValue.SetSecret()

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		width = length * 6 + 10
		self.SetBoardWidth(max(width + 50, 160))
		self.SetSlotWidth(width)
		self.inputValue.SetMax(length)

	def SetSlotWidth(self, width):
		self.inputSlot.SetSize(width, self.inputSlot.GetHeight())
		self.inputValue.SetSize(width, self.inputValue.GetHeight())
		if self.IsRTL():
			self.inputValue.SetPosition(self.inputValue.GetWidth(), 0)

	def SetBoardWidth(self, width):
		self.SetSize(max(width + 50, 160), self.GetHeight())
		self.board.SetSize(max(width + 50, 160), self.GetHeight())	
		if self.IsRTL():
			self.board.SetPosition(self.board.GetWidth(), 0)
		self.UpdateRect()

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event

	def GetText(self):
		return self.inputValue.GetText()


class InputDialogShop(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()
		self.type = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/inputdialogshop.py")

		getObject = self.GetChild
		self.board = getObject("Board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputSlot = getObject("InputSlot")
		self.inputValue = getObject("InputValue")
		self.inputType0 = getObject("InputType0")
		self.inputType1 = getObject("InputType1")
		self.inputType2 = getObject("InputType2")
		self.inputType3 = getObject("InputType3")
		
		self.inputType0.SetEvent(self.__ChangeInputType0)
		self.inputType1.SetEvent(self.__ChangeInputType1)
		self.inputType2.SetEvent(self.__ChangeInputType2)
		self.inputType3.SetEvent(self.__ChangeInputType3)
		
		
	def Open(self):
		self.inputValue.SetFocus()
		self.__ChangeInputType0()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()
		

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputSlot = None
		self.inputValue = None
		self.inputType0 = None
		self.inputType1 = None
		self.inputType2 = None
		self.inputType3 = None
		self.type = None
		self.Hide()
		
	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetNumberMode(self):
		self.inputValue.SetNumberMode()

	def SetSecretMode(self):
		self.inputValue.SetSecret()

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		width = length * 6 + 10
		self.SetBoardWidth(max(width + 50, 160))
		self.SetSlotWidth(width)
		self.inputValue.SetMax(length)

	def SetSlotWidth(self, width):
		self.inputSlot.SetSize(width, self.inputSlot.GetHeight())
		self.inputValue.SetSize(width, self.inputValue.GetHeight())
		if self.IsRTL():
			self.inputValue.SetPosition(self.inputValue.GetWidth(), 0)

	def SetBoardWidth(self, width):
		self.SetSize(max(width + 50, 160), self.GetHeight())
		self.board.SetSize(max(width + 50, 160), self.GetHeight())	
		if self.IsRTL():
			self.board.SetPosition(self.board.GetWidth(), 0)
		self.UpdateRect()

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event
		
	def __ChangeInputType0(self):
		self.type = 0
		self.GetChild("InputType0").Down()
		self.GetChild("InputType1").SetUp()
		self.GetChild("InputType2").SetUp()
		self.GetChild("InputType3").SetUp()
	
	def __ChangeInputType1(self):
		self.type = 1
		self.GetChild("InputType0").SetUp()
		self.GetChild("InputType1").Down()
		self.GetChild("InputType2").SetUp()
		self.GetChild("InputType3").SetUp()
		
	def __ChangeInputType2(self):
		self.type = 2
		self.GetChild("InputType0").SetUp()
		self.GetChild("InputType1").SetUp()
		self.GetChild("InputType2").Down()
		self.GetChild("InputType3").SetUp()
		
	def __ChangeInputType3(self):
		self.type = 3
		self.GetChild("InputType0").SetUp()
		self.GetChild("InputType1").SetUp()
		self.GetChild("InputType2").SetUp()
		self.GetChild("InputType3").Down()

	def GetText(self):
		return self.inputValue.GetText()
		
	def GetType(self):
		return int(self.type)
		
class InputDialogWithDescription(InputDialog):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		InputDialog.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/inputdialogwithdescription.py")

		try:
			getObject = self.GetChild
			self.board = getObject("Board")
			self.acceptButton = getObject("AcceptButton")
			self.cancelButton = getObject("CancelButton")
			self.inputSlot = getObject("InputSlot")
			self.inputValue = getObject("InputValue")
			self.description = getObject("Description")

		except:
			import exception
			exception.Abort("InputDialogWithDescription.LoadBoardDialog.BindObject")

	def SetText(self, text):
		self.inputValue.SetText(str(text))

	def SetDescription(self, text):
		self.description.SetText(text)
		
	def SetAcceptText(self, text):
		self.acceptButton.SetText(text)
		
	def SetCancelText(self, text):
		self.cancelButton.SetText(text)

class InputDialogWithDescription2(InputDialog):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		InputDialog.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/inputdialogwithdescription2.py")

		try:
			getObject = self.GetChild
			self.board = getObject("Board")
			self.acceptButton = getObject("AcceptButton")
			self.cancelButton = getObject("CancelButton")
			self.inputSlot = getObject("InputSlot")
			self.inputValue = getObject("InputValue")
			self.description1 = getObject("Description1")
			self.description2 = getObject("Description2")

		except:
			import exception
			exception.Abort("InputDialogWithDescription.LoadBoardDialog.BindObject")

	def SetDescription1(self, text):
		self.description1.SetText(text)

	def SetDescription2(self, text):
		self.description2.SetText(text)

class ItemQuestionDialog(ui.ScriptWindow):
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, 'uiscript/itemquestiondialog.py')
		self.board = self.GetChild('board')
		self.textLine = self.GetChild('message')
		self.textLine2 = self.GetChild('message2')
		self.deleteButton = self.GetChild('deletebutton')
		self.sellButton = self.GetChild('sellbutton')
		self.cancelButton = self.GetChild('cancelbutton')

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SetDeleteAcceptEvent(self, event):
		self.deleteButton.SetEvent(event)

	def SetSellAcceptEvent(self, event):
		self.sellButton.SetEvent(event)

	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetText2(self, text):
		self.textLine2.SetText(text)

	def OnPressEscapeKey(self):
		self.Close()
		return True		
		
class QuestionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialog.py")

		self.board = self.GetChild("board")
		self.textLine = self.GetChild("message")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SAFE_SetAcceptEvent(self, event):
		self.acceptButton.SAFE_SetEvent(event)

	def SAFE_SetCancelEvent(self, event):
		self.cancelButton.SAFE_SetEvent(event)

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)

	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetAcceptText(self, text):
		self.acceptButton.SetText(text)

	def SetCancelText(self, text):
		self.cancelButton.SetText(text)

	def OnPressEscapeKey(self):
		self.Close()
		return True
				
	def GetTextSize(self):
		if self.textLine:
			return self.textLine.GetTextSize()
			
		return (0,0)
		
	def GetLineHeight(self):
		if self.textLine:
			return self.textLine.GetLineHeight()
		
		return 0
			
	def SetLineHeight(self, Height):
		self.textLine.SetLineHeight(Height)
		
	def GetTextLineCount(self):
		return self.textLine.GetTextLineCount()
				
class QuestionDialogEfsun(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialogefsun.py")

		self.board = self.GetChild("board")
		self.textLine = self.GetChild("message")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()

	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SAFE_SetAcceptEvent(self, event):
		self.acceptButton.SAFE_SetEvent(event)

	def SAFE_SetCancelEvent(self, event):
		self.cancelButton.SAFE_SetEvent(event)

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)

	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetAcceptText(self, text):
		self.acceptButton.SetText(text)

	def SetCancelText(self, text):
		self.cancelButton.SetText(text)

	def OnPressEscapeKey(self):
		self.Close()
		return True		

class QuestionDialogExchange(QuestionDialog):

	def __init__(self):
		QuestionDialog.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		QuestionDialog.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialogticaret.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")
		self.what = self.GetChild("what")
		self.what.Hide()

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)	

	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)

	def SetWhatEvent(self, event):
		self.what.SetEvent(event)
		
	def SetText1(self, text):
		self.textLine1.SetText(text)

	def SetText2(self, text):
		self.textLine2.SetText(text)
		
	def whatcim(self, text):
		self.what.SetText(text)		
				
class QuestionDialog2(QuestionDialog):

	def __init__(self):
		QuestionDialog.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		QuestionDialog.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialog2.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def SetText1(self, text, alignLeft = False):
		self.textLine1.SetText(text)
		if alignLeft == True:
			self.textLine1.SetPosition(15, 30)
			self.textLine1.SetWindowHorizontalAlignLeft()
			self.textLine1.SetHorizontalAlignLeft()
		else:
			self.textLine1.SetPosition(0, 25)
			self.textLine1.SetWindowHorizontalAlignCenter()
			self.textLine1.SetHorizontalAlignCenter()

	def SetText2(self, text, alignLeft = False):
		self.textLine2.SetText(text)
		if alignLeft == True:
			self.textLine2.SetPosition(15, 50)
			self.textLine2.SetWindowHorizontalAlignLeft()
			self.textLine2.SetHorizontalAlignLeft()
		else:
			self.textLine2.SetPosition(0, 50)
			self.textLine2.SetWindowHorizontalAlignCenter()
			self.textLine2.SetHorizontalAlignCenter()
		
	def AutoResize(self):
		if self.textLine1.GetTextSize()[0] > self.textLine2.GetTextSize()[0]:
			self.SetWidth(self.textLine1.GetTextSize()[0] + 30)
		else:
			self.SetWidth(self.textLine2.GetTextSize()[0] + 30)
			
			
class QuestionDialog3(QuestionDialog):

	def __init__(self):
		QuestionDialog.__init__(self)
		self.__CreateDialog()

	def __del__(self):
		QuestionDialog.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialog3.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		#self.textLine3 = self.GetChild("message3")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def SetText1(self, text):
		self.textLine1.SetText(text)

	def SetText2(self, text):
		self.textLine2.SetText(text)

	#def SetText3(self, text):
	#	self.textLine3.SetText(text)

class QuestionDialogWithTimeLimit(QuestionDialog2):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()
		self.endTime = 0

	def __del__(self):
		QuestionDialog2.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialog2.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self, msg, timeout):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		self.SetText1(msg)
		self.endTime = app.GetTime() + timeout

	def OnUpdate(self):
		leftTime = max(0, self.endTime - app.GetTime())
		self.SetText2(localeInfo.UI_LEFT_TIME % (leftTime))

class MoneyInputDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.moneyHeaderText = localeInfo.MONEY_INPUT_DIALOG_SELLPRICE
		self.__CreateDialog()
		self.SetMaxLength(11)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/moneyinputdialog.py")

		getObject = self.GetChild
		self.board = self.GetChild("board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputValue = getObject("InputValue")
		self.inputValue.SetNumberMode()
		self.inputValue.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
		self.moneyText = getObject("MoneyValue")

	def Open(self):
		self.inputValue.SetText("")
		self.inputValue.SetFocus()
		self.__OnValueUpdate()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputValue = None
		self.Hide()

	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		length = min(11, length)
		self.inputValue.SetMax(length)

	def SetMoneyHeaderText(self, text):
		self.moneyHeaderText = text

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event

	def SetValue(self, value):
		value=str(value)
		self.inputValue.SetText(value)
		self.__OnValueUpdate()
		ime.SetCursorPosition(len(value))		


	def GetText(self):
		return self.inputValue.GetText()

	def __OnValueUpdate(self):
		ui.EditLine.OnIMEUpdate(self.inputValue)

		text = self.inputValue.GetText()

		money = 0
		if text and text.isdigit():
			try:
				money = long(text)
			except ValueError:
				money = 199999999999

		self.moneyText.SetText(self.moneyHeaderText + localeInfo.NumberToMoneyString(money))
		
	def __OnValueUpdate1(self):
		ui.EditLine.OnIMEUpdate(self.inputValue1)

		text = self.inputValue1.GetText()

		money = 0
		if text and text.isdigit():
			try:
				money = int(text)
			except ValueError:
				money = 299999999

		self.moneyText1.SetText("Satış Fiyatı:"+str(money)+" EP")
		
	def __OnValueUpdate2(self):
		ui.EditLine.OnIMEUpdate(self.inputValue2)

		text = self.inputValue2.GetText()

		money = 0
		if text and text.isdigit():
			try:
				money = int(text)
			except ValueError:
				money = 299999999

		self.moneyText2.SetText("Satış Fiyatı:"+str(money)+" Won")

class QuestionDialogWithTimeLimit2(QuestionDialog2):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()
		self.endTime = 0

	def __del__(self):
		QuestionDialog2.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialog2.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self, msg, timeout):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		self.SetText1(msg)
		self.endTime = app.GetTime() + timeout

	def OnUpdate(self):
		leftTime = max(0, self.endTime - app.GetTime())
		self.SetText2(localeInfo.UI_LEFT_TIME % (leftTime))
		
class MoneyInputDialogCheque(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.moneyHeaderText = localeInfo.MONEY_INPUT_DIALOG_SELLPRICE
		self.__CreateDialog()
		self.SetMaxLength(12)
		self.SetMaxLengthCheque(3)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/moneyinputdialogcheque.py")

		getObject = self.GetChild
		self.board = self.GetChild("board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputValue = getObject("InputValue")
		self.inputValue.SetNumberMode()
		self.inputValue.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdate)
		self.moneyText = getObject("MoneyValue")
		self.SellInfoText = getObject("SellInfoText")
		self.InputValue_Cheque = getObject("InputValue_Cheque")
		self.InputValue_Cheque.SetNumberMode()
		self.InputValue_Cheque.OnIMEUpdate = ui.__mem_func__(self.__OnChequeValueUpdate)
		self.chequeText = getObject("ChequeValue")

	def Open(self):
		self.inputValue.SetText("0")
		self.inputValue.SetFocus()
		self.__OnChequeValueUpdate()
		self.InputValue_Cheque.SetText("0")
		self.__OnValueUpdate()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputValue = None
		self.InputValue_Cheque = None
		self.Hide()

	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		length = min(12, length)
		self.inputValue.SetMax(length)

	def SetMoneyHeaderText(self, text):
		self.moneyHeaderText = text

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event

	def SetValue(self, value):
		value=str(value)
		self.inputValue.SetText(value)
		self.__OnValueUpdate()
		ime.SetCursorPosition(len(value))		


	def GetText(self):
		return self.inputValue.GetText()
		
	def SetChequeValue(self, value):
		value=str(value)
		self.InputValue_Cheque.SetText(value)
		self.InputValue_Cheque.SetFocus()
		self.__OnChequeValueUpdate()
		ime.SetCursorPosition(len(value))
		
	def SetMaxLengthCheque(self, length):
		length = min(3, length)
		self.InputValue_Cheque.SetMax(length)
		
	def GetChequeText(self):
		return self.InputValue_Cheque.GetText()
		
	def __OnChequeValueUpdate(self):
		ui.EditLine.OnIMEUpdate(self.InputValue_Cheque)

		text = self.InputValue_Cheque.GetText()
		
		cheque = 0
		if text and text.isdigit():
			try:
				cheque = int(text)
			except ValueError:
				cheque = 999

		self.chequeText.SetText(localeInfo.NumberToChequeString(cheque))

	def __OnValueUpdate(self):
		ui.EditLine.OnIMEUpdate(self.inputValue)

		text = self.inputValue.GetText()

		money = 0
		if text and text.isdigit():
			try:
				money = long(text)
			except ValueError:
				money = 199999999

		self.moneyText.SetText(localeInfo.NumberToMoneyString(money))
		
class InputDialogName(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/inputdialog_name.py")

		getObject = self.GetChild
		self.board = getObject("Board")
		self.acceptButton = getObject("AcceptButton")
		self.cancelButton = getObject("CancelButton")
		self.inputSlot = getObject("InputSlot")
		self.inputValue = getObject("InputValue")

	def Open(self):
		self.inputValue.SetFocus()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.board = None
		self.acceptButton = None
		self.cancelButton = None
		self.inputSlot = None
		self.inputValue = None
		self.Hide()

	def SetTitle(self, name):
		self.board.SetTitleName(name)

	def SetNumberMode(self):
		self.inputValue.SetNumberMode()

	def SetSecretMode(self):
		self.inputValue.SetSecret()

	def SetFocus(self):
		self.inputValue.SetFocus()

	def SetMaxLength(self, length):
		width = length * 6 + 10
		self.SetBoardWidth(max(width + 50, 160))
		self.SetSlotWidth(width)
		self.inputValue.SetMax(length)

	def SetSlotWidth(self, width):
		self.inputSlot.SetSize(width, self.inputSlot.GetHeight())
		self.inputValue.SetSize(width, self.inputValue.GetHeight())
		if self.IsRTL():
			self.inputValue.SetPosition(self.inputValue.GetWidth(), 0)

	def SetBoardWidth(self, width):
		self.SetSize(max(width + 50, 160), self.GetHeight())
		self.board.SetSize(max(width + 50, 160), self.GetHeight())	
		if self.IsRTL():
			self.board.SetPosition(self.board.GetWidth(), 0)
		self.UpdateRect()

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.board.SetCloseEvent(event)
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event

	def GetText(self):
		return self.inputValue.GetText()
		
		
class EventInformationDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__CreateDialog()
		
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.itemVnum = 0
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/QuestionDialog4.py")

		self.board = self.GetChild("board")
		self.textLine = self.GetChild("message")

		self.titleBar = ui.TitleBar()
		self.titleBar.SetParent(self.board)
		self.titleBar.MakeTitleBar(244, "yellow")
		self.titleBar.SetPosition(8, 7)
		self.titleBar.Show()

		self.titleName = ui.TextLine()
		self.titleName.SetParent(self.titleBar)
		self.titleName.SetPosition(0, 4)
		self.titleName.SetWindowHorizontalAlignCenter()
		self.titleName.SetHorizontalAlignCenter()
		self.titleName.Show()

		self.slotList = []
		for i in xrange(3):
			slot = ui.ImageBox()
			slot.LoadImage("d:/ymir work/ui/public/slot_base.sub")
			slot.SetParent(self)
			slot.SetWindowHorizontalAlignCenter()
			self.slotList.append(slot)

	def Open(self, vnum, title, text="", text2="", time=0):
		self.titleName.SetText(title)
		self.itemVnum = vnum
		if vnum > 0:
			item.SelectItem(vnum)
		xSlotCount, ySlotCount = item.GetItemSize()
		
		if text:
			textLine2 = ui.TextLine()
			textLine2.SetPosition(0, 80 + 32*ySlotCount)
			textLine2.SetWindowHorizontalAlignCenter()
			textLine2.SetHorizontalAlignCenter()
			textLine2.SetVerticalAlignCenter()
			textLine2.SetParent(self.board)
			textLine2.SetText(text)
			textLine2.Show()
			self.textLine2 = textLine2
			
		if text2:
			textLine3 = ui.TextLine()
			textLine3.SetPosition(0, 100 + 32*ySlotCount)
			textLine3.SetWindowHorizontalAlignCenter()
			textLine3.SetHorizontalAlignCenter()
			textLine3.SetVerticalAlignCenter()
			textLine3.SetParent(self.board)
			textLine3.SetText(text2)
			textLine3.Show()
			self.textLine3 = textLine3
				
		if time:
			self.time = time
			textLine4 = ui.TextLine()
			if text2:
				textLine4.SetPosition(0, 120 + 32*ySlotCount)
			else:
				textLine4.SetPosition(0, 100 + 32*ySlotCount)
			textLine4.SetWindowHorizontalAlignCenter()
			textLine4.SetHorizontalAlignCenter()
			textLine4.SetVerticalAlignCenter()
			textLine4.SetParent(self.board)
			leftTime = max(0, self.time - app.GetGlobalTimeStamp())
			textLine4.SetText(localeInfo.TOOLTIP_TIME % (localeInfo.FormatTime(leftTime)))
			textLine4.Show()
			self.textLine4 = textLine4

		slotGrid = ui.SlotWindow()
		slotGrid.SetParent(self)
		slotGrid.SetPosition(-16, 64)
		slotGrid.SetWindowHorizontalAlignCenter()
		slotGrid.AppendSlot(0, 0, 0, 32*xSlotCount, 32*ySlotCount)
		slotGrid.AddFlag("not_pick")
		slotGrid.Show()
		self.slotGrid = slotGrid
		if vnum > 0:
			self.slotGrid.SetItemSlot(0, vnum)
			self.slotGrid.ActivateSlot(0)
		else:
			self.height -= 25

		if text:
			self.height -= 10
			
		if text2:
			self.height += 30
				
		self.SetSize(260, 130 + 32*ySlotCount + self.height)
		self.board.SetSize(260, 130 + 32*ySlotCount + self.height)
		self.textLine.SetPosition(0, 44)

		for i in xrange(min(3, ySlotCount)):
			self.slotList[i].SetPosition(0, 32 + ySlotCount*32 - i*32)
			self.slotList[i].OnMouseOverIn = self.OverInItem
			self.slotList[i].OnMouseOverOut = lambda arg = self.tooltipItem: self.OverOutItem(arg)
			self.slotList[i].Show()

		self.GetChild("ok").SetPosition(0, 94 + 32*ySlotCount + self.height)
		self.GetChild("ok").SetEvent(ui.__mem_func__(self.Close))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		
		self.SetCenterPosition()
		self.SetTop()
		self.Show()
		
		constInfo.IS_OPEN_EVENT_INFORMATION = 1

	def SetCloseEvent(self, event):
		self.titleBar.SetCloseEvent(event)
		
	def SetMessage(self, text):
		self.textLine.SetText(text)

	def OverInItem(self):
		self.tooltipItem.AddItemData(self.itemVnum, 0, 0)
		
	def OverOutItem(self, tooltipItem):
		if 0 != tooltipItem:
			self.tooltipItem.HideToolTip()
			self.tooltipItem.ClearToolTip()
	
	def Close(self):
		self.ClearDictionary()
		self.slotList = []
		self.titleBar = None
		self.titleName = None
		self.textLine2 = None
		self.textLine3 = None
		self.textLine4 = None
		self.slotGrid = None
		self.time = 0
		
		self.tooltipItem = 0
		self.Hide()
		
		constInfo.IS_OPEN_EVENT_INFORMATION = 0
		
	def SetWidth(self, width):
		height = self.GetHeight()
		self.SetSize(width, height)
		self.board.SetSize(width, height)
		self.SetCenterPosition()
		self.UpdateRect()

	def SetText(self, text):
		self.textLine.SetText(text)

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnUpdate(self):
		leftTime = max(0, self.time - app.GetGlobalTimeStamp())
		self.textLine4.SetText(localeInfo.TOOLTIP_TIME % (localeInfo.FormatTime(leftTime)))

class QuestionDialogWithTimeLimit3(QuestionDialog2):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()
		self.endTime = 0
		self.timeOverMsg = 0
		self.timeOverEvent = None
		self.timeOverEventArgs = None

	def __del__(self):
		QuestionDialog2.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/questiondialog2.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self, timeout):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		self.endTime = app.GetTime() + timeout

	def SetTimeOverEvent(self, event, *args):
		self.timeOverEvent = event
		self.timeOverEventArgs = args

	def SetTimeOverMsg(self, msg):
		self.timeOverMsg = msg

	def OnTimeOver(self):
		if self.timeOverEvent:
			apply(self.timeOverEvent, self.timeOverEventArgs)
		if self.timeOverMsg:
			chat.AppendChat(chat.CHAT_TYPE_INFO, self.timeOverMsg)

	def OnUpdate(self):
		leftTime = max(0, self.endTime - app.GetTime())
		self.SetText2(localeInfo.UI_LEFT_TIME % (leftTime))
		if leftTime <= 0:
			self.OnTimeOver()

class QuestionDialogWithdraw(QuestionDialog2):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()

	def __del__(self):
		QuestionDialog2.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialogwithdraw.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")
		self.closeButton = self.GetChild("close")

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()
		
	def SetTextMessage1(self, text):
		self.textLine1.SetText(text)
		
	def SetTextMessage2(self, text):
		self.textLine2.SetText(text)
		
	def SetAcceptText(self, text):
		self.acceptButton.SetText(text)
		
	def SetCancelText(self, text):
		self.cancelButton.SetText(text)
		
	def SetCloseText(self, text):
		self.closeButton.SetText(text)
		
	def SetCloseEvent(self, event):
		self.closeButton.SetEvent(event)
		
		
if app.ENABLE_CAPTCHA_SYSTEM:
	class CaptchaEkran(QuestionDialog2):
		def __init__(self):
			ui.ScriptWindow.__init__(self)

			self.__CreateDialog()
			self.endTime = 0
			self.timeOverMsg = 0
			self.timeOverEvent = None
			self.timeOverEventArgs = None

		def __del__(self):
			QuestionDialog2.__del__(self)

		def __CreateDialog(self):
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/captchawindow.py")

			self.board = self.GetChild("board")
			self.textLine1 = self.GetChild("message1")
			self.textLine2 = self.GetChild("message2")
			self.textLine3 = self.GetChild("message3")
			self.InputValue = self.GetChild("InputValue")
			self.cp1 = self.GetChild("cp1")
			self.cp2 = self.GetChild("cp2")
			self.cp3 = self.GetChild("cp3")
			self.cp4 = self.GetChild("cp4")
			self.cp5 = self.GetChild("cp5")
			self.acceptButton = self.GetChild("accept")
			self.cancelButton = self.GetChild("cancel")
			self.textLine3.Hide()

		def Open(self, timeout, cpp1, cpp2, cpp3, cpp4, cpp5):
			self.SetCenterPosition()
			self.SetTop()
			self.cp1.LoadImage("captcha/"+str(cpp1)+".tga")
			self.cp1.Show()
			self.cp2.LoadImage("captcha/"+str(cpp2)+".tga")
			self.cp2.Show()
			self.cp3.LoadImage("captcha/"+str(cpp3)+".tga")
			self.cp3.Show()
			self.cp4.LoadImage("captcha/"+str(cpp4)+".tga")
			self.cp4.Show()
			self.cp5.LoadImage("captcha/"+str(cpp5)+".tga")
			self.cp5.Show()
			self.Show()

			self.endTime = app.GetTime() + timeout
	

		def SetTimeOverEvent(self, event, *args):
			self.timeOverEvent = event
			self.timeOverEventArgs = args

		def SetTimeOverMsg(self, msg):
			self.timeOverMsg = msg
	
		def SetText1(self, text):
			self.textLine1.SetText(text)
			self.textLine1.Show()

		def SetText2(self, text):
			self.textLine2.SetText(text)
			self.textLine2.Show()
	
		def SetText3(self, text):
			self.textLine3.SetText(text)
			self.textLine3.Show()
	
		def GetInput(self):
			return self.InputValue.GetText()
	
		def Temizle(self):
			self.InputValue.SetText("")

		def OnTimeOver(self):
			if self.timeOverEvent:
				apply(self.timeOverEvent, self.timeOverEventArgs)
			if self.timeOverMsg:
				chat.AppendChat(chat.CHAT_TYPE_INFO, self.timeOverMsg)

		def OnUpdate(self):
			leftTime = max(0, self.endTime - app.GetTime())
			self.SetText2(localeInfo.UI_LEFT_TIME % (leftTime))
			if leftTime <= 0:
				self.OnTimeOver()
				
class QuestionDialogDungeonInfo(QuestionDialog2):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__CreateDialog()
		self.endTime = 0
		self.timeOverMsg = 0
		self.timeOverEvent = None
		self.timeOverEventArgs = None

	def __del__(self):
		QuestionDialog2.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/questiondialog_dungeon_info.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self, timeout):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		self.endTime = app.GetTime() + timeout

	def SetTimeOverEvent(self, event, *args):
		self.timeOverEvent = event
		self.timeOverEventArgs = args

	def SetTimeOverMsg(self, msg):
		self.timeOverMsg = msg

	def OnTimeOver(self):
		if self.timeOverEvent:
			apply(self.timeOverEvent, self.timeOverEventArgs)
		if self.timeOverMsg:
			chat.AppendChat(chat.CHAT_TYPE_INFO, self.timeOverMsg)

	def OnUpdate(self):
		leftTime = max(0, self.endTime - app.GetTime())
		self.SetText2(localeInfo.UI_LEFT_TIME % (leftTime))
		if leftTime <= 0:
			self.OnTimeOver()
			
