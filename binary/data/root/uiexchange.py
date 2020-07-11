import player
import exchange
import net
import localeInfo
import chat
import item
import constInfo
import ui
import mouseModule
import uiPickMoney
import wndMgr
import chr
import guild
import uiCommon
import messenger
import app
from time import strftime
from datetime import datetime
import uiToolTip

HIGHLIGHT_SLOT = "d:/ymir work/ui/public/slot_highlight.sub"
BASE_VISUALS_ROUTE = "d:/ymir work/ui/game/exchange/"
class ExchangeDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.TitleName = 0
		self.tooltipItem = 0
		self.xStart = 0
		self.yStart = 0
		self.interface = 0
		self.wndInventory = 0
		self.lockedItems = {i:(-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
		self.questionDialog = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		PythonScriptLoader = ui.PythonScriptLoader()
		PythonScriptLoader.LoadScriptFile(self, "UIScript/exchangedialog.py")

		## Owner
		self.OwnerSlot = self.GetChild("Owner_Slot")
		self.OwnerSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectOwnerEmptySlot))
		self.OwnerSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectOwnerItemSlot))
		self.OwnerSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInOwnerItem))
		self.OwnerSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.OwnerMoney = self.GetChild("Owner_Money_Value")
		self.OwnerMoneyButton = self.GetChild("Owner_Money")
		self.OwnerMoneyButton.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog))
		self.OwnerCheque = self.GetChild("Owner_Cheque_Value")
		self.OwnerChequeButton = self.GetChild("Owner_Cheque")
		self.OwnerChequeButton.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog))

		## Target
		self.TargetSlot = self.GetChild("Target_Slot")
		self.TargetSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInTargetItem))
		self.TargetSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.TargetMoney = self.GetChild("Target_Money_Value")
		self.TargetCheque = self.GetChild("Target_Cheque_Value")

		## PickMoneyDialog
		dlgPickMoney = uiPickMoney.PickMoneyDialog()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
		dlgPickMoney.SetTitleName(localeInfo.EXCHANGE_MONEY)
		dlgPickMoney.SetMax(9)
		dlgPickMoney.Hide()
		self.dlgPickMoney = dlgPickMoney

		## Button
		self.AcceptButton = self.GetChild("Owner_Accept_Button")
		self.AcceptButton.SetToggleDownEvent(ui.__mem_func__(self.AcceptExchange))

		self.TitleName = self.GetChild("TitleName")
		self.GetChild("TitleBar").SetCloseEvent(net.SendExchangeExitPacket)
		
		self.InformationButton = self.GetChild("InformationButton")
		self.toolTipInfo = uiToolTip.ToolTip()

	def Destroy(self):
		print "---------------------------------------------------------------------------- DESTROY EXCHANGE"
		self.ClearDictionary()
		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = 0
		self.OwnerSlot = 0
		self.OwnerMoney = 0
		self.OwnerCheque = 0
		self.OwnerMoneyButton = 0
		self.TargetSlot = 0
		self.TargetMoney = 0
		self.TargetCheque = 0
		self.TitleName = 0
		self.AcceptButton = 0
		self.tooltipItem = 0
		self.toolTipInfo = 0
		self.interface = 0
		self.wndInventory = 0
		self.lockedItems = {i:(-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
		self.InformationButton = 0
		self.questionDialog = 0

	def OpenDialog(self):
		targetVID = exchange.GetTargetVID()
		self.TitleName.SetText(localeInfo.EXCHANGE_TITLE % (chr.GetNameByVID(targetVID)))
		self.toolTipInfo.ClearToolTip()
		self.toolTipInfo.AutoAppendTextLine(localeInfo.EXCHANGE_INFO_NAME % (chr.GetNameByVID(targetVID)))
		self.toolTipInfo.AutoAppendTextLine(localeInfo.EXCHANGE_INFO_LV % (chr.GetLevelByVID(targetVID)))
		if chr.GetGuildID(targetVID) == 0:
			self.toolTipInfo.AutoAppendTextLine(localeInfo.EXCHANGE_INFO_GUILD % "Lonca yok.")
		else:
			self.toolTipInfo.AutoAppendTextLine(localeInfo.EXCHANGE_INFO_GUILD % (guild.GetGuildName(chr.GetGuildID(targetVID))))
		self.toolTipInfo.AlignHorizonalCenter()
		self.AcceptButton.Enable()
		self.AcceptButton.SetUp()
		self.Show()

		(self.xStart, self.yStart, z) = player.GetMainCharacterPosition()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_EXCHANGE)
		self.interface.RefreshMarkInventoryBag()

	def CloseDialog(self):
		wndMgr.OnceIgnoreMouseLeftButtonUpEvent()

		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()
		if 0 != self.toolTipInfo:
			self.toolTipInfo.Hide()

		self.dlgPickMoney.Close()
		self.Hide()
		for exchangePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
			if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

		self.lockedItems = {i:(-1,-1) for i in range(exchange.EXCHANGE_ITEM_MAX_NUM)}
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def OpenPickMoneyDialog(self):

		if exchange.GetElkFromSelf() > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANT_EDIT_MONEY)
			return

		if exchange.GetChequeFromSelf() > 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANT_EDIT_MONEY)
			return
		
		self.dlgPickMoney.Open(player.GetElk(), player.GetCheque())
		
	def OnPickMoney(self, money, cheque = None):
		if money > 0:
			net.SendExchangeElkAddPacket(money)
		if cheque > 0:
			net.SendExchangeChequeAddPacket(cheque)

	def AcceptExchange(self):
		emptyslots = 0;
		emptyslots_self = 0;
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemVnum_self = exchange.GetItemVnumFromSelf(i)
			itemVnum = exchange.GetItemVnumFromTarget(i)
			if itemVnum == 0:
				emptyslots += 1
			if itemVnum_self == 0:
				emptyslots_self += 1

		if emptyslots_self != exchange.EXCHANGE_ITEM_MAX_NUM:
			if emptyslots == exchange.EXCHANGE_ITEM_MAX_NUM and exchange.GetElkFromTarget() <= 10000 and not exchange.GetAcceptFromSelf():
				self.questionDialog = uiCommon.QuestionDialog3()
				self.questionDialog.SetText1("Bir kiþi ile ticaret yapmak üzerisin")
				self.questionDialog.SetText2("lütfen ticaret yaparken dikkatli ol.")
				self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.DoExchange))
				self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
				self.questionDialog.Open()
			else:
				self.DoExchange()
		else:
			self.DoExchange()

	def DoExchange(self):
		if exchange.GetAcceptFromSelf() == True:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_ALREADY_ACCEPTED)
			return
		net.SendExchangeAcceptPacket()
		self.OnCloseQuestionDialog()
		
	def OnCloseQuestionDialog(self):
		if not self.questionDialog:
			return
		
		self.questionDialog.Close()
		self.questionDialog = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
		
	def SelectOwnerEmptySlot(self, SlotIndex):

		if False == mouseModule.mouseController.isAttached():
			return

		if mouseModule.mouseController.IsAttachedMoney():
			net.SendExchangeElkAddPacket(mouseModule.mouseController.GetAttachedMoneyAmount())
		else:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			if (player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType or player.SLOT_TYPE_SKILL_BOOK_INVENTORY == attachedSlotType or player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY == attachedSlotType or player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType or player.SLOT_TYPE_ATTR_INVENTORY == attachedSlotType or player.SLOT_TYPE_FLOWER_INVENTORY == attachedSlotType):

				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				SrcSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
				DstSlotNumber = SlotIndex
				
				if exchange.GetItemVnumFromSelf(SlotIndex) != 0:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_NONEMPTY_SLOT)
					mouseModule.mouseController.DeattachObject()
					return	

				itemID = player.GetItemIndex(attachedInvenType, SrcSlotNumber)
				item.SelectItem(itemID)

				if item.IsAntiFlag(item.ANTIFLAG_GIVE):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE)
					mouseModule.mouseController.DeattachObject()
					return
					
				if player.GetItemSealDate(attachedInvenType, SrcSlotNumber):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE_SEAL_ITEM)
					mouseModule.mouseController.DeattachObject()
					return
						
				if player.IsEquipmentSlot(SrcSlotNumber) and player.SLOT_TYPE_INVENTORY == attachedSlotType:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE_EQUIPPED_ITEMS)
					mouseModule.mouseController.DeattachObject()
					return

				net.SendExchangeItemAddPacket(attachedInvenType, SrcSlotNumber, DstSlotNumber)

		mouseModule.mouseController.DeattachObject()

	def SelectOwnerItemSlot(self, SlotIndex):
		if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
			if player.GetElk() < long(mouseModule.mouseController.GetAttachedItemCount()):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_ENOUGH_ELK)
				return
			money = mouseModule.mouseController.GetAttachedItemCount()
			net.SendExchangeElkAddPacket(money)
			
	def RefreshOwnerSlot(self):
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemIndex = exchange.GetItemVnumFromSelf(i)
			itemCount = exchange.GetItemCountFromSelf(i)
			if 1 == itemCount:
				itemCount = 0
			self.OwnerSlot.SetItemSlot(i, itemIndex, itemCount)
			itemTransmutedVnum = exchange.GetItemTransmutation(i, True)
			if itemTransmutedVnum:
				self.OwnerSlot.DisableCoverButton(i)
			else:
				self.OwnerSlot.EnableCoverButton(i)
		self.OwnerSlot.RefreshSlot()

	def RefreshTargetSlot(self):
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemIndex = exchange.GetItemVnumFromTarget(i)
			itemCount = exchange.GetItemCountFromTarget(i)
			if 1 == itemCount:
				itemCount = 0
			self.TargetSlot.SetItemSlot(i, itemIndex, itemCount)
			itemTransmutedVnum = exchange.GetItemTransmutation(i, False)
			if itemTransmutedVnum:
				self.TargetSlot.DisableCoverButton(i)
			else:
				self.TargetSlot.EnableCoverButton(i)
		self.TargetSlot.RefreshSlot()
		
	def Refresh(self):
		targetVID = exchange.GetTargetVID()
		self.RefreshOwnerSlot()
		self.RefreshTargetSlot()
		self.RefreshLockedSlot()

		self.OwnerMoney.SetText(localeInfo.NumberToString(exchange.GetElkFromSelf()))
		self.TargetMoney.SetText(localeInfo.NumberToString(exchange.GetElkFromTarget()))
		if app.ENABLE_CHEQUE_SYSTEM:
			self.OwnerCheque.SetText(localeInfo.NumberToString(exchange.GetChequeFromSelf()))
			self.TargetCheque.SetText(localeInfo.NumberToString(exchange.GetChequeFromTarget()))

		if exchange.GetAcceptFromSelf() and exchange.GetAcceptFromTarget():
			self.TitleName.SetText(localeInfo.EXCHANGE_TITLE_ALL_ACCEPTED % (chr.GetNameByVID(targetVID)))
		else:
			if exchange.GetAcceptFromSelf():
				self.TitleName.SetText(localeInfo.EXCHANGE_TITLE_SELF_ACCEPTED % (chr.GetNameByVID(targetVID)))
			elif exchange.GetAcceptFromTarget():
				self.TitleName.SetText(localeInfo.EXCHANGE_TITLE_TARGET_ACCEPTED % (chr.GetNameByVID(targetVID)))
			else:
				self.TitleName.SetText(localeInfo.EXCHANGE_TITLE % (chr.GetNameByVID(targetVID)))
			
	def OverInOwnerItem(self, slotIndex):

		if 0 != self.tooltipItem:
			self.tooltipItem.SetExchangeOwnerItem(slotIndex)

	def OverInTargetItem(self, slotIndex):

		if 0 != self.tooltipItem:
			self.tooltipItem.SetExchangeTargetItem(slotIndex)

	def OverOutItem(self):

		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnTop(self):
		self.tooltipItem.SetTop()
		if self.interface:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_EXCHANGE)
			self.interface.RefreshMarkInventoryBag()
			
	def BindInterface(self, interface):
		self.interface = interface
		
	def OnUpdate(self):

		USE_EXCHANGE_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xStart) > USE_EXCHANGE_LIMIT_RANGE or abs(y - self.yStart) > USE_EXCHANGE_LIMIT_RANGE:
			(self.xStart, self.yStart, z) = player.GetMainCharacterPosition()
			net.SendExchangeExitPacket()
			
		if self.InformationButton.IsIn():
			self.toolTipInfo.Show()
		else:
			self.toolTipInfo.Hide()
			
		if app.WJ_TRADABLE_ICON:
			self.RefreshLockedSlot()
			
	def CantTradableItem(self, destSlotIndex, srcSlotIndex):
		if True == exchange.GetAcceptFromTarget():
			return

		itemInvenPage = srcSlotIndex / player.INVENTORY_PAGE_SIZE
		localSlotPos = srcSlotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
		self.lockedItems[destSlotIndex] = (itemInvenPage, localSlotPos)

		if self.wndInventory.GetInventoryPageIndex() == itemInvenPage and self.IsShow():
			self.wndInventory.wndItem.SetCantMouseEventSlot(localSlotPos)

	def RefreshLockedSlot(self):
		if self.wndInventory:
			for exchangePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
				if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
					self.wndInventory.wndItem.SetCantMouseEventSlot(itemSlotPos)

			self.wndInventory.wndItem.RefreshSlot()

	def SetInven(self, wndInventory):
		from _weakref import proxy
		self.wndInventory = proxy(wndInventory)
		
