import net
import player
import chr
import item
import app
import shop
import chat
import systemSetting
import ui
import snd
import mouseModule
import uiCommon
import localeInfo
import constInfo
import os

##### DISABLE/ENABLE SHOP VISIT COLOR ###
SHOP_VISIT=True
##### SHOP VISIT COLOR #####
SHOP_VISIT_COLOR=0xFF00C8FF
###########################
g_isBuildingPrivateShop = False

g_itemPriceDict={}
g_itemChequeDict={}
g_iTotalGold = 0
g_iTotalCheque = 0

g_privateShopAdvertisementBoardDict={}
g_displayedBoardVIDList = []

if app.ENABLE_SHOPNAMES_RANGE:
	SHOP_NAME_RANGE = 5000
	from math import sqrt

	def DISTANCE_SQRT(dx, dy):
		return sqrt(dx * dx + dy * dy)


	def RefreshADBoard():
		global g_privateShopAdvertisementBoardDict
		try:
			for v in g_privateShopAdvertisementBoardDict.itervalues():
				if not v.IsShow():
					v.OnUpdate()

		except RuntimeError:
			pass

def Clear():
	global g_itemPriceDict
	global g_isBuildingPrivateShop
	g_itemPriceDict={}
	g_isBuildingPrivateShop = False
	global g_itemChequeDict
	g_itemChequeDict={}
	
def ClearBoardDict():
	global g_privateShopAdvertisementBoardDict
	g_privateShopAdvertisementBoardDict.clear()

def IsPrivateShopItemPriceList():
	global g_itemPriceDict
	if g_itemPriceDict:
		return True
	else:
		return False
		
def IsBuildingPrivateShop():
	global g_isBuildingPrivateShop
	if player.IsOpenPrivateShop() or g_isBuildingPrivateShop:
		return True
	else:
		return False

def SetPrivateShopItemPrice(itemVNum, itemPrice):
	global g_itemPriceDict
	g_itemPriceDict[int(itemVNum)]=itemPrice

def GetPrivateShopItemPrice(itemVNum):
	try:
		global g_itemPriceDict
		return g_itemPriceDict[itemVNum]
	except KeyError:
		return 0
		
def UpdateADBoard():
	for key in g_privateShopAdvertisementBoardDict.keys():
		g_privateShopAdvertisementBoardDict[key].Show()

def DeleteADBoard(vid):
	if not g_privateShopAdvertisementBoardDict.has_key(vid):
		return

	del g_privateShopAdvertisementBoardDict[vid]
	
def IsPrivateShopItemChequePriceList():
	global g_itemChequeDict
	if g_itemChequeDict:
		return True
	return False

def SetPrivateShopItemCheque(itemVNum, itemCheque):
	global g_itemChequeDict
	g_itemChequeDict[int(itemVNum)]=itemCheque

def GetPrivateShopItemCheque(itemVNum):
	try:
		global g_itemChequeDict
		return g_itemChequeDict[itemVNum]
	except KeyError:
		return 0
	
class PrivateShopAdvertisementBoard(ui.ThinBoard):
	
	def __init__(self):
		ui.ThinBoard.__init__(self, "UI_BOTTOM")
		self.shopAdvertismentBoardSeen = []
		self.vid = None
		self.__MakeTextLine()

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.Show()

	def Open(self, vid, text):
		self.vid = vid

		self.textLine.SetText(text)
		if vid in self.shopAdvertismentBoardSeen:
			self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
		self.textLine.UpdateRect()
		self.SetSize(len(text)*6 + 10*2, 20)
		self.Show() 
				
		g_privateShopAdvertisementBoardDict[vid] = self

	def OnMouseLeftButtonUp(self):
		if not self.vid:
			return
		net.SendOnClickPacket(self.vid)
		if self.vid != player.GetMainCharacterIndex():
			self.textLine.SetPackedFontColor(SHOP_VISIT_COLOR)
			self.shopAdvertismentBoardSeen.append(self.vid)
		return True

	def OnUpdate(self):
		if not self.vid:
			return
			
		if (systemSetting.IsShowSalesText() and systemSetting.IsShowOfflineShop()):
			if app.ENABLE_SHOPNAMES_RANGE:
				try:
					x1, y1, z1 = chr.GetPixelPosition(self.vid)
					x2, y2, z2 = chr.GetPixelPosition(player.GetMainCharacterIndex())
					r1 = systemSetting.GetShopNamesRange()
					d1 = DISTANCE_SQRT(x1 - x2, y1 - y2)
					if d1 <= r1 * SHOP_NAME_RANGE:
						self.Show()
						x, y = chr.GetProjectPosition(self.vid, 220)
						self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
					else:
						self.Hide()
				except (RuntimeError, TypeError):
					self.Hide()
					DeleteADBoard(self.vid)

			else:
				self.Show()
				x, y = chr.GetProjectPosition(self.vid, 220)
				self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			self.Hide()

class PrivateShopBuilder(ui.ScriptWindow):

	def __init__(self):
		#print "NEW MAKE_PRIVATE_SHOP_WINDOW ----------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)

		self.__LoadWindow()
		self.itemStock = {}
		self.tooltipItem = None
		self.priceInputBoard = None
		self.title = ""
		self.days = 0
		self.model_index = 0
		if app.WJ_TRADABLE_ICON:
			self.interface = None
			self.wndInventory = None
			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}

	def __del__(self):
		#print "------------------------------------------------------------- DELETE MAKE_PRIVATE_SHOP_WINDOW"
		ui.ScriptWindow.__del__(self)

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PrivateShopBuilder.py")
		except:
			import exception as exception
			exception.Abort("PrivateShopBuilderWindow.LoadWindow.LoadObject")

		try:
			GetObject = self.GetChild
			self.nameLine = GetObject("NameLine")
			self.itemSlot = GetObject("ItemSlot")
			self.btnOk = GetObject("OkButton")
			self.btnClose = GetObject("CloseButton")
			self.titleBar = GetObject("TitleBar")
			self.totalGold = self.GetChild('TotalGold')
			self.totalCheque = self.GetChild('TotalCheque')
		except:
			import exception as exception
			exception.Abort("PrivateShopBuilderWindow.LoadWindow.BindObject")

		self.btnOk.SetEvent(ui.__mem_func__(self.OnOk))
		self.btnClose.SetEvent(ui.__mem_func__(self.OnClose))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.OnClose))

		self.itemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.itemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))

	def Destroy(self):
		self.ClearDictionary()

		self.nameLine = None
		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.titleBar = None
		self.priceInputBoard = None
		if app.WJ_TRADABLE_ICON:
			self.interface = None
			self.wndInventory = None
			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}

	def Open(self, title,days, npc_no, interface):
		self.title = title
		self.days = days
		self.model_index = npc_no
		if len(title) > 25:
			title = title[:22] + "..."

		self.itemStock = {}
		shop.ClearPrivateShopStock()
		self.nameLine.SetText(title)
		self.SetCenterPosition()
		self.Refresh()
		
		self.Show()
		
		if app.WJ_TRADABLE_ICON:
			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}
			self.interface = interface
			self.SetInven(self.interface.GetInventoryHandle())
			self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
			self.interface.RefreshMarkInventoryBag()

		global g_isBuildingPrivateShop
		g_isBuildingPrivateShop = True

	def Close(self):
		global g_isBuildingPrivateShop
		g_isBuildingPrivateShop = False

		self.title = ""
		self.itemStock = {}
		shop.ClearPrivateShopStock()
		if self.priceInputBoard:
			self.priceInputBoard.Close()
			self.priceInputBoard = None
		global g_iTotalGold
		g_iTotalGold = 0
		global g_iTotalCheque
		g_iTotalCheque = 0
		if app.WJ_TRADABLE_ICON:
			for privatePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
				if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
					self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}
			self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
			self.interface.RefreshMarkInventoryBag()
		self.Hide()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def Refresh(self):
		getitemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setitemVNum=self.itemSlot.SetItemSlot
		delItem=self.itemSlot.ClearSlot

		for i in xrange(shop.SHOP_SLOT_COUNT):

			if not self.itemStock.has_key(i):
				delItem(i)
				continue

			pos = self.itemStock[i]

			itemCount = getItemCount(*pos)
			if itemCount <= 1:
				itemCount = 0
			setitemVNum(i, getitemVNum(*pos), itemCount)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				itemTransmutedVnum = player.GetItemTransmutation(*pos)
				if itemTransmutedVnum:
					self.itemSlot.DisableCoverButton(i)
				else:
					self.itemSlot.EnableCoverButton(i)

		self.itemSlot.RefreshSlot()
		if app.WJ_TRADABLE_ICON:
			self.RefreshLockedSlot()
		global g_iTotalGold
		global g_iTotalCheque
		self.totalGold.SetText(localeInfo.NumberToString(g_iTotalGold))
		self.totalCheque.SetText(localeInfo.NumberToString(g_iTotalCheque))	
		
	def RefreshMoney(self, bAdd, invenType, invenPos):
		global g_iTotalGold
		global g_iTotalCheque
		if bAdd == True:
			g_iTotalGold += shop.GetPrivateShopItemPrice(invenType, invenPos)
			g_iTotalCheque += shop.GetPrivateShopItemPriceCheque(invenType, invenPos)
		else:
			g_iTotalGold -= shop.GetPrivateShopItemPrice(invenType, invenPos)
			g_iTotalCheque -= shop.GetPrivateShopItemPriceCheque(invenType, invenPos)			
		self.totalGold.SetText(localeInfo.NumberToString(g_iTotalGold))
		self.totalCheque.SetText(localeInfo.NumberToString(g_iTotalCheque))		

	def OnSelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if (player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType and player.SLOT_TYPE_SKILL_BOOK_INVENTORY != attachedSlotType and player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY != attachedSlotType and player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType and player.SLOT_TYPE_CHEST_INVENTORY != attachedSlotType and player.SLOT_TYPE_ATTR_INVENTORY != attachedSlotType and player.SLOT_TYPE_FLOWER_INVENTORY != attachedSlotType):
				return
			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
			count = player.GetItemCount(attachedInvenType, attachedSlotPos)
				
			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)

			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PRIVATE_SHOP_CANNOT_SELL_ITEM)
				return
				
			priceInputBoard = uiCommon.MoneyInputDialogCheque()
			
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(selectedSlotPos, attachedSlotPos)

			priceInputBoard.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			priceInputBoard.SetMaxLength(10)
			priceInputBoard.SetMaxLengthCheque(3)
			priceInputBoard.Open()

			itemPrice=GetPrivateShopItemPrice(itemVNum)
			itemPriceCheque=GetPrivateShopItemCheque(itemVNum)

			if itemPrice>0:
				priceInputBoard.SetValue(itemPrice)
			if itemPriceCheque>0:
				priceInputBoard.SetChequeValue(itemPriceCheque)
			
			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos

	def OnSelectItemSlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()

		else:
			if not selectedSlotPos in self.itemStock:
				return

			invenType, invenPos = self.itemStock[selectedSlotPos]
			shop.DelPrivateShopItemStock(invenType, invenPos)
			snd.PlaySound("sound/ui/drop.wav")
			
			if app.WJ_TRADABLE_ICON:
				(itemInvenPage, itemSlotPos) = self.lockedItems[selectedSlotPos]
				if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
					self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

				self.lockedItems[selectedSlotPos] = (-1, -1)

			del self.itemStock[selectedSlotPos]

			self.Refresh()

	def AcceptInputPrice(self):

		if not self.priceInputBoard:
			return True

		text = self.priceInputBoard.GetText()
		text2 = self.priceInputBoard.GetChequeText()
		
		if not text:
			text = 0
		if not text2:
			text2 = 0

		if (text and not text.isdigit()) or (text2 and not text2.isdigit()):
			return True
		
		attachedInvenType = self.priceInputBoard.sourceWindowType
		sourceSlotPos = self.priceInputBoard.sourceSlotPos
		targetSlotPos = self.priceInputBoard.targetSlotPos

		for privatePos, (itemWindowType, itemSlotIndex) in self.itemStock.items():
			if itemWindowType == attachedInvenType and itemSlotIndex == sourceSlotPos:
				shop.DelPrivateShopItemStock(itemWindowType, itemSlotIndex)
				del self.itemStock[privatePos]

		price = int(self.priceInputBoard.GetText())
		cheque = int(self.priceInputBoard.GetChequeText())

		if IsPrivateShopItemPriceList():
			SetPrivateShopItemPrice(self.priceInputBoard.itemVNum, price)
		if IsPrivateShopItemChequePriceList():
			SetPrivateShopItemCheque(self.priceInputBoard.itemVNum, cheque)
	
		shop.AddPrivateShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price, cheque)
		count = player.GetItemCount(attachedInvenType, sourceSlotPos)
		vnum = player.GetItemIndex(attachedInvenType, sourceSlotPos)
		self.itemStock[targetSlotPos] = (attachedInvenType, sourceSlotPos)
		self.RefreshMoney(True, attachedInvenType, sourceSlotPos)
		snd.PlaySound("sound/ui/drop.wav")

		self.Refresh()		
		self.priceInputBoard = None
		return True
		
	def CancelInputPrice(self):
		if app.WJ_TRADABLE_ICON:
			itemInvenPage = self.priceInputBoard.sourceSlotPos / player.INVENTORY_PAGE_SIZE
			itemSlotPos = self.priceInputBoard.sourceSlotPos - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
				self.wndInventory.wndItem.SetCanMouseEventSlot(itemSlotPos)

			self.lockedItems[self.priceInputBoard.targetSlotPos] = (-1, -1)

		if self.priceInputBoard:
			self.priceInputBoard.Close()
		self.priceInputBoard = None
		return 1

	def OnOk(self):

		if not self.title:
			return

		if 0 == len(self.itemStock):
			return
			
		shop.BuildPrivateShop(self.title,self.days,self.model_index)
		#self.Close()

	def OnClose(self):
		self.Close()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnOverInItem(self, slotIndex):

		if self.tooltipItem:
			if self.itemStock.has_key(slotIndex):
				self.tooltipItem.SetPrivateShopBuilderItem(*self.itemStock[slotIndex] + (slotIndex,))

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
	def OnUpdate(self):
		if app.WJ_TRADABLE_ICON:
			self.RefreshLockedSlot()

	if app.WJ_TRADABLE_ICON:
		def CantTradableItem(self, destSlotIndex, srcSlotIndex):
			itemInvenPage = srcSlotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = srcSlotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItems[destSlotIndex] = (itemInvenPage, localSlotPos)
			if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
				self.wndInventory.wndItem.SetCantMouseEventSlot(localSlotPos)

		def RefreshLockedSlot(self):
			if self.wndInventory:
				for privatePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
					if self.wndInventory.GetInventoryPageIndex() == itemInvenPage:
						self.wndInventory.wndItem.SetCantMouseEventSlot(itemSlotPos)

				self.wndInventory.wndItem.RefreshSlot()

		def BindInterface(self, interface):
			self.interface = interface

		def OnTop(self):
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
				self.interface.RefreshMarkInventoryBag()

		def SetInven(self, wndInventory):
			from _weakref import proxy
			self.wndInventory = proxy(wndInventory)
			
