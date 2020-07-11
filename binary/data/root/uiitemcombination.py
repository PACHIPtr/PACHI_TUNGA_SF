import app
import player
import net
import item
import systemSetting
import chat
import ui
import exception
import uiCommon
import grp
import snd
import wndMgr
import localeInfo
import uiToolTip
import mouseModule
import uiScriptLocale
import uiInventory
import constInfo

class ItemCombinationWindow(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.xShopStart = 0
		self.yShopStart = 0
		self.slot_pos_index = 999
		self.slot_index1 = 999
		self.slot_index2 = 999
		self.isUsable = True
		self.isLoaded = 0
		self.interface = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/ItemCombinationWindow.py")

		except:
			exception.Abort("ItemCombinationWindow.LoadDialog.LoadScript")

		try:
			self.titleBar = self.GetChild("TitleBar")
			self.ItemCombBackgroundImage = self.GetChild("Item_Comb_Background_Image")
			self.mediumSlot = self.GetChild("MediumSlot")
			self.combSlot = self.GetChild("CombSlot")
			self.acceptButton = self.GetChild("AcceptButton")
			self.cancelButton = self.GetChild("CancelButton")
		except:
			exception.Abort("ItemCombinationWindow.LoadDialog.BindObject")

		self.mediumSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.mediumSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.mediumSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
		self.mediumSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
		
		self.combSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.combSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.combSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
		self.combSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.__OnCloseButtonClick))
		self.ItemCombBackgroundImage.Hide()
		self.combSlot.Hide()
		self.acceptButton.Hide()
		self.cancelButton.Hide()

		self.cambItemInfo = {}
		self.tooltipItem = None

	def IsOpened(self):
		if self.isLoaded == 1:
			return True

		return False

	def SetItemToolTip(self, itemTooltip):
		self.tooltipItem = itemTooltip

	def Destroy(self):
		self.ClearDictionary()
		self.titleBar = None
		self.acceptButton = None
		self.cancelButton = None
		self.combSlot = None
		self.tooltipItem = None
		self.slot_pos_index = None
		self.slot_index1 = None
		self.slot_index2 = None
		self.mediumSlot = None
		self.ItemCombBackgroundImage = None
		self.interface = None

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.cambItemInfo = {}
		self.Refresh()
		self.Show()
		self.isUsable = True
		(self.xShopStart, self.yShopStart, z) = player.GetMainCharacterPosition()

		self.slot_pos_index = 999
		self.slot_index1 = 999
		self.slot_index2 = 999
		self.combSlot.ClearSlot(1)
		self.combSlot.ClearSlot(2)
		self.combSlot.ClearSlot(3)
		self.mediumSlot.ClearSlot(0)
		self.ItemCombBackgroundImage.Hide()
		self.combSlot.Hide()
		self.acceptButton.Hide()
		self.cancelButton.Hide()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_COSTUME_COMBINATION)
		self.interface.RefreshMarkInventoryBag()

	def OnPressEscapeKey(self):
		self.__OnCloseButtonClick()
		return True

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.Hide()
		self.cambItemInfo = {}
		self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCloseButtonClick))
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def Clear(self):
		net.SendChatPacket("/item_comb del_item %d " % 1)
		self.slot_pos_index = 999
		self.slot_index1 = 999
		self.slot_index2 = 999
		self.combSlot.ClearSlot(1)
		self.combSlot.ClearSlot(2)
		self.combSlot.ClearSlot(3)
		del self.cambItemInfo[2]
		self.mediumSlot.ClearSlot(0)
		self.ItemCombBackgroundImage.Hide()
		self.combSlot.Hide()
		self.acceptButton.Hide()
		self.cancelButton.Hide()

	def Refresh(self):
		pass

	def __OnCloseButtonClick(self):
		if self.isUsable:
			self.isUsable = False

			print "item_comb close!"
			net.SendChatPacket("/item_comb close")

		self.Close()
		
	def __OnAcceptButtonClick(self):
		self.ConfirmCombination = uiCommon.QuestionDialog2()
		self.ConfirmCombination.SetText1(localeInfo.COMB_WILL_REMOVE_MATERIAL)
		self.ConfirmCombination.SetText2(localeInfo.COMB_IS_CONTINUE_PROCESS)
		self.ConfirmCombination.SetAcceptEvent(self.__YesAcceptButtonClick)
		self.ConfirmCombination.SetCancelEvent(self.__NoAcceptButtonClick)
		self.ConfirmCombination.Open()

	def __NoAcceptButtonClick(self):
		self.ConfirmCombination.Close()

	def __YesAcceptButtonClick(self):
		if self.slot_index2 != 999:
			print "item_comb make_cotume!"
			net.SendChatPacket("/item_comb make_cotume")
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOT_ALL_SLOT_APPEND_ITEM)

		self.ConfirmCombination.Close()

	def __OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			if (player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType and player.SLOT_TYPE_SKILL_BOOK_INVENTORY != attachedSlotType and player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY != attachedSlotType and player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType and player.SLOT_TYPE_CHEST_INVENTORY != attachedSlotType):
				return
				
			item.SelectItem(player.GetItemIndex(attachedSlotPos))
			itemVnum = player.GetItemIndex(attachedSlotPos)

			if selectedSlotPos == 0:
				if item.IsTransferScroll(itemVnum):
					self.cambItemInfo[selectedSlotPos] = attachedSlotPos
					self.mediumSlot.ClearSlot(selectedSlotPos)
					self.mediumSlot.SetItemSlot(selectedSlotPos, player.GetItemIndex(attachedSlotPos), player.GetItemCount(attachedSlotPos))
					net.SendChatPacket("/item_comb add_item %d %d" % (selectedSlotPos, attachedSlotPos))
					self.ItemCombBackgroundImage.Show()
					self.combSlot.Show()
					self.acceptButton.SetEvent(ui.__mem_func__(self.__OnAcceptButtonClick))
					self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCloseButtonClick))
					self.acceptButton.Show()
					self.cancelButton.Show()
				else:
					snd.PlaySound("sound/ui/make_soket.wav")
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOT_MEDIUM_ITEM)
					mouseModule.mouseController.DeattachObject()
					return
			else:
				## Cheque Costumes Types e SubTypes
				if item.GetItemType() == item.ITEM_TYPE_COSTUME and selectedSlotPos == 1 or item.GetItemType() == item.ITEM_TYPE_COSTUME and selectedSlotPos == 2:
					if item.GetItemSubType() == item.COSTUME_TYPE_SASH and selectedSlotPos == 1 or item.GetItemSubType() == item.COSTUME_TYPE_SASH and selectedSlotPos == 2:
						snd.PlaySound("sound/ui/make_soket.wav")
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_CANT_APPEND_ITEM)
						mouseModule.mouseController.DeattachObject()
						return

					if item.GetItemSubType() == item.USE_MOUNT and selectedSlotPos == 1 or item.GetItemSubType() == item.USE_MOUNT and selectedSlotPos == 2:
						snd.PlaySound("sound/ui/make_soket.wav")
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_CANT_APPEND_ITEM)
						mouseModule.mouseController.DeattachObject()
						return

					if selectedSlotPos == 2 and self.slot_pos_index == 999:
						# chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
						return

					if self.slot_index1 == attachedSlotPos and selectedSlotPos == 1 or self.slot_index2 == attachedSlotPos and selectedSlotPos == 2:
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
						return
					elif self.slot_index2 == attachedSlotPos and selectedSlotPos == 1 or self.slot_index1 == attachedSlotPos and selectedSlotPos == 2:
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
						return

					if selectedSlotPos == 1:
						self.slot_index1 = attachedSlotPos
					elif selectedSlotPos == 2:
						self.slot_index2 = attachedSlotPos

					for slotPos, invenPos in self.cambItemInfo.items():
						if invenPos == attachedSlotPos:
							del self.cambItemInfo[slotPos]

					self.cambItemInfo[selectedSlotPos] = attachedSlotPos
					self.slot_pos_index = attachedSlotPos
					self.combSlot.ClearSlot(selectedSlotPos)
					self.combSlot.SetItemSlot(selectedSlotPos, player.GetItemIndex(attachedSlotPos), player.GetItemCount(attachedSlotPos))
					net.SendChatPacket("/item_comb add_item %d %d" % (selectedSlotPos, attachedSlotPos))
					if selectedSlotPos == 2:
						self.combSlot.SetItemSlot(3, player.GetItemIndex(self.slot_index1), player.GetItemCount(self.slot_index1))

				elif selectedSlotPos == 3:
					return
				else:
					snd.PlaySound("sound/ui/make_soket.wav")
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_CANT_APPEND_ITEM)
					mouseModule.mouseController.DeattachObject()
					return

			self.Refresh()

	def __OnSelectItemSlot(self, selectedSlotPos):
		if selectedSlotPos == 3:
			return

		snd.PlaySound("sound/ui/loginfail.wav")

		if selectedSlotPos == 0:
			self.slot_pos_index = 999
			self.slot_index1 = 999
			self.slot_index2 = 999
			self.mediumSlot.ClearSlot(selectedSlotPos)
			self.combSlot.ClearSlot(1)
			self.combSlot.ClearSlot(2)
			self.combSlot.ClearSlot(3)
			net.SendChatPacket("/item_comb del_item %d " % selectedSlotPos)
			net.SendChatPacket("/item_comb del_item %d " % 1)
			net.SendChatPacket("/item_comb del_item %d " % 2)
			self.ItemCombBackgroundImage.Hide()
			self.combSlot.Hide()
			self.acceptButton.Hide()
			self.cancelButton.Hide()

		elif selectedSlotPos == 1:
			self.slot_pos_index = 999
			self.slot_index1 = 999
			self.slot_index2 = 999
			self.combSlot.ClearSlot(selectedSlotPos)
			self.combSlot.ClearSlot(2)
			self.combSlot.ClearSlot(3)
			net.SendChatPacket("/item_comb del_item %d " % selectedSlotPos)
			net.SendChatPacket("/item_comb del_item %d " % 2)

		elif selectedSlotPos == 2:
			self.slot_index2 = 999
			self.combSlot.ClearSlot(selectedSlotPos)
			self.combSlot.ClearSlot(3)
			net.SendChatPacket("/item_comb del_item %d " % selectedSlotPos)

		if not selectedSlotPos in self.cambItemInfo:
			return

		del self.cambItemInfo[selectedSlotPos]

		self.Refresh()
		
	def __OnOverInItem(self, slotIndex):
		if slotIndex == 3:
			if self.slot_index1 != 999:
				##
				self.tooltipItem.ClearToolTip()
				itemVnum = player.GetItemIndex(self.slot_index1)
				##
				metinSlot = []
				for i in xrange(player.METIN_SOCKET_MAX_NUM):
					metinSlot.append(player.GetItemMetinSocket(self.slot_index1, i))
				##
				attrSlot = []
				for w in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					attrSlot.append(player.GetItemAttribute(self.slot_index2, w))
				##
				self.tooltipItem.AddItemData(itemVnum, metinSlot, attrSlot)
			return

		if self.tooltipItem:
			if self.cambItemInfo.has_key(slotIndex):
				self.tooltipItem.SetInventoryItem(self.cambItemInfo[slotIndex])

	def __OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
	def BindInterface(self, interface):
		self.interface = interface
			
	def CantCheckInItem(self, slotPos):
		itemIndex = player.GetItemIndex(slotPos)
		
		if itemIndex != 0:
			itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
			itemTypeListBySlot = player.GetItemTypeBySlot(player.INVENTORY, slotPos)
			itemSubTypeListBySlot = player.GetItemSubTypeBySlot(player.INVENTORY, slotPos)
			if itemTypeListBySlot != item.ITEM_TYPE_COSTUME and itemSubTypeListBySlot != item.COSTUME_TYPE_SASH and itemIndex != 70065:
				return True
			if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
				return True
				
		return False

	def OnUpdate(self):
		USE_SHOP_LIMIT_RANGE = 1000
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xShopStart) > USE_SHOP_LIMIT_RANGE or abs(y - self.yShopStart) > USE_SHOP_LIMIT_RANGE:
			self.__OnCloseButtonClick()

class SkillBookCombinationWindow(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.xShopStart = 0
		self.yShopStart = 0
		self.slot0 = 999
		self.slot1 = 999
		self.slot2 = 999
		self.slot3 = 999
		self.slot4 = 999
		self.slot5 = 999
		self.slot6 = 999
		self.slot7 = 999
		self.slot8 = 999
		self.slot9 = 999
		self.isUsable = True
		self.isLoaded = 0
		self.interface = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/SkillBookCombinationDialog.py")

		except:
			exception.Abort("SkillBookCombinationWindow.LoadDialog.LoadScript")

		try:
			self.skillbookTitleBar = self.GetChild("SkillBook_TitleBar")
			self.needMoney = self.GetChild("NeedMoney")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
			self.skillbookSlot = self.GetChild("SkillBookSlot")
		except:
			exception.Abort("SkillBookCombinationWindow.LoadDialog.BindObject")

		self.skillbookTitleBar.SetCloseEvent(ui.__mem_func__(self.__OnCloseButtonClick))
		self.needMoney.SetText(localeInfo.COMB_COST % localeInfo.NumberToMoneyString(0))
		self.acceptButton.SetEvent(ui.__mem_func__(self.__OnAcceptButtonClick))
		self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCloseButtonClick))

		self.skillbookSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.skillbookSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.skillbookSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
		self.skillbookSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))

		self.cambItemInfo = {}
		self.tooltipItem = None

	def IsOpened(self):
		if self.isLoaded == 1:
			return True

		return False

	def SetItemToolTip(self, itemTooltip):
		self.tooltipItem = itemTooltip

	def Destroy(self):
		self.ClearDictionary()
		self.skillbookTitleBar = None
		self.needMoney = None
		self.acceptButton = None
		self.cancelButton = None
		self.skillbookSlot = None
		self.tooltipItem = None
		self.interface = None

	def Open(self):
		self.slot0 = 999
		self.slot1 = 999
		self.slot2 = 999
		self.slot3 = 999
		self.slot4 = 999
		self.slot5 = 999
		self.slot6 = 999
		self.slot7 = 999
		self.slot8 = 999
		self.slot9 = 999
		self.cambItemInfo = {}
		self.Refresh()
		self.Show()
		self.isUsable = True
		(self.xShopStart, self.yShopStart, z) = player.GetMainCharacterPosition()

		for i in xrange(5*2):
			self.skillbookSlot.ClearSlot(i)
			self.skillbookSlot.RefreshSlot()

		self.needMoney.SetText(localeInfo.COMB_COST % localeInfo.NumberToMoneyString(0))

	def OnPressEscapeKey(self):
		self.__OnCloseButtonClick()
		return True

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.Hide()
		self.cambItemInfo = {}

	def Clear(self):
		self.slot0 = 999
		self.slot1 = 999
		self.slot2 = 999
		self.slot3 = 999
		self.slot4 = 999
		self.slot5 = 999
		self.slot6 = 999
		self.slot7 = 999
		self.slot8 = 999
		self.slot9 = 999
		for i in xrange(5*2):
			self.skillbookSlot.ClearSlot(i)
			self.skillbookSlot.RefreshSlot()
			net.SendChatPacket("/item_comb del_item %d " % i)

		self.cambItemInfo = {}
		self.needMoney.SetText(localeInfo.COMB_COST % localeInfo.NumberToMoneyString(0))

	def Refresh(self):
		pass

	def __OnCloseButtonClick(self):
		if self.isUsable:
			self.isUsable = False

			print "item_comb close!"
			net.SendChatPacket("/item_comb close")

		self.Close()
		
	def __OnAcceptButtonClick(self):
		self.ConfirmCombination = uiCommon.QuestionDialog2()
		self.ConfirmCombination.SetText1(localeInfo.COMB_WILL_REMOVE_BOOK)
		self.ConfirmCombination.SetText2(localeInfo.COMB_IS_CONTINUE_PROCESS)
		self.ConfirmCombination.SetAcceptEvent(self.__YesAcceptButtonClick)
		self.ConfirmCombination.SetCancelEvent(self.__NoAcceptButtonClick)
		self.ConfirmCombination.Open()

	def __NoAcceptButtonClick(self):
		self.ConfirmCombination.Close()

	def __YesAcceptButtonClick(self):
		if len(self.cambItemInfo) >= 10:
			print "item_comb make_skillbook!"
			net.SendChatPacket("/item_comb make_skillbook")
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOT_FULL_BOOK)

		self.ConfirmCombination.Close()

	def __OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			if (player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_SKILL_BOOK_INVENTORY != attachedSlotType):
				return
				
			if app.ENABLE_ITEM_SEALBIND_SYSTEM:
				if player.GetItemSealDate(attachedSlotPos):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ADD_BIND_ITEM)
					return

			item.SelectItem(player.GetItemIndex(player.SKILL_BOOK_INVENTORY, attachedSlotPos))
			itemVnum = player.GetItemIndex(player.SKILL_BOOK_INVENTORY, attachedSlotPos)
			itemCount = player.GetItemCount(player.SKILL_BOOK_INVENTORY, attachedSlotPos)

			if itemCount != 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ITEM_COUNT_ERROR)
				return

			if itemVnum == 50300:
				if self.slot0 == attachedSlotPos and selectedSlotPos == 0 or self.slot1 == attachedSlotPos and selectedSlotPos == 1 or self.slot2 == attachedSlotPos and selectedSlotPos == 2 or self.slot3 == attachedSlotPos and selectedSlotPos == 3 or self.slot4 == attachedSlotPos and selectedSlotPos == 4 or self.slot5 == attachedSlotPos and selectedSlotPos == 5 or self.slot6 == attachedSlotPos and selectedSlotPos == 6 or self.slot7 == attachedSlotPos and selectedSlotPos == 7 or self.slot8 == attachedSlotPos and selectedSlotPos == 8 or self.slot9 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot0 == attachedSlotPos and selectedSlotPos == 1 or self.slot0 == attachedSlotPos and selectedSlotPos == 2 or self.slot0 == attachedSlotPos and selectedSlotPos == 3 or self.slot0 == attachedSlotPos and selectedSlotPos == 4 or self.slot0 == attachedSlotPos and selectedSlotPos == 5 or self.slot0 == attachedSlotPos and selectedSlotPos == 6 or self.slot0 == attachedSlotPos and selectedSlotPos == 7 or self.slot0 == attachedSlotPos and selectedSlotPos == 8 or self.slot0 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot1 == attachedSlotPos and selectedSlotPos == 0 or self.slot1 == attachedSlotPos and selectedSlotPos == 2 or self.slot1 == attachedSlotPos and selectedSlotPos == 3 or self.slot1 == attachedSlotPos and selectedSlotPos == 4 or self.slot1 == attachedSlotPos and selectedSlotPos == 5 or self.slot1 == attachedSlotPos and selectedSlotPos == 6 or self.slot1 == attachedSlotPos and selectedSlotPos == 7 or self.slot1 == attachedSlotPos and selectedSlotPos == 8 or self.slot1 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot2 == attachedSlotPos and selectedSlotPos == 0 or self.slot2 == attachedSlotPos and selectedSlotPos == 1 or self.slot2 == attachedSlotPos and selectedSlotPos == 3 or self.slot2 == attachedSlotPos and selectedSlotPos == 4 or self.slot2 == attachedSlotPos and selectedSlotPos == 5 or self.slot2 == attachedSlotPos and selectedSlotPos == 6 or self.slot2 == attachedSlotPos and selectedSlotPos == 7 or self.slot2 == attachedSlotPos and selectedSlotPos == 8 or self.slot2 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot3 == attachedSlotPos and selectedSlotPos == 0 or self.slot3 == attachedSlotPos and selectedSlotPos == 1 or self.slot3 == attachedSlotPos and selectedSlotPos == 2 or self.slot3 == attachedSlotPos and selectedSlotPos == 4 or self.slot3 == attachedSlotPos and selectedSlotPos == 5 or self.slot3 == attachedSlotPos and selectedSlotPos == 6 or self.slot3 == attachedSlotPos and selectedSlotPos == 7 or self.slot3 == attachedSlotPos and selectedSlotPos == 8 or self.slot3 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot4 == attachedSlotPos and selectedSlotPos == 0 or self.slot4 == attachedSlotPos and selectedSlotPos == 1 or self.slot4 == attachedSlotPos and selectedSlotPos == 2 or self.slot4 == attachedSlotPos and selectedSlotPos == 3 or self.slot4 == attachedSlotPos and selectedSlotPos == 5 or self.slot4 == attachedSlotPos and selectedSlotPos == 6 or self.slot4 == attachedSlotPos and selectedSlotPos == 7 or self.slot4 == attachedSlotPos and selectedSlotPos == 8 or self.slot4 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot5 == attachedSlotPos and selectedSlotPos == 0 or self.slot5 == attachedSlotPos and selectedSlotPos == 1 or self.slot5 == attachedSlotPos and selectedSlotPos == 2 or self.slot5 == attachedSlotPos and selectedSlotPos == 3 or self.slot5 == attachedSlotPos and selectedSlotPos == 4 or self.slot5 == attachedSlotPos and selectedSlotPos == 6 or self.slot5 == attachedSlotPos and selectedSlotPos == 7 or self.slot5 == attachedSlotPos and selectedSlotPos == 8 or self.slot5 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot6 == attachedSlotPos and selectedSlotPos == 0 or self.slot6 == attachedSlotPos and selectedSlotPos == 1 or self.slot6 == attachedSlotPos and selectedSlotPos == 2 or self.slot6 == attachedSlotPos and selectedSlotPos == 3 or self.slot6 == attachedSlotPos and selectedSlotPos == 4 or self.slot6 == attachedSlotPos and selectedSlotPos == 5 or self.slot6 == attachedSlotPos and selectedSlotPos == 7 or self.slot6 == attachedSlotPos and selectedSlotPos == 8 or self.slot6 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot7 == attachedSlotPos and selectedSlotPos == 0 or self.slot7 == attachedSlotPos and selectedSlotPos == 1 or self.slot7 == attachedSlotPos and selectedSlotPos == 2 or self.slot7 == attachedSlotPos and selectedSlotPos == 3 or self.slot7 == attachedSlotPos and selectedSlotPos == 4 or self.slot7 == attachedSlotPos and selectedSlotPos == 5 or self.slot7 == attachedSlotPos and selectedSlotPos == 6 or self.slot7 == attachedSlotPos and selectedSlotPos == 8 or self.slot7 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot8 == attachedSlotPos and selectedSlotPos == 0 or self.slot8 == attachedSlotPos and selectedSlotPos == 1 or self.slot8 == attachedSlotPos and selectedSlotPos == 2 or self.slot8 == attachedSlotPos and selectedSlotPos == 3 or self.slot8 == attachedSlotPos and selectedSlotPos == 4 or self.slot8 == attachedSlotPos and selectedSlotPos == 5 or self.slot8 == attachedSlotPos and selectedSlotPos == 6 or self.slot8 == attachedSlotPos and selectedSlotPos == 7 or self.slot8 == attachedSlotPos and selectedSlotPos == 9:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return
				elif self.slot9 == attachedSlotPos and selectedSlotPos == 0 or self.slot9 == attachedSlotPos and selectedSlotPos == 1 or self.slot9 == attachedSlotPos and selectedSlotPos == 2 or self.slot9 == attachedSlotPos and selectedSlotPos == 3 or self.slot9 == attachedSlotPos and selectedSlotPos == 4 or self.slot9 == attachedSlotPos and selectedSlotPos == 5 or self.slot9 == attachedSlotPos and selectedSlotPos == 6 or self.slot9 == attachedSlotPos and selectedSlotPos == 7 or self.slot9 == attachedSlotPos and selectedSlotPos == 8:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
					return

				if selectedSlotPos == 0:
					self.slot0 = attachedSlotPos
				elif selectedSlotPos == 1:
					self.slot1 = attachedSlotPos
				elif selectedSlotPos == 2:
					self.slot2 = attachedSlotPos
				elif selectedSlotPos == 3:
					self.slot3 = attachedSlotPos
				elif selectedSlotPos == 4:
					self.slot4 = attachedSlotPos
				elif selectedSlotPos == 5:
					self.slot5 = attachedSlotPos
				elif selectedSlotPos == 6:
					self.slot6 = attachedSlotPos
				elif selectedSlotPos == 7:
					self.slot7 = attachedSlotPos
				elif selectedSlotPos == 8:
					self.slot8 = attachedSlotPos
				elif selectedSlotPos == 9:
					self.slot9 = attachedSlotPos

				for slotPos, invenPos in self.cambItemInfo.items():
					if invenPos == attachedSlotPos:
						del self.cambItemInfo[slotPos]
						
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)

				if itemCount == 1:
					self.skillbookSlot.ClearSlot(selectedSlotPos)
					self.skillbookSlot.SetItemSlot(selectedSlotPos, itemVnum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (selectedSlotPos, attachedSlotPos, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(selectedSlotPos)
					self.skillbookSlot.SetItemSlot(selectedSlotPos, itemVnum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (selectedSlotPos, attachedSlotPos, player.SKILL_BOOK_INVENTORY))

				self.cambItemInfo[selectedSlotPos] = attachedSlotPos
			else:
				snd.PlaySound("sound/ui/make_soket.wav")
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_CANT_APPEND_ITEM)
				mouseModule.mouseController.DeattachObject()
				return

			if len(self.cambItemInfo) > 0 and len(self.cambItemInfo) < 2:
				NEED_MONEY = 1000000
				if self.needMoney:
					self.needMoney.SetText(localeInfo.COMB_COST % localeInfo.NumberToMoneyString(NEED_MONEY))

			self.Refresh()

	def __OnSelectItemSlot(self, selectedSlotPos):
		snd.PlaySound("sound/ui/loginfail.wav")
		net.SendChatPacket("/item_comb del_item %d " % selectedSlotPos)
		self.skillbookSlot.ClearSlot(selectedSlotPos)
		self.skillbookSlot.RefreshSlot()

		if selectedSlotPos == 0:
			self.slot0 = 999
		elif selectedSlotPos == 1:
			self.slot1 = 999
		elif selectedSlotPos == 2:
			self.slot2 = 999
		elif selectedSlotPos == 3:
			self.slot3 = 999
		elif selectedSlotPos == 4:
			self.slot4 = 999
		elif selectedSlotPos == 5:
			self.slot5 = 999
		elif selectedSlotPos == 6:
			self.slot6 = 999
		elif selectedSlotPos == 7:
			self.slot7 = 999
		elif selectedSlotPos == 8:
			self.slot8 = 999
		elif selectedSlotPos == 9:
			self.slot9 = 999

		if not selectedSlotPos in self.cambItemInfo:
			return

		del self.cambItemInfo[selectedSlotPos]

		if len(self.cambItemInfo) <= 0:
			self.needMoney.SetText(localeInfo.COMB_COST % localeInfo.NumberToMoneyString(0))

		self.Refresh()
		
	def AddItemWithoutMouse(self, slotIndex):
		itemVNum = player.GetItemIndex(player.SKILL_BOOK_INVENTORY, slotIndex)
		itemCount = player.GetItemCount(player.SKILL_BOOK_INVENTORY, slotIndex)
		item.SelectItem(itemVNum)

		if itemCount != 1:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ITEM_COUNT_ERROR)
			return

		if itemVNum == 50300:
			if self.slot0 == slotIndex or self.slot1 == slotIndex or self.slot2 == slotIndex or self.slot3 == slotIndex or self.slot4 == slotIndex or self.slot5 == slotIndex or self.slot6 == slotIndex or self.slot7 == slotIndex or self.slot8 == slotIndex or self.slot9 == slotIndex:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_ALREADY_APPEND)
				return				
		if itemVNum == 50300:
			if self.slot0 == 999:
				self.slot0 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(0)
					self.skillbookSlot.SetItemSlot(0, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (0, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(0)
					self.skillbookSlot.SetItemSlot(0, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (0, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[0] = slotIndex					
			elif self.slot1 == 999:
				self.slot1 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(1)
					self.skillbookSlot.SetItemSlot(1, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (1, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(1)
					self.skillbookSlot.SetItemSlot(1, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (1, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[1] = slotIndex
			elif self.slot2 == 999:
				self.slot2 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(2)
					self.skillbookSlot.SetItemSlot(2, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (2, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(2)
					self.skillbookSlot.SetItemSlot(2, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (2, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[2] = slotIndex
			elif self.slot3 == 999:
				self.slot3 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(3)
					self.skillbookSlot.SetItemSlot(3, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (3, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(3)
					self.skillbookSlot.SetItemSlot(3, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (3, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[3] = slotIndex
			elif self.slot4 == 999:
				self.slot4 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(4)
					self.skillbookSlot.SetItemSlot(4, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (4, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(4)
					self.skillbookSlot.SetItemSlot(4, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (4, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[4] = slotIndex
			elif self.slot5 == 999:
				self.slot5 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(5)
					self.skillbookSlot.SetItemSlot(5, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (5, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(5)
					self.skillbookSlot.SetItemSlot(5, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (5, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[5] = slotIndex
			elif self.slot6 == 999:
				self.slot6 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(6)
					self.skillbookSlot.SetItemSlot(6, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (6, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(6)
					self.skillbookSlot.SetItemSlot(6, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (6, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[6] = slotIndex
			elif self.slot7 == 999:
				self.slot7 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(7)
					self.skillbookSlot.SetItemSlot(7, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (7, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(7)
					self.skillbookSlot.SetItemSlot(7, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (7, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[7] = slotIndex
			elif self.slot8 == 999:
				self.slot8 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(8)
					self.skillbookSlot.SetItemSlot(8, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (8, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(8)
					self.skillbookSlot.SetItemSlot(8, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (8, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[8] = slotIndex
			elif self.slot9 == 999:
				self.slot9 = slotIndex
				if itemCount == 1:
					self.skillbookSlot.ClearSlot(9)
					self.skillbookSlot.SetItemSlot(9, itemVNum, 0)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (9, slotIndex, player.SKILL_BOOK_INVENTORY))
				else:
					self.skillbookSlot.ClearSlot(9)
					self.skillbookSlot.SetItemSlot(9, itemVNum, itemCount)
					net.SendChatPacket("/item_comb add_item2 %d %d %d" % (9, slotIndex, player.SKILL_BOOK_INVENTORY))
				self.cambItemInfo[9] = slotIndex
				NEED_MONEY = 1000000
				if self.needMoney:
					self.needMoney.SetText(localeInfo.COMB_COST % localeInfo.NumberToMoneyString(NEED_MONEY))
			else:
				return

	def __OnOverInItem(self, slotIndex):
		if self.tooltipItem:
			if self.cambItemInfo.has_key(slotIndex):
				self.tooltipItem.SetInventoryItem(self.cambItemInfo[slotIndex], player.SKILL_BOOK_INVENTORY)

	def __OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
	def BindInterface(self, interface):
		self.interface = interface
			
	def CantCheckInItem(self, slotPos):
		itemIndex = player.GetItemIndex(slotPos)
		
		if itemIndex != 0:
			itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
			if itemIndex == 50300:
				return True
			if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
				return True
				
		return False

	def OnUpdate(self):
		USE_SHOP_LIMIT_RANGE = 1000
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xShopStart) > USE_SHOP_LIMIT_RANGE or abs(y - self.yShopStart) > USE_SHOP_LIMIT_RANGE:
			self.__OnCloseButtonClick()
