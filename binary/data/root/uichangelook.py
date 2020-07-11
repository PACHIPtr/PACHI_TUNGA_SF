import player
import changelook
import item
import chat
import app
import ui
import uiCommon
import uiToolTip
import localeInfo
import mouseModule
import grp

class Window(ui.ScriptWindow):
	def NumberToMoneyString(self, number):
		number = str(number)
		strNumber = "%s" % (','.join([number[::-1][k : k + 3][::-1] for k in xrange(len(number)+1, -1, -3)]))
		strNumber = strNumber[1:]
		return "%s" % (strNumber)

	def __init__(self, wndInventory, interface):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		self.dialog = None
		self.wndInventory = wndInventory
		self.interface = interface

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.titleBar = None
		self.titleName = None
		self.accept = None
		self.cancel = None
		self.slot = None
		self.cost = None
		self.PositionOut = 0
		self.PositionStartX = 0
		self.PositionStartY = 0
		self.dialog = None

	def LoadWindow(self):
		if self.isLoaded:
			return
		
		self.isLoaded = 1
		
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/changelookwindow.py")
			
		except:
			import exception
			exception.Abort("ChangeLookWindow.LoadDialog.LoadScript")
		
		try:
			self.titleBar = self.GetChild("TitleBar")
			self.titleName = self.GetChild("TitleName")
			self.accept = self.GetChild("AcceptButton")
			self.cancel = self.GetChild("CancelButton")
			self.cost = self.GetChild("Cost")
			self.slot = self.GetChild("ChangeLookSlot")
			self.ChangeLookToolTIpButton = self.GetChild("ChangeLookToolTIpButton")
		except:
			import exception
			exception.Abort("ChangeLookWindow.LoadDialog.BindObject")
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.OnClose))
		self.titleName.SetText(localeInfo.CHANGE_LOOK_TITLE)
		self.cancel.SetEvent(ui.__mem_func__(self.OnClose))
		self.accept.SetEvent(ui.__mem_func__(self.OnPressAccept))
		self.cost.SetText(localeInfo.CHANGE_LOOK_COST % (self.NumberToMoneyString(changelook.GetCost())))
		self.slot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.slot.SetUnselectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.slot.SetUseSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.slot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.slot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))
		self.tooltipItem = None
		self.tl = uiToolTip.ItemToolTip()
		self.tlInfo = uiToolTip.ItemToolTip()
		self.tl.Hide()
		self.tlInfo.Hide()
		self.tooltipItem = self.tl
		self.tooltipInfo = [self.tlInfo]*7
		self.InformationText = [localeInfo.CHANGE_TOOLTIP_TITLE,
								localeInfo.CHANGE_TOOLTIP_LINE1,
								localeInfo.CHANGE_TOOLTIP_LINE2,
								localeInfo.CHANGE_TOOLTIP_LINE3,
								localeInfo.CHANGE_TOOLTIP_LINE4,
								localeInfo.CHANGE_TOOLTIP_LINE5,
								localeInfo.CHANGE_TOOLTIP_LINE6
		]

		for i in xrange(7):
			self.tooltipInfo[i].SetFollow(True)
			self.tooltipInfo[i].AlignHorizonalCenter()
			if i == 0:
				TITLE_COLOR = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)
				self.tooltipInfo[i].AutoAppendTextLine(self.InformationText[i], TITLE_COLOR)
			else:
				self.tooltipInfo[i].AutoAppendTextLine(self.InformationText[i])
			self.tooltipInfo[i].Hide()
			self.tooltipInfo[i].toolTipWidth += 55
		self.is_open_dialog = 0

	def SetItemToolTip(self, itemTooltip):
		self.tooltipItem = itemTooltip

	def IsOpened(self):
		if self.IsShow() and self.isLoaded:
			return True
		
		return False
	
	if app.ENABLE_MOUNT_CHANGELOOK_SYSTEM:
		def Open(self, bMount):
			self.PositionOut = 0
			(self.PositionStartX, self.PositionStartY, z) = player.GetMainCharacterPosition()
			self.cost.SetText(localeInfo.CHANGE_LOOK_COST % (localeInfo.NumberToString(0)))
			for i in xrange(changelook.WINDOW_MAX_MATERIALS):
				self.slot.ClearSlot(i)
			
			if bMount:
				self.titleName.SetText(localeInfo.CHANGE_LOOK_TITLE2)
			else:
				self.titleName.SetText(localeInfo.CHANGE_LOOK_TITLE)
			self.Show()
			self.interface.SetOnTopWindow(player.ON_TOP_WND_CHANGE_LOOK)
			self.interface.RefreshMarkInventoryBag()
	else:
		def Open(self):
			self.PositionOut = 0
			(self.PositionStartX, self.PositionStartY, z) = player.GetMainCharacterPosition()
			self.cost.SetText(localeInfo.CHANGE_LOOK_COST % (localeInfo.NumberToString(0)))
			for i in xrange(changelook.WINDOW_MAX_MATERIALS):
				self.slot.ClearSlot(i)
			
			self.Show()
			self.interface.SetOnTopWindow(player.ON_TOP_WND_CHANGE_LOOK)
			self.interface.RefreshMarkInventoryBag()

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
		for i in xrange(7):
			if self.tooltipInfo != None:
				self.tooltipInfo[i].Hide()	
		self.tlInfo.Hide()
		self.OnCancelAccept()
		self.Hide()
		self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
		self.interface.RefreshMarkInventoryBag()

	def OnClose(self):
		changelook.SendCloseRequest()

	def OnPressEscapeKey(self):
		self.OnClose()
		return True

	def OnPressAccept(self):
		(isHere, iCell) = changelook.GetAttachedItem(1)
		if not isHere:
			return
		
		dialog = uiCommon.QuestionDialog()
		dialog.SetText(localeInfo.CHANGE_LOOK_CHANGE_ITEM)
		dialog.SetAcceptEvent(ui.__mem_func__(self.OnAccept))
		dialog.SetCancelEvent(ui.__mem_func__(self.OnCancelAccept))
		dialog.Open()
		self.dialog = dialog
		self.is_open_dialog = 1

	def OnAccept(self):
		changelook.SendRefineRequest()
		self.OnCancelAccept()

	def OnCancelAccept(self):
		if self.dialog:
			self.dialog.Close()
		
		self.dialog = None
		self.is_open_dialog = 0
		return True

	def OnUpdate(self):
		LIMIT_RANGE = changelook.LIMIT_RANGE
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.PositionStartX) >= LIMIT_RANGE or abs(y - self.PositionStartY) >= LIMIT_RANGE:
			if not self.PositionOut:
				self.PositionOut += 1
				self.OnClose()
		for i in xrange(7):
			if self.ChangeLookToolTIpButton.IsIn():
				self.tooltipInfo[i].Show()
			else:
				self.tooltipInfo[i].Hide()	
		for i in xrange(changelook.WINDOW_MAX_MATERIALS):
			(isHere, iCell) = changelook.GetAttachedItem(i)
			if isHere:
				if self.cost:
					if player.GetItemIndex(iCell) == changelook.SCROLL_VNUM:
						self.cost.SetText(localeInfo.CHANGE_LOOK_COST % (localeInfo.NumberToString(0)))
					else:
						self.cost.SetText(localeInfo.CHANGE_LOOK_COST % (localeInfo.NumberToString(changelook.GetCost())))

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if not isAttached:
			return
		
		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
		mouseModule.mouseController.DeattachObject()
		
		if player.IsEquipmentSlot(attachedSlotPos):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHANGE_LOOK_CANNOT_CHECKIN_EQUIPPED_ITEM)
			return
			
		if player.GetItemSealDate(attachedInvenType, attachedSlotPos):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHANGE_LOOK_CANNOT_CHECKIN_SEALED_ITEM)
			return
			
		if player.IsBasicItem(attachedInvenType, attachedSlotPos):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHANGE_LOOK_CANNOT_CHECKIN_BASIC_ITEM)
			return
		
		if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedInvenType == player.INVENTORY:
			changelook.Add(attachedInvenType, attachedSlotPos, selectedSlotPos)

	def OnSelectItemSlot(self, selectedSlotPos):
		mouseModule.mouseController.DeattachObject()
		changelook.Remove(selectedSlotPos)

	def OnOverInItem(self, selectedSlotPos):
		if self.tooltipItem:
			(isHere, iCell) = changelook.GetAttachedItem(selectedSlotPos)
			if isHere:
				self.tooltipItem.SetInventoryItem(iCell)

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def Refresh(self):
		for i in xrange(changelook.WINDOW_MAX_MATERIALS):
			self.slot.ClearSlot(i)
			(isHere, iCell) = changelook.GetAttachedItem(i)
			if isHere:
				self.slot.SetItemSlot(i, player.GetItemIndex(iCell), 1)

	def CantCheckInItem(self, slotPos):
		itemIndex = player.GetItemIndex(slotPos)
		
		if itemIndex != 0:
			itemSealDate = player.GetItemSealDate(player.INVENTORY, slotPos)
			itemTypeListBySlot = player.GetItemTypeBySlot(player.INVENTORY, slotPos)
			itemSubTypeListBySlot = player.GetItemSubTypeBySlot(player.INVENTORY, slotPos)
			if itemTypeListBySlot != item.ITEM_TYPE_WEAPON and itemTypeListBySlot != item.ITEM_TYPE_ARMOR and itemTypeListBySlot != item.ITEM_TYPE_COSTUME and itemIndex != 72326:
				return True
			if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
				return True
				
		return False
		
