import net
import app
import player
import item
import background
import safebox
import chat
import shop
import ui
import mouseModule
import snd
import localeInfo
import uiScriptLocale
import uiCommon

class PasswordDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()

		self.sendMessage = "/safebox_password "
		self.lastPassword = ""

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		self.buttonList = []
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/passworddialog.py")
		except:
			import exception as exception
			exception.Abort("PasswordDialog.__LoadDialog.LoadObject")

		try:
			GetObject = self.GetChild
			self.passwordValue = self.GetChild("password_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
			self.buttonList.append(GetObject("safebox_button"))
			self.buttonList.append(GetObject("mall_button"))
			self.titleName = self.GetChild("TitleName")
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.CloseDialog))
		except:
			import exception as exception
			exception.Abort("PasswordDialog.__LoadDialog.BindObject")

		self.passwordValue.OnIMEReturn = self.OnAccept
		self.passwordValue.OnPressEscapeKey = self.OnCancel
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.OnCancel))
		self.buttonList[0].SAFE_SetEvent(self.__SafeButton)
		self.buttonList[1].SAFE_SetEvent(self.__MallButton)
		self.__SafeButton()

	def Destroy(self):
		self.ClearDictionary()
		self.passwordValue = None
		self.acceptButton = None
		self.cancelButton = None
		self.titleName = None

	def SetTitle(self, title):
		self.titleName.SetText(title)

	def SetSendMessage(self, msg):
		self.sendMessage = msg

	def ShowDialog(self):
		self.passwordValue.SetText("")
		self.passwordValue.SetFocus()
		self.SetCenterPosition()
		self.__SafeButton()
		self.Show()

	def CloseDialog(self):
		self.passwordValue.KillFocus()
		self.Hide()
		
	def __SafeButton(self):
		self.buttonList[0].Down()
		self.buttonList[1].SetUp()
		self.SetSendMessage("/safebox_password ")

	def __MallButton(self):
		self.buttonList[0].SetUp()
		self.buttonList[1].Down()
		self.SetSendMessage("/mall_password ")
		
	def InitSafeboxPassword(self):
		self.lastPassword = ""

	def GetSafeboxPwd(self):
		return self.lastPassword

	def OnAccept(self):
		self.lastPassword = str(self.passwordValue.GetText())
		net.SendChatPacket(self.sendMessage + self.lastPassword)
		self.CloseDialog()
		return True

	def OnCancel(self):
		self.CloseDialog()
		return True

class ChangePasswordDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		self.dlgMessage = ui.ScriptWindow()
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgMessage, "UIScript/popupdialog.py")
			self.dlgMessage.GetChild("message").SetText(localeInfo.SAFEBOX_WRONG_PASSWORD)
			self.dlgMessage.GetChild("accept").SetEvent(ui.__mem_func__(self.OnCloseMessageDialog))
		except:
			import exception as exception
			exception.Abort("SafeboxWindow.__LoadDialog.LoadObject")

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/changepassworddialog.py")

		except:
			import exception as exception
			exception.Abort("ChangePasswordDialog.LoadDialog.LoadObject")

		try:
			self.GetChild("accept_button").SetEvent(ui.__mem_func__(self.OnAccept))
			self.GetChild("cancel_button").SetEvent(ui.__mem_func__(self.OnCancel))
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.OnCancel))
			oldPassword = self.GetChild("old_password_value")
			newPassword = self.GetChild("new_password_value")
			newPasswordCheck = self.GetChild("new_password_check_value")
		except:
			import exception as exception
			exception.Abort("ChangePasswordDialog.LoadDialog.BindObject")

		oldPassword.SetTabEvent(lambda arg=1: self.OnNextFocus(arg))
		newPassword.SetTabEvent(lambda arg=2: self.OnNextFocus(arg))
		newPasswordCheck.SetTabEvent(lambda arg=3: self.OnNextFocus(arg))
		oldPassword.SetReturnEvent(lambda arg=1: self.OnNextFocus(arg))
		newPassword.SetReturnEvent(lambda arg=2: self.OnNextFocus(arg))
		newPasswordCheck.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		oldPassword.OnPressEscapeKey = self.OnCancel
		newPassword.OnPressEscapeKey = self.OnCancel
		newPasswordCheck.OnPressEscapeKey = self.OnCancel

		self.oldPassword = oldPassword
		self.newPassword = newPassword
		self.newPasswordCheck = newPasswordCheck

	def OnNextFocus(self, arg):
		if 1 == arg:
			self.oldPassword.KillFocus()
			self.newPassword.SetFocus()
		elif 2 == arg:
			self.newPassword.KillFocus()
			self.newPasswordCheck.SetFocus()
		elif 3 == arg:
			self.newPasswordCheck.KillFocus()
			self.oldPassword.SetFocus()

	def Destroy(self):
		self.ClearDictionary()
		self.dlgMessage.ClearDictionary()
		self.oldPassword = None
		self.newPassword = None
		self.newPasswordCheck = None

	def Open(self):
		self.oldPassword.SetText("")
		self.newPassword.SetText("")
		self.newPasswordCheck.SetText("")
		self.oldPassword.SetFocus()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.oldPassword.SetText("")
		self.newPassword.SetText("")
		self.newPasswordCheck.SetText("")
		self.oldPassword.KillFocus()
		self.newPassword.KillFocus()
		self.newPasswordCheck.KillFocus()
		self.Hide()

	def OnAccept(self):
		oldPasswordText = self.oldPassword.GetText()
		newPasswordText = self.newPassword.GetText()
		newPasswordCheckText = self.newPasswordCheck.GetText()
		if newPasswordText != newPasswordCheckText:
			self.dlgMessage.SetCenterPosition()
			self.dlgMessage.SetTop()
			self.dlgMessage.Show()
			return True
		net.SendChatPacket("/safebox_change_password %s %s" % (oldPasswordText, newPasswordText))
		self.Close()
		return True

	def OnCancel(self):
		self.Close()
		return True

	def OnCloseMessageDialog(self):
		self.newPassword.SetText("")
		self.newPasswordCheck.SetText("")
		self.newPassword.SetFocus()
		self.dlgMessage.Hide()

class SafeboxWindow(ui.ScriptWindow):

	BOX_WIDTH = 176

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = None
		if app.WJ_TRADABLE_ICON:
			self.interface = None
		self.sellingSlotNumber = -1
		self.pageButtonList = []
		self.curPageIndex = 0
		self.isLoaded = 0
		self.xSafeBoxStart = 0
		self.ySafeBoxStart = 0
		self.pageCount = 0
		if app.ENABLE_SAFEBOX_MONEY_SYSTEM:
			self.questionDialogGold = None
			self.inputDialogGold = None
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)		

	def Destroy(self):
		self.ClearDictionary()

		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = None
		self.dlgChangePassword.Destroy()
		self.dlgChangePassword = None

		self.tooltipItem = None
		self.wndMoneySlot = None
		self.wndMoney = None
		self.wndBoard = None
		self.wndItem = None
		if app.WJ_TRADABLE_ICON:
			self.interface = None
		if app.ENABLE_SAFEBOX_MONEY_SYSTEM:
			self.questionDialogGold = None
			self.inputDialogGold = None

		self.pageButtonList = []

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/SafeboxWindow.py")

		from _weakref import proxy

		## Item
		wndItem = ui.GridSlotWindow()
		wndItem.SetParent(self)
		wndItem.SetPosition(8, 35)
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		wndItem.Show()

		## PickMoneyDialog
		import uiPickMoney as uiPickMoney
		dlgPickMoney = uiPickMoney.PickMoneyDialog2()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
		dlgPickMoney.Hide()

		## ChangePasswrod
		dlgChangePassword = ChangePasswordDialog()
		dlgChangePassword.LoadDialog()
		dlgChangePassword.Hide()

		## Close Button
		self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
		self.GetChild("ChangePasswordButton").SetEvent(ui.__mem_func__(self.OnChangePassword))
		self.GetChild("ExitButton").SetEvent(ui.__mem_func__(self.Close))

		self.wndItem = wndItem
		self.dlgPickMoney = dlgPickMoney
		self.dlgChangePassword = dlgChangePassword
		self.wndBoard = self.GetChild("board")
		
		if app.ENABLE_SAFEBOX_MONEY_SYSTEM:
			self.wndMoney = self.GetChild("Money")
			self.wndMoneySlot = self.GetChild("MoneySlot")
			self.wndMoneySlot.SetEvent(ui.__mem_func__(self.OpenGoldBankQuestionDialog))
			self.wndCheque = self.GetChild("Cheque")
			self.wndChequeSlot = self.GetChild("ChequeSlot")
			self.wndChequeSlot.SetEvent(ui.__mem_func__(self.OpenChequeBankQuestionDialog))
		
		## Initialize
		self.SetTableSize(3)
		self.RefreshSafeboxMoney()
		
	if app.ENABLE_SAFEBOX_MONEY_SYSTEM:
		def OpenGoldBankQuestionDialog(self):
			questionDialogGold = uiCommon.QuestionDialogWithdraw()
			questionDialogGold.SetTextMessage1(localeInfo.SAFEBOX_MONEY_QUESTION_DIALOG_TEXT)
			questionDialogGold.SetAcceptEvent(self.OnGoldSave)
			questionDialogGold.SetCancelEvent(self.OnGoldWithdraw)
			questionDialogGold.SetCloseEvent(self.CloseQuestionDialog)
			questionDialogGold.SetAcceptText(localeInfo.SAFEBOX_SAVE)
			questionDialogGold.SetCancelText(localeInfo.SAFEBOX_WITHDRAW)
			questionDialogGold.Open()
			self.questionDialogGold = questionDialogGold
			
		def OpenChequeBankQuestionDialog(self):
			questionDialogGold = uiCommon.QuestionDialogWithdraw()
			questionDialogGold.SetTextMessage1(localeInfo.SAFEBOX_MONEY_QUESTION_DIALOG_TEXT)
			questionDialogGold.SetAcceptEvent(self.OnChequeSave)
			questionDialogGold.SetCancelEvent(self.OnChequeWithdraw)
			questionDialogGold.SetCloseEvent(self.CloseQuestionDialog)
			questionDialogGold.SetAcceptText(localeInfo.SAFEBOX_SAVE)
			questionDialogGold.SetCancelText(localeInfo.SAFEBOX_WITHDRAW)
			questionDialogGold.Open()
			self.questionDialogGold = questionDialogGold
			
		def CloseQuestionDialog(self):
			if self.questionDialogGold:
				self.questionDialogGold.Close()

		def OnGoldSave(self):
			self.questionDialogGold.Close()
			self.inputDialogGold = uiCommon.InputDialogWithDescription()
			self.inputDialogGold.SetMaxLength(12)
			self.inputDialogGold.SetAcceptEvent(lambda arg=True: self.GoldSave())
			self.inputDialogGold.SetCancelEvent(ui.__mem_func__(self.__CancelGoldInput))		
			self.inputDialogGold.SetAcceptText("+")
			self.inputDialogGold.SetTitle(localeInfo.SAFEBOX_SAVE)
			self.inputDialogGold.SetDescription("Senin paran : " + localeInfo.NumberToMoneyString(player.GetElk()))
			self.inputDialogGold.Open()
			
		def OnGoldWithdraw(self):
			self.questionDialogGold.Close()
			self.inputDialogGold = uiCommon.InputDialogWithDescription()
			self.inputDialogGold.SetMaxLength(12)
			self.inputDialogGold.SetAcceptEvent(lambda arg=True: self.GoldWithdraw())
			self.inputDialogGold.SetCancelEvent(ui.__mem_func__(self.__CancelGoldInput))		
			self.inputDialogGold.SetAcceptText("-")
			self.inputDialogGold.SetTitle(localeInfo.SAFEBOX_WITHDRAW)
			self.inputDialogGold.SetDescription("Depondaki para : " + localeInfo.NumberToMoneyString(safebox.GetMoney()))
			self.inputDialogGold.Open()
			
		def GoldSave(self):
			net.SendSafeboxSaveMoneyPacket(long(self.inputDialogGold.GetText()))
			self.inputDialogGold.Close()
			
		def GoldWithdraw(self):
			net.SendSafeboxWithdrawMoneyPacket(long(self.inputDialogGold.GetText()))
			self.inputDialogGold.Close()
			
		def __CancelGoldInput(self):
			if self.inputDialogGold:
				self.inputDialogGold.Close()
				
		def OnChequeSave(self):
			self.questionDialogGold.Close()
			self.inputDialogGold = uiCommon.InputDialogWithDescription()
			self.inputDialogGold.SetMaxLength(12)
			self.inputDialogGold.SetAcceptEvent(lambda arg=True: self.ChequeSave())
			self.inputDialogGold.SetCancelEvent(ui.__mem_func__(self.__CancelChequeInput))		
			self.inputDialogGold.SetAcceptText("+")
			self.inputDialogGold.SetTitle(localeInfo.SAFEBOX_SAVE)
			self.inputDialogGold.SetDescription("Senin paran : " + localeInfo.NumberToChequeString(player.GetCheque()))
			self.inputDialogGold.Open()
			
		def OnChequeWithdraw(self):
			self.questionDialogGold.Close()
			self.inputDialogGold = uiCommon.InputDialogWithDescription()
			self.inputDialogGold.SetMaxLength(12)
			self.inputDialogGold.SetAcceptEvent(lambda arg=True: self.ChequeWithdraw())
			self.inputDialogGold.SetCancelEvent(ui.__mem_func__(self.__CancelChequeInput))		
			self.inputDialogGold.SetAcceptText("-")
			self.inputDialogGold.SetTitle(localeInfo.SAFEBOX_WITHDRAW)
			self.inputDialogGold.SetDescription("Depondaki para : " + localeInfo.NumberToChequeString(safebox.GetCheque()))
			self.inputDialogGold.Open()
			
		def ChequeSave(self):
			net.SendSafeboxSaveChequePacket(int(self.inputDialogGold.GetText()))
			self.inputDialogGold.Close()
			
		def ChequeWithdraw(self):
			net.SendSafeboxWithdrawChequePacket(int(self.inputDialogGold.GetText()))
			self.inputDialogGold.Close()
			
		def __CancelChequeInput(self):
			if self.inputDialogGold:
				self.inputDialogGold.Close()

	def OpenPickMoneyDialog(self):

		if mouseModule.mouseController.isAttached():

			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			if player.SLOT_TYPE_INVENTORY == mouseModule.mouseController.GetAttachedType():

				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

			mouseModule.mouseController.DeattachObject()

		else:
			curMoney = safebox.GetMoney()

			if curMoney <= 0:
				return

			self.dlgPickMoney.Open(curMoney)

	def ShowWindow(self, size):

		(self.xSafeBoxStart, self.ySafeBoxStart, z) = player.GetMainCharacterPosition()
		
		if app.WJ_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_SAFEBOX)
			self.interface.RefreshMarkInventoryBag()

		self.SetTableSize(size)
		self.Show()
		
	def __MakePageButton(self, pageCount):

		self.curPageIndex = 0
		self.pageButtonList = []
		self.pageCount = pageCount

		text = "I"
		pos = -int(float(pageCount-1)/2 * 52)
		for i in xrange(pageCount):
			button = ui.RadioButton()
			button.SetParent(self)
			button.SetUpVisual("d:/ymir work/ui/game/windows/tab_button_middle_01.sub")
			button.SetOverVisual("d:/ymir work/ui/game/windows/tab_button_middle_02.sub")
			button.SetDownVisual("d:/ymir work/ui/game/windows/tab_button_middle_03.sub")
			button.SetWindowHorizontalAlignCenter()
			button.SetWindowVerticalAlignBottom()
			button.SetPosition(pos, 115)
			button.SetText(text)
			button.SetEvent(lambda arg=i: self.SelectPage(arg))
			button.Show()
			self.pageButtonList.append(button)

			pos += 52
			text += "I"

		self.pageButtonList[0].Down()

	def SelectPage(self, index):

		self.curPageIndex = index

		for btn in self.pageButtonList:
			btn.SetUp()

		self.pageButtonList[index].Down()
		self.RefreshSafebox()

	def __LocalPosToGlobalPos(self, local):
		return self.curPageIndex*safebox.SAFEBOX_PAGE_SIZE + local

	def SetTableSize(self, size):

		pageCount = max(1, size / safebox.SAFEBOX_SLOT_Y_COUNT)
		pageCount = min(3, pageCount)
		size = safebox.SAFEBOX_SLOT_Y_COUNT
		self.__MakePageButton(pageCount)

		self.wndItem.ArrangeSlot(0, safebox.SAFEBOX_SLOT_X_COUNT, size, 32, 32, 0, 0)
		self.wndItem.RefreshSlot()
		self.wndItem.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)

		wnd_height = 160 + 32 * size
		self.wndBoard.SetSize(self.BOX_WIDTH, wnd_height)
		self.SetSize(self.BOX_WIDTH, wnd_height)
		self.UpdateRect()

	def RefreshSafebox(self):
		getItemID=safebox.GetItemID
		getItemCount=safebox.GetItemCount
		setItemID=self.wndItem.SetItemSlot

		for i in xrange(safebox.SAFEBOX_PAGE_SIZE):
			slotIndex = self.__LocalPosToGlobalPos(i)
			itemCount = getItemCount(slotIndex)
			if itemCount <= 1:
				itemCount = 0
			setItemID(i, getItemID(slotIndex), itemCount)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				itemTransmutedVnum = safebox.GetItemTransmutation(slotIndex)
				if itemTransmutedVnum:
					self.wndItem.DisableCoverButton(i)
				else:
					self.wndItem.EnableCoverButton(i)

		self.wndItem.RefreshSlot()

	def RefreshSafeboxMoney(self):
		if app.ENABLE_SAFEBOX_MONEY_SYSTEM:
			self.wndMoney.SetText(localeInfo.NumberToMoneyString(safebox.GetMoney()))
			self.wndCheque.SetText(localeInfo.NumberToChequeString(safebox.GetCheque()))
		else:
			pass

	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip

	def Close(self):
		safebox.Close()
		if app.WJ_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		net.SendChatPacket("/safebox_close")

	#def CommandCloseSafebox(self):
	#	if self.tooltipItem:
	#		self.tooltipItem.HideToolTip()

	#	self.dlgPickMoney.Close()
	#	self.dlgChangePassword.Close()
	#	if app.WJ_TRADABLE_ICON:
	#		self.interface.RefreshMarkInventoryBag()
	#	self.Hide()
	def CommandCloseSafebox(self):
		pList = [self.dlgPickMoney, self.dlgChangePassword, self]
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
		for a in pList:
			a.Hide()
		if app.ENABLE_CHECK_IF_SAFEBOX_OPEN:
			safebox.Close()
	## Slot Event
	def SelectEmptySlot(self, selectedSlotPos):

		selectedSlotPos = self.__LocalPosToGlobalPos(selectedSlotPos)

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			
			if player.SLOT_TYPE_SAFEBOX == attachedSlotType:
	
				net.SendSafeboxItemMovePacket(attachedSlotPos, selectedSlotPos, 0)
				#snd.PlaySound("sound/ui/drop.wav")
			else:
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				if player.RESERVED_WINDOW == attachedInvenType:
					return
	
				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")
	
				else:
					net.SendSafeboxCheckinPacket(attachedInvenType, attachedSlotPos, selectedSlotPos)
					#snd.PlaySound("sound/ui/drop.wav")

		mouseModule.mouseController.DeattachObject()

	def SelectItemSlot(self, selectedSlotPos):

		selectedSlotPos = self.__LocalPosToGlobalPos(selectedSlotPos)

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()

			if player.SLOT_TYPE_INVENTORY == attachedSlotType:

				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

				else:
					attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
					#net.SendSafeboxCheckinPacket(attachedSlotPos, selectedSlotPos)
					#snd.PlaySound("sound/ui/drop.wav")

			mouseModule.mouseController.DeattachObject()

		else:

			curCursorNum = app.GetCursor()
			if app.SELL == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SAFEBOX_SELL_DISABLE_SAFEITEM)

			elif app.BUY == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)

			else:
				selectedItemID = safebox.GetItemID(selectedSlotPos)
				mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_SAFEBOX, selectedSlotPos, selectedItemID)
				snd.PlaySound("sound/ui/pick.wav")

	def UseItemSlot(self, slotIndex):
		mouseModule.mouseController.DeattachObject()

	def __ShowToolTip(self, slotIndex):
		if self.tooltipItem:
			self.tooltipItem.SetSafeBoxItem(slotIndex)

	def OverInItem(self, slotIndex):
		slotIndex = self.__LocalPosToGlobalPos(slotIndex)
		self.wndItem.SetUsableItem(False)
		self.__ShowToolTip(slotIndex)

	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnPickMoney(self, money):
		mouseModule.mouseController.AttachMoney(self, player.SLOT_TYPE_SAFEBOX, money)

	def OnChangePassword(self):
		self.dlgChangePassword.Open()
			
	if app.ENABLE_FAST_ATTACH_ITEMS_SYSTEM:
		def __GetCurrentItemGrid(self):
			itemGrid = [[False for cell in xrange(safebox.SAFEBOX_PAGE_SIZE)] for page in xrange(self.pageCount)]

			for page in xrange(self.pageCount):
				for slot in xrange(safebox.SAFEBOX_PAGE_SIZE):
					itemVnum = safebox.GetItemID(slot + page * safebox.SAFEBOX_PAGE_SIZE)
					if itemVnum <> 0:
						item.SelectItem(itemVnum)
						(w, h) = item.GetItemSize()
						for i in xrange(h):
							itemGrid[page][slot + i * safebox.SAFEBOX_SLOT_X_COUNT] = True

			return itemGrid

		def __FindEmptyCellForSize(self, itemGrid, size):
			for page in xrange(self.pageCount):
				for slot in xrange(safebox.SAFEBOX_PAGE_SIZE):
					if itemGrid[page][slot] == False:
						possible = True
						for i in xrange(size):
							p = slot + safebox.SAFEBOX_SLOT_X_COUNT * i
							try:
								if itemGrid[page][p] == True:
									possible = False
									break
							except IndexError:
								possible = False
								break

						if possible:
							return slot + page * safebox.SAFEBOX_PAGE_SIZE

			return -1

		def AttachItemFromInventory(self, slotWindow, slotIndex):
			attachedItemID = player.GetItemIndex(slotWindow, slotIndex)
			item.SelectItem(attachedItemID)
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_SAFEBOX):
				return False

			itemGrid = self.__GetCurrentItemGrid()

			item.SelectItem(attachedItemID) 
			emptySafeboxSlot = self.__FindEmptyCellForSize(itemGrid, item.GetItemSize()[1])
			if emptySafeboxSlot == -1 or item.GetItemType() == item.ITEM_TYPE_ELK:
				return False

			net.SendSafeboxCheckinPacket(slotWindow, slotIndex, emptySafeboxSlot)
			return True

		def UseItemSlot(self, slotIndex):
			if mouseModule.mouseController.isAttached():
				mouseModule.mouseController.DeattachObject()

			if self.interface:
				globalSlotIndex = self.__LocalPosToGlobalPos(slotIndex)
				self.interface.AttachItemFromSafebox(globalSlotIndex, safebox.GetItemID(globalSlotIndex))

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self):

		USE_SAFEBOX_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xSafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE or abs(y - self.ySafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE:
			self.Close()
			
	if app.WJ_TRADABLE_ICON:
		def BindInterface(self, interface):
			self.interface = interface
			
		def CantCheckInItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemIsAntiFlagBySlot = player.IsAntiFlagBySlot(player.INVENTORY, slotPos, item.ITEM_ANTIFLAG_SAFEBOX)
				if itemIsAntiFlagBySlot:
					return True
			
			return False

		def OnTop(self):
			self.interface.SetOnTopWindow(player.ON_TOP_WND_SAFEBOX)
			self.interface.RefreshMarkInventoryBag()

class MallWindow(ui.ScriptWindow):

	BOX_WIDTH = 176
	
	if app.ENABLE_SAFEBOX_UPDATE:
		PAGE_TEXT = ( "I", "II", "III", "IV", "V")

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = None
		self.sellingSlotNumber = -1
		self.pageButtonList = []
		self.curPageIndex = 0
		self.isLoaded = 0
		self.xSafeBoxStart = 0
		self.ySafeBoxStart = 0

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)		

	def Destroy(self):
		self.ClearDictionary()

		self.tooltipItem = None
		self.wndBoard = None
		self.wndItem = None

		self.pageButtonList = []

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/MallWindow.py")

		from _weakref import proxy

		## Item
		wndItem = ui.GridSlotWindow()
		wndItem.SetParent(self)
		wndItem.SetPosition(8, 35)
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		wndItem.Show()

		## Close Button
		self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
		self.GetChild("ExitButton").SetEvent(ui.__mem_func__(self.Close))

		self.wndItem = wndItem
		self.wndBoard = self.GetChild("board")

		## Initialize
		self.SetTableSize(3)

	def ShowWindow(self, size):

		(self.xSafeBoxStart, self.ySafeBoxStart, z) = player.GetMainCharacterPosition()

		self.SetTableSize(size)
		self.Show()

	def SetTableSize(self, size):

		pageCount = max(1, size / safebox.SAFEBOX_SLOT_Y_COUNT)
		if app.ENABLE_SAFEBOX_UPDATE:
			pageCount = min(5, pageCount)
		else:
			pageCount = min(3, pageCount)
		size = safebox.SAFEBOX_SLOT_Y_COUNT
		
		if app.ENABLE_SAFEBOX_UPDATE:
			self.__MakePageButton(pageCount)

		self.wndItem.ArrangeSlot(0, safebox.SAFEBOX_SLOT_X_COUNT, size, 32, 32, 0, 0)
		self.wndItem.RefreshSlot()
		self.wndItem.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
		if app.ENABLE_SAFEBOX_UPDATE:
			self.wndBoard.SetSize(self.BOX_WIDTH, 102 + 32*size)
			self.SetSize(self.BOX_WIDTH, 105 + 32*size)
		else:
			self.wndBoard.SetSize(self.BOX_WIDTH, 82 + 32*size)
			self.SetSize(self.BOX_WIDTH, 85 + 32*size)
		self.UpdateRect()
		
	if app.ENABLE_SAFEBOX_UPDATE:
		def __MakePageButton(self, pageCount):
			
			self.curPageIndex = 0
			self.pageButtonList = []
			
			pos = -int(float(pageCount-1)/2 * 32)
			for i in xrange(pageCount):
				text = self.PAGE_TEXT[i]
				button = ui.RadioButton()
				button.SetParent(self)
				button.SetUpVisual("d:/ymir work/ui/game/windows/tab_button_small_01.sub")
				button.SetOverVisual("d:/ymir work/ui/game/windows/tab_button_small_02.sub")
				button.SetDownVisual("d:/ymir work/ui/game/windows/tab_button_small_03.sub")
				button.SetWindowHorizontalAlignCenter()
				button.SetWindowVerticalAlignBottom()
				button.SetPosition(pos, 65)
				button.SetText(text)
				button.SetEvent(lambda arg=i: self.SelectPage(arg))
				button.Show()
				self.pageButtonList.append(button)
				
				pos += 32
			
			self.pageButtonList[0].Down()
			
		def SelectPage(self, index):
		
			self.curPageIndex = index
		
			for btn in self.pageButtonList:
				btn.SetUp()
		
			self.pageButtonList[index].Down()
			self.RefreshMall()
			
			
		def __LocalPosToGlobalPos(self, local):
			return self.curPageIndex*safebox.SAFEBOX_PAGE_SIZE + local

	def RefreshMall(self):
		getItemID=safebox.GetMallItemID
		getItemCount=safebox.GetMallItemCount
		setItemID=self.wndItem.SetItemSlot

		if app.ENABLE_SAFEBOX_UPDATE:
			for i in xrange(safebox.SAFEBOX_PAGE_SIZE):
				slotIndex = self.__LocalPosToGlobalPos(i)
				itemCount = getItemCount(slotIndex)
				if itemCount <= 1:
					itemCount = 0
				setItemID(i, getItemID(slotIndex), itemCount)
				if app.ENABLE_CHANGELOOK_SYSTEM:
					itemTransmutedVnum = safebox.GetMallItemTransmutation(i)
					if itemTransmutedVnum:
						self.wndItem.DisableCoverButton(i)
					else:
						self.wndItem.EnableCoverButton(i)

		self.wndItem.RefreshSlot()

	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip

	def Close(self):
		net.SendChatPacket("/mall_close")

	def CommandCloseMall(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.Hide()

	## Slot Event
	def SelectEmptySlot(self, selectedSlotPos):

		if mouseModule.mouseController.isAttached():

			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MALL_CANNOT_INSERT)
			mouseModule.mouseController.DeattachObject()

	def SelectItemSlot(self, selectedSlotPos):
		if app.ENABLE_SAFEBOX_UPDATE:
			selectedSlotPos = self.__LocalPosToGlobalPos(selectedSlotPos)
			
		if mouseModule.mouseController.isAttached():

			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MALL_CANNOT_INSERT)
			mouseModule.mouseController.DeattachObject()

		else:

			curCursorNum = app.GetCursor()
			selectedItemID = safebox.GetMallItemID(selectedSlotPos)
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_MALL, selectedSlotPos, selectedItemID)
			snd.PlaySound("sound/ui/pick.wav")

	def UseItemSlot(self, slotIndex):
		mouseModule.mouseController.DeattachObject()

	def __ShowToolTip(self, slotIndex):
		if self.tooltipItem:
			self.tooltipItem.SetMallItem(slotIndex)

	def OverInItem(self, slotIndex):
		self.__ShowToolTip(slotIndex)

	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self):

		USE_SAFEBOX_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xSafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE or abs(y - self.ySafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE:
			self.Close()
