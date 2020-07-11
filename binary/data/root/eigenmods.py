import os
import ui
import locale
import uiToolTip
import grp
from _weakref import proxy
import mouseModule
import constInfo
import app
import shop
import chat
import player
import net
import pack

itemCount = 9
BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
DARK_COLOR = grp.GenerateColor(0.3, 0.3, 0.3, 1.0)
DARK_COLOR2 = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
BRIGHT_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)

SELECT_COLOR = grp.GenerateColor(0.9, 0.03, 0.01, 0.4)

WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)

Tab = 0
Value = [[[0,0],[0,0]],[[0,0],[0,0]],[[0,0],[0,0]],[[0,0],[0,0]],[[0,0],[0,0]]]

class EigenartigSwitchBotDialog(ui.ScriptWindow):
	
	Cfg = [
	[0, [0,0], 0, [], [], 0, "", 0, [0,0], 0, [], [], 0],
	[0, [0,0], 0, [], [], 0, "", 0, [0,0], 0, [], [], 0],
	[0, [0,0], 0, [], [], 0, "", 0, [0,0], 0, [], [], 0],
	[0, [0,0], 0, [], [], 0, "", 0, [0,0], 0, [], [], 0],
	[0, [0,0], 0, [], [], 0, "", 0, [0,0], 0, [], [], 0]
	]

	Config = [0,0,0,0,0]
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Gui = []
		self.AddGui()
		
	def __del__(self):
		self.Gui[0].Hide()
		ui.ScriptWindow.__del__(self)
		if constInfo.eigenmod:
			constInfo.eigenmod = 0
		
	def AddGui(self):
		Gui = [
		[[ui.BoardWithTitleBar, ""], [508, 404], [0,0], [["SetCenterPosition", [""]], ["SetCloseEvent", [self.__del__]], ["SetTitleName", ["Metin2 Normal Efsun Botu "]]], ["float","movable"]],			
		[[ui.ImageBox, 0], [0, 0], [0, 355], [['LoadImage', ["efsun/tab_1.sub"]]], []],
		]
		
		for i in xrange(5):
			x =102*i
			button = [[ui.Button, 0], [98, 45], [x, 365], [['SetEvent', [lambda Index = i: self.SelectTab(Index)]]], []]
			Gui.append(button)		
		
		self.lenGui = len(Gui)
		
		for i in xrange(5):
			tab =  [[TabGui, 0], [470, 320], [30, 40], [["SetBtnEvents", [self.StartBOT]]], []]
			Gui.append(tab)

		GuiParser(Gui, self.Gui)
		
		self.Gui[-1].Hide()
		self.Gui[-2].Hide()
		
		self.SelectTab(0)
				
	def SelectTab(self, Index):
		global Tab
		
		self.Gui[1].LoadImage("efsun/tab_%s.sub" % (Index+1))
		
		for i in xrange(5):
			self.Gui[self.lenGui+i].Hide()
			
		self.Gui[self.lenGui+Index].Show()
		
		Tab = Index
		
		self.Gui[self.lenGui+Tab].SetbtnText(self.Cfg[Tab][0])
		self.Gui[self.lenGui+Tab].SetbtnText67(self.Cfg[Tab][7])
		
	def OnUpdate(self):
		for i in xrange(5):
			if self.Cfg[i][0]:
				self.UpdateBoni(i)
			if self.Cfg[i][7]:
				self.UpdateBoni67(i)

	def StartBOT(self, type):
		self.Config[Tab] = self.Gui[self.lenGui+Tab].GetItemInformation()
		
		if not type:	
			if not self.Cfg[Tab][0]:
				if self.Config[Tab][0] == -1:
					chat.AppendChat(chat.CHAT_TYPE_INFO, "Hicbir Item Secilmemis!")
					return			
			
				for Slot in xrange(player.INVENTORY_PAGE_SIZE*5):
					itemVNum = player.GetItemIndex(player.ATTR_ITEMS_INVENTORY, Slot)
					if itemVNum == 71084:
						self.Cfg[Tab][1] = [Slot, player.GetItemCount(player.ATTR_ITEMS_INVENTORY, Slot)]
						break
			
				self.DefineBoni()
				chat.AppendChat(1, "Efsunbot basladi.")
				self.Cfg[Tab][0] = 1			
				self.Gui[self.lenGui+Tab].SetbtnText(self.Cfg[Tab][0])
			else:
				self.Cfg[Tab][0] = 0
				chat.AppendChat(1, "Efsunbot durdu.")
				self.Gui[self.lenGui+Tab].SetbtnText(self.Cfg[Tab][0])
		else:
			if not self.Cfg[Tab][7]:
				if self.Config[Tab][0] == -1:
					chat.AppendChat(chat.CHAT_TYPE_INFO, "Hicbir Item Secilmemis!")
					return			
			
				for Slot in xrange(player.INVENTORY_PAGE_SIZE*4):
					itemVNum = player.GetItemIndex(player.ATTR_ITEMS_INVENTORY, Slot)
					if itemVNum == 71052:
						self.Cfg[Tab][8] = [Slot, player.GetItemCount(player.ATTR_ITEMS_INVENTORY, Slot)]
						break
			
				self.DefineBoni67()
				chat.AppendChat(1, "6/7 Efsunbot basladi.")
				self.Cfg[Tab][7] = 1			
				self.Gui[self.lenGui+Tab].SetbtnText67(self.Cfg[Tab][7])
			else:
				self.Cfg[Tab][7] = 0
				chat.AppendChat(1, "6/7 Efsunbot basladi.")
				self.Gui[self.lenGui+Tab].SetbtnText67(self.Cfg[Tab][7])		
			
	def DefineBoni(self):
		ItemAttr = [player.GetItemAttribute(self.Config[Tab][0], AttributeIndex) for AttributeIndex in xrange(5)]
		
		self.Cfg[Tab][3] = [bonus for bonus, value in ItemAttr if bonus]
		self.Cfg[Tab][4] = [value for bonus, value in ItemAttr if bonus]
		self.Cfg[Tab][5] = len([bonus for bonus, value in ItemAttr if bonus])
	
		self.ControllBoni(self.Cfg[Tab][3], self.Cfg[Tab][4], Tab)

	def DefineBoni67(self):
		ItemAttr = [player.GetItemAttribute(self.Config[Tab][0], AttributeIndex) for AttributeIndex in xrange(6,8)]
		
		self.Cfg[Tab][10] = [bonus for bonus, value in ItemAttr if bonus]
		self.Cfg[Tab][11] = [value for bonus, value in ItemAttr if bonus]
		self.Cfg[Tab][12] = len([bonus for bonus, value in ItemAttr if bonus])
	
		self.ControllBoni67(self.Cfg[Tab][10], self.Cfg[Tab][11], Tab)		

	def ControllBoni(self, Boni, Values, tabNumber):
		try:
			for i in xrange(len(Value[tabNumber][0][0])):
				Index = Boni.index(Value[tabNumber][0][0][i])
				if Values[Index] < Value[tabNumber][0][1][i]:
					Boni.index("-1")					
			self.Cfg[tabNumber][0] = 0
			self.Gui[self.lenGui+tabNumber].SetbtnText(self.Cfg[tabNumber][0])
			self.Gui[self.lenGui+Tab].RefreshToolTip()
			chat.AppendChat(1, "Efsun Sayisi: " + str(self.Cfg[tabNumber][5]))
			chat.AppendChat(1, "Efsunlar: " + str(Boni))
			chat.AppendChat(1, "Degerler: " + str(Values))
		except:
			if player.GetItemCountByVnumNew(71084) <= 0:
				if shop.IsOpen():
					for EachShopSlot in xrange(shop.SHOP_SLOT_COUNT):
						ShopItemValue = shop.GetItemID(EachShopSlot)
						if ShopItemValue == 71084:
							net.SendShopBuyPacket(EachShopSlot)
				else:
					chat.AppendChat(1, "Efsun nesnesi kalmadi")
					self.Cfg[tabNumber][0] = 0
					self.Gui[self.lenGui+tabNumber].SetbtnText(self.Cfg[tabNumber][0])
					return
				
			for Slot in xrange(player.INVENTORY_PAGE_SIZE*5):
				ItemValue = player.GetItemIndex(player.ATTR_ITEMS_INVENTORY, Slot)
				if ItemValue == 71084:
					if self.Cfg[tabNumber][0] == 0:
						return
					self.Cfg[tabNumber][1] = [Slot, player.GetItemCount(player.ATTR_ITEMS_INVENTORY, Slot)]
					net.SendItemUseToItemPacket(player.ATTR_ITEMS_INVENTORY, Slot, player.INVENTORY, self.Config[tabNumber][0])
					self.Gui[self.lenGui+Tab].RefreshToolTip()
					break

	def ControllBoni67(self, Boni, Values, tabNumber):
		try:
			for i in xrange(len(Value[tabNumber][1][0])):
				Index = Boni.index(Value[tabNumber][1][0][i])
				if Values[Index] < Value[tabNumber][1][1][i]:
					Boni.index("-1")					
			self.Cfg[tabNumber][7] = 0
			self.Gui[self.lenGui+tabNumber].SetbtnText67(self.Cfg[tabNumber][7])
			self.Gui[self.lenGui+Tab].RefreshToolTip()
			chat.AppendChat(1, "Efsun Sayisi: " + str(self.Cfg[tabNumber][12]))
			chat.AppendChat(1, "Efsunlar: " + str(Boni))
			chat.AppendChat(1, "Degerler: " + str(Values))
		except:
			if player.GetItemCountByVnum(71052) <= 0:
				if shop.IsOpen():
					for EachShopSlot in xrange(shop.SHOP_SLOT_COUNT):
						ShopItemValue = shop.GetItemID(EachShopSlot)
						if ShopItemValue == 71052:
							net.SendShopBuyPacket(EachShopSlot)
				else:
					chat.AppendChat(1, "2.Efsun nesnesi kalmadi")
					self.Cfg[tabNumber][7] = 0
					self.Gui[self.lenGui+tabNumber].SetbtnText67(self.Cfg[tabNumber][7])
					return
				
			for Slot in xrange(player.INVENTORY_PAGE_SIZE*4):
				ItemValue = player.GetItemIndex(Slot)
				if ItemValue == 71052:
					if self.Cfg[tabNumber][7] == 0:
						return
					self.Cfg[tabNumber][8] = [Slot, player.GetItemCount(player.ATTR_ITEMS_INVENTORY, Slot)]
					net.SendItemUseToItemPacket(player.ATTR_ITEMS_INVENTORY, Slot, player.INVENTORY, self.Config[tabNumber][0])
					self.Gui[self.lenGui+Tab].RefreshToolTip()
					break

	def UpdateBoni(self, Index):
		if self.Cfg[Index][0] == 0:
			return
		Values = []
		Boni = []
		for AttributeIndex in xrange(self.Cfg[Index][5]):
			Bonus, Valuex = player.GetItemAttribute(self.Config[Index][0], AttributeIndex)
			if Bonus == 0:
				return
			Values.append(Valuex)
			Boni.append(Bonus)
		
		if (player.GetItemCount(player.ATTR_ITEMS_INVENTORY, self.Cfg[Index][1][0]) <= 0 and (self.Cfg[Index][3] != Boni or self.Cfg[Index][4] != Values)) or (self.Cfg[Index][3] != Boni or self.Cfg[Index][4] != Values) or app.GetTime() >= self.Cfg[Index][2] + 0.8:
			self.Cfg[Index][2] = app.GetTime()
			self.Cfg[Index][3] = Boni
			self.Cfg[Index][4] = Values
			self.ControllBoni(Boni, Values, Index)
			
	def UpdateBoni67(self, Index):
		if self.Cfg[Index][7] == 0:
			return
		Values = []
		Boni = []
		for AttributeIndex in xrange(6, 6+self.Cfg[Index][12]):
			Bonus, Valuex = player.GetItemAttribute(self.Config[Index][0], AttributeIndex)
			if Bonus == 0:
				return
			Values.append(Valuex)
			Boni.append(Bonus)
		
		if (player.GetItemCount(self.Cfg[Index][8][0]) <= 0 and (self.Cfg[Index][10] != Boni or self.Cfg[Index][11] != Values)) or (self.Cfg[Index][10] != Boni or self.Cfg[Index][11] != Values) or app.GetTime() >= self.Cfg[Index][9] + 0.3:
			self.Cfg[Index][9] = app.GetTime()
			self.Cfg[Index][10] = Boni
			self.Cfg[Index][11] = Values
			self.ControllBoni67(Boni, Values, Index)

class TabGui(ui.ScriptWindow):
	
	SavedBonus=[]
	class BonusList(ui.ScriptWindow):

		class ModifiedListBox(ui.ListBox):
			def __init__(self):
				ui.ListBox.__init__(self)
				self.selectedLine = -1
			
			def GetSelectedItemText(self):
				return self.textDict.get(self.selectedLine, "")
		
			def GetSelectedLine(self):
				return self.selectedLine

			def ArrangeItem(self):
				self.SetSize(self.width, itemCount*self.stepSize)
				self._LocateItem()	
			
			def SelectItem2(self, line):
				self.selectedLine = line

			def OnRender(self):
				xRender, yRender = self.GetGlobalPosition()
				yRender -= self.TEMPORARY_PLACE
				widthRender = self.width
				heightRender = self.height + self.TEMPORARY_PLACE*2
		
				if -1 != self.overLine:
					if self.overLine - self.basePos < self.showLineCount:
						grp.SetColor(HALF_WHITE_COLOR)
						grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)				

				if -1 != self.selectedLine:
					if self.selectedLine >= self.basePos:
						if self.selectedLine - self.basePos < self.showLineCount:
							grp.SetColor(SELECT_COLOR)
							grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)			
	
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.x = 0
			self.y = 0
			self.width = 0
			self.height = 0
			self.isSelected = False
			self.isOver = False
			self.isListOpened = False
			self.event = lambda *arg: None
			self.openEvent = lambda *arg: None
			self.closeEvent = lambda *arg: None
			self.enable = True
		
			self.ToolTipText = None
		
			self.imgLeft = ui.ImageBox()
			self.imgLeft.AddFlag("not_pick")
			self.imgLeft.SetParent(self)
			self.imgLeft.LoadImage("d:/ymir work/ui/game/taskbar/quickslot_downbutton_01.sub")
			self.imgLeft.Show()			
		
			self.textLine = ui.TextLine()
			self.textLine.SetText("Nesne")
			self.textLine.SetParent(self)
			self.textLine.SetWindowHorizontalAlignCenter()
			self.textLine.SetHorizontalAlignCenter()
			self.textLine.SetPosition(0,2)
			self.textLine.SetOutline()
			self.textLine.Show()

			self.bar = ui.Bar3D()
			self.bar.SetParent(self)
			self.bar.Hide()
		
			self.listBox = self.ModifiedListBox()
			self.listBox.SetParent(self.bar)
			self.listBox.SetEvent(ui.__mem_func__(self.OnSelectItem))
			self.listBox.SetPickAlways()
			self.listBox.Hide()
		
			self.scrollBar = ui.SmallThinScrollBar()
			self.scrollBar.SetParent(self.bar)
			self.scrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))
			self.scrollBar.Hide()

		def __del__(self):
			ui.ScriptWindow.__del__(self)

		def Destroy(self):
			self.textLine = None
			self.listBox = None

		def SetPosition(self, x, y):
			ui.ScriptWindow.SetPosition(self, x, y)
			self.x = x
			self.y = y
			self.__ArrangeListBox()

		def SetSize(self, width, height):
			ui.ScriptWindow.SetSize(self, width, height)
			self.width = width
			self.height = height
			self.textLine.UpdateRect()
			self.imgLeft.SetPosition(width-13,7)
			self.__ArrangeListBox()
##		
		def SetFormToolTipText(self, type, text, x, y):
			if not self.ToolTipText:		
				toolTip = ui.createToolTipWindowDict[type]()
				toolTip.SetParent(self)
				toolTip.SetSize(0, 0)
				toolTip.SetHorizontalAlignCenter()
				toolTip.SetOutline()
				toolTip.Hide()
				toolTip.SetPosition(x + self.GetWidth()/2, y)
				self.ToolTipText=toolTip

			self.ToolTipText.SetText(text)

		def SetToolTipWindow(self, toolTip):		
			self.ToolTipText = toolTip		
			self.ToolTipText.SetParentProxy(self)

		def SetToolTipText(self, text, x=0, y = -19):
			self.SetFormToolTipText("TEXT", text, x, y)

		def ShowToolTip(self):
			if self.ToolTipText:
				self.ToolTipText.Show()

		def HideToolTip(self):
			if self.ToolTipText:
				self.ToolTipText.Hide()
		
		def __OnScroll(self):
			scrollLen = self.listBox.GetItemCount()-self.listBox.GetViewItemCount()
			if scrollLen < 0:
				scrollLen = 0
			self.listBox.SetBasePos(int(self.scrollBar.GetPos()*scrollLen))
	
		def SetViewBase(self, selectedLine):
			if not selectedLine:
				self.scrollBar.SetPos(0.0)
				return
			self.scrollBar.SetPos(selectedLine/float(self.lenList-2))
		
		def __ArrangeListBox(self):
			self.bar.SetPosition(0, self.height + 5)
			self.bar.SetSize(self.width, 4+itemCount*17)
		
			self.listBox.SetPosition(0, 1)
			self.listBox.SetWidth(self.width-13)
		
			self.scrollBar.SetPosition(self.width-14,3)
			self.scrollBar.SetScrollBarSize(itemCount*17)

		def Enable(self):
			self.enable = True

		def Disable(self):
			self.enable = False
			self.textLine.SetText("")
			self.CloseListBox()

		def SetEvent(self, event):
			self.event = event

		def ClearItem(self):
			self.CloseListBox()
			self.listBox.ClearItem()

		def InsertList(self, list):
			for name, index in list:
				self.listBox.InsertItem(index, name)
			self.listBox.ArrangeItem()
			self.lenList = len(list)

		def SetCurrentItem(self, text):
			self.textLine.SetText(text)

		def SelectItem(self, key):
			self.listBox.SelectItem(key)
		
		def SelectItem2(self, key):
			self.listBox.SelectItem2(key)

		def OnSelectItem(self, index, name):
			self.CloseListBox()
			#self.SetCurrentItem(name)
			self.event(index)

		def CloseListBox(self):
			self.isListOpened = False
			if self.closeEvent:
				self.closeEvent()
			ui.ScriptWindow.SetSize(self, self.width, self.height)
			self.bar.Hide()

		def OnMouseLeftButtonDown(self):
			if not self.enable:
				return

			self.isSelected = True
		
		def SelectTrue(self):
			self.isSelected = True
		
		def GetSelectedLine(self):
			return self.listBox.GetSelectedLine()
		
		def GetSelectedItem(self):
			return self.listBox.GetSelectedItem()
		
		def GetSelectedItemText(self):
			return self.listBox.GetSelectedItemText()

		def OnMouseLeftButtonUp(self):
			if not self.enable:
				return

			self.isSelected = False

			if self.isListOpened:
				self.CloseListBox()
			else:
				if self.listBox.GetItemCount() > 0:
					if self.openEvent:
						self.openEvent()
			
					self.isListOpened = True
			
					ui.ScriptWindow.SetSize(self, self.width, self.height+self.bar.GetHeight())
					self.bar.Show()			
			
					self.listBox.ArrangeItem()
					self.listBox.LocateItem()
					self.listBox.Show()
			
					self.scrollBar.Show()
			
		def OnUpdate(self):
			if not self.enable:
				return

			if self.IsIn():
				self.isOver = True
				self.ShowToolTip()
			else:
				self.isOver = False
				self.HideToolTip()

		def OnRender(self):
			self.x, self.y = self.GetGlobalPosition()
			xRender = self.x
			yRender = self.y
			widthRender = self.width
			heightRender = self.height
			grp.SetColor(BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
			grp.SetColor(DARK_COLOR)
			grp.RenderLine(xRender, yRender, widthRender, 0)
			grp.RenderLine(xRender, yRender, 0, heightRender)
			grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
			grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

			if self.isOver:
				grp.SetColor(HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

			if self.isSelected:
					grp.SetColor(WHITE_COLOR)
					grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

		def IsIn(self):
			return ui.Window.IsIn(self)

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		
		self.BonusIDListe = [["Seçilmedi", 0], ["Max HP.", 1], ["Max SP.", 2], ["Canlýlýk", 3], ["Zeka", 4], ["Güç", 5], ["Çeviklik", 6], ["Saldýrý Hýzý", 7], ["Hareket Hýzý", 8], ["Büyü Hýzý", 9], ["Hp Üretimi", 10], ["SP Üretimi", 11], ["Zehirleme Deðiþimi", 12], ["Kanama Saldýrýsý", 90], ["Sersemletme Þansý", 13], ["Yavaþlatma Þansý", 14], ["Kritik Vuruþ", 15], ["Delici Vuruþ", 16], ["Yarý Ýnsanlara Karþý Güçlü", 17], ["Hayvanlara Karþý Güçlü", 18], ["Orklara Karþý Güçlü", 19], ["Mistiklere Karþý Güçlü", 20], ["Ölümsüzlere Karþý Güçlü", 21], ["Þeytanlara Karþý Güçlü", 22], ["HP Absorbe", 23], ["SP Absorbe", 24], ["Beden K. Ataklarýn Blok.", 27], ["Oklardan Korunma Þansý", 28], ["Kýlýç Savunmasý", 29], ["Çift El Savunmasý", 30], ["Býçak Savunmasý", 31], ["Pençe Savunmasý", 94],["Ok K. Dayanýklýlýk", 34], ["Ateþe Karþý Dayanýklýlýk", 35], ["Þimþeðe Karþý Dayanýklýlýk", 36], ["Büyüye K. Dayanýklýlýk", 37], ["Rüzgara K. Dayanýklýlýk", 38], ["Vücut D. Yansýtmasý", 39], ["Zehre Karþý Koyma", 41], ["Kanama Saldýrýlarýnda Direnç", 91], ["Exp-Bonus", 43], ["Ýki Kat Yang", 44], ["Ýki Kat Ýtem", 45], ["Sersemlik K. Baðýþýklýlýk", 48], ["Yavaþlama K. Baðýþýklýlýk", 49], ["Saldýrý Deðeri", 53], ["Beceri Hasarý", 71], ["Ortalama Hasar", 72], ["Çan Savunmasý", 32], ["Yelpaze Savunmasý", 33]]
		self.BonusIDListe2 = [["Seçilmedi", 0], ["Max HP.", 1], ["Max SP.", 2], ["Canlýlýk", 3], ["Zeka", 4], ["Güç", 5], ["Çeviklik", 6], ["Kritik Vuruþ", 15], ["Delici Vuruþ", 16], ["Saldýrý Deðeri", 53], ["Savaþçýya K. Güçlü", 59], ["Ninjaya K. Güçlü", 60], ["Suraya K. Güçlü", 61], ["Þamana K. Güçlü", 62], ["Lycana K. Güçlü", 92], ["Beceri Hasarý", 71], ["Ortalama Hasar", 72]]
		
		self.SearchBoni = []
		self.SearchBoni2 = []
		self.SearchBoniValues = [0,0,0,0,0]
		self.SearchBoniValues2 = [0,0]
		self.Config=[-1, 0]
		
		self.eventFunc = None

		self.Gui=[]
			
		self.gridSlowWind = ui.GridSlotWindow()
		self.gridSlowWind.SetParent(self)
		self.gridSlowWind.SetPosition(0,0)
		self.gridSlowWind.ArrangeSlot(0, 1, 3, 32, 32, 0, 0)
		self.gridSlowWind.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
		self.gridSlowWind.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		self.gridSlowWind.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		self.gridSlowWind.Show()
		
		self.itemToolTip = ToolTipWithoutMetins()
		self.itemToolTip.SetPosition(40, 0)
		self.itemToolTip.SetParent(self)
		self.itemToolTip.SetFollow(False)
		self.itemToolTip.Show()

		self.delete_bonus = ui.Button()
		self.delete_bonus.SetParent(self)
		self.delete_bonus.SetUpVisual("d:/ymir work/ui/public/small_thin_button_01.sub")
		self.delete_bonus.SetOverVisual("d:/ymir work/ui/public/small_thin_button_02.sub")
		self.delete_bonus.SetDownVisual("d:/ymir work/ui/public/small_thin_button_03.sub")
		self.delete_bonus.SetText("Temizle")
		self.delete_bonus.SetPosition(330, 200)
		self.delete_bonus.SetEvent(lambda : self.DeleteAll())
		self.delete_bonus.Show()
		
		self.save_bonus = ui.Button()
		self.save_bonus.SetParent(self)
		self.save_bonus.SetUpVisual("d:/ymir work/ui/public/small_thin_button_01.sub")
		self.save_bonus.SetOverVisual("d:/ymir work/ui/public/small_thin_button_02.sub")
		self.save_bonus.SetDownVisual("d:/ymir work/ui/public/small_thin_button_03.sub")
		self.save_bonus.SetText("Kaydet")
		self.save_bonus.SetPosition(395, 200)
		self.save_bonus.SetEvent(lambda : self.SaveBonusSet())
		self.save_bonus.Show()

		self.texLine = ui.TextLine()
		self.texLine.SetParent(self)
		self.texLine.SetPosition(262,110+22)
		self.texLine.SetText("6/7. Efsun:")
		self.texLine.Hide()
		
		self.norm_bonus = ui.Button()
		self.norm_bonus.SetParent(self)
		self.norm_bonus.SetUpVisual("d:/ymir work/ui/public/xlarge_thin_button_01.sub")
		self.norm_bonus.SetOverVisual("d:/ymir work/ui/public/xlarge_thin_button_02.sub")
		self.norm_bonus.SetDownVisual("d:/ymir work/ui/public/xlarge_thin_button_03.sub")
		self.norm_bonus.SetText("Baþlat")
		self.norm_bonus.SetPosition(285, 250)
		self.norm_bonus.SetEvent(lambda : self.doThisEvent(0))
		self.norm_bonus.Show()
		
		self.six_seven_bonus = ui.Button()
		self.six_seven_bonus.SetParent(self)
		self.six_seven_bonus.SetUpVisual("d:/ymir work/ui/public/xlarge_thin_button_01.sub")
		self.six_seven_bonus.SetOverVisual("d:/ymir work/ui/public/xlarge_thin_button_02.sub")
		self.six_seven_bonus.SetDownVisual("d:/ymir work/ui/public/xlarge_thin_button_03.sub")
		self.six_seven_bonus.SetText("6/7 Baþlat")
		self.six_seven_bonus.SetPosition(375, 250)
		self.six_seven_bonus.SetEvent(lambda : self.doThisEvent(1))
		self.six_seven_bonus.Hide()
		
		self.load_bonus = SveBnsComboBox()
		self.load_bonus.SetParent(self)
		self.load_bonus.SetSize(60, 17)
		self.load_bonus.SetCurrentItem("Yükle")
		self.load_bonus.SetPosition(260, 201)
		self.load_bonus.SetEvent(self.LoadBonusSets)
		self.load_bonus.Show()
		
		for i in xrange(2):
				
			inverseLineIndex = 1 - i
			yPos = (inverseLineIndex+7)*22-3
			y_Pos = (i+7)*22-3
			
			selectBonus = self.BonusList()
			selectBonus.SetParent(self)
			selectBonus.SetCurrentItem("Seçilmedi")
			selectBonus.InsertList(self.BonusIDListe2)
			selectBonus.SetSize(140, 18)
			selectBonus.SetPosition(260, yPos)
			selectBonus.SetEvent(lambda selectedLine, lineIndex=inverseLineIndex, argSelf=proxy(self): argSelf.AddBonus2(lineIndex, selectedLine))
			selectBonus.Hide()
			
			slotBar = ui.SlotBar()
			slotBar.SetParent(self)
			slotBar.SetSize(42, 18)
			slotBar.SetPosition(410, y_Pos)
			slotBar.Hide()
		
			editLine = ui.EditLine()
			editLine.SetParent(slotBar)
			editLine.SetSize(35, 17)
			editLine.SetPosition(6, 2)
			editLine.SetMax(5)
			editLine.SetNumberMode()
			editLine.SetText("0")
			editLine.Hide()	
			
			self.Gui.append(selectBonus)
			self.Gui.append(slotBar)
			self.Gui.append(editLine)		
		
		for i in xrange(5):
				
			inverseLineIndex = 4 - i
			yPos = inverseLineIndex*22
			y_Pos = i*22
			
			selectBonus = self.BonusList()
			selectBonus.SetParent(self)
			selectBonus.SetCurrentItem("Seçilmedi")
			selectBonus.InsertList(self.BonusIDListe)
			selectBonus.SetSize(140, 18)
			selectBonus.SetPosition(260, yPos)
			selectBonus.SetEvent(lambda selectedLine, lineIndex=inverseLineIndex, argSelf=proxy(self): argSelf.AddBonus(lineIndex, selectedLine))
			selectBonus.Show()
			
			slotBar = ui.SlotBar()
			slotBar.SetParent(self)
			slotBar.SetSize(42, 18)
			slotBar.SetPosition(410, y_Pos)
			slotBar.Show()
		
			editLine = ui.EditLine()
			editLine.SetParent(slotBar)
			editLine.SetSize(35, 17)
			editLine.SetPosition(6, 2)
			editLine.SetMax(5)
			editLine.SetNumberMode()
			editLine.SetText("0")
			editLine.Show()	
			
			self.Gui.append(selectBonus)
			self.Gui.append(slotBar)
			self.Gui.append(editLine)
			
		self.LoadBnsCfgFile()
		
	def SetBtnEvents(self, func):
		self.eventFunc = func
		
	def SetbtnText(self, arg):
		if not arg:
			self.norm_bonus.SetText("Baþlat")
		else:
			self.norm_bonus.SetText("Durdur")
	
	def SetbtnText67(self, arg):
		if not arg:
			self.six_seven_bonus.SetText("6/7 Baþlat")
		else:
			self.six_seven_bonus.SetText("6/7 Durdur")
		
	def doThisEvent(self, arg):
		self.eventFunc(arg)		
	
	def LoadBnsCfgFile(self):
		self.SavedBonus = []
		try:
			for information in open("efsnCfg.eig", "r").readlines():
				bonuses, values, bonuses2, values2 = information.split(":")[0], information.split(":")[1], information.split(":")[2], information.split(":")[3].split("\n")[0]
				vtint, btint, vtint2, btint2 = values.split(","), bonuses.split(","), values2.split(","), bonuses2.split(",")
			
				actList = [[[],[]], [[],[]]]

				try:
					actList[0][0] = [int(bonus) for bonus in btint]
				except:
					actList[0][0] = 0
				
				try:
					actList[0][1] = [int(bonus) for bonus in vtint]
				except:
					actList[0][1] = 0
				
				try:
					actList[1][0] = [int(bonus) for bonus in btint2]
				except:
					actList[1][0] = 0
				
				try:
					actList[1][1] = [int(bonus) for bonus in vtint2]			
				except:
					actList[1][1] = 0
				
				self.SavedBonus.append(actList)
		except:
			pass
		self.load_bonus.ClearItem()
		
		for i in xrange(len(self.SavedBonus)):
			self.load_bonus.InsertItem(i, "Kayýt %s" % (i+1))
			
		self.load_bonus.InsertItem(31, "Hepsini Sil")
		
	def LoadBonusSets(self, index):
		if index ==31:
			try: os.remove("efsnCfg.eig")
			except: pass
			chat.AppendChat(1,"Tüm setler silindi...")
			self.LoadBnsCfgFile()
			self.load_bonus.SelectItem2(-1)
			return
		self.DeleteAll()
		set = self.SavedBonus[index]
		
		self.SearchBoni = set[0][0]
		self.SearchBoni2 = set[1][0]
		
		self.UpdateBonusList()
		self.UpdateBonusList2()
		
		try:
			for i in set[0][1]:
				Index = set[0][1].index(i)
				self.Gui[(Index*3)+8].SetText(str(i))
		except:
			pass		
		
		try:
			for i in set[1][1]:
				Index = set[1][1].index(i)
				self.Gui[(Index*3)+2].SetText(str(i))
		except:
			chat.AppendChat(1,"silinemedi...")
		self.load_bonus.SelectItem2(-1)
		
	def DeleteAll(self):
		self.SearchBoni = []
		self.SearchBoni2 = []
		for Index in xrange(5):
			self.Gui[(Index*3)+8].SetText("0")
		for Index in xrange(2):
			self.Gui[(Index*3)+2].SetText("0")
		self.UpdateBonusList()
		self.UpdateBonusList2()

	def SaveBonusSet(self):
		if len(self.SavedBonus) > 15:
			chat.AppendChat(1, "Kaydedilemedi. Daha fazla kayýt yapabilmek için diðerlerini silin")
			return
		if not Value[Tab][0][0]:
			chat.AppendChat(1, "Hiçbir deðer seçilmemiþ")
			return
		
		dosya = open("efsnCfg.eig", "a+")
		
		try:
			boni_1 = ','.join([str(a) for a in Value[Tab][0][0]])
		except:
			boni_1 = ''
		try:
			boni_2 = ','.join([str(a) for a in Value[Tab][1][0]])
		except:
			boni_2 = ''
			
		value_1 = ','.join([str(a) for a in Value[Tab][0][1]])
		value_2 = ','.join([str(a) for a in Value[Tab][1][1]])
		
		dosya.write("%s:%s:%s:%s\n" % (boni_1, value_1, boni_2, value_2))
		
		dosya.close()
		chat.AppendChat(1, "Kayýt Baþarýlý")
		
		self.LoadBnsCfgFile()		
		
	def SelectEmptySlot(self, asfgsdfg):
		self.Config[1], self.Config[0] = mouseModule.mouseController.GetAttachedItemIndex(), mouseModule.mouseController.GetAttachedSlotNumber()
		if mouseModule.mouseController.isAttached():
			self.gridSlowWind.SetItemSlot(0, self.Config[1])
			self.itemToolTip.SetInventoryItem(self.Config[0])
		mouseModule.mouseController.DeattachObject()
		self.gridSlowWind.RefreshSlot()
	
	def UseItemSlot(self, slotIndex):
		self.Config[0] = -1
		self.Config[1] = 0
		
		self.gridSlowWind.ClearSlot(0)
		self.gridSlowWind.RefreshSlot()
		
		self.itemToolTip.ClearToolTip()
		self.itemToolTip.SetSize(0,0)
		
	def HideBonusList(self):
		self.board.Hide()	

	def OnUpdate(self):
		global Value
		TmpValues = []
		for Index in xrange(5):
			try:
				SearchValue = int(self.Gui[(Index*3)+8].GetText())
			except:
				SearchValue = 0

			TmpValues.append(SearchValue)
			
		if TmpValues != self.SearchBoniValues:
			self.SearchBoniValues = TmpValues
		
		TmpValues2 = []
		for Index in xrange(2):
			try:
				SearchValue2 = int(self.Gui[(Index*3)+2].GetText())
			except:
				SearchValue2 = 0

			TmpValues2.append(SearchValue2)
			
		if TmpValues2 != self.SearchBoniValues2:
			self.SearchBoniValues2 = TmpValues2
			


		if Value[Tab][0][0] != self.SearchBoni:
			Value[Tab][0][0] = self.SearchBoni
		
		if Value[Tab][1][0] != self.SearchBoni2:
			Value[Tab][1][0] = self.SearchBoni2

		if Value[Tab][0][1] != self.SearchBoniValues:
			Value[Tab][0][1] = self.SearchBoniValues 
			
		if Value[Tab][1][1] != self.SearchBoniValues2:
			Value[Tab][1][1] = self.SearchBoniValues2 
			
	def AddBonus(self, Index, BonusValue):
		if BonusValue in self.SearchBoni:
			self.Gui[(Index+1)*-3].SetViewBase(0)
			self.Gui[(Index+1)*-3].SelectItem2(0)
			self.Gui[(Index+1)*-3].SetCurrentItem("Seçilmedi")
			chat.AppendChat(1, "Bu efsun zaten seçili!")
			self.UpdateBonusList()
			return
			
		if not BonusValue:
			try:
				del self.SearchBoni[Index]
			except:
				pass
		else:
			try:
				self.SearchBoni[Index] = BonusValue
			except:
				self.SearchBoni.append(BonusValue)
		
		self.UpdateBonusList()

	def AddBonus2(self, Index, BonusValue):
		if BonusValue in self.SearchBoni2:
			self.Gui[(1-Index)*3].SetViewBase(0)
			self.Gui[(1-Index)*3].SelectItem2(0)
			self.Gui[(1-Index)*3].SetCurrentItem("Seçilmedi")
			chat.AppendChat(1, "Bu efsun zaten seçili!")
			self.UpdateBonusList2()
			return
			
		if not BonusValue:
			try:
				del self.SearchBoni2[Index]
			except:
				pass
		else:
			try:
				self.SearchBoni2[Index] = BonusValue
			except:
				self.SearchBoni2.append(BonusValue)
		
		self.UpdateBonusList2()		

	def UpdateBonusList(self):
		tmp = {}
		for Bonus in self.BonusIDListe:
			tmp[Bonus[1]] = Bonus[0]
			
		for i in xrange(1,7):
			self.Gui[i*-3].SetViewBase(0)
			self.Gui[i*-3].SelectItem2(0)
			self.Gui[i*-3].SetCurrentItem("Seçilmedi")	
		try:
			for i in self.SearchBoni:
				Index = self.SearchBoni.index(i)
				bnsIndex = self.BonusIDListe.index([tmp[i], i])
				self.Gui[(Index+1)*-3].SetViewBase(bnsIndex)
				self.Gui[(Index+1)*-3].SelectItem2(bnsIndex)
				self.Gui[(Index+1)*-3].SetCurrentItem(tmp[i])
		except:
			pass
			
	def UpdateBonusList2(self):
		tmp = {}
		for Bonus in self.BonusIDListe2:
			tmp[Bonus[1]] = Bonus[0]
			
		for i in xrange(2):
			self.Gui[i*3].SetViewBase(0)
			self.Gui[i*3].SelectItem2(0)
			self.Gui[i*3].SetCurrentItem("Seçilmedi")
			
		try:
			for i in self.SearchBoni2:
				Index = self.SearchBoni2.index(i)
				self.Gui[(1-Index)*3].SetViewBase(i)
				self.Gui[(1-Index)*3].SelectItem2(i)
				self.Gui[(1-Index)*3].SetCurrentItem(tmp[i])
		except:
			pass
	
	def RefreshToolTip(self):
		self.itemToolTip.SetInventoryItem(self.Config[0])
		
	def GetItemInformation(self):
		return self.Config
	
	def Show(self):
		self.LoadBnsCfgFile()
		
		ui.ScriptWindow.Show(self)
		
def GuiParser(guiobjects, list):
	for object in guiobjects:
		Object = object[0][0]()
		if object[0][1] != "":
			Object.SetParent(list[object[0][1]])
		if object[1][0] + object[1][1] != 0:
			Object.SetSize(object[1][0], object[1][1])
		if object[2][0] + object[2][1] != 0:
			Object.SetPosition(object[2][0], object[2][1])
				
		for command in object[3]:
			cmd = command[0]	
			attr = getattr(Object,cmd)			
			if callable(attr):
				argument = command[1]
				lenght = len(argument)
				if lenght == 1:
					if argument[0] == "":
						attr()
					else:
						attr(argument[0])
				elif lenght == 2:
					attr(argument[0], argument[1])
				elif lenght == 3:
					attr(argument[0], argument[1], argument[2])
				elif lenght == 4:
					attr(argument[0], argument[1], argument[2], argument[3])
		for flag in object[4]:
			Object.AddFlag(str(flag))
		Object.Show()
	
		list.append(Object)
		
def IsDivideAble(x, y):
	if x == 0:
		return
	if float(x/y) == DivideToFloat(x, y):
		return 1
	
def DivideToFloat(x, y):
	try:
		return x * (y**-1)
	except:
		return 0
		
class ToolTipWithoutMetins(uiToolTip.ItemToolTip):
	def __init__(self, *args, **kwargs):
		uiToolTip.ToolTip.__init__(self, *args, **kwargs)
		
		self.bCannotUseItemForceSetDisableColor = True 
		
	def SetInventoryItem(self, slotIndex):
		itemVnum = player.GetItemIndex(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		attrSlot = [player.GetItemAttribute(slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

		self.ResizeToolTip()
		self.AddItemData(itemVnum, [0,0,0,0,0,0], attrSlot)	

	def ResizeToolTip(self):
		self.toolTipWidth = 210
		self.SetSize(self.toolTipWidth, self.TOOL_TIP_HEIGHT + self.toolTipHeight)			

class SveBnsComboBox(ui.ComboBox):
	
	class ListBoxWithBoard(ui.ListBox):
		def __init__(self, layer):
			ui.ListBox.__init__(self, layer)

		def OnRender(self):
			xRender, yRender = self.GetGlobalPosition()
			yRender -= self.TEMPORARY_PLACE
			widthRender = self.width
			heightRender = self.height + self.TEMPORARY_PLACE*2
			grp.SetColor(BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender+3, widthRender, heightRender-3)
			grp.SetColor(DARK_COLOR)
			grp.RenderLine(xRender, yRender+2, widthRender, 0)
			grp.RenderLine(xRender, yRender+2, 0, heightRender-3)
			grp.RenderLine(xRender, yRender+heightRender-1, widthRender, 0)
			grp.RenderLine(xRender+widthRender, yRender+2, 0, heightRender-3)

			if -1 != self.overLine:
				grp.SetColor(HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)				

			if -1 != self.selectedLine:
				if self.selectedLine >= self.basePos:
					if self.selectedLine - self.basePos < self.showLineCount:
						grp.SetColor(SELECT_COLOR)
						grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)
						
		def SelectItem2(self, line):
			self.selectedLine = line
							
	def __init__(self):
		ui.ComboBox.__init__(self)
		
		self.imgLeft = ui.ImageBox()
		self.imgLeft.AddFlag("not_pick")
		self.imgLeft.SetParent(self)
		self.imgLeft.LoadImage("d:/ymir work/ui/game/taskbar/quickslot_downbutton_01.sub")
		self.imgLeft.Show()	
		
	def SetSize(self, width, height):
		self.imgLeft.SetPosition(width-13,7)
		ui.ComboBox.SetSize(self, width, height)
	
	def SelectItem2(self, key):
		self.listBox.SelectItem2(key)
		
	def OnRender(self):
		self.x, self.y = self.GetGlobalPosition()
		xRender = self.x
		yRender = self.y
		widthRender = self.width
		heightRender = self.height
		grp.SetColor(BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

			if self.isSelected:
				grp.SetColor(WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

	def IsIn(self):
		return ui.Window.IsIn(self)	

EigenartigSwitchBotDialog().Show()
