import net
import app
import player
import sash
import item
import ui
import grp
import snd
import uiToolTip
import localeInfo
import uiInventory
import mouseModule
import uiScriptLocale

class CombineWindow(ui.ScriptWindow):
	def __init__(self, wndInventory, interface):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		if app.WJ_TRADABLE_ICON:
			self.interface = interface
			self.wndInventory = wndInventory

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.titleBar = None
		self.btnAccept = None
		self.btnCancel = None
		self.sashSlot = None
		self.needMoney = None
		self.Result = None
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		if app.WJ_TRADABLE_ICON:
			self.interface = None
			self.wndInventory = None

	def LoadWindow(self):
		if self.isLoaded:
			return
		
		self.isLoaded = 1
		
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/sash_combinewindow.py")
			
		except:
			import exception as exception
			exception.Abort("Sash_CombineWindow.LoadDialog.LoadScript")
		
		try:
			self.titleBar = self.GetChild("TitleBar")
			self.btnAccept = self.GetChild("AcceptButton")
			self.btnCancel = self.GetChild("CancelButton")
			self.needMoney = self.GetChild("NeedMoney")
			self.Result = self.GetChild("Result")
			self.sashSlot = self.GetChild("SashSlot")
		except:
			import exception as exception
			exception.Abort("Sash_CombineWindow.LoadDialog.BindObject")
		
		self.sashSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.sashSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.sashSlot.SetUseSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.sashSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.sashSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.OnClose))
		self.btnCancel.SetEvent(ui.__mem_func__(self.OnClose))
		self.btnAccept.SetEvent(ui.__mem_func__(self.OnAccept))
		self.tooltipItem = None

	def SetItemToolTip(self, itemTooltip):
		self.tooltipItem = itemTooltip

	def IsOpened(self):
		if self.IsShow() and self.isLoaded:
			return True
		
		return False

	def Open(self):
		self.PositionOut = 0
		(self.PositionStartX, self.PositionStartY, z) = player.GetMainCharacterPosition()
		self.needMoney.SetText(localeInfo.SASH_REFINE_COST % (sash.GetPrice()))
		for i in xrange(sash.WINDOW_MAX_MATERIALS + 1):
			self.sashSlot.ClearSlot(i)
		
		self.Show()
		if app.WJ_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_ACCE_COMBINE)
			self.interface.RefreshMarkInventoryBag()

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.Hide()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def OnClose(self):
		sash.SendCloseRequest()

	def OnPressEscapeKey(self):
		self.OnClose()
		return True

	def OnAccept(self):
		sash.SendRefineRequest()

	def OnUpdate(self):
		LIMIT_RANGE = sash.LIMIT_RANGE
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.PositionStartX) >= LIMIT_RANGE or abs(y - self.PositionStartY) >= LIMIT_RANGE:
			if not self.PositionOut:
				self.PositionOut += 1
				self.OnClose()

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if not isAttached or selectedSlotPos == sash.WINDOW_MAX_MATERIALS:
			return

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
		mouseModule.mouseController.DeattachObject()
		
		if player.IsEquipmentSlot(attachedSlotPos):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SASH_CANNOT_CHECKIN_EQUIPPED_ITEM)
			return
			
		if player.GetItemSealDate(attachedInvenType, attachedSlotPos):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SASH_CANNOT_CHECKIN_SEALED_ITEM)
			return
			
		if player.IsBasicItem(attachedInvenType, attachedSlotPos):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SASH_CANNOT_CHECKIN_BASIC_ITEM)
			return

		if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedInvenType == player.INVENTORY:
			sash.Add(attachedInvenType, attachedSlotPos, selectedSlotPos)

	def OnSelectItemSlot(self, selectedSlotPos):
		if selectedSlotPos == sash.WINDOW_MAX_MATERIALS:
			return

		mouseModule.mouseController.DeattachObject()
		sash.Remove(selectedSlotPos)

	def OnOverInItem(self, selectedSlotPos):
		if self.tooltipItem:
			if selectedSlotPos == sash.WINDOW_MAX_MATERIALS:
				(isHere, iCell) = sash.GetAttachedItem(0)
				if isHere:
					self.tooltipItem.SetSashResultItem(iCell)
			else:
				(isHere, iCell) = sash.GetAttachedItem(selectedSlotPos)
				if isHere:
					self.tooltipItem.SetInventoryItem(iCell)

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def Refresh(self, iAct):
		self.needMoney.SetText(localeInfo.SASH_REFINE_COST % (sash.GetPrice()))
		self.sashSlot.ClearSlot(sash.WINDOW_MAX_MATERIALS)
		for i in xrange(sash.WINDOW_MAX_MATERIALS):
			self.sashSlot.ClearSlot(i)
			(isHere, iCell) = sash.GetAttachedItem(i)
			if isHere:
				self.sashSlot.SetItemSlot(i, player.GetItemIndex(iCell), 0)
				if i == int(sash.WINDOW_MAX_MATERIALS - 1):
					(itemVnum, MinAbs, MaxAbs) = sash.GetResultItem()
					if not itemVnum:
						break
					
					self.sashSlot.SetItemSlot(i + 1, itemVnum, 0)
					break

	if app.WJ_TRADABLE_ICON:
		def CantCheckInItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
				itemTypeListBySlot = player.GetItemTypeBySlot(player.INVENTORY, slotPos)
				itemSubTypeListBySlot = player.GetItemSubTypeBySlot(player.INVENTORY, slotPos)
				itemIsAcce = itemTypeListBySlot == item.ITEM_TYPE_COSTUME and itemSubTypeListBySlot == item.COSTUME_TYPE_SASH
				if itemTypeListBySlot != item.ITEM_TYPE_WEAPON and itemTypeListBySlot != item.ITEM_TYPE_ARMOR and not itemIsAcce:
					return True
				if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
					return True
					
			return False

class AbsorbWindow(ui.ScriptWindow):
	def __init__(self, wndInventory, interface):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		if app.WJ_TRADABLE_ICON:
			self.interface = interface
			self.wndInventory = wndInventory

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.titleBar = None
		self.btnAccept = None
		self.btnCancel = None
		self.sashSlot = None
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		if app.WJ_TRADABLE_ICON:
			self.interface = None
			self.wndInventory = None

	def LoadWindow(self):
		if self.isLoaded:
			return
		
		self.isLoaded = 1
		
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/sash_absorbwindow.py")
			
		except:
			import exception as exception
			exception.Abort("Sash_AbsorbtionWindow.LoadDialog.LoadScript")
		
		try:
			self.titleBar = self.GetChild("TitleBar")
			self.btnAccept = self.GetChild("AcceptButton")
			self.btnCancel = self.GetChild("CancelButton")
			self.sashSlot = self.GetChild("SashSlot")
		except:
			import exception as exception
			exception.Abort("Sash_AbsorbtionWindow.LoadDialog.BindObject")
		
		self.sashSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.sashSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.sashSlot.SetUseSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.sashSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.sashSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.OnClose))
		self.btnCancel.SetEvent(ui.__mem_func__(self.OnClose))
		self.btnAccept.SetEvent(ui.__mem_func__(self.OnAccept))
		
		self.tooltipItem = None

	def SetItemToolTip(self, itemTooltip):
		self.tooltipItem = itemTooltip

	def IsOpened(self):
		if self.IsShow() and self.isLoaded:
			return True
		
		return False

	def Open(self):
		self.PositionOut = 0
		(self.PositionStartX, self.PositionStartY, z) = player.GetMainCharacterPosition()
		for i in xrange(sash.WINDOW_MAX_MATERIALS + 1):
			self.sashSlot.ClearSlot(i)
		
		self.Show()
		if app.WJ_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_ACCE_ABSORB)
			self.interface.RefreshMarkInventoryBag()

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
		self.Hide()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def OnClose(self):
		sash.SendCloseRequest()

	def OnPressEscapeKey(self):
		self.OnClose()
		return True

	def OnAccept(self):
		sash.SendRefineRequest()

	def OnUpdate(self):
		LIMIT_RANGE = sash.LIMIT_RANGE
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.PositionStartX) >= LIMIT_RANGE or abs(y - self.PositionStartY) >= LIMIT_RANGE:
			if not self.PositionOut:
				self.PositionOut += 1
				self.OnClose()

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if not isAttached or selectedSlotPos == sash.WINDOW_MAX_MATERIALS:
			return
		
		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
		attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
		if attachedItemIndex >= 21900 and attachedItemIndex <= 21999:
			return
		mouseModule.mouseController.DeattachObject()
		if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedInvenType == player.INVENTORY:
			sash.Add(attachedInvenType, attachedSlotPos, selectedSlotPos)

	def OnSelectItemSlot(self, selectedSlotPos):
		if selectedSlotPos == sash.WINDOW_MAX_MATERIALS:
			return
		
		mouseModule.mouseController.DeattachObject()
		sash.Remove(selectedSlotPos)

	def OnOverInItem(self, selectedSlotPos):
		if self.tooltipItem:
			if selectedSlotPos == sash.WINDOW_MAX_MATERIALS:
				(isHere1, iCell1) = sash.GetAttachedItem(0)
				(isHere2, iCell2) = sash.GetAttachedItem(1)
				if isHere1 and isHere2:
					self.tooltipItem.SetSashResultAbsItem(iCell1, iCell2)
			else:
				(isHere, iCell) = sash.GetAttachedItem(selectedSlotPos)
				if isHere:
					self.tooltipItem.SetInventoryItem(iCell)

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def Refresh(self, iAct):
		self.sashSlot.ClearSlot(sash.WINDOW_MAX_MATERIALS)
		for i in xrange(sash.WINDOW_MAX_MATERIALS):
			self.sashSlot.ClearSlot(i)
			(isHere, iCell) = sash.GetAttachedItem(i)
			if isHere:
				self.sashSlot.SetItemSlot(i, player.GetItemIndex(iCell), 0)
				if i == int(sash.WINDOW_MAX_MATERIALS - 1):
					(itemVnum, MinAbs, MaxAbs) = sash.GetResultItem()
					if not itemVnum:
						break
					
					self.sashSlot.SetItemSlot(i + 1, itemVnum, 0)
					break

	if app.WJ_TRADABLE_ICON:
		def CantCheckInItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
				itemTypeListBySlot = player.GetItemTypeBySlot(player.INVENTORY, slotPos)
				itemSubTypeListBySlot = player.GetItemSubTypeBySlot(player.INVENTORY, slotPos)
				itemIsAcce = itemTypeListBySlot == item.ITEM_TYPE_COSTUME and itemSubTypeListBySlot == item.COSTUME_TYPE_SASH
				if itemTypeListBySlot != item.ITEM_TYPE_WEAPON and itemTypeListBySlot != item.ITEM_TYPE_ARMOR and not itemIsAcce:
					return True
				if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
					return True
					
			return False
			
