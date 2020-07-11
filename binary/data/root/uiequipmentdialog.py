import chr
import player
import item
import ui

class CostumeEquipmentDialog(ui.ScriptWindow):
	def __init__(self, wndEquipment):
		if not wndEquipment:
			import exception
			exception.Abort("wndEquipment parameter must be set to CostumeEquipmentDialog")
			return

		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.wndEquipment = wndEquipment;

		self.wndCostumeEquipmentLayer = None
		self.wndCostumeEquipmentSlot = None
		self.expandBtn = None
		self.minBtn = None

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()
		ui.ScriptWindow.Show(self)

		self.CloseCostumeEquipment()

	def Close(self):
		self.Hide()

	def IsOpenedCostumeEquipment(self):
		return self.wndCostumeEquipmentLayer.IsShow()

	def OpenCostumeEquipment(self):
		self.wndCostumeEquipmentLayer.Show()
		self.expandBtn.Hide()

		self.AdjustPositionAndSize()
		self.RefreshSlot()

	def CloseCostumeEquipment(self):
		self.wndCostumeEquipmentLayer.Hide()
		self.expandBtn.Show()

		self.AdjustPositionAndSize()

	def GetBasePosition(self):
		x, y = self.wndEquipment.GetGlobalPosition()
		return x - 139, y + 30

	def AdjustPositionAndSize(self):
		bx, by = self.GetBasePosition()

		if self.IsOpenedCostumeEquipment():
			self.SetPosition(bx, by)
			self.SetSize(self.ORIGINAL_WIDTH, self.GetHeight())

		else:
			self.SetPosition(bx + 129, by);
			self.SetSize(10, self.GetHeight())

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/CostumeEquipmentDialog.py")
		except:
			import exception
			exception.Abort("CostumeEquipmentDialog.LoadWindow.LoadObject")

		try:
			self.ORIGINAL_WIDTH = self.GetWidth()
			wndCostumeEquipmentSlot = self.GetChild("CostumeEquipmentSlot")
			self.wndCostumeEquipmentLayer = self.GetChild("CostumeEquipmentLayer")
			self.expandBtn = self.GetChild("ExpandButton")
			self.minBtn = self.GetChild("MinimizeButton")

			self.expandBtn.SetEvent(ui.__mem_func__(self.OpenCostumeEquipment))
			self.minBtn.SetEvent(ui.__mem_func__(self.CloseCostumeEquipment))

		except:
			import exception
			exception.Abort("CostumeEquipmentDialog.LoadWindow.BindObject")

		wndCostumeEquipmentSlot.SetOverInItemEvent(ui.__mem_func__(self.wndEquipment.OverInItem))
		wndCostumeEquipmentSlot.SetOverOutItemEvent(ui.__mem_func__(self.wndEquipment.OverOutItem))
		self.wndCostumeEquipmentSlot = wndCostumeEquipmentSlot

	def RefreshSlot(self):
		equipmentDict = self.wndEquipment.itemDataDict
		for i in [11, 12, 13, 14, 15, 16, 17]:
			if equipmentDict.has_key(i):
				self.wndCostumeEquipmentSlot.SetItemSlot(i, equipmentDict[i][0], equipmentDict[i][1])

class EquipmentDialog(ui.ScriptWindow):
	wndCostumeEquipment = None

	def __init__(self):
		#print "NEW EQUIPMENT DIALOG ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()

		self.vid = None
		self.eventClose = None
		self.itemDataDict = {}
		self.tooltipItem = None

	def __del__(self):
		#print "---------------------------------------------------------------------------- DELETE EQUIPMENT DIALOG "
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/EquipmentDialog.py")

			getObject = self.GetChild
			self.board = getObject("Board")
			self.slotWindow = getObject("EquipmentSlot")

			self.wndCostumeEquipment = CostumeEquipmentDialog(self)

		except:
			import exception
			exception.Abort("EquipmentDialog.LoadDialog.BindObject")

		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		self.slotWindow.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.slotWindow.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

	def Open(self, vid):
		self.vid = vid
		self.itemDataDict = {}

		name = chr.GetNameByVID(vid)
		self.board.SetTitleName(name)

		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		if self.wndCostumeEquipment:
			self.wndCostumeEquipment.Show()

	def Close(self):
		self.itemDataDict = {}
		self.tooltipItem = None
		self.Hide()

		if self.eventClose:
			self.eventClose(self.vid)

		if self.wndCostumeEquipment:
			self.wndCostumeEquipment.Close()

	def Destroy(self):
		self.eventClose = None

		self.Close()
		self.ClearDictionary()

		self.board = None
		self.slotWindow = None

		if self.wndCostumeEquipment:
			self.wndCostumeEquipment.Destroy()
			self.wndCostumeEquipment = None

	def SetEquipmentDialogItem(self, slotIndex, vnum, count, sealbind, transmutation):
		if count <= 1:
			count = 0

		self.slotWindow.SetItemSlot(slotIndex, vnum, count)
		
		if transmutation:
			self.slotWindow.DisableCoverButton(slotIndex)
		else:
			self.slotWindow.EnableCoverButton(slotIndex)

		emptySocketList = []
		emptyAttrList = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			emptySocketList.append(0)
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			emptyAttrList.append((0, 0))
		item.SelectItem(vnum)
		self.itemDataDict[slotIndex] = (vnum, count, sealbind, transmutation, emptySocketList, emptyAttrList)

	def SetEquipmentDialogSocket(self, slotIndex, socketIndex, value):
		if not slotIndex in self.itemDataDict:
			return
		if socketIndex < 0 or socketIndex > player.METIN_SOCKET_MAX_NUM:
			return
		self.itemDataDict[slotIndex][4][socketIndex] = value

	def SetEquipmentDialogAttr(self, slotIndex, attrIndex, type, value):
		if not slotIndex in self.itemDataDict:
			return
		if attrIndex < 0 or attrIndex > player.ATTRIBUTE_SLOT_MAX_NUM:
			return
		self.itemDataDict[slotIndex][5][attrIndex] = (type, value)
	
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def SetCloseEvent(self, event):
		self.eventClose = event

	def OverInItem(self, slotIndex):

		if None == self.tooltipItem:
			return

		if not slotIndex in self.itemDataDict:
			return

		itemVnum = self.itemDataDict[slotIndex][0]
		if 0 == itemVnum:
			return

		self.tooltipItem.ClearToolTip()
		metinSlot = self.itemDataDict[slotIndex][4]
		attrSlot = self.itemDataDict[slotIndex][5]
		sealbind = self.itemDataDict[slotIndex][2]
		transmutation = self.itemDataDict[slotIndex][3]
		self.tooltipItem.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, 0, 0, 0, transmutation)
		self.tooltipItem.__AppendSealInformation(player.INVENTORY, slotIndex)
		self.tooltipItem.ShowToolTip()

	def OverOutItem(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnMoveWindow(self, x, y):
		if self.wndCostumeEquipment:
			self.wndCostumeEquipment.AdjustPositionAndSize()
