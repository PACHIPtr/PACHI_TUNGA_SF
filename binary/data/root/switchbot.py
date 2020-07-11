import net
import player
import item
import shop
import chat
import ui
import localeInfo
import grp
import wndMgr
import mouseModule

from uiToolTip import ItemToolTip

WEAR_NAMES = ItemToolTip.WEAR_NAMES
AFFECT_DICT = ItemToolTip.AFFECT_DICT

#Efsun Listesi
BONI_AVAIL = [1,2,3,4,5,6,9,10,12,13,14,15,16,17,18,19,20,21,22,23,27,28,29,30,31,32,33,34,37,39,41,43,44,45,48,53,71,72,138,139,140,141,142] 

BONI_RARE_AVAIL = [1,2] 

# Efsun Kodu
SWITCH_VNUM = 71151
SWITCH_RARE_VNUM = 71152

# Efsun Atma Süresi
MIN_SWITCH_DELAY = 15
MAX_SWITCH_DELAY_APPEND = 120

# Slot Sayýsý Max 7!
MAX_NUM = 7

proposals = {
	1: { # 1 = Weapon
		"":[
			[9,10],
			[15,10],
			[17,10],
			[16,10],
			[5,8],
			[3,8],
		],
		"":[
			[9,10],
			[15,10],
			[17,10],
			[6,8],
			[5,10],
			[16,10],
		],
		"": [
			[5,10],
			[19,20],
		],
	},
	2: [ # Armor
		[ #BODY,
		],
		[ #HEAD,
		],
		[ #SHIELD,
		],
		[ #WRIST,
		],
		[ #FOOTS,
		],
		[ #NECK,
		],
		[ #EAR,
		],
	],
}

# Farben :)
# COLOR_BG = grp.GenerateColor(0.0, 0.0, 0.0, 0.5)
# COLOR_INACTIVE = grp.GenerateColor(0.0, 0.0, 1.0, 0.2)
# COLOR_ACTIVE   = grp.GenerateColor(0.1, 0.6, 1.0, 0.2)
# COLOR_FINISHED = grp.GenerateColor(0.0, 0.8, 1.0, 0.3)

# COLOR_INACTIVE_RARE = grp.GenerateColor(0.0, 0.2, 1.0, 0.2)
# COLOR_ACTIVE_RARE   = grp.GenerateColor(0.2, 0.7, 1.0, 0.2)

# COLOR_HIGHLIGHT_RARE = grp.GenerateColor(0.2, 0.2, 1.0, 0.05)

# COLOR_PIN_HINT = grp.GenerateColor(0.0, 0.5, 1.0, 0.3)

# COLOR_CHECKBOX_NOT_SELECTED = grp.GenerateColor(0.0, 0.3, 1.0, 0.1)
# COLOR_CHECKBOX_SELECTED = grp.GenerateColor(0.0, 0.3, 1.0, 0.3)

# Standardfarben:
COLOR_BG = grp.GenerateColor(0.0, 0.0, 0.0, 0.5)
COLOR_INACTIVE = grp.GenerateColor(1.0, 0.0, 0.0, 0.2)
COLOR_ACTIVE   = grp.GenerateColor(1.0, 0.6, 0.1, 0.2)
COLOR_FINISHED = grp.GenerateColor(0.0, 1.0, 0.0, 0.2)

COLOR_INACTIVE_RARE = grp.GenerateColor(1.0, 0.2, 0.0, 0.2)
COLOR_ACTIVE_RARE   = grp.GenerateColor(1.0, 0.7, 0.2, 0.2)

COLOR_HIGHLIGHT_RARE = grp.GenerateColor(1.0, 0.2, 0.2, 0.05)

COLOR_PIN_HINT = grp.GenerateColor(0.0, 0.5, 1.0, 0.3)


COLOR_CHECKBOX_NOT_SELECTED = grp.GenerateColor(1.0, 0.3, 0.0, 0.1)
COLOR_CHECKBOX_SELECTED = grp.GenerateColor(0.3, 1.0, 1.0, 0.3)


DISTANCE_BOTTOM = 36

class Bar(ui.Bar):
	def __init__(self,layer = "UI"):
		ui.Bar.__init__(self,layer)
	def SetColor(self,color):
		wndMgr.SetColor(self.hWnd, color)
		self.color = color


class BotDialog(ui.Bar):
	def Activate(self):
		self.sub_parent.resetSwitch()
		self.Status_new.SetColor(COLOR_ACTIVE)
		self.sub_parent.StatusBar.SetColor(COLOR_ACTIVE)
		self.sub_parent.StatusText.SetText("Aktif")
		self.Starter.SetText("Efsun ("+str(self.index+1)+") Durdur")
		self.sub_parent.boni_active = 1
		if self.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.swib_normal.SetColor(COLOR_ACTIVE)
		pass
	def Deactivate(self):
		self.sub_parent.resetSwitch()
		self.Status_new.SetColor(COLOR_INACTIVE)
		self.sub_parent.StatusBar.SetColor(COLOR_INACTIVE)
		self.sub_parent.StatusText.SetText("Deaktif")
		self.Starter.SetText("Efsun ("+str(self.index+1)+") Baþlat")
		self.sub_parent.boni_active = 0
		if self.sub_parent.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.swib_normal.SetColor(COLOR_INACTIVE)
		pass
	
	def Activate_rare(self):
		self.sub_parent.resetSwitch_rare()
		self.sub_parent.StatusBar_rare.SetColor(COLOR_ACTIVE_RARE)
		self.sub_parent.StatusText_rare.SetText("6/7 Aktif")
		self.Starter_rare_boni.SetText("6/7 Durdur")
		self.sub_parent.boni_rare_active = 1
		if self.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.swib_rare.SetColor(COLOR_ACTIVE_RARE)
		pass
	def Deactivate_rare(self):
		self.sub_parent.resetSwitch_rare()
		# self.Status_new.SetColor(COLOR_INACTIVE)
		self.sub_parent.StatusBar_rare.SetColor(COLOR_INACTIVE_RARE)
		self.sub_parent.StatusText_rare.SetText("6/7 Baþlatýlmadý")
		self.Starter_rare_boni.SetText("6/7 Baþlat")
		self.Starter_rare_boni.SetUp()
		self.sub_parent.boni_rare_active = 0
		if self.sub_parent.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.swib_rare.SetColor(COLOR_INACTIVE_RARE)
		pass
		
	def Finish(self):
		self.Status_new.SetColor(COLOR_FINISHED)
		self.sub_parent.StatusBar.SetColor(COLOR_FINISHED)
		self.sub_parent.StatusText.SetText("Baþarýlý")
		self.Starter.SetText("Efsun ("+str(self.index+1)+") Devam")
		self.sub_parent.boni_active = 0
		if self.sub_parent.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.swib_normal.SetColor(COLOR_FINISHED)
		pass
		
	def Finish_rare(self):
		# self.Status_new.SetColor(COLOR_FINISHED)
		self.sub_parent.StatusBar_rare.SetColor(COLOR_FINISHED)
		self.sub_parent.StatusText_rare.SetText("6/7 Efsun Baþarýlý")
		self.Starter_rare_boni.SetText("6/7 Devam")
		self.sub_parent.boni_rare_active = 0
		if self.sub_parent.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.swib_rare.SetColor(COLOR_FINISHED)
		pass
	def Block(self):
		self.BlockBar.Show()
		self.BlockBar.sub.Show()
		self.Starter.Hide()
		pass
	def Unblock(self):
		self.BlockBar.sub.Hide()
		self.BlockBar.Hide()
		self.Starter.Show()
		pass
		
		
	def enable_rare_boni(self):
		for i in range(0,4):
			self.boni[6][i].Show()
			self.boni[7][i].Show()
		self.Starter.SetPosition(0,140+45)
		self.bg_rare_boni.Show()
		self.bg_rare_boni_but.Show()
		self.Starter_rare_boni.Show()
		self.sub_parent.StatusBar_rare.Show()
		
		if self.sub_parent.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.Enable_rare(1)
		
	def disable_rare_boni(self):
		for i in range(0,4):
			self.boni[6][i].Hide()
			self.boni[7][i].Hide()
		self.Starter.SetPosition(0,140)
		self.bg_rare_boni.Hide()
		self.bg_rare_boni_but.Hide()
		self.Starter_rare_boni.Hide()
		self.sub_parent.StatusBar_rare.Hide()
		self.Deactivate_rare()
		if self.sub_parent.parentWindow.parentWindow.gameWindow != None:
			self.sub_parent.blockBar.Enable_rare(0)
		
	def __init__(self,sub_parent):
		ui.Bar.__init__(self, "UI")
		self.sub_parent = sub_parent
		self.index = sub_parent.index
		self.SetColor(COLOR_BG)
		self.SetSize(500,225+10)
		self.boni = {}
		self.Status_new = ui.Bar()
		self.Status_new.SetParent(self)
		self.Status_new.SetColor(COLOR_INACTIVE)
		self.Status_new.SetSize(500,5)
		self.Status_new.Show()
		
		
		self.bg_rare_boni = ui.Bar()
		self.bg_rare_boni.SetColor(COLOR_HIGHLIGHT_RARE)
		self.bg_rare_boni.SetParent(self)
		self.bg_rare_boni.SetPosition(0,132)
		self.bg_rare_boni.SetSize(500,51)
		self.bg_rare_boni.Hide()
		
		self.bg_rare_boni_but = ui.Bar()
		self.bg_rare_boni_but.SetColor(COLOR_HIGHLIGHT_RARE)
		self.bg_rare_boni_but.SetParent(self)
		self.bg_rare_boni_but.SetPosition(400,133+50)
		self.bg_rare_boni_but.SetSize(100,25)
		self.bg_rare_boni_but.Hide()
		
		self.EnableRareBoni = CheckBox("6/7 Efsun")
		self.EnableRareBoni.SetParent(self)
		self.EnableRareBoni.SetPosition(10,185)
		self.EnableRareBoni.eventUp   = self.disable_rare_boni
		self.EnableRareBoni.eventDown = self.enable_rare_boni
		self.EnableRareBoni.Show()
		
		self.Starter = ui.ToggleButton()
		self.Starter.SetWindowHorizontalAlignCenter()
		self.Starter.SetParent(self)
		self.Starter.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
		self.Starter.SetOverVisual("d:/ymir work/ui/public/XLarge_button_02.sub")
		self.Starter.SetDownVisual("d:/ymir work/ui/public/XLarge_button_03.sub")
		self.Starter.SetPosition(0,140)
		self.Starter.SetText("Efsun At")
		self.Starter.SetToggleDownEvent(self.Activate)
		self.Starter.SetToggleUpEvent(self.Deactivate)
		self.Starter.Show()
		
		self.Starter_rare_boni = ui.ToggleButton()
		self.Starter_rare_boni.SetWindowHorizontalAlignCenter()
		self.Starter_rare_boni.SetParent(self)
		self.Starter_rare_boni.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.Starter_rare_boni.SetOverVisual("d:/ymir work/ui/public/Large_button_02.sub")
		self.Starter_rare_boni.SetDownVisual("d:/ymir work/ui/public/Large_button_03.sub")
		self.Starter_rare_boni.SetPosition(200,140+45)
		self.Starter_rare_boni.SetText("6/7 Baþlat")
		self.Starter_rare_boni.SetToggleDownEvent(self.Activate_rare)
		self.Starter_rare_boni.SetToggleUpEvent(self.Deactivate_rare)
		self.Starter_rare_boni.Hide()
		
		for i in range(7,5,-1):
			vas = i
			self.boni[vas] = {}
			self.boni[vas][0] = ui.TextLine()
			self.boni[vas][0].SetParent(self)
			self.boni[vas][0].SetText("Bonus "+str(vas))
			self.boni[vas][0].SetPosition(15,10+25*(vas-1))
			self.boni[vas][0].Hide()
			self.boni[vas][1] = DropDown(self,"Efsun Seç")
			self.boni[vas][1].SetPosition(70,10+25*(vas-1))
			for x in AFFECT_DICT:
				if x in BONI_RARE_AVAIL:
					self.boni[vas][1].AppendItem(str(AFFECT_DICT[x](0)),x)
			self.boni[vas][1].SetSize(330,20)
			self.boni[vas][1].Hide()
			self.boni[vas][2] = ui.Bar("UI")
			self.boni[vas][2].SetParent(self)
			self.boni[vas][2].SetPosition(410,10+25*(vas-1))
			self.boni[vas][2].SetColor(0xC0000000)
			self.boni[vas][2].SetSize(80,20)
			self.boni[vas][2].Hide()
			self.boni[vas][3] = Edit2("0",14)
			self.boni[vas][3].SetParent(self.boni[vas][2])
			self.boni[vas][3].SetNumberMode()
			self.boni[vas][3].SetSize(80,20)
			self.boni[vas][3].SetPosition(4,3)
			self.boni[vas][3].Hide()
		
		for i in range(0,5):
			vas = 5-i
			self.boni[vas] = {}
			self.boni[vas][0] = ui.TextLine()
			self.boni[vas][0].SetParent(self)
			self.boni[vas][0].SetText("Bonus "+str(vas))
			self.boni[vas][0].SetPosition(15,10+25*(vas-1))
			self.boni[vas][0].Show()
			self.boni[vas][1] = DropDown(self,"Efsun Seç")
			self.boni[vas][1].SetPosition(70,10+25*(vas-1))
			for x in AFFECT_DICT:
				if x in BONI_AVAIL:
					self.boni[vas][1].AppendItem(str(AFFECT_DICT[x](0)),x)
			self.boni[vas][1].SetSize(330,20)
			self.boni[vas][1].Show()
			self.boni[vas][2] = ui.Bar("UI")
			self.boni[vas][2].SetParent(self)
			self.boni[vas][2].SetPosition(410,10+25*(vas-1))
			self.boni[vas][2].SetColor(0xC0000000)
			self.boni[vas][2].SetSize(80,20)
			self.boni[vas][2].Show()
			self.boni[vas][3] = Edit2("0",14)
			self.boni[vas][3].SetParent(self.boni[vas][2])
			self.boni[vas][3].SetNumberMode()
			self.boni[vas][3].SetSize(80,20)
			self.boni[vas][3].SetPosition(4,3)
			self.boni[vas][3].Show()
		
		self.but_propose = DropDown(self,' ',0)
		self.but_propose.SetParent(self)
		self.but_propose.OnChange=self.change_boni
			
		self.BlockBar = ui.Bar()
		self.BlockBar.SetParent(self)
		self.BlockBar.SetColor(COLOR_INACTIVE)
		self.BlockBar.SetPosition(0,5)
		self.BlockBar.SetSize(500,170-5+35+2)
		self.BlockBar.Hide()
		
		self.BlockBar.sub = ui.Bar()
		self.BlockBar.sub.SetParent(self)
		self.BlockBar.sub.SetColor(COLOR_INACTIVE)
		self.BlockBar.sub.SetPosition(500-122,5+170-5+35+2)
		self.BlockBar.sub.SetSize(122,30)
		self.BlockBar.sub.Hide()
		
		self.BlockText = ui.TextLine()
		self.BlockText.SetParent(self.BlockBar)
		self.BlockText.SetWindowHorizontalAlignCenter()
		self.BlockText.SetHorizontalAlignCenter()
		self.BlockText.SetPosition(0,140)
		self.BlockText.SetText("Hata")
		self.BlockText.Show()
		
		
		self.prepare_propose()
		
	def change_boni(self):
		use = self.but_propose.DropList.GetSelectedItem().value
		bon = 1
		item.SelectItem(self.sub_parent.vnum)
		for x in use:
			# TODO: Check if bonus exists on item!!
			ok = 1
			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				if affectType == x[0]:
					ok = 0
			if ok == 0:
				continue
			y = self.boni[bon][1]
			y.SelectByAffectId(x[0])
			self.boni[bon][1].act.SetText(y.DropList.GetSelectedItem().textBox.GetText()) 
			self.boni[bon][3].SetText(str(x[1]))
			bon+=1

		for x in range(bon,6):
			y = self.boni[x][1].DropList
			y.SelectIndex(0)
			self.boni[x][1].act.SetText(y.GetSelectedItem().textBox.GetText()) 
			self.boni[x][3].SetText("0")
			
		
	def prepare_propose(self):
		self.but_propose.DropList.RemoveAllItems()
		self.but_propose.maxh = 95
		self.but_propose.c = 0
		self.but_propose.AppendItem("",[])
		
		item.SelectItem(self.sub_parent.vnum)
		type = item.GetItemType()
		
		use = proposals[type]
		if type == 2:
			use = use[item.GetItemSubType()]
		
		for prop in use:
			self.but_propose.AppendItem(prop,use[prop])
			
		
			
		
class ItemTabBar(ui.Window):
	
	class BlockBar(ui.Window):
		size_res = 32
		multi = 1
		def SetSize(self,i=1):
			self.multi = i
			ui.Window.SetSize(self,self.size_res,self.size_res*i)
			if self.rare_enabled == 1:
				self.swib_normal.SetSize(self.size_res,self.size_res*i/3*2)
				pos1 = int(1.0*self.size_res*i/3.0*2.0)
				self.swib_normal.SetSize(self.size_res,pos1)
				self.swib_rare.SetSize(self.size_res,self.size_res*i-pos1)
				self.swib_rare.SetPosition(0,pos1)
				self.swib_rare.Show()
			else:
				self.swib_normal.SetSize(self.size_res,self.size_res*i)
				self.swib_rare.Hide()
			
		def Enable_rare(self,o = 1):
			self.rare_enabled = o
			self.SetSize(self.multi)
			
		def __init__(self):
			ui.Window.__init__(self)
			self.rare_enabled = 0
			self.swib_normal = ui.Bar()
			self.swib_normal.SetParent(self)
			self.swib_normal.SetSize(self.size_res,self.size_res*self.multi)
			self.swib_normal.SetColor(COLOR_INACTIVE)
			self.swib_normal.SetPosition(0,0)
			self.swib_normal.Show()
			
			self.swib_rare = ui.Bar()
			self.swib_rare.SetParent(self)
			self.swib_rare.SetSize(self.size_res,self.size_res)
			self.swib_rare.SetColor(COLOR_INACTIVE_RARE)
			self.swib_rare.SetPosition(0,0)
			self.swib_rare.Hide()
			self.SetSize(1)
			
	
	class ItemTab(ui.Bar):
		
		height_selected   = 36*3+8
		height_unselected = 36*3+5
		
		def Destroy(self):
			self.parentWindow.parentWindow.enable_rebuy.SetParent(self.parentWindow.parentWindow)
			self.parentWindow.parentWindow.help_show_rare.SetParent(self.parentWindow.parentWindow)
			self.parentWindow.parentWindow.help_rebuy.SetParent(self.parentWindow.parentWindow)
			# self.parentWindow.parentWindow.help_stop_all.SetParent(self.parentWindow.parentWindow)
			self.parentWindow.parentWindow.help_duration.SetParent(self.parentWindow.parentWindow)
			# self.parentWindow.parentWindow.but_deactivate_all.SetParent(self.parentWindow.parentWindow)
			# self.parentWindow.parentWindow.but_deactivate_all.Hide()
			self.parentWindow.parentWindow.enable_rebuy.Hide()
			
			self.BotDialog.Hide()
			self.BotDialog.__del__()
			self.Hide()
			self.__del__()
		
		def DeleteMe(self):
			self.parentWindow.DeleteTab(self.tabnum)
		
		def __init__(self,parent,tabnum,index = 0,vnum = 0): ## Init ItemTab
			ui.Bar.__init__(self)
			self.SetColor(COLOR_BG)
			self.SetSize(self.width,self.height_unselected)
			self.index = index
			self.tabnum = tabnum
			self.vnum = vnum
			self.count = 0
			self.count_rare = 0
			self.parentWindow = parent
			self.SetParent(parent)

			
			
			if self.parentWindow.parentWindow.gameWindow != None:
				self.blockBar = ItemTabBar.BlockBar()
				self.blockBar.SetParent(self.parentWindow.parentWindow.gameWindow.interface.wndInventory.wndItem)
				ipi = self.parentWindow.parentWindow.gameWindow.interface.wndInventory.inventoryPageIndex
				self.blockBar.Show()
				ip2 = self.index - ipi*45
				self.blockBar.SetPosition(((ip2-int(ip2/5)*5)*self.blockBar.size_res),int(ip2/5)*self.blockBar.size_res)
			
			self.ItemIcon = ui.ImageBox()
			self.ItemIcon.SetParent(self)
			self.ItemIcon.AddFlag("not_pick")
			self.ItemIcon.SetWindowHorizontalAlignCenter()
			self.ItemIcon.SetWindowVerticalAlignCenter()
			self.ItemIcon.Show()
			
			self.SlotName = ui.TextLine()
			self.SlotName.SetParent(self)
			self.SlotName.SetWindowHorizontalAlignCenter()
			self.SlotName.SetHorizontalAlignCenter()
			self.SlotName.SetPosition(0,5)
			self.SlotName.SetText("Slot %d" % (self.index+1))
			self.SlotName.AddFlag("not_pick")
			self.SlotName.Show()
			
			self.StatusBar = Bar() # Special Bar
			self.StatusBar.SetParent(self)
			self.StatusBar.SetWindowVerticalAlignBottom()
			self.StatusBar.SetSize(self.width,20)
			self.StatusBar.SetPosition(0,20)
			self.StatusBar.SetColor(COLOR_INACTIVE)
			self.StatusBar.AddFlag("not_pick")
			self.StatusBar.Show()
			
			self.StatusText = ui.TextLine()
			self.StatusText.SetParent(self.StatusBar)
			# self.StatusText.SetParent(self)
			self.StatusText.SetWindowHorizontalAlignCenter()
			self.StatusText.SetWindowVerticalAlignCenter()
			self.StatusText.SetHorizontalAlignCenter()
			self.StatusText.SetVerticalAlignCenter()
			self.StatusText.SetPosition(0,0)
			self.StatusText.SetText("Baþlatýlmadý")
			self.StatusText.Show()
			
			
			self.StatusBar_rare = Bar() # Special Bar
			self.StatusBar_rare.SetParent(self)
			self.StatusBar_rare.SetWindowVerticalAlignBottom()
			self.StatusBar_rare.SetSize(self.width,20)
			self.StatusBar_rare.SetPosition(0,40)
			self.StatusBar_rare.SetColor(COLOR_INACTIVE_RARE)
			self.StatusBar_rare.AddFlag("not_pick")
			self.StatusBar_rare.Hide()
			
			self.StatusText_rare = ui.TextLine()
			self.StatusText_rare.SetParent(self.StatusBar_rare)
			self.StatusText_rare.SetWindowHorizontalAlignCenter()
			self.StatusText_rare.SetWindowVerticalAlignCenter()
			self.StatusText_rare.SetHorizontalAlignCenter()
			self.StatusText_rare.SetVerticalAlignCenter()
			self.StatusText_rare.SetPosition(0,0)
			self.StatusText_rare.SetText("6/7 Baþlatýlmadý")
			self.StatusText_rare.Show()
			
			
			self.CloseBut = ui.Button()
			self.CloseBut.SetParent(self)
			self.CloseBut.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
			self.CloseBut.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
			self.CloseBut.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
			self.CloseBut.SetToolTipText("Sekme Kapat", 0, -23)
			self.CloseBut.SetEvent(self.DeleteMe)
			self.CloseBut.SetPosition(0,0)
			self.CloseBut.Show()
			
		
			
			self.OnMouseLeftButtonDown = lambda: self.Select()
			
			self.boni_active = 0
			self.boni_rare_active = 0
			
			self.BotDialog = BotDialog(self)
			self.BotDialog.SetParentProxy(self.parentWindow.parentWindow)
			self.BotDialog.SetPosition(10,35+36*3+4*2)
			self.BotDialog.Hide()
			self.vnum = 0
			self.SetIndex(index)
			
			self.resetSwitch()
			self.resetSwitch_rare()

		def IsActive(self):
			return self.boni_active == 1
		def IsActive_rare(self):
			return self.boni_rare_active == 1
			
		def SetParentProxy(self,parent):
			ui.Bar.SetParentProxy(self,parent)
		def Select(self):
			for a in self.parentWindow.tabList:
				self.parentWindow.tabList[a].UnSelect()
				self.parentWindow.tabList[a].BotDialog.Hide()
			self.BotDialog.Show()
			self.SetSize(self.width,self.height_selected)
			self.Update()
			# self.parentWindow.parentWindow.but_deactivate_all.SetParent(self.BotDialog)
			# self.parentWindow.parentWindow.but_deactivate_all.SetPosition(415-10,200+10)
			# self.parentWindow.parentWindow.but_deactivate_all.Show()
			self.parentWindow.parentWindow.enable_rebuy.SetParent(self.BotDialog)
			self.parentWindow.parentWindow.enable_rebuy.SetPosition(10,200+10)
			
			self.parentWindow.parentWindow.help_show_rare.SetParent(self.BotDialog)
			self.parentWindow.parentWindow.help_rebuy.SetParent(self.BotDialog)
			# self.parentWindow.parentWindow.help_stop_all.SetParent(self.BotDialog)
			self.parentWindow.parentWindow.help_duration.SetParent(self.BotDialog)
			
			
		def UnSelect(self):
			self.SetSize(self.width,self.height_unselected)
			self.Update()
			
		def Update(self):
			self.StatusBar.SetPosition(0,20)
			self.SetPosition((self.width+self.dist)*self.tabnum,0)
			self.SlotName.SetText("Slot %d" % (self.index+1))
			
		def resetSwitch(self):
			self.values = [0,0,0,0,0]
		def resetSwitch_rare(self):
			self.values_rare = [0,0]
		
		# last_switch = -1
		rebuyed = 0
		def nachkauf(self):
			if self.parentWindow.parentWindow.buyed != 0:
				return
			else:
				self.parentWindow.parentWindow.buyed = 1
			if self.parentWindow.parentWindow.enable_rebuy.checked == 1:
				for i in range(0,shop.SHOP_SLOT_COUNT):
					me = shop.GetItemID(i)
					if me == SWITCH_VNUM:
						if shop.GetItemPrice(i) > player.GetMoney():
							# chat.AppendChat(1,"-- Switchitem gefunden! Aber wir haben kein Geld :(")
							self.BotDialog.Deactivate()
							return
						else:
							net.SendShopBuyPacket(i)
							self.rebuyed = 1
							# self.Switch()
							return
							
		rebuyed_rare = 0
		def nachkauf_rare(self):
			if self.parentWindow.parentWindow.buyed_rare != 0:
				return
			else:
				self.parentWindow.parentWindow.buyed_rare = 1
			if self.parentWindow.parentWindow.enable_rebuy.checked == 1:
				for i in range(0,shop.SHOP_SLOT_COUNT):
					me = shop.GetItemID(i)
					if me == SWITCH_RARE_VNUM:
						if shop.GetItemPrice(i) > player.GetMoney():
							# chat.AppendChat(1,"-- Switchitem gefunden! Aber wir haben kein Geld :(")
							self.BotDialog.Deactivate_rare()
							return
						else:
							net.SendShopBuyPacket(i)
							self.rebuyed_rare = 1
							# self.Switch()
							return
			# self.Vectors.Deactivate()
		
		def Switch_rare(self):
			lc = 0
			for i in range(0,90*2):
				if player.GetItemIndex(i) == SWITCH_RARE_VNUM:
					lc+=player.GetItemCount(i)
					
			if lc == 0 and self.parentWindow.parentWindow.enable_rebuy.checked == 0:
				chat.AppendChat(2,"Slot %d: 6/7 Efsun abgebrochen -> Keine Switchitems & Nachkauf war nicht erfolgreich!" % (self.index+1))
				self.BotDialog.Deactivate_rare()
				return
			
			if lc < 15:
				self.nachkauf_rare()
			
			if lc == 0:
				self.BotDialog.Deactivate_rare()
				chat.AppendChat(2,"Slot %d: 6/7 Efsun abgebrochen -> Keine Switchitems!" % (self.index+1))
				return
			
			for i in range(0,90*4):
				if player.GetItemIndex(i) == SWITCH_RARE_VNUM:
					net.SendItemUseToItemPacket(i,self.index)
					return
		
		def Switch(self):
			lc = 0
			for i in range(0,90*4):
				if player.GetItemIndex(i) == SWITCH_VNUM:
					lc+=player.GetItemCount(i)
			
			if lc == 0 and self.parentWindow.parentWindow.enable_rebuy.checked == 0:
				self.BotDialog.Deactivate()
				return
			
			if lc < 15:
				self.nachkauf()
				
			if lc == 0:
				self.BotDialog.Deactivate()
				return

			for i in range(0,90*2):
				if player.GetItemIndex(i) == SWITCH_VNUM:
					net.SendItemUseToItemPacket(i,self.index)
					return
			# self.nachkauf()
			
		def checkSwitch(self):
			ok = 0
			for i in range(0,5):
				if player.GetItemAttribute(self.index, i) != self.values[i]:
					ok = 1
			self.prob = self.GetProb()
			self.StatusText.SetText("Aktif (%d%%)" %self.prob)
			if ok == 1 or self.prob >= 90:
				if self.prob >= 90:
					chat.AppendChat(1,"Slot %d: Baþarýlý!" % (self.index+1))
					self.BotDialog.Finish()
					return
				self.values  = [player.GetItemAttribute(self.index, i) for i in range(0,5)]
				self.Switch()
			elif self.rebuyed > 0:
				self.rebuyed = 0
				self.Switch()
			elif ok == 0 and self.count < 5:
				self.count+=1
			elif ok == 0 and self.count >= 5:
				self.count = 0
				self.Switch()
			# elif self.last_switch > -1 and player.GetItemCount(self.last_switch) == 0:
			# chat.AppendChat(2,"%d"%player.GetItemCount(self.last_switch))
				# self.last_switch = 0
				# self.Switch()
			# else:
				# if self.GetProb() != 100:
					# self.Switch()
			pass
		
		def checkSwitch_rare(self):
			ok = 0
			for i in range(0,2):
				if player.GetItemAttribute(self.index, i+6) != self.values_rare[i]:
					ok = 1
			self.prob_rare = self.GetProb_rare()
			self.StatusText_rare.SetText("Aktif (%d%%)" %self.prob_rare)
			if ok == 1 or self.prob_rare >= 90:
				if self.prob_rare >= 90:
					self.BotDialog.Finish_rare()
					chat.AppendChat(1,"Slot %d: 6/7 Baþarýlý" % (self.index+1))
					return
				self.values_rare  = [player.GetItemAttribute(self.index, i) for i in range(5,7)]
				self.Switch_rare()
			elif self.rebuyed_rare > 0:
				self.rebuyed_rare = 0
				self.Switch_rare()
			elif ok == 0 and self.count_rare < 5:
				self.count_rare+=1
			elif ok == 0 and self.count_rare >= 5:
				self.count_rare = 0
				self.Switch_rare()
				
			pass
		def UpdateItem(self):
			# try:
			vnum = player.GetItemIndex(self.index)
			if vnum == 0 and self.vnum != 0:
				self.resetSwitch()
				self.resetSwitch_rare()
				self.vnum = 0
				self.BotDialog.Deactivate()
				self.BotDialog.Block()
				self.BotDialog.prepare_propose()
				self.ItemIcon.Hide()
				if self.parentWindow.parentWindow.gameWindow != None:
					self.blockBar.SetSize(1)
				return
			elif vnum != self.vnum:
				self.resetSwitch()
				self.resetSwitch_rare()
				self.vnum = vnum
				self.BotDialog.Deactivate()
				self.BotDialog.prepare_propose()
				item.SelectItem(self.vnum)
				if self.parentWindow.parentWindow.gameWindow != None:
					(w,h) = item.GetItemSize()
					self.blockBar.SetSize(h)
				
				if item.GetItemType() != 1 and item.GetItemType() != 2:
					self.BotDialog.Block()
				else:
					self.BotDialog.Unblock()
				
				
				self.ItemIcon.Show()
				self.ItemIcon.LoadImage(item.GetIconImageFileName())
				# self.values = [player.GetItemAttribute(self.index, i) for i in range(0,5)]
				return
			if self.IsActive():
				self.checkSwitch()
				
			if self.IsActive_rare():
				self.checkSwitch_rare()
				
			# except:
				# pass
			
		def SetIndex(self,index):
			self.index = index
			self.BotDialog.index = index
			self.BotDialog.Starter.SetText("Efsun ("+str(index+1)+") Baþlat")
			self.Update()
			self.UpdateItem()
			
		def GetProb_rare(self):
			values = [player.GetItemAttribute(self.index, i+6) for i in range(0,2)]
			val2 = {}
			for i in range(0,2):
				try:
					affectString = AFFECT_DICT[values[i][0]](values[i][1])
					val2[values[i][0]] = values[i][1]
					self.BotDialog.ibi[i+6].SetText(affectString)
				except:
					pass
			prob = 0
			max  = 0
			
			yp = self.GenList_rare()
			for x in yp:
				if yp[x] in val2 and val2[yp[x]] >= int(self.BotDialog.boni[x+5][3].GetText()):
					prob = prob+1
				max = max+1
			if max > 0:
				prozent = 100/max*prob
			else:
				prozent = 100
			return prozent
			pass
		def GetProb(self):
			values = [player.GetItemAttribute(self.index, i) for i in range(0,5)]
			val2 = {}
			# for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			for i in range(0,5):
				try:
					affectString = AFFECT_DICT[values[i][0]](values[i][1])
					val2[values[i][0]] = values[i][1]
					self.BotDialog.ibi[i].SetText(affectString)
				except:
					pass
			prob = 0
			max  = 0
			yp = self.GenList()
			for x in yp:
				if yp[x] in val2 and val2[yp[x]] >= int(self.BotDialog.boni[x][3].GetText()):
					prob = prob+1
				max = max+1
			if max > 0:
				prozent = 100/max*prob
			else:
				prozent = 100
			return prozent
			
		def GenList(self):
			ret = {}
			for i in range(0,5):
				if self.BotDialog.boni[5-i][1].selected.value != 0:
					ret[5-i] = self.BotDialog.boni[5-i][1].selected.value
			return ret
		def GenList_rare(self):
			ret = {}
			for i in range(0,2):
				if self.BotDialog.boni[6+i][1].selected.value != 0:
					ret[i+1] = self.BotDialog.boni[6+i][1].selected.value
			return ret
			
	def __init__(self,parent):
		ui.Window.__init__(self)
		self.SetSize(500,36*3+4*2)
		self.tabCount = 0
		self.tabList = {}
		
		self.parentWindow = parent
		self.SetParent(parent)
		self.plusBar = ui.Bar()
		self.plusBar.SetParent(self)
		self.plusBar.SetSize(90,30)
		self.plusBar.SetWindowVerticalAlignCenter()
		self.plusBar.SetColor(COLOR_BG)
		# self.plusBar.OnMouseLeftButtonDown = lambda: self.AddTab_pre()
		self.plusBar.OnMouseLeftButtonUp = lambda: self.AddTab_pre()
		self.OnMouseLeftButtonUp = lambda: self.AddTab_pre()
		self.plusBar.SetPosition(30,0)
		self.plusBar.Show()
		
		self.AddText = ui.TextLine()
		self.AddText.SetParent(self.plusBar)
		self.AddText.SetText("Boþ Slot")
		self.AddText.SetWindowVerticalAlignCenter()
		self.AddText.SetWindowHorizontalAlignCenter()
		self.AddText.SetVerticalAlignCenter()
		self.AddText.SetHorizontalAlignCenter()
		self.AddText.SetPosition(0,0)
		self.AddText.Show()
		
		dist = [
			[500,  0  ], #1
			[240, 10  ],
			[160, 10  ],
			[117, 10.5],
			# [ 80, 25  ], #5
			[ 92, 10  ], #5
			[ 75, 10  ], #6
			[ 64,  9  ], #7
			[ 56,  7.5], #8
			[ 50,  6.5], #9
			[ 45,  5.5], #10
		][MAX_NUM-1]
		self.ItemTab.width = dist[0]
		self.ItemTab.dist  = dist[1]

		
		#For 8
		# dist  = 9-1.5
		# width = 56
		
		# For 9
		# dist  = 9-2.5
		# width = 50
		
		# For 10
		# dist  = 9-3.5
		# width = 45
		
		
	def DeleteTab(self,id):
		if self.parentWindow.gameWindow != None:
			self.tabList[id].blockBar.Hide()
			self.tabList[id].blockBar.Destroy()
		self.tabList[id].Destroy()
		# del self.tabList[id]
		self.tabCount = self.tabCount -1 
		if self.tabCount > id and id < 5:
			for i in xrange(id,self.tabCount):
				self.tabList[i] = self.tabList[i+1] 
				self.tabList[i].tabnum = i
				self.tabList[i].SetPosition((self.tabList[i].width+self.tabList[i].dist)*i,0)
				
			del self.tabList[self.tabCount]
			
		else:
			del self.tabList[id]
			
		
		if self.tabCount > 0:
			self.tabList[0].Select()
		
		if (self.ItemTab.width+self.ItemTab.dist)*self.tabCount < 20:
			self.parentWindow.SetSize(520,387-210)
			self.plusBar.SetPosition(0,0)
		else:
			(x,y) = self.tabList[self.tabCount-1].GetLocalPosition()
			self.plusBar.SetPosition(x+self.ItemTab.width+self.ItemTab.dist,0)
			# self.plusBar.SetPosition((self.ItemTab.width+self.ItemTab.dist)*self.tabCount-10,0)
		self.plusBar.Show()
		
	def AddTab_pre(self):
		if mouseModule.mouseController.isAttached():
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedSlotVnum = mouseModule.mouseController.GetAttachedItemIndex()
			
			item.SelectItem(attachedSlotVnum)
			if item.GetItemType() != 1 and item.GetItemType() != 2:
				mouseModule.mouseController.DeattachObject()
				chat.AppendChat(2,"Geçersiz Ýtem.")
				return
			
			for a in self.tabList:
				if self.tabList[a].index == attachedSlotPos:
					mouseModule.mouseController.DeattachObject()
					chat.AppendChat(2,"Bu Ýtem Zaten Ekli!")
					return
				
				
			if player.SLOT_TYPE_INVENTORY == attachedSlotType:
				self.AddTab(attachedSlotPos,attachedSlotVnum)
				chat.AppendChat(2,"Ýtem Eklendi!")

			mouseModule.mouseController.DeattachObject()
		
	def AddTab(self,id,vnum = 0):
		if self.tabCount < MAX_NUM:
			self.tabList[self.tabCount] = self.ItemTab(self,self.tabCount,id,vnum)
			
			self.tabList[self.tabCount].Select()
			self.tabList[self.tabCount].Show()
			self.tabCount+= 1
			if (self.ItemTab.width+self.ItemTab.dist)*self.tabCount < 20:
				self.plusBar.SetPosition(20,0)
			else:
				(x,y) = self.tabList[self.tabCount-1].GetLocalPosition()
				self.plusBar.SetPosition(x+self.ItemTab.width+self.ItemTab.dist,0)
		
		if self.tabCount == MAX_NUM:
			self.plusBar.Hide()
			
		self.parentWindow.SetSize(520,387+10)
			# return 0

class Bot(ui.BoardWithTitleBar):
	class PinGroup(ui.Bar):
		def OnUpdate(self):
			## Now check position.
			(x,y) = self.GetGlobalPosition()
			max_x = wndMgr.GetScreenWidth()-self.GetWidth()
			max_y = wndMgr.GetScreenHeight()-self.GetHeight()-DISTANCE_BOTTOM
			if not x == self.pos_x  or not y == self.pos_y:
				old_dir = self.dir
				if self.pos_x == 0 and not self.pos_y == 0 and not self.pos_y == max_y and old_dir != 1:
					self.parse_dir(1)
				elif self.pos_x == max_x and not self.pos_y == 0 and not self.pos_y == max_y and old_dir != 2:
					self.parse_dir(2)
				elif self.pos_y == max_y and not self.pos_x == 0 and not self.pos_x == max_x and old_dir != 4:
					self.parse_dir(4)
				elif self.pos_y == 0 and not self.pos_x == 0 and not self.pos_x == max_x and old_dir != 3:
					self.parse_dir(3)
					
				max_x = wndMgr.GetScreenWidth()-self.GetWidth()
				max_y = wndMgr.GetScreenHeight()-self.GetHeight()-DISTANCE_BOTTOM	
					
				
				if self.pos_x == 0 and not self.pos_y == 0 and not self.pos_y == max_y:
					x = 0
				elif self.pos_x == max_x and not self.pos_y == 0 and not self.pos_y == max_y:
					x = max_x
				elif self.pos_y == 0 and not self.pos_x == 0 and not self.pos_x == max_x:
					y = 0
				elif self.pos_y == max_y and not self.pos_x == 0 and not self.pos_x == max_x:
					y = max_y
				if x > 0 and x < max_x and y > 0 and y < max_y:
					if y < int(max_y/2):
						y = 0
					else:
						y = max_y
					
					if x < int(max_x/2):
						x = 0
					else:
						x = max_x
						
				
				x = min(max(0,x),wndMgr.GetScreenWidth()-self.GetWidth())
				y = min(max(0,y),wndMgr.GetScreenHeight()-self.GetHeight()-DISTANCE_BOTTOM)
				self.SetPosition(x,y)
				self.pos_x = x
				self.pos_y = y
			# (self.pos_x,self.pos_y) = self.GetGlobalPosition()
			self.parent.OnUpdate()
			for c in self.txtlist:
				c.SetColor(c.item.StatusBar.color)
				c.txt2.SetText("Durum: %s" % c.item.StatusText.GetText())
				try:
					c.listWin2.SetColor(c.item.StatusBar_rare.color)
					c.txt3.SetText("Durum: %s" % c.item.StatusText_rare.GetText())
				except:
					pass
				
		def ShowMainWindow(self):
			(x,y) = self.parent.GetGlobalPosition()
			x = min(max(32,x),wndMgr.GetScreenWidth()-self.parent.GetWidth()-32)
			y = min(max(32,y),wndMgr.GetScreenHeight()-self.parent.GetHeight()-DISTANCE_BOTTOM-32)
			self.parent.SetPosition(x,y)
			self.parent.Show()
			self.__del__()
		def parse_dir(self,dir):
			self.dir = dir
			w,h = 100,50
			for listWin in self.txtlist:
				itm = listWin.item
				listWin.AddFlag("not_pick")
				if dir >= 3:
					listWin.SetPosition(w,4)
					listWin.SetSize(90,h-8)
					w+=92
					if itm.BotDialog.EnableRareBoni.checked == 1:
						w+=15
						listWin.SetSize(105,4+12+12)
						listWin.listWin2.SetSize(105,14)
						listWin.listWin2.Show()
						listWin.txt3.Show()
				else:
					listWin.SetPosition(0,h)
					listWin.SetSize(w,4+12+12+2)
					
					if itm.BotDialog.EnableRareBoni.checked == 1:
						listWin.SetSize(w,4+12+12)
						listWin.listWin2.SetSize(w,14)
						listWin.listWin2.Show()
						listWin.txt3.Show()
						# itl.append(txt3)
						h+=12
						pass
					h+=4+12+12+4
			self.SetSize(w,h)
		
		def __init__(self,parent,dir = 1):
			self.parent = parent
			self.dir = dir
			ui.Bar.__init__(self)
			# Direction: 1 = left; 2 = right; 3 = top
			self.SetColor(COLOR_BG)
			w,h = 100,50
			
			self.AddFlag("float")
			self.AddFlag("movable")

			
			
			self.maximise_but = ui.Button()
			self.maximise_but.SetParent(self)
			self.maximise_but.SetPosition(4,4)
			self.maximise_but.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
			self.maximise_but.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
			self.maximise_but.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
			self.maximise_but.SetText("Büyült")
			self.maximise_but.SetEvent(self.ShowMainWindow)
			self.maximise_but.Show()
			
			self.stop_but = ui.Button()
			self.stop_but.SetParent(self)
			self.stop_but.SetPosition(4,24)
			self.stop_but.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
			self.stop_but.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
			self.stop_but.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
			self.stop_but.SetText("Tümünü Durdur")
			self.stop_but.SetEvent(self.parent.deactivate_all)
			self.stop_but.Show()
			
			self.txtlist = []
			for a in self.parent.itemTabBar.tabList:
				itm = self.parent.itemTabBar.tabList[a]
				
				listWin = ui.Bar()
				listWin.item = itm
				self.txtlist.append(listWin)
				listWin.SetColor(itm.StatusBar.color)
				listWin.SetParent(self)
				listWin.AddFlag("not_pick")
				listWin.Show()
				
				listWin.txt1 = ui.TextLine()
				listWin.txt1.SetParent(listWin)
				listWin.txt1.SetText("Slot %d:" %(itm.index+1))
				listWin.txt1.Show()
				listWin.txt1.SetPosition(4,2)
				
				listWin.txt2 = ui.TextLine()
				listWin.txt2.SetParent(listWin)
				listWin.txt2.SetText("Durum: %s" % itm.StatusText.GetText())
				listWin.txt2.SetPosition(4,2+12)
				listWin.txt2.Show()
				
				listWin.listWin2 = ui.Bar()
				listWin.listWin2.AddFlag("not_pick")
				listWin.listWin2.SetColor(itm.StatusBar_rare.color)
				listWin.listWin2.SetParent(listWin)
				listWin.listWin2.SetPosition(0,4+12+12)
				listWin.listWin2.SetSize(w,12+2)
				listWin.listWin2.Hide()
				
				listWin.txt3 = ui.TextLine()
				listWin.txt3.SetParent(listWin.listWin2)
				listWin.txt3.SetText("Durum: %s" % itm.StatusText_rare.GetText())
				listWin.txt3.SetPosition(4,0)
				listWin.txt3.Hide()
				
				if dir >= 3:
					listWin.SetPosition(w,4)
					listWin.SetSize(90,h-8)
					w+=92
					if itm.BotDialog.EnableRareBoni.checked == 1:
						w+=15
						listWin.SetSize(105,4+12+12)
						listWin.listWin2.SetSize(105,14)
						listWin.listWin2.Show()
						listWin.txt3.Show()
				else:
					listWin.SetPosition(0,h)
					listWin.SetSize(w,4+12+12+2)
					
					if itm.BotDialog.EnableRareBoni.checked == 1:
						listWin.SetSize(w,4+12+12)
						listWin.listWin2.Show()
						listWin.txt3.Show()
						# itl.append(txt3)
						h+=12
						pass
					
					h+=4+12+12+4
				# else:
					
				# self.txtlist.append(itl)
			
			self.SetSize(w,h)
			(x,y) = self.parent.GetGlobalPosition()
			
			x = min(max(0,x),wndMgr.GetScreenWidth()-self.GetWidth())
			y = min(max(0,y),wndMgr.GetScreenHeight()-self.GetHeight()-DISTANCE_BOTTOM)
			if dir == 1:
				# self.SetWindowHorizontalAlignLeft()
				self.SetPosition(0,y)
			elif dir == 2:
				# self.SetWindowHorizontalAlignRight()
				self.SetPosition(wndMgr.GetScreenWidth()-self.GetWidth(),y)
			elif dir == 3:
				# self.SetWindowVerticalAlignTop()
				self.SetPosition(x,0)
			else:
				# self.SetWindowVerticalAlignBottom()
				self.SetPosition(x,wndMgr.GetScreenHeight()-(DISTANCE_BOTTOM+h))
				
			(self.pos_x,self.pos_y) = self.GetGlobalPosition()
				
			self.parse_dir(dir)
			# if dir == 1:
				# self.SetWindowHorizontalAlignLeft()
				# self.SetPosition(0,0)
			# elif dir == 2:
				# self.SetWindowHorizontalAlignRight()
				# self.SetPosition(self.GetWidth(),0)
			# elif dir == 3:
				# self.SetWindowHorizontalAlignCenter()
				# self.SetPosition(0,0)
			# elif dir == 4:
				# self.SetWindowHorizontalAlignCenter()
				# self.SetPosition(0,36+h)
			# else:
				# return # ERR
				
				
	
	pinhint = 0
	def ShowPinHint(self,type):
		self.pinhint = type
		if type == 0:
			self.PinHint.Hide()
			return
		# type=2
		
		(x,y) = self.GetGlobalPosition()
		if type == 1:  # Left
			self.PinHint.SetWindowHorizontalAlignLeft()
			self.PinHint.SetWindowVerticalAlignCenter()
			self.PinHint.SetSize(max(min(30,30-x),3),wndMgr.GetScreenHeight())
			self.PinHint.SetPosition(0,0)
		elif type == 2: # Right
			self.PinHint.SetWindowHorizontalAlignRight()
			self.PinHint.SetWindowVerticalAlignCenter()
			self.PinHint.SetSize(30,wndMgr.GetScreenHeight())
			self.PinHint.SetPosition(max(min(30,30-(wndMgr.GetScreenWidth()-(x+self.GetWidth()))),3),0)
		elif type == 3: # Top
			self.PinHint.SetWindowHorizontalAlignCenter()
			self.PinHint.SetWindowVerticalAlignTop()
			self.PinHint.SetSize(wndMgr.GetScreenWidth(),max(min(30,30-y),3))
			self.PinHint.SetPosition(0,0)
		elif type == 4: # Top
			self.PinHint.SetWindowHorizontalAlignCenter()
			self.PinHint.SetWindowVerticalAlignBottom()
			self.PinHint.SetSize(wndMgr.GetScreenWidth(),30)
			self.PinHint.SetPosition(0,36+max(min(30,30-(wndMgr.GetScreenHeight()-36-(y+self.GetHeight()))),3))
		self.PinHint.Show()
		# else:
			
	
	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)
		self.gameWindow = None
		
		self.AddFlag("float")
		self.AddFlag("movable")
		self.SetPosition(30,30)
		# self.SetSize(520,387)
		# self.SetSize(520,387-210)
		self.SetSize(520,387-210)
		
		self.PinHint = ui.Bar()
		self.PinHint.SetColor(COLOR_PIN_HINT)
		self.PinHint.Show()
		
		self.OnMouseLeftButtonDown = self.drag_start
		self.OnMouseLeftButtonUp   = self.drag_end
		
		self.titleBar.MinimizeBut = ui.Button()
		self.titleBar.MinimizeBut.SetParent(self.titleBar)
		self.titleBar.MinimizeBut.SetUpVisual("d:/ymir work/ui/public/minimize_button_01.sub")
		self.titleBar.MinimizeBut.SetOverVisual("d:/ymir work/ui/public/minimize_button_02.sub")
		self.titleBar.MinimizeBut.SetDownVisual("d:/ymir work/ui/public/minimize_button_03.sub")
		self.titleBar.MinimizeBut.SetToolTipText("Simge Durumunda Küçült", 0, -23)
		self.titleBar.MinimizeBut.SetPosition(520 - self.titleBar.btnClose.GetWidth()-3- 32 - 3, 3)
		self.titleBar.MinimizeBut.SetEvent(lambda: self.PinShow(1))
		self.titleBar.MinimizeBut.Show()
		
		self.titleBar.HelpBut = ui.ToggleButton()
		self.titleBar.HelpBut.SetParent(self.titleBar)
		self.titleBar.HelpBut.SetPosition(3, 0)
		self.titleBar.HelpBut.SetToggleDownEvent(lambda: self.ToggleHelp(1))
		self.titleBar.HelpBut.SetToggleUpEvent(lambda: self.ToggleHelp(0))
		self.titleBar.HelpBut.Show()
		
		
		self.but_deactivate_all = ui.Button()
		# self.but_deactivate_all.SetParent(self)
		self.but_deactivate_all.SetParent(self.titleBar)
		# self.but_deactivate_all.SetPosition(415,350)
		self.but_deactivate_all.SetPosition(3+5+self.titleBar.HelpBut.GetWidth(), 0)
		self.but_deactivate_all.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
		self.but_deactivate_all.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
		self.but_deactivate_all.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
		self.but_deactivate_all.SetText("Tümünü Durdur")
		self.but_deactivate_all.SetEvent(self.deactivate_all)
		self.but_deactivate_all.Show()
		
		self.titleBar.but_deactivate_all = ui.Button()
		self.titleBar.but_deactivate_all.SetParent(self.titleBar)
		self.titleBar.but_deactivate_all.SetPosition(3+5+self.titleBar.HelpBut.GetWidth()+5+self.but_deactivate_all.GetWidth(), 0)
		self.titleBar.but_deactivate_all.SetEvent(self.about)
		self.titleBar.but_deactivate_all.Show()
		self.titleName.SetPosition(60, 4)
		
		self.enable_rebuy = CheckBox("")
		self.enable_rebuy.Toggle()
		
		self.but_speed = ui.SliderBar()
		self.but_speed.SetParent(self)
		self.but_speed.SetWindowHorizontalAlignCenter()
		self.but_speed.SetPosition(0,360)
		self.but_speed.SetSliderPos(0.0)
		# self.but_speed.SetToolTipText("Switch-Geschwindigkeit", 0, -23)
		# self.but_speed.Show()
		
		self.itemTabBar = ItemTabBar(self)
		self.itemTabBar.SetPosition(10,35)
		self.itemTabBar.Show()
		
		
		self.buyed = 0
		self.buyed_rare = 0
		
		self.SetCloseEvent(self._Hide)
		
		self.Hide = self._Hide
		
		
		### NOW initialize the HELP stuff!
		
		self.help_add_item = HelpBar(0.8,'Boþ Slot')
		self.help_add_item.SetParent(self)
		self.help_add_item.SetPosition(60,50)
		
		self.help_minimize = HelpBar(0.8,'',1)
		self.help_minimize.SetParent(self.titleBar)
		self.help_minimize.SetWindowHorizontalAlignCenter()
		self.help_minimize.SetPosition(100,-30)
		
		self.help_show_rare = HelpBar(0.85,'')
		self.help_show_rare.SetParent(self)
		# self.help_show_rare.SetPosition(35,312)
		
		self.help_rebuy = HelpBar(0.5,'')
		self.help_rebuy.SetParent(self)
		# self.help_rebuy.SetPosition(25,297)
		
		self.help_stop_all = HelpBar(0.8,'',1)
		self.help_stop_all.SetParent(self.titleBar)
		self.help_stop_all.SetPosition(3+5+self.titleBar.HelpBut.GetWidth()*1.2,-30)
		
		self.help_duration = HelpBar(0.8,'',1)
		self.help_duration.SetParent(self)
		
		
	def ToggleHelp(self,val):
		self.help_on = val
	
	def Show(self):
		ui.BoardWithTitleBar.Show(self)
		self.is_open = 1
		
	def _Hide(self):
		## ONLY FOR TESTING
		# for a in range(0,self.itemTabBar.tabCount):
			# self.itemTabBar.DeleteTab(0)
		
		ui.BoardWithTitleBar.Hide(self)
		self.is_open = 0
		
		
	def deactivate_all(self):
		for a in self.itemTabBar.tabList:
			self.itemTabBar.tabList[a].BotDialog.Starter.SetUp()
			self.itemTabBar.tabList[a].BotDialog.Starter.OnToggleUp()
			self.itemTabBar.tabList[a].BotDialog.Starter_rare_boni.SetUp()
			self.itemTabBar.tabList[a].BotDialog.Starter_rare_boni.OnToggleUp()
		pass
		
	drag = 0
	def drag_start(self):
		self.drag = 1
		
	def drag_end(self):
		self.drag = 0
		if self.pinhint > 0:
			self.PinShow(self.pinhint)
			# self.PinGroupBox = self.PinGroup()
			pass
		self.ShowPinHint(0)
		
	def PinShow(self,dir):
		self.PinGroupBox = self.PinGroup(self,dir)
		self.Hide()
		self.PinGroupBox.Show()
		
	def EnableInventoryTweak(self,gameWindow):
		self.gameWindow=gameWindow
		self.gameWindow.interface.wndInventory.inventoryTab[0].SetEvent(lambda arg=0: self.__SetInventoryPage(arg))
		self.gameWindow.interface.wndInventory.inventoryTab[1].SetEvent(lambda arg=1: self.__SetInventoryPage(arg))
	def __SetInventoryPage(self,arg):
		self.gameWindow.interface.wndInventory.SetInventoryPage(arg)
		for a in self.itemTabBar.tabList:
			itm = self.itemTabBar.tabList[a]
			if itm.index >= arg*45 and itm.index < (arg+1)*45:
				itm.blockBar.Show()
			else:
				itm.blockBar.Hide()
	help_on = 0
	counter = 0
	AboutWindow = None
	def OnUpdate(self):
		if self.AboutWindow:
			if self.AboutWindow.x_counter > 1:
				self.AboutWindow.x_counter -=1
				self.AboutWindow.text6.SetText("EndMt2: %0.1f" % (self.AboutWindow.x_counter/45.0))
			elif self.AboutWindow.x_counter == 1:
				self.AboutWindow.Hide()
				# self.AboutWindow.Delete()
				
		if self.help_on == 1:
			(x,y) = self.itemTabBar.plusBar.GetLocalPosition()
			self.help_add_item.SetPosition(x+20,50)
			self.help_add_item.Show()
			self.help_stop_all.Show()
			
			self.help_minimize.Show()
			if self.itemTabBar.tabCount > 0:
				self.help_show_rare.SetPosition(25,164)
				self.help_show_rare.Show()
				self.help_rebuy.SetPosition(15,147)
				self.help_rebuy.Show()
				
				# self.help_duration.SetPosition(300,180)
				self.help_duration.Show()
				
			else:
				self.help_show_rare.Hide()
				self.help_rebuy.Hide()
				self.help_duration.Hide()
			
		else:
			self.help_add_item.Hide()
			self.help_minimize.Hide()
			self.help_show_rare.Hide()
			self.help_rebuy.Hide()
			self.help_stop_all.Hide()
			self.help_duration.Hide()
			
		if self.drag == 1:
			(x1, y1) = self.GetGlobalPosition()
			# if x1 < 0:
				# x1 = 0
			# elif x1 > wndMgr.GetScreenWidth()-520:
				# x1 = wndMgr.GetScreenWidth()-520
			# if y1 < 0:
				# y1 = 0
			# elif y1 > wndMgr.GetScreenHeight()-36-self.GetHeight():
				# y1 = wndMgr.GetScreenHeight()-36-self.GetHeight()
			x1 = max(min(wndMgr.GetScreenWidth()-520,x1),0)
			y1 = max(min(wndMgr.GetScreenHeight()-36-self.GetHeight(),y1),0)
			self.SetPosition(x1,y1)
			if x1 < 30:
				self.ShowPinHint(1)
			elif wndMgr.GetScreenWidth()-x1-520 < 30:
				self.ShowPinHint(2)
			elif y1 < 30:
				self.ShowPinHint(3)
			elif wndMgr.GetScreenHeight()-y1-self.GetHeight() < 60:
				self.ShowPinHint(4)
			else:
				self.ShowPinHint(0)
			# self.SetPosition(x1+(x-self.drag_pos[0]),y1)
			pass
		
		if self.gameWindow != None:
			for a in self.itemTabBar.tabList:
				itm = self.itemTabBar.tabList[a]
			
			
			
		self.counter+=1
		if self.counter >= int(self.but_speed.GetSliderPos()*MAX_SWITCH_DELAY_APPEND+MIN_SWITCH_DELAY):
			self.buyed = 0
			self.buyed_rare = 0
			self.counter = 0
			for a in self.itemTabBar.tabList:
				itm = self.itemTabBar.tabList[a]
				itm.UpdateItem()
				# if itm.
				

	def about(self):
		self.AboutWindow = ui.ThinBoard()
		self.AboutWindow.SetParent(self)
		self.AboutWindow.SetSize(250,100)
		self.AboutWindow.SetWindowHorizontalAlignCenter()
		self.AboutWindow.SetWindowVerticalAlignCenter()
		self.AboutWindow.SetPosition(0,0)
		self.AboutWindow.Show()
		self.AboutWindow.x_counter = 450
		
		self.AboutWindow.text1 = ui.TextLine()
		self.AboutWindow.text1.SetParent(self.AboutWindow)
		self.AboutWindow.text1.SetWindowHorizontalAlignCenter()
		self.AboutWindow.text1.SetHorizontalAlignCenter()
		self.AboutWindow.text1.SetPosition(0,5)
		self.AboutWindow.text1.SetText("EndMt2 - Efsun Botu")
		self.AboutWindow.text1.SetPackedFontColor(ui.GenerateColor(58, 141, 221))
		self.AboutWindow.text1.Show()
		
		self.AboutWindow.text2 = ui.TextLine()
		self.AboutWindow.text2.SetParent(self.AboutWindow)
		self.AboutWindow.text2.SetPosition(8,18)
		self.AboutWindow.text2.SetText("")
		self.AboutWindow.text2.Show()
		
		self.AboutWindow.text3 = ui.TextLine()
		self.AboutWindow.text3.SetParent(self.AboutWindow)
		self.AboutWindow.text3.SetPosition(8,18+13)
		self.AboutWindow.text3.SetText("")
		self.AboutWindow.text3.Show()
		
		self.AboutWindow.text4 = ui.TextLine()
		self.AboutWindow.text4.SetParent(self.AboutWindow)
		self.AboutWindow.text4.SetPosition(8,18+13*3)
		self.AboutWindow.text4.SetText("")
		self.AboutWindow.text4.Show()
		
		self.AboutWindow.text5 = ui.TextLine()
		self.AboutWindow.text5.SetParent(self.AboutWindow)
		self.AboutWindow.text5.SetPosition(8,18+13*4)
		self.AboutWindow.text5.SetText("")
		self.AboutWindow.text5.Show()
		
		self.AboutWindow.text6 = ui.TextLine()
		self.AboutWindow.text6.SetParent(self.AboutWindow)
		self.AboutWindow.text6.SetPosition(200,18+13*5)
		self.AboutWindow.text6.SetText("EndMt2: %d" % self.AboutWindow.x_counter)
		self.AboutWindow.text6.Show()
		
		
		
class HelpBar(ui.Window):
	def __init__(self,width,text,centered = 0):
		ui.Window.__init__(self)
		self.AddFlag("not_pick")
		self.AddFlag("attach")
		
		img = ui.ExpandedImageBox()
		
		img.SetParent(self)
		img.LoadImage("d:/ymir work/ui/pattern/help_stick.tga")
		img.SetRenderingRect(0.0, -width, 0.0, 0.0)
		img.Show()
		
		self.img = img
		
		txt = ui.TextLine()
		
		txt=ui.TextLine()
		txt.SetParent(self)
		txt.SetText(text)
		txt.Show()
		
		img.SetPosition(0,18-width*img.GetHeight())
		txt.SetPosition(0,0)
		txt.SetWindowHorizontalAlignCenter()
		if centered != 0:
			txt.SetHorizontalAlignCenter()
		# txt.SetPosition(0,0)
		
		self.txt = txt
	
class DropDown(ui.Window):
	dropped  = 0
	dropstat = 0
	last = 0
	lastS = 0
	maxh = 95
	OnChange = None
	class Item(ui.ListBoxEx.Item):
		def __init__(self,parent, text,value=0):
			ui.ListBoxEx.Item.__init__(self)

			self.textBox=ui.TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			# self.textBox.SetLimitWidth(parent.GetWidth()-132)
			self.textBox.Show()
			self.value = value
		def GetValue(self):
			return self.value
		def __del__(self):
			ui.ListBoxEx.Item.__del__(self)
			
	def __init__(self,parent,tt = "",down=1):
		ui.Window.__init__(self,"TOP_MOST")
		self.down = down
		self.SetParentProxy(parent)
		self.bg = ui.Bar("TOP_MOST")
		self.bg.SetParent(self)
		self.bg.SetPosition(0,0)
		self.bg.SetColor(0xc0000000)
		self.bg.OnMouseOverIn = self.bgMouseIn
		self.bg.OnMouseOverOut = self.bgMouseOut
		self.bg.OnMouseLeftButtonDown = self.ExpandMe
		self.bg.Show()
		self.act = ui.TextLine()
		self.act.SetParent(self.bg)
		self.act.SetPosition(4,2)
		self.act.SetText(tt)
		self.act.Show()
		self.GetText = self.act.GetText
		
		self.Drop = ui.Bar("TOP_MOST")
		self.Drop.SetParent(self.GetParentProxy())
		self.Drop.SetPosition(0,21)
		# self.Drop.SetSize(150,95)
		self.Drop.SetSize(150,0)
		# self.Drop.SetColor(0xc00a0a0a)
		self.Drop.SetColor(0xff0a0a0a)
		
		
		self.ScrollBar = ui.ThinScrollBar()
		self.ScrollBar.SetParent(self.Drop)
		self.ScrollBar.SetPosition(132,0)
		# self.ScrollBar.SetScrollBarSize(95)
		self.ScrollBar.SetScrollBarSize(0)
		# self.ScrollBar.Show()
		
		self.DropList = ui.ListBoxEx()
		self.DropList.SetParent(self.Drop)
		self.DropList.itemHeight = 12
		self.DropList.itemStep = 13
		self.DropList.SetPosition(0,0)
		# self.DropList.SetSize(132,self.maxh)
		self.DropList.SetSize(132,13) 
		self.DropList.SetScrollBar(self.ScrollBar)
		self.DropList.SetSelectEvent(self.SetTitle)
		self.DropList.SetViewItemCount(0)
		self.DropList.Show()
		if tt != "":
			self.AppendItemAndSelect(tt)
		self.selected = self.DropList.GetSelectedItem()
		
			
		self.SetSize(120,20)
	def __del__(self): 
		ui.Window.__del__(self)
	c = 1
	def AppendItem(self,text,value=0):
		self.c+=1   
		self.DropList.AppendItem(self.Item(self,text,value))
		self.maxh = min(95,13*self.c)
		if self.c > 7:
			self.ScrollBar.Show()
			
		
	def AppendItemAndSelect(self,text,value=0):
		self.DropList.AppendItem(self.Item(self,text,value))
		self.DropList.SelectIndex(len(self.DropList.itemList)-1)
		
	def SelectByAffectId(self,id):
		for x in self.DropList.itemList:
			if x.value == id:
				self.DropList.SelectItem(x)
				break
				
	def SetTitle(self,item):
		self.act.SetText(str(item.textBox.GetText()))
		self.last = self.DropList.basePos
		self.lastS = self.ScrollBar.GetPos()
		self.dropped = 0
		self.selected = item
		if self.OnChange:
			self.OnChange()
		# self.Drop.Hide()
		
	def SetPosition(self,w,h):
		ui.Window.SetPosition(self,w,h)
		if self.down == 1:
			self.Drop.SetPosition(w,h+21)
		else:
			self.Drop.SetPosition(w,h-self.Drop.GetHeight())
		
	def SetSize(self,w,h):
		ui.Window.SetSize(self,w,h)
		self.bg.SetSize(w,h)
		self.Drop.SetSize(w,0)
		self.DropList.SetSize(w-18,self.maxh)
		for x in self.DropList.itemList:
			x.SetSize(w-18,12)
		self.ScrollBar.SetPosition(w-18,0)
		
		
	def ExpandMe(self):
		if self.dropped == 1:
			# self.Drop.Hide()
			self.dropped = 0
		else:
			# self.Drop.Show()
			self.dropped = 1
			
	def OnUpdate(self):
		iter = 6
		if self.Drop.GetHeight() < 50:
			self.ScrollBar.Hide()
		else:
			self.ScrollBar.Show()
			
		if self.dropped == 0 and self.dropstat == 1:
			if self.Drop.GetHeight() <=0:
				self.dropstat = 0
				self.Drop.SetSize(self.Drop.GetWidth(),0)
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
				self.Drop.Hide()
			else:
				if self.Drop.GetHeight()-iter < 0:
					self.Drop.SetSize(self.Drop.GetWidth(),0)
				else:
					self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()-iter)
					(w,h) = self.GetLocalPosition()
					self.SetPosition(w,h)
						
					
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)
		elif self.dropped == 1 and self.dropstat == 0:
			self.Drop.Show()
			self.SetTop()
			if self.Drop.GetHeight() >=self.maxh:
				self.Drop.SetSize(self.Drop.GetWidth(),self.maxh)
				self.ScrollBar.SetScrollBarSize(self.maxh)
				self.dropstat = 1
				self.DropList.SetViewItemCount(7)
				self.ScrollBar.SetPos(self.lastS)
			else:
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight()+iter)
				self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()+iter)
				(w,h) = self.GetLocalPosition()
				self.SetPosition(w,h)
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)
		
	## BG Hover
	def bgMouseIn(self):
		self.bg.SetColor(0xc00a0a0a)
	def bgMouseOut(self):
		self.bg.SetColor(0xc0000000)
		

class Edit2(ui.EditLine):
	def __init__(self,main = "",ml = 99):
		ui.EditLine.__init__(self)
		self.SetText(main)
		self.main = main
		self.SetMax(ml)
		self.SetUserMax(ml)
	def GetText(self):
		res = ui.EditLine.GetText(self)
		if res == "":
			return "0"
		else:
			return res
			
	def __del__(self):
		ui.EditLine.__del__(self)
	def OnSetFocus(self):
		ui.EditLine.OnSetFocus(self)
		if ui.EditLine.GetText(self) == self.main:
			self.SetText("")
	def OnKillFocus(self):
		ui.EditLine.OnKillFocus(self)
		if ui.EditLine.GetText(self) == "":
			self.SetText(self.main)
			

class CheckBox(ui.Window):
	checked = 0
	eventUp  =None
	eventDown=None
	def __init__(self,cont = ""):
		ui.Window.__init__(self)
		self.BG = ui.Bar("UI")
		self.BG.SetParent(self)
		self.BG.SetPosition(0,0)
		self.BG.SetSize(20,20)
		# self.BG.SetColor(0xc00b0b0b)
		self.BG.SetColor(COLOR_CHECKBOX_NOT_SELECTED)
		self.BG.OnMouseLeftButtonUp = self.Toggle
		self.OnMouseLeftButtonUp = self.Toggle
		self.BG.Show()
		self.Title = ui.TextLine()
		self.Title.SetParent(self)
		self.Title.SetPosition(25,2)
		self.Title.SetText(cont)
		self.Title.Show()
		self.stat = ui.TextLine()
		self.stat.SetParent(self.BG)
		self.stat.SetPosition(0,0)
		self.stat.SetWindowHorizontalAlignCenter()
		self.stat.SetWindowVerticalAlignCenter()
		self.stat.SetHorizontalAlignCenter()
		self.stat.SetVerticalAlignCenter()
		self.stat.SetSize(0,0)
		self.stat.SetText("")
		self.SetSize(25+self.Title.GetTextSize()[0]+5,20)
		self.stat.Show()
	def __del__(self):
		ui.ToggleButton.__del__(self)
	def Toggle(self):
		if self.checked == 1:
			self.OnToggleUp()
		else:
			self.OnToggleDown()
	def OnToggleUp(self):
		self.stat.SetText("")
		# self.BG.SetColor(0xc00b0b0b)
		self.BG.SetColor(COLOR_CHECKBOX_NOT_SELECTED)
		self.checked = 0
		if self.eventUp:
			self.eventUp()
	def OnToggleDown(self):
		# self.BG.SetColor(0xf00b0b0b)
		self.BG.SetColor(COLOR_CHECKBOX_SELECTED) 
		self.stat.SetText("X")
		self.checked = 1
		if self.eventDown:
			self.eventDown()

			
# Zum Testen
# a = Bot()
# a.Show()

# oder;
# Bot().Show()
