import app
import player
import net
import item
import changelook
import sash
import chat
import background
import exchange
import ime
import event
import safebox
import shop
import systemSetting
import aura
import ui
import mouseModule
import snd
import uiScriptLocale
import uiRefine
import uiAttachMetin
import uiPickMoney
import uiCommon
import uiPrivateShopBuilder # ∞≥¿ŒªÛ¡° ø≠µøæ» ItemMove πÊ¡ˆ
import localeInfo
import constInfo
import time
import wndMgr
import interfaceModule
import uiPrivateShopBuilder
import dbg
import uiToolTip
import grp
import uinpcekran
import translate
from switchbot import Bot
	
ITEM_MALL_BUTTON_ENABLE = True
ITEM_FLAG_APPLICABLE = 1 << 14
SYSTEMS_WINDOW=1

if app.ENABLE_EXTEND_INVENTORY_SYSTEM:
	EX_INVEN_COVER_IMG_CLOSE = "d:/ymir work/ui/game/windows/ex_inven_cover_button_close.sub"
	EX_INVEN_COVER_IMG_OPEN = "d:/ymir work/ui/game/windows/ex_inven_cover_button_open.sub"
	
class SystemsWindow(ui.ScriptWindow):
	def __init__(self, wndInventory, tooltip):
		import exception

		if not wndInventory:
			return

		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.selectedSlotPos=0
		self.wndInventory = wndInventory
		self.rangeshop = uinpcekran.NPCEkran()
		self.rangeshop.Hide()
		self.switchbots = Bot()
		self.switchbots.Hide()

		self.toolTip = tooltip
		self.expandBtn = None
		self.minBtn = None
		self.interface = None
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetTooltip(self, tooltip):
		self.toolTip = tooltip

	def Show(self, gelen):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)

		if gelen == 1:
			constInfo.SYSTEMS_WINDOW_OPEN=1
			self.OpenInventory()
		else:
			constInfo.SYSTEMS_WINDOW_OPEN=0
			self.CloseInventory()

	def Close(self):
		constInfo.SYSTEMS_WINDOW_OPEN=0
		self.Hide()

	def Kapat(self):
		constInfo.SYSTEMS_WINDOW_OPEN=0
		self.Hide()

	def IsOpeningInventory(self):
		return self.wndSystemsWindowLayer.IsShow()

	def OpenInventory(self):
		global SYSTEMS_WINDOW
		self.wndSystemsWindowLayer.Show()
		SYSTEMS_WINDOW = 1

		self.AdjustPositionAndSize()

	def CloseInventory(self):
		global SYSTEMS_WINDOW
		self.wndSystemsWindowLayer.Hide()
		SYSTEMS_WINDOW = 0
		
		self.AdjustPositionAndSize()

	def GetBasePosition(self):
		x, y = self.wndInventory.GetGlobalPosition()
		return x - 148 + 90, y + 60

	def AdjustPositionAndSize(self):
		bx, by = self.GetBasePosition()
		byEK=37
		if self.IsOpeningInventory():			
			self.SetPosition(bx+6, by+6-byEK)
			self.SetSize(self.ORIGINAL_WIDTH, self.GetHeight())
		else:
			self.SetPosition(bx + 138 - 90, by+6-byEK);
			self.SetSize(10, self.GetHeight())

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/SystemsWindow.py")
		except:
			import exception
			exception.Abort("SystemsWindow.LoadWindow.LoadObject")

		try:
			self.ORIGINAL_WIDTH = self.GetWidth()
			self.wndSystemsWindowLayer = self.GetChild("SystemsWindowLayer")
			#self.expandBtn = self.GetChild("ExpandBtn")
			#self.minBtn = self.GetChild("MinimizeBtn")
			
			#self.expandBtn.SetEvent(ui.__mem_func__(self.OpenInventory))
			#self.minBtn.SetEvent(ui.__mem_func__(self.CloseInventory))
			
			self.shop_buttons = self.GetChild("shop_button")
			self.shop_buttons.SetEvent(ui.__mem_func__(self.__shop_button))
			self.shop_buttons.ShowToolTip = lambda arg=1: self.__OverInButtonMenu(arg)
			self.shop_buttons.HideToolTip = lambda arg=1: self.__OverOutButtonMenu()
			
			self.ek_envanters = self.GetChild("ek_envanter")
			self.ek_envanters.SetEvent(ui.__mem_func__(self.__ek_envanter))
			self.ek_envanters.ShowToolTip = lambda arg=2: self.__OverInButtonMenu(arg)
			self.ek_envanters.HideToolTip = lambda arg=2: self.__OverOutButtonMenu()
			
			self.uzak_markets = self.GetChild("uzak_market")
			self.uzak_markets.SetEvent(ui.__mem_func__(self.__uzak_market))
			self.uzak_markets.ShowToolTip = lambda arg=3: self.__OverInButtonMenu(arg)
			self.uzak_markets.HideToolTip = lambda arg=3: self.__OverOutButtonMenu()
			
			self.switchbot = self.GetChild("bgm_button")
			self.switchbot.SetEvent(ui.__mem_func__(self.__ClickSecimPenceresi))
			self.switchbot.ShowToolTip = lambda arg=4: self.__OverInButtonMenu(arg)
			self.switchbot.HideToolTip = lambda arg=4: self.__OverOutButtonMenu()
			
			self.switchbot2 = self.GetChild("bgm_button2")
			self.switchbot2.SetEvent(ui.__mem_func__(self.__switch_bot2))
			self.switchbot2.ShowToolTip = lambda arg=5: self.__OverInButtonMenu(arg)
			self.switchbot2.HideToolTip = lambda arg=5: self.__OverOutButtonMenu()
			
			self.lonca_siralama = self.GetChild("lonca_siralama")
			self.lonca_siralama.SetEvent(ui.__mem_func__(self.__lonca_siralama))
			self.lonca_siralama.ShowToolTip = lambda arg=6: self.__OverInButtonMenu(arg)
			self.lonca_siralama.HideToolTip = lambda arg=6: self.__OverOutButtonMenu()
			
		except:
			import exception
			exception.Abort("SystemsWindow.LoadWindow.BindObject")

	def __shop_button(self):
		net.SendChatPacket("/shop_open")

	def __ek_envanter(self):
		self.interface.ToggleExtendedInventoryWindow()
		
	def __uzak_market(self):
		if self.rangeshop.IsShow():
			self.rangeshop.Hide()
		else:
			self.rangeshop.Show()
			
	def __ShowMenuToolTip(self, statDesc):
		self.toolTip.ClearToolTip()
		self.toolTip.AppendTextLine(statDesc)
		self.toolTip.Show()
			
	def __OverInButtonMenu(self, num):	
		try:
			self.__ShowMenuToolTip(translate.SystemMenuText[num])
		except KeyError:
			pass
			
	def __OverOutButtonMenu(self):
		self.toolTip.Hide()
			
	def __ClickSecimPenceresi(self):
		self.EfsunBotDialog2 = uiCommon.QuestionDialog()
		self.EfsunBotDialog2.SetText("Hangi efsun botunu kullanmak istiyorsunuz?")
		self.EfsunBotDialog2.SetAcceptText("Normal")
		self.EfsunBotDialog2.SetCancelText("Ye˛il")
		self.EfsunBotDialog2.SetAcceptEvent(lambda arg=TRUE: self.NewOnEfsunBotDialog(1))
		self.EfsunBotDialog2.SetCancelEvent(lambda arg=FALSE: self.NewOnEfsunBotDialog(2))
		self.EfsunBotDialog2.Open()
			
	def NewOnEfsunBotDialog(self,gelen):
		self.EfsunBotDialog2.Close()
		if gelen==1:
			if not constInfo.eigenmod:
				execfile('eigenmods.py',{})
				constInfo.eigenmod = 1
		else:
			if self.switchbots.is_open == 1:
				self.switchbots.Hide()	
			else:
				self.switchbots.Show()
			
	def __switch_bot2(self):
		if self.interface:
			self.interface.OpenPrivateShopSearch(1)
			
	def __lonca_siralama(self):
		net.SendChatPacket("/get_guild_rank_list")
		
	def BindInterface(self, interface):
		self.interface = interface

	def OnUpdate(self):
		global SYSTEMS_WINDOW
		self.AdjustPositionAndSize()
			
		if constInfo.SYSTEMS_WINDOW_KAPAT==1:
			self.Close()
			constInfo.SYSTEMS_WINDOW_KAPAT=0

class CostumeWindow(ui.ScriptWindow):

	def __init__(self, wndInventory):
		import exception
		
		if not app.ENABLE_COSTUME_SYSTEM:			
			exception.Abort("What do you do?")
			return

		if not wndInventory:
			exception.Abort("wndInventory parameter must be set to InventoryWindow")
			return						
			 	 
		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.wndInventory = wndInventory;

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()
		self.RefreshCostumeSlot()

		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/CostumeWindow.py")
		except:
			import exception
			exception.Abort("CostumeWindow.LoadWindow.LoadObject")

		try:
			wndEquip = self.GetChild("CostumeSlot")
			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
			
		except:
			import exception
			exception.Abort("CostumeWindow.LoadWindow.BindObject")

		## Equipment
		wndEquip.SetOverInItemEvent(ui.__mem_func__(self.wndInventory.OverInItem))
		wndEquip.SetOverOutItemEvent(ui.__mem_func__(self.wndInventory.OverOutItem))
		wndEquip.SetUnselectItemSlotEvent(ui.__mem_func__(self.wndInventory.UseItemSlot))
		wndEquip.SetUseSlotEvent(ui.__mem_func__(self.wndInventory.UseItemSlot))						
		wndEquip.SetSelectEmptySlotEvent(ui.__mem_func__(self.wndInventory.SelectEmptySlot))
		wndEquip.SetSelectItemSlotEvent(ui.__mem_func__(self.wndInventory.SelectItemSlot))

		self.wndEquip = wndEquip

	def RefreshCostumeSlot(self):
		getItemVNum=player.GetItemIndex
		
		for i in xrange(item.COSTUME_SLOT_COUNT):
			slotNumber = item.COSTUME_SLOT_START + i
			self.wndEquip.SetItemSlot(slotNumber, getItemVNum(slotNumber), 0)
			
			if app.ENABLE_CHANGELOOK_SYSTEM:
				itemTransmutedVnum = player.GetItemTransmutation(slotNumber)
				if itemTransmutedVnum:
					self.wndEquip.DisableCoverButton(slotNumber)
				else:
					self.wndEquip.EnableCoverButton(slotNumber)
			
		for i in xrange(2):
			slotNumber = player.NEW_EQUIPMENT_SLOT_START + i
			self.wndEquip.SetItemSlot(slotNumber, getItemVNum(slotNumber), 0)
		
		self.wndEquip.RefreshSlot()

class BeltInventoryWindow(ui.ScriptWindow):

	def __init__(self, wndInventory):
		import exception

		if not app.ENABLE_NEW_EQUIPMENT_SYSTEM:
			exception.Abort("What do you do?")
			return

		if not wndInventory:
			exception.Abort("wndInventory parameter must be set to InventoryWindow")
			return

		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.wndInventory = wndInventory;

		self.wndBeltInventoryLayer = None
		self.wndBeltInventorySlot = None
		self.expandBtn = None
		self.minBtn = None

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self, openBeltSlot = False):
		self.__LoadWindow()
		self.RefreshSlot()

		ui.ScriptWindow.Show(self)

		if openBeltSlot:
			self.OpenInventory()
		else:
			self.CloseInventory()

	def Close(self):
		self.Hide()

	def IsOpeningInventory(self):
		return self.wndBeltInventoryLayer.IsShow()

	def OpenInventory(self):
		self.wndBeltInventoryLayer.Show()
		self.expandBtn.Hide()

		self.AdjustPositionAndSize()
		
	def CloseInventory(self):
		self.wndBeltInventoryLayer.Hide()
		self.expandBtn.Show()
		
		self.AdjustPositionAndSize()

	## «ˆ¿Á ¿Œ∫•≈‰∏Æ ¿ßƒ°∏¶ ±‚¡ÿ¿∏∑Œ BASE ¿ßƒ°∏¶ ∞ËªÍ, ∏Æ≈œ.. º˝¿⁄ «œµÂƒ⁄µ˘«œ±‚ ¡§∏ª Ω»¡ˆ∏∏ πÊπ˝¿Ã æ¯¥Ÿ..
	def GetBasePosition(self):
		x, y = self.wndInventory.GetGlobalPosition()
		return x - 148, y + 241

	def AdjustPositionAndSize(self):
		bx, by = self.GetBasePosition()

		if self.IsOpeningInventory():
			self.SetPosition(bx, by)
			self.SetSize(self.ORIGINAL_WIDTH, self.GetHeight())

		else:
			self.SetPosition(bx + 138, by);
			self.SetSize(10, self.GetHeight())

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/BeltInventoryWindow.py")
		except:
			import exception
			exception.Abort("CostumeWindow.LoadWindow.LoadObject")

		try:
			self.ORIGINAL_WIDTH = self.GetWidth()
			wndBeltInventorySlot = self.GetChild("BeltInventorySlot")
			self.wndBeltInventoryLayer = self.GetChild("BeltInventoryLayer")
			self.expandBtn = self.GetChild("ExpandBtn")
			self.minBtn = self.GetChild("MinimizeBtn")

			self.expandBtn.SetEvent(ui.__mem_func__(self.OpenInventory))
			self.minBtn.SetEvent(ui.__mem_func__(self.CloseInventory))
			
			for i in xrange(item.BELT_INVENTORY_SLOT_COUNT):
				slotNumber = item.BELT_INVENTORY_SLOT_START + i
				wndBeltInventorySlot.SetCoverButton(slotNumber,	"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/belt_inventory/slot_disabled.tga", False, False)

		except:
			import exception
			exception.Abort("CostumeWindow.LoadWindow.BindObject")

		## Equipment
		wndBeltInventorySlot.SetOverInItemEvent(ui.__mem_func__(self.wndInventory.OverInItem))
		wndBeltInventorySlot.SetOverOutItemEvent(ui.__mem_func__(self.wndInventory.OverOutItem))
		wndBeltInventorySlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.wndInventory.UseItemSlot))
		wndBeltInventorySlot.SetUseSlotEvent(ui.__mem_func__(self.wndInventory.UseItemSlot))
		wndBeltInventorySlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.wndInventory.SelectEmptySlot))
		wndBeltInventorySlot.SetSelectItemSlotEvent(ui.__mem_func__(self.wndInventory.SelectItemSlot))

		self.wndBeltInventorySlot = wndBeltInventorySlot

	def RefreshSlot(self):
		getItemVNum=player.GetItemIndex

		for i in xrange(item.BELT_INVENTORY_SLOT_COUNT):
			slotNumber = item.BELT_INVENTORY_SLOT_START + i
			self.wndBeltInventorySlot.SetItemSlot(slotNumber, getItemVNum(slotNumber), player.GetItemCount(slotNumber))
			self.wndBeltInventorySlot.SetAlwaysRenderCoverButton(slotNumber, True)

			avail = "0"

			if player.IsAvailableBeltInventoryCell(slotNumber):
				self.wndBeltInventorySlot.EnableCoverButton(slotNumber)
			else:
				self.wndBeltInventorySlot.SetCoverButton(slotNumber,	"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/quest/slot_button_01.sub",\
												"d:/ymir work/ui/game/belt_inventory/slot_disabled.tga", False, False)
				self.wndBeltInventorySlot.DisableCoverButton(slotNumber)

		self.wndBeltInventorySlot.RefreshSlot()

if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
	class ExtendedInventoryWindow(ui.ScriptWindow):
		tooltipItem = None
		tooltipInfo = None
		interface = None
		dlgPickMoney = None
		sellingSlotNumber = -1
		isLoaded = 0
		
		BOOK_TYPE = 0
		UPGRADE_TYPE = 1
		STONE_TYPE = 2
		CHEST_TYPE = 3
		ATTR_TYPE = 4
		FLOWER_TYPE = 5
		
		SLOT_WINDOW_TYPE = {
			BOOK_TYPE	:	{"window" : player.SKILL_BOOK_INVENTORY, "slot" : player.SLOT_TYPE_SKILL_BOOK_INVENTORY},
			UPGRADE_TYPE	:	{"window" : player.UPGRADE_ITEMS_INVENTORY, "slot" : player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY},
			STONE_TYPE	:	{"window" : player.STONE_ITEMS_INVENTORY, "slot" : player.SLOT_TYPE_STONE_INVENTORY},
			CHEST_TYPE	:	{"window" : player.CHEST_ITEMS_INVENTORY, "slot" : player.SLOT_TYPE_CHEST_INVENTORY},
			ATTR_TYPE	:	{"window" : player.ATTR_ITEMS_INVENTORY, "slot" : player.SLOT_TYPE_ATTR_INVENTORY},
			FLOWER_TYPE	:	{"window" : player.FLOWER_ITEMS_INVENTORY, "slot" : player.SLOT_TYPE_FLOWER_INVENTORY}
		}
		
		WINDOW_NAMES = {
			BOOK_TYPE	:	localeInfo.EXTEND_INVENTORY_SKILL_BOOK_TOOLTIP,
			UPGRADE_TYPE	:	localeInfo.EXTEND_INVENTORY_UPGRADE_ITEM_TOOLTIP,
			STONE_TYPE	:	localeInfo.EXTEND_INVENTORY_STONE_TOOLTIP,
			CHEST_TYPE	:	localeInfo.EXTEND_INVENTORY_CHEST_TOOLTIP,
			ATTR_TYPE	:	localeInfo.EXTEND_INVENTORY_ATTR_TOOLTIP,
			FLOWER_TYPE	:	localeInfo.EXTEND_INVENTORY_FLOWER_TOOLTIP
		}
		
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.inventoryPageIndex = 0
			self.categoryPageIndex = 0
			self.__LoadWindow()
			
		def __del__(self):
			ui.ScriptWindow.__del__(self)

		def Show(self):
			self.__LoadWindow()
			ui.ScriptWindow.Show(self)	
			
		def BindInterfaceClass(self, interface):
			self.interface = interface

		def __LoadWindow(self):
			if self.isLoaded == 1:
				return

			self.isLoaded = 1		
			
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "UIScript/ExtendedInventoryWindow.py")
			except:
				import exception
				exception.Abort("ExtendedInventoryWindow.LoadWindow.LoadObject")	

			try:
				wndItem = self.GetChild("ItemSlot")
				self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
				self.titleName = self.GetChild("TitleName")
				self.inventoryTab = []
				self.inventoryTab.append(self.GetChild("Inventory_Tab_01"))
				self.inventoryTab.append(self.GetChild("Inventory_Tab_02"))
				self.inventoryTab.append(self.GetChild("Inventory_Tab_03"))
				self.inventoryTab.append(self.GetChild("Inventory_Tab_04"))
				self.inventoryTab.append(self.GetChild("Inventory_Tab_05"))
				self.ExtendInventoryToolTIpButton = self.GetChild("ExtendInventoryToolTIpButton")
				self.categoryTab = []
				self.categoryTab.append(self.GetChild("SkillBookButton"))
				self.categoryTab.append(self.GetChild("UpgradeItemsButton"))
				self.categoryTab.append(self.GetChild("StoneButton"))
				self.categoryTab.append(self.GetChild("ChestButton"))
				self.categoryTab.append(self.GetChild("AttrButton"))
				self.categoryTab.append(self.GetChild("FlowerButton"))
				self.inventoryopen = []
				self.inventoryopen.append(self.GetChild("inventory_open_on"))
				self.inventoryopen.append(self.GetChild("inventory_open_of"))
			except:
				import exception
				exception.Abort("ExtendedInventoryWindow.LoadWindow.BindObject")		

			## Item
			wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
			wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
			wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
			wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
			wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))	
			
			self.categoryTab[0].SetEvent(lambda arg=0: self.SetCategoryPage(arg))
			self.categoryTab[1].SetEvent(lambda arg=1: self.SetCategoryPage(arg))
			self.categoryTab[2].SetEvent(lambda arg=2: self.SetCategoryPage(arg))
			self.categoryTab[3].SetEvent(lambda arg=3: self.SetCategoryPage(arg))
			self.categoryTab[4].SetEvent(lambda arg=4: self.SetCategoryPage(arg))
			self.categoryTab[5].SetEvent(lambda arg=5: self.SetCategoryPage(arg))
			self.categoryTab[0].Down()

			self.inventoryTab[0].SetEvent(lambda arg=0: self.SetInventoryPage(arg))
			self.inventoryTab[1].SetEvent(lambda arg=1: self.SetInventoryPage(arg))
			self.inventoryTab[2].SetEvent(lambda arg=2: self.SetInventoryPage(arg))
			self.inventoryTab[3].SetEvent(lambda arg=3: self.SetInventoryPage(arg))
			self.inventoryTab[4].SetEvent(lambda arg=4: self.SetInventoryPage(arg))
			self.inventoryTab[0].Down()
			
			self.inventoryopen[0].SAFE_SetEvent(self.OpenInventoryK_On)
			self.inventoryopen[1].SAFE_SetEvent(self.OpenInventoryK_Of)	
			
			## PickMoneyDialog
			self.dlgSplitItems = uiPickMoney.PickMoneyDialog2()
			self.dlgSplitItems.LoadDialog()
			self.dlgSplitItems.Hide()
			
			if app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
				self.listHighlightedSlotBK = []
				self.listHighlightedSlotUP = []
				self.listHighlightedSlotSTONE = []
				self.listHighlightedSlotCHEST = []
				self.listHighlightedSlotATTR = []
				self.listHighlightedSlotFLOWER = []
				
			self.tlInfo = uiToolTip.ItemToolTip()
			self.tlInfo.Hide()
			self.tooltipInfo = [self.tlInfo]*7
			self.InformationText = [localeInfo.EXTEND_INVENTORY_TOOLTIP_TITLE,
									localeInfo.EXTEND_INVENTORY_TOOLTIP_LINE1,
									localeInfo.EXTEND_INVENTORY_TOOLTIP_LINE2,
									localeInfo.EXTEND_INVENTORY_TOOLTIP_LINE3,
									localeInfo.EXTEND_INVENTORY_TOOLTIP_LINE4,
									localeInfo.EXTEND_INVENTORY_TOOLTIP_LINE5,
									localeInfo.EXTEND_INVENTORY_TOOLTIP_LINE6
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
			
			self.wndItem = wndItem
			self.SetInventoryPage(0)
			self.SetCategoryPage(0)
			self.RefreshItemSlot()
			self.RefreshBagSlotWindow()
			self.KInventoryOpen_Refresh()
			
		def Destroy(self):
			self.ClearDictionary()
			self.dlgSplitItems.Destroy()
			self.dlgSplitItems = 0
			self.tooltipItem = None
			self.wndItem = 0
			self.interface = None

			self.inventoryTab = []
			self.tlInfo.Hide()
			self.inventoryopen = []
			
		def Hide(self):
			if None != self.tooltipItem:
				self.tooltipItem.HideToolTip()
			for i in xrange(7):
				if self.tooltipInfo != None:
					self.tooltipInfo[i].Hide()	
			wndMgr.Hide(self.hWnd)
			
		def Close(self):
			self.Hide()
			
		def OpenInventoryK_On(self):
			constInfo.KInventorymode = 1
			self.inventoryopen[0].Down()
			self.inventoryopen[1].SetUp()
			
		def OpenInventoryK_Of(self):
			constInfo.KInventorymode = 0
			self.inventoryopen[1].Down()
			self.inventoryopen[0].SetUp()
			
		def KInventoryOpen_Refresh(self):
			if constInfo.KInventorymode == 1:
				self.inventoryopen[0].Down()
				self.inventoryopen[1].SetUp()
			else:
				self.inventoryopen[0].SetUp()
				self.inventoryopen[1].Down()
			
		if app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
			def HighlightSlot(self, slot, inven_type):
				if inven_type == player.SKILL_BOOK_INVENTORY:
					if not slot in self.listHighlightedSlotBK:
						self.listHighlightedSlotBK.append(slot)
				elif inven_type == player.UPGRADE_ITEMS_INVENTORY:
					if not slot in self.listHighlightedSlotUP:
						self.listHighlightedSlotUP.append(slot)
				elif inven_type == player.STONE_ITEMS_INVENTORY:
					if not slot in self.listHighlightedSlotSTONE:
						self.listHighlightedSlotSTONE.append(slot)
				elif inven_type == player.CHEST_ITEMS_INVENTORY:
					if not slot in self.listHighlightedSlotCHEST:
						self.listHighlightedSlotCHEST.append(slot)
				elif inven_type == player.ATTR_ITEMS_INVENTORY:
					if not slot in self.listHighlightedSlotATTR:
						self.listHighlightedSlotATTR.append(slot)
				elif inven_type == player.FLOWER_ITEMS_INVENTORY:
					if not slot in self.listHighlightedSlotFLOWER:
						self.listHighlightedSlotFLOWER.append(slot)
		
		def SetCategoryPage(self, page):			
			self.categoryTab[self.categoryPageIndex].SetUp()
			self.categoryPageIndex = page
			self.categoryTab[self.categoryPageIndex].Down()
			
			self.titleName.SetText(self.WINDOW_NAMES[self.categoryPageIndex])
			self.RefreshBagSlotWindow()
		
		def SetInventoryPage(self, page):
			self.inventoryPageIndex = page
			for i in range(0,len(self.inventoryTab)):
				self.inventoryTab[i].SetUp()
			self.inventoryTab[page].Down()
			self.RefreshBagSlotWindow()

		def OnPickItem(self, count):
			itemSlotIndex = self.dlgSplitItems.itemGlobalSlotIndex
			selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
			mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, count)
				
		def __InventoryLocalSlotPosToGlobalSlotPos(self, localSlotPos):
			return self.inventoryPageIndex * 45 + localSlotPos
				
		def GetInventoryPageIndex(self):
			return self.inventoryPageIndex
			
		def RefreshBagSlotWindow(self):
			getItemVNum=player.GetItemIndex
			getItemCount=player.GetItemCount
			setItemVNum=self.wndItem.SetItemSlot
			
			for i in xrange(45):
				#self.wndItem.EnableSlot(i)
				slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)

				itemCount = getItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
				itemVnum = getItemVNum(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotNumber)
				
				if 0 == itemCount:
					self.wndItem.ClearSlot(i)
					continue
				elif 1 == itemCount:
					itemCount = 0
					
				setItemVNum(i, itemVnum, itemCount)
				if self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.SKILL_BOOK_INVENTORY:
					if slotNumber in self.listHighlightedSlotBK:
						if systemSetting.IsItemHighlight() == True:
							self.wndItem.ActivateSlot(slotNumber)
				elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.UPGRADE_ITEMS_INVENTORY:
					if slotNumber in self.listHighlightedSlotUP:
						if systemSetting.IsItemHighlight() == True:
							self.wndItem.ActivateSlot(slotNumber)
				elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.STONE_ITEMS_INVENTORY:
					if slotNumber in self.listHighlightedSlotSTONE:
						if systemSetting.IsItemHighlight() == True:
							self.wndItem.ActivateSlot(slotNumber)
				elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.CHEST_ITEMS_INVENTORY:
					if slotNumber in self.listHighlightedSlotCHEST:
						if systemSetting.IsItemHighlight() == True:
							self.wndItem.ActivateSlot(slotNumber)
				elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.ATTR_ITEMS_INVENTORY:
					if slotNumber in self.listHighlightedSlotATTR:
						if systemSetting.IsItemHighlight() == True:
							self.wndItem.ActivateSlot(slotNumber)
				elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.FLOWER_ITEMS_INVENTORY:
					if slotNumber in self.listHighlightedSlotFLOWER:
						if systemSetting.IsItemHighlight() == True:
							self.wndItem.ActivateSlot(slotNumber)
			self.wndItem.RefreshSlot()
			
		def RefreshItemSlot(self):
			self.RefreshBagSlotWindow()
			
		def RefreshStatus(self):
			pass
			
		def SetItemToolTip(self, tooltipItem):
			self.tooltipItem = tooltipItem
			
		def SelectEmptySlot(self, selectedSlotPos):
			if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
				return
			
			selectedSlotPos = self.__InventoryLocalSlotPosToGlobalSlotPos(selectedSlotPos)
			
			if mouseModule.mouseController.isAttached():

				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
				attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
				attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)

				if player.SLOT_TYPE_SKILL_BOOK_INVENTORY == attachedSlotType:	
					itemCount = player.GetItemCount(player.SKILL_BOOK_INVENTORY, attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)
				elif player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY == attachedSlotType:
					itemCount = player.GetItemCount(player.UPGRADE_ITEMS_INVENTORY, attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)					
				elif player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType:
					itemCount = player.GetItemCount(player.STONE_ITEMS_INVENTORY, attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)
				elif player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType:
					itemCount = player.GetItemCount(player.CHEST_ITEMS_INVENTORY, attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)
				elif player.SLOT_TYPE_ATTR_INVENTORY == attachedSlotType:
					itemCount = player.GetItemCount(player.ATTR_ITEMS_INVENTORY, attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)
				elif player.SLOT_TYPE_FLOWER_INVENTORY == attachedSlotType:
					itemCount = player.GetItemCount(player.FLOWER_ITEMS_INVENTORY, attachedSlotPos)
					attachedCount = mouseModule.mouseController.GetAttachedItemCount()
					self.__SendMoveItemPacket(attachedInvenType, attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos, attachedCount)

					if item.IsRefineScroll(attachedItemIndex):
						self.wndItem.SetUseMode(False)
				elif player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
					mouseModule.mouseController.RunCallBack("INVENTORY")
				
				elif player.SLOT_TYPE_SHOP == attachedSlotType:
					net.SendShopBuyPacket(attachedSlotPos)

				elif player.SLOT_TYPE_SAFEBOX == attachedSlotType:
					net.SendSafeboxCheckoutPacket(attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos)
		
				elif player.SLOT_TYPE_MALL == attachedSlotType:
					net.SendMallCheckoutPacket(attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], selectedSlotPos)
					
				mouseModule.mouseController.DeattachObject()
				
		def SelectItemSlot(self, itemSlotIndex):
			if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
				return
							
			itemSlotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(itemSlotIndex)
			selectedItemVNum = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
			itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)		
			
			if mouseModule.mouseController.isAttached():
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
				attachedItemVID = mouseModule.mouseController.GetAttachedItemIndex()
				attachedItemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], attachedSlotPos)	
				
				if self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"] == attachedSlotType:
					if attachedItemVID == selectedItemVNum:
						net.SendItemMovePacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex, attachedItemCount)
					else:
						net.SendItemUseToItemPacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], attachedSlotPos, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
		
				mouseModule.mouseController.DeattachObject()
			else:

				curCursorNum = app.GetCursor()
				if app.SELL == curCursorNum:
					self.__SellItem(itemSlotIndex)
					
				elif app.BUY == curCursorNum:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)

				elif app.IsPressed(app.DIK_LALT):
					link = player.GetItemLink(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotIndex)
					ime.PasteString(link)

				elif app.IsPressed(app.DIK_LSHIFT):
					if itemCount > 1:
						self.dlgSplitItems.SetTitleName(localeInfo.PICK_ITEM_TITLE)
						self.dlgSplitItems.SetAcceptEvent(ui.__mem_func__(self.OnPickItem))
						self.dlgSplitItems.Open(itemCount)
						self.dlgSplitItems.itemGlobalSlotIndex = itemSlotIndex
				else:
					mouseModule.mouseController.AttachObject(self, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["slot"], itemSlotIndex, selectedItemVNum, itemCount)
					self.wndItem.SetUseMode(True)

					snd.PlaySound("sound/ui/pick.wav")
					
		def __SellItem(self, itemSlotPos):
			self.sellingSlotNumber = itemSlotPos
			itemIndex = player.GetItemIndex(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotPos)
			itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], itemSlotPos)			
			
			self.sellingSlotitemIndex = itemIndex
			self.sellingSlotitemCount = itemCount

			item.SelectItem(itemIndex)
			## æ»∆º «√∑π±◊ ∞ÀªÁ ∫¸¡Æº≠ √ﬂ∞°
			## 20140220
			if item.IsAntiFlag(item.ANTIFLAG_SELL):
				popup = uiCommon.PopupDialog()
				popup.SetText(localeInfo.SHOP_CANNOT_SELL_ITEM)
				popup.SetAcceptEvent(self.__OnClosePopupDialog)
				popup.Open()
				self.popup = popup
				return

			itemPrice = player.GetISellItemPrice(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"],itemSlotPos)

			if item.Is1GoldItem():
				itemPrice = itemCount / itemPrice / 5
			else:
				itemPrice = itemPrice * itemCount / 5

			item.GetItemName(itemIndex)
			itemName = item.GetItemName()

			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.DO_YOU_SELL_ITEM(itemName, itemCount, itemPrice))
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.Sat))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			self.questionDialog.Open()
			self.questionDialog.count = itemCount
		
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
			
		def Sat(self):
			if self.sellingSlotitemIndex == player.GetItemIndex(self.sellingSlotNumber):
				if self.sellingSlotitemCount == player.GetItemCount(self.sellingSlotNumber):
					net.SendShopSellPacketNew(self.sellingSlotNumber, self.questionDialog.count, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])
					snd.PlaySound("sound/ui/money.wav")
			self.OnCloseQuestionDialog()
			
		def __OnClosePopupDialog(self):
			self.pop = None
			
		def OverOutItem(self):
			self.wndItem.SetUsableItem(False)
			if None != self.tooltipItem:
				self.tooltipItem.HideToolTip()

		def OverInItem(self, overSlotPos):
			overSlotPosGlobal = self.__InventoryLocalSlotPosToGlobalSlotPos(overSlotPos)
			if self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.SKILL_BOOK_INVENTORY:
				if overSlotPosGlobal in self.listHighlightedSlotBK:
					self.listHighlightedSlotBK.remove(overSlotPosGlobal)
					self.wndItem.DeactivateSlot(overSlotPos)
			elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.UPGRADE_ITEMS_INVENTORY:
				if overSlotPosGlobal in self.listHighlightedSlotUP:
					self.listHighlightedSlotUP.remove(overSlotPosGlobal)
					self.wndItem.DeactivateSlot(overSlotPos)
			elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.STONE_ITEMS_INVENTORY:
				if overSlotPosGlobal in self.listHighlightedSlotSTONE:
					self.listHighlightedSlotSTONE.remove(overSlotPosGlobal)
					self.wndItem.DeactivateSlot(overSlotPos)
			elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.CHEST_ITEMS_INVENTORY:
				if overSlotPosGlobal in self.listHighlightedSlotCHEST:
					self.listHighlightedSlotCHEST.remove(overSlotPosGlobal)
					self.wndItem.DeactivateSlot(overSlotPos)
			elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.ATTR_ITEMS_INVENTORY:
				if overSlotPosGlobal in self.listHighlightedSlotATTR:
					self.listHighlightedSlotATTR.remove(overSlotPosGlobal)
					self.wndItem.DeactivateSlot(overSlotPos)
			elif self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.FLOWER_ITEMS_INVENTORY:
				if overSlotPosGlobal in self.listHighlightedSlotFLOWER:
					self.listHighlightedSlotFLOWER.remove(overSlotPosGlobal)
					self.wndItem.DeactivateSlot(overSlotPos)
			self.wndItem.SetUsableItem(False)
			self.ShowToolTip(overSlotPosGlobal)
			
		def ShowToolTip(self, slotIndex):
			if None != self.tooltipItem:
				self.tooltipItem.SetInventoryItem(slotIndex, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])
				
		def OnPressEscapeKey(self):
			self.Close()
			return True	
			
		def UseItemSlot(self, slotIndex):
			if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
				return
				
			slotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(slotIndex)
			itemCount = player.GetItemCount(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
			if app.ENABLE_ITEM_COMBINATION_SYSTEM:
				if self.isShowCombinationWindow():
					self.wndSkillBookCombination.AddItemWithoutMouse(slotIndex)
					return
					
			if app.IsPressed(app.DIK_LCONTROL):
				if app.IsPressed(app.DIK_LCONTROL) and app.IsPressed(app.DIK_X) and self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"] == player.STONE_ITEMS_INVENTORY:
					net.SendItemSellPacket(slotIndex, self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"])
					snd.PlaySound('sound/ui/money.wav')
			else:
				if item.GetItemType() == item.ITEM_TYPE_GIFTBOX or item.GetItemType() == item.ITEM_TYPE_GACHA:
					if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
						if self.interface:
							if self.interface.dlgChestDrop:
								if not self.interface.dlgChestDrop.IsShow():
									self.interface.dlgChestDrop.Open(slotIndex)
									net.SendChestDropInfo(slotIndex)
				else:
					net.SendItemUsePacket(self.SLOT_WINDOW_TYPE[self.categoryPageIndex]["window"], slotIndex)
					mouseModule.mouseController.DeattachObject()
					self.OverOutItem()
				

		def __UseItemQuestionDialog_OnCancel(self):
			self.OnCloseQuestionDialog()

		def __UseItemQuestionDialog_OnAccept(self):
			self.__SendUseItemPacket(self.questionDialog.slotIndex)
			self.OnCloseQuestionDialog()	
			
		def __SendMoveItemPacket(self, srcSlotWindow, srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount):
			if uiPrivateShopBuilder.IsBuildingPrivateShop():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
				return
				
			net.SendItemMovePacket(srcSlotWindow , srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount)
			
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			def SetCombinationWindow(self, wndItemCombination, wndSkillBookCombination):
				self.wndItemCombination = wndItemCombination
				self.wndSkillBookCombination = wndSkillBookCombination

			def isShowCombinationWindow(self):					
				if self.wndSkillBookCombination:
					if self.wndSkillBookCombination.IsShow():
						return 1

				return 0
				
		def OnUpdate(self):
			for i in xrange(7):
				if self.ExtendInventoryToolTIpButton.IsIn():
					self.tooltipInfo[i].Show()
				else:
					self.tooltipInfo[i].Hide()	
										
class InventoryWindow(ui.ScriptWindow):

	USE_TYPE_TUPLE = ("USE_CLEAN_SOCKET", "USE_CHANGE_ATTRIBUTE", "USE_ADD_ATTRIBUTE", "USE_ADD_ATTRIBUTE2", "USE_ADD_ACCESSORY_SOCKET", "USE_PUT_INTO_ACCESSORY_SOCKET", "USE_PUT_INTO_BELT_SOCKET", "USE_PUT_INTO_RING_SOCKET", "USE_CHANGE_COSTUME_ATTR", "USE_RESET_COSTUME_ATTR")
	questionDialog = None
	tooltipItem = None
	wndCostume = None
	wndBelt = None
	dlgPickMoney = None
	wndSystemsWindow = None
	
	interface = None
	if app.WJ_TRADABLE_ICON:
		bindWnds = []
	
	sellingSlotNumber = -1
	isLoaded = 0
	isOpenedCostumeWindowWhenClosingInventory = 0		# ¿Œ∫•≈‰∏Æ ¥›¿ª ∂ß ƒ⁄Ω∫√ı¿Ã ø≠∑¡¿÷æ˙¥¬¡ˆ ø©∫Œ-_-; ≥◊¿Ãπ÷ §∏§µ
	isOpenedBeltWindowWhenClosingInventory = 0		# ¿Œ∫•≈‰∏Æ ¥›¿ª ∂ß ∫ß∆Æ ¿Œ∫•≈‰∏Æ∞° ø≠∑¡¿÷æ˙¥¬¡ˆ ø©∫Œ-_-; ≥◊¿Ãπ÷ §∏§µ

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isOpenedBeltWindowWhenClosingInventory = 0		# ¿Œ∫•≈‰∏Æ ¥›¿ª ∂ß ∫ß∆Æ ¿Œ∫•≈‰∏Æ∞° ø≠∑¡¿÷æ˙¥¬¡ˆ ø©∫Œ-_-; ≥◊¿Ãπ÷ §∏§µ
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		global SYSTEMS_WINDOW
		self.__LoadWindow()
		ui.ScriptWindow.Show(self)
			
		# ¿Œ∫•≈‰∏Æ∏¶ ¥›¿ª ∂ß ƒ⁄Ω∫√ı¿Ã ø≠∑¡¿÷æ˙¥Ÿ∏È ¿Œ∫•≈‰∏Æ∏¶ ø≠ ∂ß ƒ⁄Ω∫√ıµµ ∞∞¿Ã ø≠µµ∑œ «‘.
		if self.isOpenedCostumeWindowWhenClosingInventory and self.wndCostume:
			self.wndCostume.Show() 

		# Belt System
		if self.wndBelt:
			self.wndBelt.Show(self.isOpenedBeltWindowWhenClosingInventory)
			
		constInfo.SYSTEMS_WINDOW_KAPAT=0
		if SYSTEMS_WINDOW == 0:
			self.wndSystemsWindow.Show(0)
			self.wndSystemsWindow.BindInterface(self.interface)
		else:
			self.wndSystemsWindow.Show(1)
			self.wndSystemsWindow.BindInterface(self.interface)
			
	def BindInterfaceClass(self, interface):
		self.interface = interface
		
	if app.WJ_TRADABLE_ICON:
		def BindWindow(self, wnd):
			self.bindWnds.append(wnd)

	def BindGameClass(self, game):
		self.game = game

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		tooltip = uiToolTip.ToolTip()
		self.toolTip = tooltip
		self.wndSystemsWindow = SystemsWindow(self, tooltip)
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/InventoryWindow.py")
		except:
			import exception
			exception.Abort("InventoryWindow.LoadWindow.LoadObject")

		try:
			wndItem = self.GetChild("ItemSlot")
			wndEquip = self.GetChild("EquipmentSlot")
			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
			self.DSSButton = self.GetChild2("DSSButton")
			self.costumeButton = self.GetChild2("CostumeButton")
			self.mallButton = self.GetChild2("MallButton")
			
			self.inventoryTab = []
			self.inventoryTab.append(self.GetChild("Inventory_Tab_01"))
			self.inventoryTab.append(self.GetChild("Inventory_Tab_02"))
			self.inventoryTab.append(self.GetChild("Inventory_Tab_03"))
			self.inventoryTab.append(self.GetChild("Inventory_Tab_04"))

			self.equipmentTab = []
			self.equipmentTab.append(self.GetChild("Equipment_Tab_01"))
			self.equipmentTab.append(self.GetChild("Equipment_Tab_02"))
			
			if self.costumeButton and not app.ENABLE_COSTUME_SYSTEM:
				self.costumeButton.Hide()
				self.costumeButton.Destroy()
				self.costumeButton = 0

			# Belt Inventory Window
			self.wndBelt = None
			
			if app.ENABLE_NEW_EQUIPMENT_SYSTEM:
				self.wndBelt = BeltInventoryWindow(self)
						
			if app.ENABLE_EXTEND_INVENTORY_SYSTEM:
				self.ExInvenButton = []
				
		except:
			import exception
			exception.Abort("InventoryWindow.LoadWindow.BindObject")
			
		## Item
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		## Equipment
		wndEquip.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndEquip.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndEquip.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndEquip.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndEquip.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndEquip.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		## PickMoneyDialog
		dlgPickMoney = uiPickMoney.PickMoneyDialog2()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.Hide()
		
		## RefineDialog
		self.refineDialog = uiRefine.RefineDialog()
		self.refineDialog.Hide()

		## AttachMetinDialog
		if app.WJ_TRADABLE_ICON:  
			self.attachMetinDialog = uiAttachMetin.AttachMetinDialog(self)
			self.BindWindow(self.attachMetinDialog)
		else:
			self.attachMetinDialog = uiAttachMetin.AttachMetinDialog()
		self.attachMetinDialog.Hide()
		
		self.inventoryTab[0].SetEvent(lambda arg=0: self.SetInventoryPage(arg))
		self.inventoryTab[1].SetEvent(lambda arg=1: self.SetInventoryPage(arg))
		self.inventoryTab[2].SetEvent(lambda arg=2: self.SetInventoryPage(arg))
		self.inventoryTab[3].SetEvent(lambda arg=3: self.SetInventoryPage(arg))
		self.inventoryPageIndex = 0

		self.equipmentTab[0].SetEvent(lambda arg=0: self.SetEquipmentPage(arg))
		self.equipmentTab[1].SetEvent(lambda arg=1: self.SetEquipmentPage(arg))
		self.equipmentTab[0].Down()
		self.equipmentTab[0].Hide()
		self.equipmentTab[1].Hide()
		
		self.wndItem = wndItem
		self.wndEquip = wndEquip
		self.dlgPickMoney = dlgPickMoney

		if self.mallButton:
			self.mallButton.SetEvent(ui.__mem_func__(self.ClickMallButton))

		if self.DSSButton:
			self.DSSButton.SetEvent(ui.__mem_func__(self.ClickDSSButton)) 			
		
		# Costume Button
		if self.costumeButton:
			self.costumeButton.SetEvent(ui.__mem_func__(self.ClickCostumeButton))
		
		self.wndCostume = None
		self.listUnusableSlot = []
		if app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
			self.listHighlightedSlot = []
		if app.ENABLE_CHANGELOOK_SYSTEM:
			self.listAttachedCl = []
		if app.ENABLE_ACCE_SYSTEM:
			self.listAttachedSashs = []
		if app.ENABLE_AURA_SYSTEM:
			self.listAttachedAuras = []
 		#####

		## Refresh
		self.SetInventoryPage(0)
		self.SetEquipmentPage(0)
		self.RefreshItemSlot()
		self.RefreshStatus()
		
		self.wndPetFeedWindow = None

	def Destroy(self):
		self.ClearDictionary()

		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = 0

		self.refineDialog.Destroy()
		self.refineDialog = 0

		self.attachMetinDialog.Destroy()
		self.attachMetinDialog = 0
		
		self.tooltipItem = None
		self.wndItem = 0
		self.wndEquip = 0
		self.dlgPickMoney = 0
		self.dlgPickPetName = 0
		self.questionDialog = None
		self.mallButton = None
		self.DSSButton = None
		self.interface = None
		if app.WJ_TRADABLE_ICON:
			self.bindWnds = []

		if self.wndCostume:
			self.wndCostume.Destroy()
			self.wndCostume = 0

		if self.wndBelt:
			self.wndBelt.Destroy()
			self.wndBelt = None
						
		self.inventoryTab = []
		self.equipmentTab = []
		if app.ENABLE_EXTEND_INVENTORY_SYSTEM:	
			self.ExInvenButton = []
	
	def Hide(self):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			self.OnCloseQuestionDialog()
			return
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

		if self.wndCostume:
			self.isOpenedCostumeWindowWhenClosingInventory = self.wndCostume.IsShow()			# ¿Œ∫•≈‰∏Æ √¢¿Ã ¥›»˙ ∂ß ƒ⁄Ω∫√ı¿Ã ø≠∑¡ ¿÷æ˙¥¬∞°?
			self.wndCostume.Close()

		if self.wndBelt:
			self.isOpenedBeltWindowWhenClosingInventory = self.wndBelt.IsOpeningInventory()		# ¿Œ∫•≈‰∏Æ √¢¿Ã ¥›»˙ ∂ß ∫ß∆Æ ¿Œ∫•≈‰∏Æµµ ø≠∑¡ ¿÷æ˙¥¬∞°?
			print "Is Opening Belt Inven?? ", self.isOpenedBeltWindowWhenClosingInventory
			self.wndBelt.Close()

		if self.dlgPickMoney:
			self.dlgPickMoney.Close()
			
		self.OnCloseQuestionDialog()
		
		if self.wndSystemsWindow:
			self.wndSystemsWindow.Kapat()
		
		wndMgr.Hide(self.hWnd)
		
	if app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
		def HighlightSlot(self, slot):
			if not slot in self.listHighlightedSlot:
				self.listHighlightedSlot.append(slot)
	
	def Close(self):
		if self.wndSystemsWindow:
			self.wndSystemsWindow.Kapat()
		self.Hide()
		
	def SetInventoryPage(self, page):
		self.inventoryPageIndex = page
		for i in range(0,len(self.inventoryTab)):
			self.inventoryTab[i].SetUp()
		self.inventoryTab[page].Down()
		self.RefreshBagSlotWindow()

	def SetEquipmentPage(self, page):
		self.equipmentPageIndex = page
		self.equipmentTab[1-page].SetUp()
		self.RefreshEquipSlotWindow()
		
	if app.ENABLE_EXTEND_INVENTORY_SYSTEM:				
		def __CreateExtendInvenButton(self):
			cur_stage = player.GetExtendInvenStage()
			ButtonPosition = 9
			if cur_stage < 9 and self.inventoryPageIndex == 3:
				ButtonPosition = cur_stage		
		
			for i in range(ButtonPosition, player.EX_INVENTORY_STAGE_MAX):	
				increase_y	= 32 * (i % (player.EX_INVENTORY_STAGE_MAX / player.EX_INVENTORY_PAGE_COUNT))
				button = ui.Button()
				button.SetParent(self.GetChild("board"))			
				#button.SetPosition(8, 246 + i * (player.EX_INVENTORY_STAGE_MAX * player.EX_INVENTORY_PAGE_COUNT + 5)) # TODO -> player.EX_INVENTORY_STAGE_MAX * player.EX_INVENTORY_PAGE_COUNT
				button.SetPosition(8, 246 + (increase_y))
				button.SetUpVisual(EX_INVEN_COVER_IMG_CLOSE)
				button.SetOverVisual(EX_INVEN_COVER_IMG_CLOSE)
				button.SetDownVisual(EX_INVEN_COVER_IMG_CLOSE)
				button.SetDisableVisual(EX_INVEN_COVER_IMG_CLOSE)
				button.SetDownVisual(EX_INVEN_COVER_IMG_CLOSE)
				button.Show()
				button.SetEvent(ui.__mem_func__(self.__ClickExtendInvenButton))
				self.ExInvenButton.append(button)			
			
		def __ClickExtendInvenButton(self):
			net.SendExtendInvenButtonClick(player.GetExtendInvenStage())

		def __HideAllExtendInvenButton(self):
			for button in self.ExInvenButton:
				button.Hide()
				button.Destroy()

		def __ShowExtendInvenButton(self, cur_stage):
			for button in self.ExInvenButton:
				button.Destroy()
					
			self.__CreateExtendInvenButton()
			ButtonPosition = 0
			
			if cur_stage < 9 and self.inventoryPageIndex == 3:
				ButtonPosition = cur_stage

			button = ui.Button()
			button.SetParent(self.GetChild("board"))	
			#button.SetPosition(8, 246 + ButtonPosition * (player.EX_INVENTORY_STAGE_MAX * player.EX_INVENTORY_PAGE_COUNT + 5)) # TODO -> player.EX_INVENTORY_STAGE_MAX * player.EX_INVENTORY_PAGE_COUNT	 
			increase_y	= 32 * (ButtonPosition % (player.EX_INVENTORY_STAGE_MAX / player.EX_INVENTORY_PAGE_COUNT))
			button.SetPosition(8, 246 + (increase_y))
			button.SetUpVisual(EX_INVEN_COVER_IMG_OPEN)
			button.SetOverVisual(EX_INVEN_COVER_IMG_OPEN)
			button.SetDownVisual(EX_INVEN_COVER_IMG_OPEN)
			button.SetDisableVisual(EX_INVEN_COVER_IMG_OPEN)
			button.SetDownVisual(EX_INVEN_COVER_IMG_OPEN)
			button.Hide()
			button.SetEvent(ui.__mem_func__(self.__ClickExtendInvenButton))
			self.ExInvenButton.append(button)
			
			if cur_stage < 9 and self.inventoryPageIndex == 3:
				button.Show()

		def ExInvenItemUseMsg(self, current_stage, need_left, need_count):
			if current_stage >= player.EX_INVENTORY_STAGE_MAX * player.EX_INVENTORY_PAGE_COUNT:	
				self.__OpenExInvenMsgDlg(current_stage)
				return
				
			if need_left > 0:
				self.__OpenExInvenFallShortCountMsgDlg(need_left)
				return

			self.__OpenExInvenQuestionDlg(need_count)

		def __OpenExInvenMsgDlg(self, current_stage):
				self.wndPopupDialog = uiCommon.PopupDialog()
				self.wndPopupDialog.SetText(localeInfo.EXINVEN_USE_ITEM_FAIL_FOURTH_PAGE_STAGE_MAX)
				self.wndPopupDialog.Open()	

		def __OpenExInvenFallShortCountMsgDlg(self, need_left):
			self.wndPopupDialog = uiCommon.PopupDialog()
			self.wndPopupDialog.SetText(localeInfo.EXINVEN_USE_ITEM_FAIL_FALL_SHORT % (need_left))
			self.wndPopupDialog.Open()			

		def __OpenExInvenQuestionDlg(self,	need_count):
			ExInvquestionDialog = uiCommon.QuestionDialog2()
			ExInvquestionDialog.SetText1(localeInfo.EXINVEN_USE_ITEM_QUESTION_2 % (need_count))
			ExInvquestionDialog.SetText2(localeInfo.EXINVEN_USE_ITEM_QUESTION)
			ExInvquestionDialog.SetAcceptEvent(ui.__mem_func__(self.__AcceptExInvenItemUse))
			ExInvquestionDialog.SetCancelEvent(ui.__mem_func__(self.__CancelExInvenItemUse))
			ExInvquestionDialog.Open()
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
			self.ExInvquestionDialog = ExInvquestionDialog

		def __AcceptExInvenItemUse(self):
			self.ExInvquestionDialog.Close()
			self.ExInvquestionDialog = None
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)		
		
			if player.GetExtendInvenStage() >= player.EX_INVENTORY_STAGE_MAX * player.EX_INVENTORY_PAGE_COUNT:	
				return
			
			net.SendExtendInvenUpgrade()

		def __CancelExInvenItemUse(self):
			if self.ExInvquestionDialog:
				self.ExInvquestionDialog.Close()

			self.ExInvquestionDialog = None		
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	def ClickMallButton(self):
		net.SendChatPacket("/click_safebox")
				
	# DSSButton
	def ClickDSSButton(self):
		print "click_dss_button"
		self.interface.ToggleDragonSoulWindow()

	def ClickCostumeButton(self):
		print "Click Costume Button"
		if self.wndCostume:
			if self.wndCostume.IsShow(): 
				self.wndCostume.Hide()
			else:
				self.wndCostume.Show()
		else:
			self.wndCostume = CostumeWindow(self)
			self.wndCostume.Show()

	def OpenPickMoneyDialog(self):

		if mouseModule.mouseController.isAttached():

			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			if player.SLOT_TYPE_SAFEBOX == mouseModule.mouseController.GetAttachedType():

				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxWithdrawMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

			mouseModule.mouseController.DeattachObject()

		else:
			curMoney = player.GetElk()

			if curMoney <= 0:
				return

			self.dlgPickMoney.SetTitleName(localeInfo.PICK_MONEY_TITLE)
			self.dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
			if app.ENABLE_CHEQUE_SYSTEM:
				self.dlgPickMoney.Open(curMoney, player.GetCheque())
			else:
				self.dlgPickMoney.Open(curMoney)
				self.dlgPickMoney.SetMax(7) # ¿Œ∫•≈‰∏Æ 990000 ¡¶«— πˆ±◊ ºˆ¡§f

	def OnPickMoney(self, money, cheque = None):
		mouseModule.mouseController.AttachMoney(self, player.SLOT_TYPE_INVENTORY, money)

	def OnPickItem(self, count):
		itemSlotIndex = self.dlgPickMoney.itemGlobalSlotIndex
		selectedItemVNum = player.GetItemIndex(itemSlotIndex)
		mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_INVENTORY, itemSlotIndex, selectedItemVNum, count)
		
	def GetInventoryPageIndex(self):
		return self.inventoryPageIndex

	def __InventoryLocalSlotPosToGlobalSlotPos(self, local):
		if player.IsEquipmentSlot(local) or player.IsCostumeSlot(local) or (app.ENABLE_NEW_EQUIPMENT_SYSTEM and player.IsBeltInventorySlot(local)):
			return local

		return self.inventoryPageIndex*player.INVENTORY_PAGE_SIZE + local

	def RefreshBagSlotWindow(self):
		onTopWindow = 0
		getItemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setItemVNum=self.wndItem.SetItemSlot
		
		if self.interface:
			onTopWindow = self.interface.GetOnTopWindow()
		
		for i in xrange(player.INVENTORY_PAGE_SIZE):
			slotNumber = self.__InventoryLocalSlotPosToGlobalSlotPos(i)
			itemCount = getItemCount(slotNumber)
			if 0 == itemCount:
				self.wndItem.ClearSlot(i)
				continue
			elif 1 == itemCount:
				itemCount = 0
			
			itemVnum = getItemVNum(slotNumber)
			setItemVNum(i, itemVnum, itemCount)
			
			self.wndItem.DeactivateSlot(i)
			self.wndItem.DeactivateSlot2(i)
			if app.ENABLE_ITEM_COLOR_HIGHLIGHT_SYSTEM:
				self.wndItem.DeactivateSlotColor(i)
			
			if app.ENABLE_CHANGELOOK_SYSTEM:
				itemTransmutedVnum = player.GetItemTransmutation(slotNumber)
				if itemTransmutedVnum:
					self.wndItem.DisableCoverButton(i)
				else:
					self.wndItem.EnableCoverButton(i)
					
			if constInfo.IS_AUTO_POTION(itemVnum):
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]
				
				if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
				
				isActivated = 0 != metinSocket[0]
				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
					potionType = 0;
					if constInfo.IS_AUTO_POTION_HP(itemVnum):
						potionType = player.AUTO_POTION_TYPE_HP
					elif constInfo.IS_AUTO_POTION_SP(itemVnum):
						potionType = player.AUTO_POTION_TYPE_SP						
					
					usedAmount = int(metinSocket[1])
					totalAmount = int(metinSocket[2])					
					player.SetAutoPotionInfo(potionType, isActivated, (totalAmount - usedAmount), totalAmount, self.__InventoryLocalSlotPosToGlobalSlotPos(i))
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
			elif constInfo.IS_NEW_PET_ITEM(itemVnum):
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]# <!> globalSlotNumber may be different <!>
				isActivated = 0 != metinSocket[0]
				
				if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
				
				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
			elif constInfo.IS_PET_ITEM(itemVnum):
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]# <!> globalSlotNumber may be different <!>
				isActivated = 0 != metinSocket[1]
				
				if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
				
				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
			elif constInfo.IS_SUPPORT_SHAMAN_ITEM(itemVnum):
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]# <!> globalSlotNumber may be different <!>
				
				if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
				
				isActivated = 0 != metinSocket[1]
				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
			elif item.IsNewPotions(itemVnum):
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]# <!> globalSlotNumber may be different <!>
				
				if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
				
				isActivated = 0 != metinSocket[1]
				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
			elif constInfo.IS_NEW_TYPE_POTION(itemVnum):
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)] ## player.METIN_SOCKET_MAX_NUM -> 3

				if slotNumber >= player.INVENTORY_PAGE_SIZE * self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE * self.inventoryPageIndex

				isActivated = 0 != metinSocket[0]

				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
					
			elif player.GetItemTypeBySlot(slotNumber) == item.ITEM_TYPE_SOUL:
				metinSocket = [player.GetItemMetinSocket(slotNumber, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]# <!> globalSlotNumber may be different <!>
				
				if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
					slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
				
				isActivated = 0 != metinSocket[1]
				if isActivated:
					#self.wndItem.ActivateSlot(slotNumber)
					self.wndItem.ActivateSlotColor(slotNumber, (0.00 / 255.0), (109.00 / 255.0), (173.00 / 255.0), 1.0)
				else:
					#self.wndItem.DeactivateSlot(slotNumber)
					self.wndItem.DeactivateSlotColor(slotNumber)
			elif app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
				if slotNumber in self.listHighlightedSlot:
					if slotNumber >= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex:
						slotNumber -= player.INVENTORY_PAGE_SIZE*self.inventoryPageIndex
					if systemSetting.IsItemHighlight() == True:
						self.wndItem.ActivateSlot(slotNumber)

			if app.ENABLE_ACCE_SYSTEM:
				slotNumberChecked = 0
				for j in xrange(sash.WINDOW_MAX_MATERIALS):
					(isHere, iCell) = sash.GetAttachedItem(j)
					if isHere:
						if iCell == slotNumber:
							if not constInfo.IS_PET_ITEM(itemVnum):
								self.wndItem.ActivateSlot2(i, (36.00 / 255.0), (222.00 / 255.0), (3.00 / 255.0), 1.0)
							if not slotNumber in self.listAttachedSashs:
								self.listAttachedSashs.append(slotNumber)
							
							slotNumberChecked = 1
					else:
						if slotNumber in self.listAttachedSashs and not slotNumberChecked:
							self.wndItem.DeactivateSlot2(i)
							self.listAttachedSashs.remove(slotNumber)

			if app.ENABLE_CHANGELOOK_SYSTEM:
				slotClNumberChecked = 0
				
				for q in xrange(changelook.WINDOW_MAX_MATERIALS):
					(isHere, iCell) = changelook.GetAttachedItem(q)
					if isHere:
						if iCell == slotNumber:
							if not constInfo.IS_PET_ITEM(itemVnum):
								self.wndItem.ActivateSlot2(i, (238.00 / 255.0), (11.00 / 255.0), (11.00 / 255.0), 1.0)
							if not slotNumber in self.listAttachedCl:
								self.listAttachedCl.append(slotNumber)
							
							slotClNumberChecked = 1
					else:
						if slotNumber in self.listAttachedCl and not slotClNumberChecked:
							self.wndItem.DeactivateSlot2(i)
							self.listAttachedCl.remove(slotNumber)
							
			if app.ENABLE_AURA_SYSTEM:
				slotNumberChecked = 0
				for j in xrange(aura.WINDOW_MAX_MATERIALS):
					(isHere, iCell) = aura.GetAttachedItem(j)
					if isHere:
						if iCell == slotNumber:
							if not constInfo.IS_PET_ITEM(itemVnum):
								self.wndItem.ActivateSlot2(i, (36.00 / 255.0), (222.00 / 255.0), (3.00 / 255.0), 1.0)
							if not slotNumber in self.listAttachedAuras:
								self.listAttachedAuras.append(slotNumber)
							
							slotNumberChecked = 1
					else:
						if slotNumber in self.listAttachedAuras and not slotNumberChecked:
							self.wndItem.DeactivateSlot2(i)
							self.listAttachedAuras.remove(slotNumber)
			if not itemVnum or not onTopWindow:
				self.wndItem.SetUsableSlotOnTopWnd(i)
			else:
				if self.interface.MarkUnusableInvenSlotOnTopWnd(onTopWindow, slotNumber):
					print "Mark Unusuable slotNumber %d" % slotNumber
					self.wndItem.SetUnusableSlotOnTopWnd(i)
				else:
					print "Mark Useable slotNumber %d" % slotNumber
					self.wndItem.SetUsableSlotOnTopWnd(i)		
			#self.__SetCooldownForTimedItemSlot(slotNumber, itemVnum)
		
		self.wndItem.RefreshSlot()
		
		if app.ENABLE_EXTEND_INVENTORY_SYSTEM:
			self.__RefreshExinvenCoverSlot()	
		
		#if app.WJ_TRADABLE_ICON:
			#map(lambda wnd:wnd.RefreshLockedSlot(), self.bindWnds)
			
		if self.wndBelt:
			self.wndBelt.RefreshSlot()

	def RefreshEquipSlotWindow(self):
		getItemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setItemVNum=self.wndEquip.SetItemSlot
		for i in xrange(player.EQUIPMENT_PAGE_COUNT):
			slotNumber = player.EQUIPMENT_SLOT_START + i
			itemCount = getItemCount(slotNumber)
			if itemCount <= 1:
				itemCount = 0
			setItemVNum(slotNumber, getItemVNum(slotNumber), itemCount)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				itemTransmutedVnum = player.GetItemTransmutation(slotNumber)
				if itemTransmutedVnum:
					self.wndEquip.DisableCoverButton(slotNumber)
				else:
					self.wndEquip.EnableCoverButton(slotNumber)

		if app.ENABLE_NEW_EQUIPMENT_SYSTEM:
			for i in xrange(player.NEW_EQUIPMENT_SLOT_COUNT):
				slotNumber = player.NEW_EQUIPMENT_SLOT_START + i
				itemCount = getItemCount(slotNumber)
				if itemCount <= 1:
					itemCount = 0
				setItemVNum(slotNumber, getItemVNum(slotNumber), itemCount)
				if app.ENABLE_CHANGELOOK_SYSTEM:
					itemTransmutedVnum = player.GetItemTransmutation(slotNumber)
					if itemTransmutedVnum:
						self.wndEquip.DisableCoverButton(slotNumber)
					else:
						self.wndEquip.EnableCoverButton(slotNumber)
				


		self.wndEquip.RefreshSlot()
		
		if self.wndCostume:
			self.wndCostume.RefreshCostumeSlot()

	def RefreshItemSlot(self):
		self.RefreshBagSlotWindow()
		self.RefreshEquipSlotWindow()
		
	if app.ENABLE_EXTEND_INVENTORY_SYSTEM:				
		def __RefreshExinvenCoverSlot(self):
			cur_stage = player.GetExtendInvenStage()
			self.__HideAllExtendInvenButton()
			if self.inventoryPageIndex >= 3:				
				self.__ShowExtendInvenButton(cur_stage)
			else:
				self.__HideAllExtendInvenButton()	
			
		def __RefreshExinvenSlotState(self):
			pass

	def RefreshStatus(self):
		pass
		
	def RefreshSkill(self):
		pass

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def SellItem(self):
		if self.sellingSlotitemIndex == player.GetItemIndex(self.sellingSlotNumber):
			if self.sellingSlotitemCount == player.GetItemCount(self.sellingSlotNumber):
				## øÎ»•ºÆµµ ∆»∏Æ∞‘ «œ¥¬ ±‚¥… √ﬂ∞°«œ∏Èº≠ ¿Œ¿⁄ type √ﬂ∞°
				net.SendShopSellPacketNew(self.sellingSlotNumber, self.questionDialog.count, player.INVENTORY)
				snd.PlaySound("sound/ui/money.wav")
		self.OnCloseQuestionDialog()
        
	def OnDetachMetinFromItem(self):
		if None == self.questionDialog:
			return
			
		#net.SendItemUseToItemPacket(self.questionDialog.sourcePos, self.questionDialog.targetPos)		
		self.__SendUseItemToItemPacket(self.questionDialog.sourcePos, self.questionDialog.targetPos)
		self.OnCloseQuestionDialog()

	def OnCloseQuestionDialog(self):
		if not self.questionDialog:
			return
		
		self.questionDialog.Close()
		self.questionDialog = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
		
	def DeleteItem(self, slotPos, invenType):
		itemIndex = player.GetItemIndex(invenType, slotPos)
		item.SelectItem(itemIndex)
		itemQuestionDialog = uiCommon.ItemQuestionDialog()
		itemQuestionDialog.SetText('[%s] nesnesine ne yapmak istiyorsun?' % item.GetItemName())
		itemQuestionDialog.SetText2('(Fiyat: %s)' % localeInfo.NumberToMoneyString(item.GetISellItemPrice() * player.GetItemCount(invenType, slotPos) * 97 / 100))
		itemQuestionDialog.SetDeleteAcceptEvent(lambda arg = 0: self.__AnswerDeleteItem(arg))
		itemQuestionDialog.SetSellAcceptEvent(lambda arg = 1: self.__AnswerDeleteItem(arg))
		itemQuestionDialog.SetCancelEvent(lambda arg = 2: self.__AnswerDeleteItem(arg))
		itemQuestionDialog.Open()
		itemQuestionDialog.slotPos = slotPos
		itemQuestionDialog.invenType = invenType
		self.itemQuestionDialog = itemQuestionDialog

	def __AnswerDeleteItem(self, answer):
		if not self.itemQuestionDialog:
			return
		else:
			if answer == 0:
				net.SendItemDeletePacket(self.itemQuestionDialog.slotPos, self.itemQuestionDialog.invenType)
				snd.PlaySound('sound/ui/drop.wav')
			elif answer == 1:
				net.SendItemSellPacket(self.itemQuestionDialog.slotPos, self.itemQuestionDialog.invenType)
				snd.PlaySound('sound/ui/money.wav')
			self.itemQuestionDialog.Close()
			self.itemQuestionDialog = None
			return
		
	## Slot Event
	def SelectEmptySlot(self, selectedSlotPos):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		selectedSlotPos = self.__InventoryLocalSlotPosToGlobalSlotPos(selectedSlotPos)

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()

			if player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or player.SLOT_TYPE_ATTR_INVENTORY == attachedSlotType:
				itemCount = player.GetItemCount(attachedSlotPos)
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				self.__SendMoveItemPacket(attachedSlotPos, selectedSlotPos, attachedCount)

				if item.IsRefineScroll(attachedItemIndex):
					self.wndItem.SetUseMode(False)

			elif player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
				mouseModule.mouseController.RunCallBack("INVENTORY")

			elif player.SLOT_TYPE_SHOP == attachedSlotType:
				net.SendShopBuyPacket(attachedSlotPos)

			elif player.SLOT_TYPE_SAFEBOX == attachedSlotType:

				if player.ITEM_MONEY == attachedItemIndex:
					net.SendSafeboxWithdrawMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

				else:
					net.SendSafeboxCheckoutPacket(attachedSlotPos, selectedSlotPos)

			elif player.SLOT_TYPE_MALL == attachedSlotType:
				net.SendMallCheckoutPacket(attachedSlotPos, selectedSlotPos)
				
			mouseModule.mouseController.DeattachObject()

	def SelectItemSlot(self, itemSlotIndex):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return

		itemSlotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(itemSlotIndex)

		if mouseModule.mouseController.isAttached():
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemVID = mouseModule.mouseController.GetAttachedItemIndex()

			if player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or player.SLOT_TYPE_ATTR_INVENTORY == attachedSlotType:
				self.__DropSrcItemToDestItemInInventory(attachedItemVID, attachedSlotPos, itemSlotIndex)

			mouseModule.mouseController.DeattachObject()

		else:

			curCursorNum = app.GetCursor()
			if app.SELL == curCursorNum:
				self.__SellItem(itemSlotIndex)
				
			elif app.BUY == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)

			elif app.IsPressed(app.DIK_LALT):
				link = player.GetItemLink(itemSlotIndex)
				ime.PasteString(link)

			elif app.IsPressed(app.DIK_LSHIFT):
				itemCount = player.GetItemCount(itemSlotIndex)
				
				if itemCount > 1:
					self.dlgPickMoney.SetTitleName(localeInfo.PICK_ITEM_TITLE)
					self.dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickItem))
					self.dlgPickMoney.Open(itemCount)
					self.dlgPickMoney.itemGlobalSlotIndex = itemSlotIndex
				#else:
					#selectedItemVNum = player.GetItemIndex(itemSlotIndex)
					#mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_INVENTORY, itemSlotIndex, selectedItemVNum)

			elif app.IsPressed(app.DIK_LCONTROL):
				itemIndex = player.GetItemIndex(itemSlotIndex)

				if True == item.CanAddToQuickSlotItem(itemIndex):
					player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_INVENTORY, itemSlotIndex)
				else:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.QUICKSLOT_REGISTER_DISABLE_ITEM)

			else:
				selectedItemVNum = player.GetItemIndex(itemSlotIndex)
				itemCount = player.GetItemCount(itemSlotIndex)
				mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_INVENTORY, itemSlotIndex, selectedItemVNum, itemCount)
				
				if self.__IsUsableItemToItem(selectedItemVNum, itemSlotIndex):				
					self.wndItem.SetUseMode(True)
				else:					
					self.wndItem.SetUseMode(False)

				snd.PlaySound("sound/ui/pick.wav")
		
	def UseTransportBox(self):
		self.__SendUseItemToItemPacket(self.questionDialog.src, self.questionDialog.dst)
		self.OnCloseQuestionDialog()
		
	def UseProtein(self):
		self.__SendUseItemToItemPacket(self.questionDialog.src, self.questionDialog.dst)
		self.OnCloseQuestionDialog()				

	def __DropSrcItemToDestItemInInventory(self, srcItemVID, srcItemSlotPos, dstItemSlotPos):
		if srcItemSlotPos == dstItemSlotPos:
			return
			
		dstItemVnum = player.GetItemIndex(dstItemSlotPos)
		if dstItemVnum != 0:
			item.SelectItem(srcItemVID)
			srcItemType = item.GetItemType()
			srcItemSubType = item.GetItemSubType()
			item.SelectItem(dstItemVnum)
			if srcItemType == item.ITEM_TYPE_USE and srcItemSubType == item.USE_EXTEND_TIME and item.GetItemType() == item.ITEM_TYPE_COSTUME and (item.GetItemSubType() == item.COSTUME_TYPE_BODY or item.GetItemSubType() == item.COSTUME_TYPE_HAIR):
				self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)
			
		if srcItemVID == player.GetItemIndex(dstItemSlotPos):
			self.__SendMoveItemPacket(srcItemSlotPos, dstItemSlotPos, 0)
			return
			
		if srcItemVID == 71084:
			self.__SendUseItemToItemPacketAttr(srcItemSlotPos, dstItemSlotPos)
			return
		
		if app.ENABLE_ITEM_SEALBIND_SYSTEM and (item.IsSealScroll(srcItemVID) or item.IsUnSealScroll(srcItemVID)):
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)
			return
		
		if srcItemVID >= 55701 and srcItemVID <= 55709 and player.GetItemIndex(dstItemSlotPos) == 55002:
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.PET_SYSTEM_ADD_BOX_PET)
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.UseTransportBox))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			self.questionDialog.Open()
			self.questionDialog.src = srcItemSlotPos
			self.questionDialog.dst = dstItemSlotPos
			return
			
		if player.GetItemIndex(dstItemSlotPos) >= 55701 and player.GetItemIndex(dstItemSlotPos) <= 55709 and srcItemVID == 55008:
			self.interface.OpenInputNameDialogPet(dstItemSlotPos)
			return
					
		if srcItemVID == 55001 and player.GetItemIndex(dstItemSlotPos) >= 55701 and player.GetItemIndex(dstItemSlotPos) <= 55709:
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.PET_SYSTEM_FEED_PROTEIN)
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.UseProtein))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			self.questionDialog.Open()
			self.questionDialog.src = srcItemSlotPos
			self.questionDialog.dst = dstItemSlotPos
			return
			
		if srcItemVID >= 81705 and srcItemVID <= 81711:	# evrim parsomeni
			item.SelectItem(player.GetItemIndex(dstItemSlotPos))
			if item.GetItemType() == item.ITEM_TYPE_WEAPON:
				self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)
				self.game.wndRR.UseItemSlot(player.INVENTORY,dstItemSlotPos,True)
		
		if item.IsRefineScroll(srcItemVID):
			self.RefineItem(srcItemSlotPos, dstItemSlotPos, player.INVENTORY)
			self.wndItem.SetUseMode(False)

		elif item.IsMetin(srcItemVID):
			self.AttachMetinToItem(srcItemSlotPos, dstItemSlotPos)
		
		elif item.IsDetachScroll(srcItemVID):
			self.DetachMetinFromItem(srcItemSlotPos, dstItemSlotPos)

		elif item.IsKey(srcItemVID):
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)			

		elif (player.GetItemFlags(srcItemSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)

		elif item.GetUseType(srcItemVID) in self.USE_TYPE_TUPLE:
			self.__SendUseItemToItemPacket(srcItemSlotPos, dstItemSlotPos)	

		########################################################################
		elif app.ENABLE_UPGRADE_SOCKET_SYSTEM and item.IsUpgradeSocketScroll(srcItemVID):
			self.AttachUpgradeMetinToItem(srcItemSlotPos, dstItemSlotPos)
		########################################################################			

		else:
			#snd.PlaySound("sound/ui/drop.wav")

			## ¿ÃµøΩ√≈≤ ∞˜¿Ã ¿Â¬¯ ΩΩ∑‘¿œ ∞ÊøÏ æ∆¿Ã≈€¿ª ªÁøÎ«ÿº≠ ¿Â¬¯ Ω√≈≤¥Ÿ - [levites]
			if player.IsEquipmentSlot(dstItemSlotPos):

				## µÈ∞Ì ¿÷¥¬ æ∆¿Ã≈€¿Ã ¿Â∫Ò¿œ∂ß∏∏
				if item.IsEquipmentVID(srcItemVID):
					self.__UseItem(srcItemSlotPos)

			else:
				self.__SendMoveItemPacket(srcItemSlotPos, dstItemSlotPos, 0)
				#net.SendItemMovePacket(srcItemSlotPos, dstItemSlotPos, 0)

	def __SellItem(self, itemSlotPos):
		if not player.IsEquipmentSlot(itemSlotPos):
			self.sellingSlotNumber = itemSlotPos
			itemIndex = player.GetItemIndex(itemSlotPos)
			itemCount = player.GetItemCount(itemSlotPos)
			
			
			self.sellingSlotitemIndex = itemIndex
			self.sellingSlotitemCount = itemCount

			item.SelectItem(itemIndex)
			## æ»∆º «√∑π±◊ ∞ÀªÁ ∫¸¡Æº≠ √ﬂ∞°
			## 20140220
			if item.IsAntiFlag(item.ANTIFLAG_SELL):
				popup = uiCommon.PopupDialog()
				popup.SetText(localeInfo.SHOP_CANNOT_SELL_ITEM)
				popup.SetAcceptEvent(self.__OnClosePopupDialog)
				popup.Open()
				self.popup = popup
				return			
			itemPrice = player.GetISellItemPrice(itemSlotPos)

			item.GetItemName(itemIndex)
			itemName = item.GetItemName()

			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.DO_YOU_SELL_ITEM(itemName, itemCount, itemPrice))
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.SellItem))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			self.questionDialog.Open()
			self.questionDialog.count = itemCount

			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

	def __OnClosePopupDialog(self):
		self.pop = None			
			
	def RefineItem(self, scrollSlotPos, targetSlotPos, targetWindowType):

		scrollIndex = player.GetItemIndex(scrollSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)

		if player.REFINE_OK != player.CanRefine(scrollIndex, targetSlotPos):
			return
			
		constInfo.AUTO_REFINE_TYPE = 1
		constInfo.AUTO_REFINE_DATA["ITEM"][0] = scrollSlotPos
		constInfo.AUTO_REFINE_DATA["ITEM"][1] = targetSlotPos

		self.__SendUseItemToItemPacket(scrollSlotPos, targetSlotPos)

	def CostumeMetinFromItem(self, scrollSlotPos, targetSlotPos):
		scrollIndex = player.GetItemIndex(scrollSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)

		self.__SendUseItemToItemPacket(scrollSlotPos, targetSlotPos)		
		
	def DetachMetinFromItem(self, scrollSlotPos, targetSlotPos):
		scrollIndex = player.GetItemIndex(scrollSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)
		if app.ENABLE_CHANGELOOK_SYSTEM or app.ENABLE_ACCE_SYSTEM:
			if not player.CanDetach(scrollIndex, targetSlotPos):
				item.SelectItem(scrollIndex)
				if item.GetValue(0) == changelook.CLEAN_ATTR_VALUE0:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHANGE_LOOK_FAILURE_CLEAN)
				elif item.GetValue(0) == sash.CLEAN_ATTR_VALUE0:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SASH_FAILURE_CLEAN)
				else:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_METIN_INSEPARABLE_ITEM)
				
				return
		else:
			if not player.CanDetach(scrollIndex, targetSlotPos):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_METIN_INSEPARABLE_ITEM)
				return
		
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.REFINE_DO_YOU_SEPARATE_METIN)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				item.SelectItem(targetIndex)
				if item.GetItemType() == item.ITEM_TYPE_WEAPON or item.GetItemType() == item.ITEM_TYPE_ARMOR or item.GetItemType() == item.ITEM_TYPE_COSTUME:
					item.SelectItem(scrollIndex)
					if item.GetValue(0) == changelook.CLEAN_ATTR_VALUE0:
						self.questionDialog.SetText(localeInfo.CHANGE_LOOK_DO_YOU_CLEAN)
		if app.ENABLE_ACCE_SYSTEM:
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.REFINE_DO_YOU_SEPARATE_METIN)
			item.SelectItem(targetIndex)
			if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_SASH:
				item.SelectItem(scrollIndex)
				if item.GetValue(0) == sash.CLEAN_ATTR_VALUE0:
					self.questionDialog.SetText(localeInfo.SASH_DO_YOU_CLEAN)
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnDetachMetinFromItem))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			self.questionDialog.Open()
			self.questionDialog.sourcePos = scrollSlotPos
			self.questionDialog.targetPos = targetSlotPos

	def AttachMetinToItem(self, metinSlotPos, targetSlotPos):
		metinIndex = player.GetItemIndex(player.STONE_ITEMS_INVENTORY, metinSlotPos)
		targetIndex = player.GetItemIndex(targetSlotPos)

		item.SelectItem(metinIndex)
		itemName = item.GetItemName()

		result = player.CanAttachMetin(metinIndex, targetSlotPos)

		if player.ATTACH_METIN_NOT_MATCHABLE_ITEM == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_CAN_NOT_ATTACH(itemName))

		if player.ATTACH_METIN_NO_MATCHABLE_SOCKET == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_NO_SOCKET(itemName))

		elif player.ATTACH_METIN_NOT_EXIST_GOLD_SOCKET == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_NO_GOLD_SOCKET(itemName))

		elif player.ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT == result:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.REFINE_FAILURE_EQUIP_ITEM)

		if player.ATTACH_METIN_OK != result:
			return

		self.attachMetinDialog.Open(metinSlotPos, targetSlotPos)

	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OverInItem(self, overSlotPos):
		overSlotPosGlobal = self.__InventoryLocalSlotPosToGlobalSlotPos(overSlotPos)
		self.wndItem.SetUsableItem(False)
		
		if app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
			if overSlotPosGlobal in self.listHighlightedSlot:
				self.listHighlightedSlot.remove(overSlotPosGlobal)
				self.wndItem.DeactivateSlot(overSlotPos)

		if mouseModule.mouseController.isAttached():
			attachedItemType = mouseModule.mouseController.GetAttachedType()
			if player.SLOT_TYPE_INVENTORY == attachedItemType or player.SLOT_TYPE_STONE_INVENTORY == attachedItemType or player.SLOT_TYPE_ATTR_INVENTORY == attachedItemType:

				attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
				attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()
				
				if self.__CanUseSrcItemToDstItem(attachedItemVNum, attachedSlotPos, overSlotPosGlobal):
					self.wndItem.SetUsableItem(True)
					self.wndItem.SetUseMode(True)
					self.ShowToolTip(overSlotPosGlobal)
					return
				
		self.ShowToolTip(overSlotPosGlobal)


	def __IsUsableItemToItem(self, srcItemVNum, srcSlotPos):
		"¥Ÿ∏• æ∆¿Ã≈€ø° ªÁøÎ«“ ºˆ ¿÷¥¬ æ∆¿Ã≈€¿Œ∞°?"

		if item.IsRefineScroll(srcItemVNum):
			return True
		
		if srcItemVNum >= 55701 and srcItemVNum <= 55706:
			return True
		
		if srcItemVNum == 55001:
			return True		
			
		if srcItemVNum == 55030:
			return True
			
		if srcItemVNum != 0:
			item.SelectItem(srcItemVNum)
			if item.GetItemType() == item.ITEM_TYPE_USE and item.GetItemSubType() == item.USE_EXTEND_TIME:
				return True
		
		if item.IsRefineScroll(srcItemVNum):
			return True
		elif item.IsMetin(srcItemVNum):
			return True	
		elif item.IsDetachScroll(srcItemVNum):
			return True
		elif item.IsKey(srcItemVNum):
			return True
		elif (player.GetItemFlags(srcSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			return True
		elif item.IsSealScroll(srcItemVNum) or item.IsUnSealScroll(srcItemVNum):
			return True
		elif app.ENABLE_UPGRADE_SOCKET_SYSTEM and item.IsUpgradeSocketScroll(srcItemVNum):
			return True
		else:
			if item.GetUseType(srcItemVNum) in self.USE_TYPE_TUPLE:
				return True
			
		return False

	def __CanUseSrcItemToDstItem(self, srcItemVNum, srcSlotPos, dstSlotPos):
		"¥ÎªÛ æ∆¿Ã≈€ø° ªÁøÎ«“ ºˆ ¿÷¥¬∞°?"

		if srcItemVNum >= 55701 and  srcItemVNum <= 55706 and player.GetItemIndex(dstSlotPos) == 55002:			
			return True		
		
		if srcItemVNum == 55001 and player.GetItemIndex(dstSlotPos) >= 55701 and player.GetItemIndex(dstSlotPos) <= 55706:			
			return True		
			
		if srcItemVNum == 55030 and player.GetItemIndex(dstSlotPos) >= 55701 and player.GetItemIndex(dstSlotPos) <= 55706:			
			return True		
		
		if srcSlotPos == dstSlotPos and not item.IsMetin(srcItemVNum) and not srcItemVNum == 71084:
			return False
			
		dstItemVnum = player.GetItemIndex(dstSlotPos)
		if dstItemVnum != 0:
			item.SelectItem(srcItemVNum)
			srcItemType = item.GetItemType()
			srcItemSubType = item.GetItemSubType()
			item.SelectItem(dstItemVnum)

			if srcItemType == item.ITEM_TYPE_USE and srcItemSubType == item.USE_EXTEND_TIME and item.GetItemType() == item.ITEM_TYPE_COSTUME and (item.GetItemSubType() == item.COSTUME_TYPE_BODY or item.GetItemSubType() == item.COSTUME_TYPE_HAIR):
				return True	

		if srcItemVNum == player.GetItemIndex(dstSlotPos):	
			if item.GetItemType() == item.ITEM_TYPE_USE:
				if player.GetItemCount(dstSlotPos) < 200:
					return True
		elif item.IsRefineScroll(srcItemVNum):
			if player.REFINE_OK == player.CanRefine(srcItemVNum, dstSlotPos):
				return True
		elif item.IsMetin(srcItemVNum):
			if player.ATTACH_METIN_OK == player.CanAttachMetin(srcItemVNum, dstSlotPos):
				return True
		elif item.IsDetachScroll(srcItemVNum):
			if player.DETACH_METIN_OK == player.CanDetach(srcItemVNum, dstSlotPos):
				return True
		elif item.IsKey(srcItemVNum):
			if player.CanUnlock(srcItemVNum, dstSlotPos):
				return True
		elif app.ENABLE_ITEM_SEALBIND_SYSTEM and item.IsSealScroll(srcItemVNum):
			if (item.IsSealScroll(srcItemVNum) and player.CanSealItem(dstSlotPos)) or (item.IsUnSealScroll(srcItemVNum) and player.CanUnSealItem(dstSlotPos)):
				return True
		elif app.ENABLE_UPGRADE_SOCKET_SYSTEM and item.IsUpgradeSocketScroll(srcItemVNum):
			item.SelectItem(player.GetItemIndex(dstSlotPos))
			if item.GetItemType() == item.ITEM_TYPE_ARMOR or item.GetItemType() == item.ITEM_TYPE_WEAPON:
				return True	
		elif (player.GetItemFlags(srcSlotPos) & ITEM_FLAG_APPLICABLE) == ITEM_FLAG_APPLICABLE:
			return True

		else:
			useType=item.GetUseType(srcItemVNum)

			if "USE_CLEAN_SOCKET" == useType:
				if self.__CanCleanBrokenMetinStone(dstSlotPos):
					return True
			elif "USE_CHANGE_ATTRIBUTE" == useType:
				if self.__CanChangeItemAttrList(dstSlotPos):
					return True
			elif "USE_ADD_ATTRIBUTE" == useType:
				if self.__CanAddItemAttr(dstSlotPos):
					return True
			elif "USE_ADD_ATTRIBUTE2" == useType:
				if self.__CanAddItemAttr(dstSlotPos):
					return True
			elif "USE_ADD_ACCESSORY_SOCKET" == useType:
				if self.__CanAddAccessorySocket(dstSlotPos):
					return True
			elif "USE_PUT_INTO_ACCESSORY_SOCKET" == useType:								
				if self.__CanPutAccessorySocket(dstSlotPos, srcItemVNum):
					return True;
			elif "USE_PUT_INTO_BELT_SOCKET" == useType:								
				dstItemVNum = player.GetItemIndex(dstSlotPos)
				print "USE_PUT_INTO_BELT_SOCKET", srcItemVNum, dstItemVNum

				item.SelectItem(dstItemVNum)
		
				if item.ITEM_TYPE_BELT == item.GetItemType():
					return True
			elif "USE_CHANGE_COSTUME_ATTR" == useType:
				if not app.ENABLE_ENCHANT_COSTUME_SYSTEM:
					return False
					
				if self.__CanChangeCostumeAttrList(dstSlotPos):
					return True;

			elif "USE_RESET_COSTUME_ATTR" == useType:
				if not app.ENABLE_ENCHANT_COSTUME_SYSTEM:
					return False
					
				if self.__CanResetCostumeAttr(dstSlotPos):
					return True;
				
		return False
		
	if app.ENABLE_UPGRADE_SOCKET_SYSTEM:
		def AttachUpgradeMetinToItem(self, srcSlotPos, dstSlotPos):
			dstItemVNum = player.GetItemIndex(dstSlotPos)
			if dstItemVNum == 0:
				return False
				
			item.SelectItem(dstItemVNum)
			
			if item.ITEM_TYPE_WEAPON != item.GetItemType() and item.ITEM_TYPE_ARMOR != item.GetItemType():
				return False
				
			item_name = item.GetItemName()	
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.UPGRADE_METIN_ITEM % (item_name))
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnAcceptUpgradeMetinItem))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			self.questionDialog.Open()
			self.questionDialog.sourcePos = srcSlotPos
			self.questionDialog.targetPos = dstSlotPos
			
		def OnAcceptUpgradeMetinItem(self):
			if self.questionDialog == None:
				return
				
			self.__SendUseItemToItemPacket(self.questionDialog.sourcePos, self.questionDialog.targetPos)
			self.OnCloseQuestionDialog()
		
	if app.ENABLE_ENCHANT_COSTUME_SYSTEM:
		def __CanChangeCostumeAttrList(self, dstSlotPos):
			dstItemVNum = player.GetItemIndex(dstSlotPos)
			if dstItemVNum == 0:
				return False

			item.SelectItem(dstItemVNum)

			if item.ITEM_TYPE_COSTUME != item.GetItemType() and not item.GetItemSubType() in (item.COSTUME_TYPE_BODY, item.COSTUME_TYPE_HAIR):
				return False

			for i in xrange(player.COSTUME_ATTRIBUTE_MAX_NUM):
				if player.GetItemAttribute(dstSlotPos, i) != 0:
					return True

			return False

		def __CanResetCostumeAttr(self, dstSlotPos):
			dstItemVNum = player.GetItemIndex(dstSlotPos)
			if dstItemVNum == 0:
				return False

			item.SelectItem(dstItemVNum)
			if item.ITEM_TYPE_COSTUME != item.GetItemType() and not item.GetItemSubType() in (item.COSTUME_TYPE_BODY, item.COSTUME_TYPE_HAIR):
				return False

			return True

	def __CanCleanBrokenMetinStone(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)
		
		if item.ITEM_TYPE_WEAPON != item.GetItemType():
			return False

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemMetinSocket(dstSlotPos, i) == constInfo.ERROR_METIN_STONE:
				return True

		return False

	def __CanChangeItemAttrList(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)
		
		if not item.GetItemType() in (item.ITEM_TYPE_WEAPON, item.ITEM_TYPE_ARMOR):	
			return False

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				return True

		return False
		
	def __CanPutAccessorySocket(self, dstSlotPos, mtrlVnum):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.GetItemType() != item.ITEM_TYPE_ARMOR:
			return False

		if not item.GetItemSubType() in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
			return False

		curCount = player.GetItemMetinSocket(dstSlotPos, 0)
		maxCount = player.GetItemMetinSocket(dstSlotPos, 1)

		if mtrlVnum != constInfo.GET_ACCESSORY_MATERIAL_VNUM(dstItemVNum, item.GetItemSubType()):
			return False
		
		if curCount>=maxCount:
			return False

		return True

	def __CanAddAccessorySocket(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)

		if item.GetItemType() != item.ITEM_TYPE_ARMOR:
			return False

		if not item.GetItemSubType() in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
			return False

		curCount = player.GetItemMetinSocket(dstSlotPos, 0)
		maxCount = player.GetItemMetinSocket(dstSlotPos, 1)
		
		ACCESSORY_SOCKET_MAX_SIZE = 3
		if maxCount >= ACCESSORY_SOCKET_MAX_SIZE:
			return False

		return True

	def __CanAddItemAttr(self, dstSlotPos):
		dstItemVNum = player.GetItemIndex(dstSlotPos)
		if dstItemVNum == 0:
			return False

		item.SelectItem(dstItemVNum)
		
		if not item.GetItemType() in (item.ITEM_TYPE_WEAPON, item.ITEM_TYPE_ARMOR):	 
			return False
			
		attrCount = 0
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if player.GetItemAttribute(dstSlotPos, i) != 0:
				attrCount += 1

		if attrCount<5:
			return True
								
		return False

	def ShowToolTip(self, slotIndex):
		if None != self.tooltipItem:
			self.tooltipItem.SetInventoryItem(slotIndex)

	def OnTop(self):
		if None != self.tooltipItem:
			self.tooltipItem.SetTop()
		#if app.WJ_TRADABLE_ICON:
			#map(lambda wnd:wnd.RefreshLockedSlot(), self.bindWnds)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def UseItemSlot(self, slotIndex):
	
		curCursorNum = app.GetCursor()
		if app.SELL == curCursorNum:
			return

		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS():
			return

		slotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(slotIndex)

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			if self.wndDragonSoulRefine.IsShow():
				self.wndDragonSoulRefine.AutoSetItem((player.INVENTORY, slotIndex), 1)
				return
				
		if app.ENABLE_CHANGELOOK_SYSTEM:
			if self.isShowChangeLookWindow():
				changelook.Add(player.INVENTORY, slotIndex, 255)
				return
				
		if app.ENABLE_ACCE_SYSTEM:
			itemVNum = player.GetItemIndex(slotIndex)
			if self.isShowSashWindow():
				if itemVNum >= 21900 and itemVNum <= 21999:
					return
				sash.Add(player.INVENTORY, slotIndex, 255)
				return
 
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			if self.isShowCombinationWindow():
				self.wndSkillBookCombination.AddItemWithoutMouse(slotIndex)
				return
				
		if self.isShowPetFeedWindow():
			itemVNum = player.GetItemIndex(slotIndex)
			itemCount = player.GetItemCount(slotIndex)
			self.wndPetFeedWindow.AddPetItemFromInventory(slotIndex, itemVNum, itemCount)
			return
			
		if app.ENABLE_AURA_SYSTEM:
			itemVNum = player.GetItemIndex(slotIndex)
			if self.isShowAuraWindow():
				if itemVNum >= 21900 and itemVNum <= 21999:
					return
				aura.Add(player.INVENTORY, slotIndex, 255)
				return
				
		if (self.game is not None and self.game.wndRR.IsShow()):
			self.game.wndRR.UseItemSlot(player.INVENTORY, slotIndex)
			return
 
		self.__UseItem(slotIndex)
		mouseModule.mouseController.DeattachObject()
		self.OverOutItem()

	if app.ENABLE_FAST_ATTACH_ITEMS_SYSTEM:
		def __GetCurrentItemGrid(self):
			itemGrid = [[False for slot in xrange(player.INVENTORY_PAGE_SIZE)] for page in xrange(player.INVENTORY_PAGE_COUNT)]

			for page in xrange(player.INVENTORY_PAGE_COUNT):
				for slot in xrange(player.INVENTORY_PAGE_SIZE):
					itemVnum = player.GetItemIndex(slot + page * player.INVENTORY_PAGE_SIZE)
					if itemVnum <> 0:
						(w, h) = item.GetItemSize(item.SelectItem(itemVnum))
						for i in xrange(h):
							itemGrid[page][slot + i * 5] = True

			return itemGrid

		def __FindEmptyCellForSize(self, itemGrid, size):
			for page in xrange(player.INVENTORY_PAGE_COUNT):
				for slot in xrange(player.INVENTORY_PAGE_SIZE):
					if itemGrid[page][slot] == False:
						possible = True
						for i in xrange(size):
							p = slot + (i * 5)

							try:
								if itemGrid[page][p] == True:
									possible = False
									break
							except IndexError:
								possible = False
								break

						if possible:
							return slot + page * player.INVENTORY_PAGE_SIZE

			return -1

		def AttachItemFromSafebox(self, slotIndex, itemIndex):
			itemGrid = self.__GetCurrentItemGrid()

			if item.GetItemType(item.SelectItem(itemIndex)) == item.ITEM_TYPE_DS:
				return

			emptySlotIndex = self.__FindEmptyCellForSize(itemGrid, item.GetItemSize()[1])
			if emptySlotIndex <> -1:
				net.SendSafeboxCheckoutPacket(slotIndex, player.INVENTORY, emptySlotIndex)

			return True

	def __UseItem(self, slotIndex):
		if app.ENABLE_FAST_ATTACH_ITEMS_SYSTEM:
			if self.interface and self.interface.AttachInvenItemToOtherWindowSlot(player.INVENTORY, slotIndex):#slotIndex is the argument of __UseItem
				return
		ItemVNum = player.GetItemIndex(slotIndex)
		item.SelectItem(ItemVNum)
		if item.IsFlag(item.ITEM_FLAG_CONFIRM_WHEN_USE):
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.INVENTORY_REALLY_USE_ITEM)
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnAccept))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnCancel))
			self.questionDialog.Open()
			self.questionDialog.slotIndex = slotIndex
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
		elif app.IsPressed(app.DIK_LCONTROL):
			if item.GetItemType() == item.ITEM_TYPE_GIFTBOX or item.GetItemType() == item.ITEM_TYPE_GACHA:
				for i in xrange(10):
					net.SendItemUsePacket(slotIndex)
		elif app.IsPressed(app.DIK_LALT):
			if item.GetItemType() == item.ITEM_TYPE_GIFTBOX or item.GetItemType() == item.ITEM_TYPE_GACHA:
				if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
					if self.interface:
						if self.interface.dlgChestDrop:
							if not self.interface.dlgChestDrop.IsShow():
								self.interface.dlgChestDrop.Open(slotIndex)
								net.SendChestDropInfo(slotIndex)
		else:
			self.__SendUseItemPacket(slotIndex)

	def __UseItemQuestionDialog_OnCancel(self):
		self.OnCloseQuestionDialog()

	def __UseItemQuestionDialog_OnAccept(self):
		self.__SendUseItemPacket(self.questionDialog.slotIndex)
		self.OnCloseQuestionDialog()

	def __SendUseItemToItemPacket(self, srcSlotPos, dstSlotPos):
		# ∞≥¿ŒªÛ¡° ø≠∞Ì ¿÷¥¬ µøæ» æ∆¿Ã≈€ ªÁøÎ πÊ¡ˆ
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_PRIVATE_SHOP)
			return
				
		net.SendItemUseToItemPacket(srcSlotPos, dstSlotPos)
		
	def __SendUseItemToItemPacketAttr(self, srcSlotPos, dstSlotPos):
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_PRIVATE_SHOP)
			return
				
		net.SendItemUseToItemPacket(player.ATTR_ITEMS_INVENTORY, srcSlotPos, player.INVENTORY, dstSlotPos)

	def __SendUseItemPacket(self, slotPos):
		# ∞≥¿ŒªÛ¡° ø≠∞Ì ¿÷¥¬ µøæ» æ∆¿Ã≈€ ªÁøÎ πÊ¡ˆ
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_PRIVATE_SHOP)
			return
			
		net.SendItemUsePacket(slotPos)
	
	def __SendMoveItemPacket(self, srcSlotPos, dstSlotPos, srcItemCount):
		# ∞≥¿ŒªÛ¡° ø≠∞Ì ¿÷¥¬ µøæ» æ∆¿Ã≈€ ªÁøÎ πÊ¡ˆ
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
			return
			
		net.SendItemMovePacket(srcSlotPos, dstSlotPos, srcItemCount)

	def SetDragonSoulRefineWindow(self, wndDragonSoulRefine):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoulRefine = wndDragonSoulRefine
			
	if app.ENABLE_CHANGELOOK_SYSTEM:
		def SetChangeLookWindow(self, wndChangeLook):
			self.wndChangeLook = wndChangeLook

		def isShowChangeLookWindow(self):
			if self.wndChangeLook:
				if self.wndChangeLook.IsShow():
					return 1
			
			return 0
			
	if app.ENABLE_ACCE_SYSTEM:
		def SetSashWindow(self, wndSashCombine, wndSashAbsorption):
			self.wndSashCombine = wndSashCombine
			self.wndSashAbsorption = wndSashAbsorption

		def isShowSashWindow(self):
			if self.wndSashCombine:
				if self.wndSashCombine.IsShow():
					return 1

			if self.wndSashAbsorption:
				if self.wndSashAbsorption.IsShow():
					return 1
			
			return 0
			
	if app.ENABLE_ITEM_COMBINATION_SYSTEM:
		def SetCombinationWindow(self, wndItemCombination, wndSkillBookCombination):
			self.wndItemCombination = wndItemCombination
			self.wndSkillBookCombination = wndSkillBookCombination

		def isShowCombinationWindow(self):					
			if self.wndSkillBookCombination:
				if self.wndSkillBookCombination.IsShow():
					return 1

			return 0
			
	def SetPetFeedWindow(self, wndPetFeedWindow):
		self.wndPetFeedWindow = wndPetFeedWindow
	
	def isShowPetFeedWindow(self):
		if self.wndPetFeedWindow:
			if self.wndPetFeedWindow.IsShow():
				return 1
				
		return 0
		
	if app.ENABLE_AURA_SYSTEM:
		def SetAuraWindow(self, wndAuraAbsorption, wndAuraRefine):
			self.wndAuraRefine = wndAuraRefine
			self.wndAuraAbsorption = wndAuraAbsorption

		def isShowAuraWindow(self):
			if self.wndAuraRefine:
				if self.wndAuraRefine.IsShow():
					return 1

			if self.wndAuraAbsorption:
				if self.wndAuraAbsorption.IsShow():
					return 1
			
			return 0
			
	def __SetCooldownForTimedItemSlot(self, slotNumber, itemVnum):
		if itemVnum == 0:
			return

		item.SelectItem(itemVnum)
		if not constInfo.IS_PET_ITEM(itemVnum) and not constInfo.IS_NEW_PET_ITEM(itemVnum) and not constInfo.IS_MOUNT_ITEM(itemVnum) and not constInfo.IS_SUPPORT_SHAMAN_ITEM(itemVnum):
			return
			
		if player.IsBasicItem(slotNumber):
			return

		if constInfo.IS_NEW_PET_ITEM(itemVnum):
			limitValueTmp	= player.GetItemAttribute(slotNumber, 4)
			limitValue = abs(limitValueTmp[1])*60
			sock_time	= player.GetItemMetinSocket(slotNumber, 2)-app.GetGlobalTimeStamp()
		else:
			(limitType, limitValue) = item.GetLimit(0)
			sock_time   = player.GetItemMetinSocket(slotNumber, 0)-app.GetGlobalTimeStamp()
	
		if slotNumber >= player.INVENTORY_PAGE_SIZE:
			slotNumber -= player.INVENTORY_PAGE_SIZE
			
		if sock_time > 0 and limitValue > 0:
			self.wndItem.SetSlotCoolTimeInverse(slotNumber, limitValue, limitValue-sock_time)
			
	def OnMoveWindow(self, x, y):
		if self.wndBelt:
			self.wndBelt.AdjustPositionAndSize()
			
