import net
import app
import player
import aura
import item
import ui
import grp
import snd
import uiToolTip
import localeInfo
import uiInventory
import mouseModule
import uiScriptLocale

class RefineWindow(ui.ScriptWindow):
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
		self.auraSlot = None
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		self.AuraToolTip1 = None
		self.AuraToolTip2 = None
		self.AuraToolTip3 = None
		self.AuraToolTip4 = None
		self.AuraToolTip5 = None
		self.AuraToolTip6 = None
		self.titleName = None
		if app.WJ_TRADABLE_ICON:
			self.interface = None
			self.wndInventory = None

	def LoadWindow(self):
		if self.isLoaded:
			return
		
		self.isLoaded = 1
		
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/aura_levelupwindow.py")
			
		except:
			import exception as exception
			exception.Abort("Aura_LevelUpWindow.LoadDialog.LoadScript")
		
		try:
			self.titleBar = self.GetChild("TitleBar")
			self.btnAccept = self.GetChild("AcceptButton")
			self.btnCancel = self.GetChild("CancelButton")
			self.auraSlot = self.GetChild("AuraSlot")
			self.AuraToolTip1 = self.GetChild("AuraToolTip1")
			self.AuraToolTip2 = self.GetChild("AuraToolTip2")
			self.AuraToolTip3 = self.GetChild("AuraToolTip3")
			self.AuraToolTip4 = self.GetChild("AuraToolTip4")
			self.AuraToolTip5 = self.GetChild("AuraToolTip5")
			self.AuraToolTip6 = self.GetChild("AuraToolTip6")
			self.TitleName = self.GetChild("TitleName")
		except:
			import exception as exception
			exception.Abort("Aura_LevelUpWindow.LoadDialog.BindObject")
		
		self.auraSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.auraSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.auraSlot.SetUseSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.auraSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.auraSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))
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
		for i in xrange(aura.WINDOW_MAX_MATERIALS + 1):
			self.auraSlot.ClearSlot(i)
		
		self.Show()
		if app.WJ_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_AURA_REFINE)
			self.interface.RefreshMarkInventoryBag()
		self.TitleName.SetText(localeInfo.AURA_TITLE_EVOLUTION)

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
		self.AuraToolTip1.SetText("")
		self.AuraToolTip2.SetText("")
		self.AuraToolTip3.SetText("")
		self.AuraToolTip4.SetText("")
		self.AuraToolTip5.SetText("")
		self.AuraToolTip6.SetText("")
		self.Hide()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def OnClose(self):
		aura.SendCloseRequest()

	def OnPressEscapeKey(self):
		self.OnClose()
		return True

	def OnAccept(self):
		aura.SendRefineRequest()

	def OnUpdate(self):
		LIMIT_RANGE = aura.LIMIT_RANGE
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.PositionStartX) >= LIMIT_RANGE or abs(y - self.PositionStartY) >= LIMIT_RANGE:
			if not self.PositionOut:
				self.PositionOut += 1
				self.OnClose()

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if not isAttached or selectedSlotPos == aura.WINDOW_MAX_MATERIALS:
			return

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
		mouseModule.mouseController.DeattachObject()
		if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedInvenType == player.INVENTORY:
			aura.Add(attachedInvenType, attachedSlotPos, selectedSlotPos)

	def OnSelectItemSlot(self, selectedSlotPos):
		if selectedSlotPos == aura.WINDOW_MAX_MATERIALS:
			return

		mouseModule.mouseController.DeattachObject()
		aura.Remove(selectedSlotPos)

	def OnOverInItem(self, selectedSlotPos):
		if self.tooltipItem:
			if selectedSlotPos == aura.WINDOW_MAX_MATERIALS:
				(isHere, iCell) = aura.GetAttachedItem(0)
				if isHere:
					self.tooltipItem.SetAuraResultItem(iCell)
			else:
				(isHere, iCell) = aura.GetAttachedItem(selectedSlotPos)
				if isHere:
					self.tooltipItem.SetInventoryItem(iCell)

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def Refresh(self, iAct):
		self.auraSlot.ClearSlot(aura.WINDOW_MAX_MATERIALS)
		for i in xrange(aura.WINDOW_MAX_MATERIALS):
			self.auraSlot.ClearSlot(i)
			(isHereTool, iCellTool) = aura.GetAttachedItem(0)
			(isHere, iCell) = aura.GetAttachedItem(i)
			if isHere:
				self.auraSlot.SetItemSlot(i, player.GetItemIndex(iCell), player.GetItemCount(iCell))
				metinSocket = [player.GetItemMetinSocket(iCellTool, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
				self.AuraToolTip1.SetText(localeInfo.AURA_TOOLTIP_NEXT)
				self.AuraToolTip2.SetText(localeInfo.AURA_LEVEL_STEP % (metinSocket[aura.LEVEL_SOCKET], metinSocket[aura.LEVEL_SOCKET]))
				self.AuraToolTip3.SetText(localeInfo.AURA_DRAIN_PER % (metinSocket[aura.ABSORPTION_SOCKET] / 10.0))
				self.AuraToolTip4.SetText(localeInfo.AURA_TOOLTIP_RESOURCE)
				item.SelectItem(49990)
				self.AuraToolTip5.SetText(localeInfo.AURA_TOOLTIP_RESOURCE_ITEM % (item.GetItemName(), 1))
				self.AuraToolTip6.SetText(localeInfo.AURA_TOOLTIP_NEED_YANG % (aura.GetPrice()))
				if i == int(aura.WINDOW_MAX_MATERIALS - 1):
					(itemVnum, MinAbs, MaxAbs) = aura.GetResultItem()
					if not itemVnum:
						break
					
					#self.auraSlot.SetItemSlot(i + 1, itemVnum, 0)
					break

	if app.WJ_TRADABLE_ICON:
		def CantCheckInItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
				itemTypeListBySlot = player.GetItemTypeBySlot(player.INVENTORY, slotPos)
				itemSubTypeListBySlot = player.GetItemSubTypeBySlot(player.INVENTORY, slotPos)
				itemIsAura = itemTypeListBySlot == item.ITEM_TYPE_COSTUME and itemSubTypeListBySlot == item.COSTUME_TYPE_AURA
				if itemTypeListBySlot != item.ITEM_TYPE_ARMOR and not itemIsAura:
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
		self.auraSlot = None
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
			pyScrLoader.LoadScriptFile(self, "UIScript/aura_absorbwindow.py")
			
		except:
			import exception as exception
			exception.Abort("Aura_AbsorbtionWindow.LoadDialog.LoadScript")
		
		try:
			self.titleBar = self.GetChild("TitleBar")
			self.btnAccept = self.GetChild("AcceptButton")
			self.btnCancel = self.GetChild("CancelButton")
			self.auraSlot = self.GetChild("AuraSlot")
		except:
			import exception as exception
			exception.Abort("Aura_AbsorbtionWindow.LoadDialog.BindObject")
		
		self.auraSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.auraSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.auraSlot.SetUseSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.auraSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.auraSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))
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
		for i in xrange(aura.WINDOW_MAX_MATERIALS + 1):
			self.auraSlot.ClearSlot(i)
		
		self.Show()
		if app.WJ_TRADABLE_ICON:
			self.interface.SetOnTopWindow(player.ON_TOP_WND_AURA)
			self.interface.RefreshMarkInventoryBag()

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
		self.Hide()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def OnClose(self):
		aura.SendCloseRequest()

	def OnPressEscapeKey(self):
		self.OnClose()
		return True

	def OnAccept(self):
		aura.SendRefineRequest()

	def OnUpdate(self):
		LIMIT_RANGE = aura.LIMIT_RANGE
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.PositionStartX) >= LIMIT_RANGE or abs(y - self.PositionStartY) >= LIMIT_RANGE:
			if not self.PositionOut:
				self.PositionOut += 1
				self.OnClose()

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if not isAttached or selectedSlotPos == aura.WINDOW_MAX_MATERIALS:
			return
		
		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
		attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
		if attachedItemIndex >= 21900 and attachedItemIndex <= 21999:
			return
		mouseModule.mouseController.DeattachObject()
		if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedInvenType == player.INVENTORY:
			aura.Add(attachedInvenType, attachedSlotPos, selectedSlotPos)

	def OnSelectItemSlot(self, selectedSlotPos):
		if selectedSlotPos == aura.WINDOW_MAX_MATERIALS:
			return
		
		mouseModule.mouseController.DeattachObject()
		aura.Remove(selectedSlotPos)

	def OnOverInItem(self, selectedSlotPos):
		if self.tooltipItem:
			if selectedSlotPos == aura.WINDOW_MAX_MATERIALS:
				(isHere1, iCell1) = aura.GetAttachedItem(0)
				(isHere2, iCell2) = aura.GetAttachedItem(1)
				if isHere1 and isHere2:
					self.tooltipItem.SetAuraResultAbsItem(iCell1, iCell2)
			else:
				(isHere, iCell) = aura.GetAttachedItem(selectedSlotPos)
				if isHere:
					self.tooltipItem.SetInventoryItem(iCell)

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def Refresh(self, iAct):
		self.auraSlot.ClearSlot(aura.WINDOW_MAX_MATERIALS)
		for i in xrange(aura.WINDOW_MAX_MATERIALS):
			self.auraSlot.ClearSlot(i)
			(isHere, iCell) = aura.GetAttachedItem(i)
			if isHere:
				self.auraSlot.SetItemSlot(i, player.GetItemIndex(iCell), 0)
				if i == int(aura.WINDOW_MAX_MATERIALS - 1):
					(itemVnum, MinAbs, MaxAbs) = aura.GetResultItem()
					if not itemVnum:
						break
					
					self.auraSlot.SetItemSlot(i + 1, itemVnum, 0)
					break
		import dbg
		dbg.TraceError("{0}".format(int(iAct)))

	if app.WJ_TRADABLE_ICON:
		def CantCheckInItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
				itemTypeListBySlot = player.GetItemTypeBySlot(player.INVENTORY, slotPos)
				itemSubTypeListBySlot = player.GetItemSubTypeBySlot(player.INVENTORY, slotPos)
				itemIsAura = itemTypeListBySlot == item.ITEM_TYPE_COSTUME and itemSubTypeListBySlot == item.COSTUME_TYPE_AURA
				if itemTypeListBySlot != item.ITEM_TYPE_ARMOR and not itemIsAura:
					return True
				if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
					return True
					
			return False
			
