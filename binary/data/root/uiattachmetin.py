import app
import net
import player
import item
import dbg
import snd
import ui
import uiToolTip
import localeInfo
import mouseModule

class AttachMetinDialog(ui.ScriptWindow):
	if app.WJ_TRADABLE_ICON:  
		def __init__(self, wndInventory):
			ui.ScriptWindow.__init__(self)
			self.__LoadScript()
			self.baseinv = 0
			self.stoneinv = 0
			self.metinItemPos = 0
			self.targetItemPos = 0
			self.wndInventory = wndInventory
			self.lockedItems = {i:(-1,-1) for i in range(2)}
	else:
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.__LoadScript()
			self.baseinv = 0
			self.stoneinv = 0
			self.metinItemPos = 0
			self.targetItemPos = 0

	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/attachstonedialog.py")

		except:
			import exception
			exception.Abort("AttachStoneDialog.__LoadScript.LoadObject")

		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")
			self.metinImage = self.GetChild("MetinImage")
			self.GetChild("AcceptButton").SetEvent(ui.__mem_func__(self.Accept))
			self.GetChild("CancelButton").SetEvent(ui.__mem_func__(self.Close))
		except:
			import exception
			exception.Abort("AttachStoneDialog.__LoadScript.BindObject")

		oldToolTip = uiToolTip.ItemToolTip()
		oldToolTip.SetParent(self)
		oldToolTip.SetPosition(15, 38)
		oldToolTip.SetFollow(False)
		oldToolTip.Show()
		self.oldToolTip = oldToolTip

		newToolTip = uiToolTip.ItemToolTip()
		newToolTip.SetParent(self)
		newToolTip.SetPosition(230 + 20, 38)
		newToolTip.SetFollow(False)
		newToolTip.Show()
		self.newToolTip = newToolTip

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.board = 0
		self.titleBar = 0
		self.metinImage = 0
		self.toolTip = 0
		if app.WJ_TRADABLE_ICON:  
			self.wndInventory = 0
		self.baseinv = 0
		self.stoneinv = 0

	def CanAttachMetin(self, slot, metin):
		if item.METIN_NORMAL == metin:
			if player.METIN_SOCKET_TYPE_SILVER == slot or player.METIN_SOCKET_TYPE_GOLD == slot:
				return True

		elif item.METIN_GOLD == metin:
			if player.METIN_SOCKET_TYPE_GOLD == slot:
				return True

	def Open(self, metinItemPos, targetItemPos):
		self.metinItemPos = metinItemPos
		self.targetItemPos = targetItemPos

		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		if player.INVENTORY == attachedSlotType:
			metinIndex = player.GetItemIndex(metinItemPos)
			self.baseinv = 1
		else:
			metinIndex = player.GetItemIndex(player.STONE_ITEMS_INVENTORY, metinItemPos)
			self.stoneinv = 1
		itemIndex = player.GetItemIndex(targetItemPos)
		self.oldToolTip.ClearToolTip()
		self.newToolTip.ClearToolTip()

		item.SelectItem(metinIndex)

		## Metin Image
		try:
			self.metinImage.LoadImage(item.GetIconImageFileName())
		except:
			dbg.TraceError("AttachMetinDialog.Open.LoadImage - Failed to find item data")

		## Old Item ToolTip
		metinSlot = []
		if app.ENABLE_WEAPON_RARITY_SYSTEM:
			attrSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))
		if app.ENABLE_WEAPON_RARITY_SYSTEM:
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				if i == 7:
					attrSlot.append(player.GetItemAttribute(targetItemPos, i))
				else:
					attrSlot.append([0,0])
			self.oldToolTip.AddItemData(itemIndex, metinSlot, attrSlot)
		else:
			self.oldToolTip.AddItemData(itemIndex, metinSlot)

		## New Item ToolTip
		item.SelectItem(metinIndex)
		metinSubType = item.GetItemSubType()

		metinSlot = []
		if app.ENABLE_WEAPON_RARITY_SYSTEM:
			attrSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			slotData = metinSlot[i]
			if self.CanAttachMetin(slotData, metinSubType):
				metinSlot[i] = metinIndex
				break
		if app.ENABLE_WEAPON_RARITY_SYSTEM:
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				if i == 7:
					attrSlot.append(player.GetItemAttribute(targetItemPos, i))
				else:
					attrSlot.append([0,0])
			self.newToolTip.AddItemData(itemIndex, metinSlot, attrSlot)
		else:
			self.newToolTip.AddItemData(itemIndex, metinSlot)

		self.UpdateDialog()
		self.SetTop()
		if app.WJ_TRADABLE_ICON:  
			self.SetCantMouseEventSlot(0, self.metinItemPos)
			self.SetCantMouseEventSlot(1, self.targetItemPos)
		self.Show()

	def UpdateDialog(self):
		newWidth = self.newToolTip.GetWidth() + 230 + 15 + 20
		newHeight = self.newToolTip.GetHeight() + 98


		self.board.SetSize(newWidth, newHeight)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def Accept(self):
		if self.stoneinv == 1:
			net.SendItemUseToItemPacket(player.STONE_ITEMS_INVENTORY, self.metinItemPos, player.INVENTORY, self.targetItemPos)
		elif self.baseinv == 1:
			net.SendItemUseToItemPacket(self.metinItemPos, self.targetItemPos)
		snd.PlaySound("sound/ui/metinstone_insert.wav")
		self.Close()

	def Close(self):
		if app.WJ_TRADABLE_ICON:  
			self.SetCanMouseEventSlot(0, self.metinItemPos)
			self.SetCanMouseEventSlot(1, self.targetItemPos)
		self.Hide()
		self.stoneinv = 0
		self.baseinv = 0

	def OnUpdate(self):
		if app.WJ_TRADABLE_ICON:
			self.RefreshLockedSlot()
		
	if app.WJ_TRADABLE_ICON:  
		def SetCanMouseEventSlot(self, what, slotIndex):
			itemInvenPage = slotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = slotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItems[what] = (-1, -1)

			if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.wndInventory.wndItem.SetCanMouseEventSlot(localSlotPos)

		def SetCantMouseEventSlot(self, what, slotIndex):
			itemInvenPage = slotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = slotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItems[what] = (itemInvenPage, localSlotPos)

			if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.wndInventory.wndItem.SetCantMouseEventSlot(localSlotPos)

		def RefreshLockedSlot(self):
			if self.wndInventory:
				for what, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
					if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
						self.wndInventory.wndItem.SetCantMouseEventSlot(itemSlotPos)

				self.wndInventory.wndItem.RefreshSlot()
