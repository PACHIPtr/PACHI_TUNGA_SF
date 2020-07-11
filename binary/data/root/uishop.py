import net
import app
import player
import item
import chr
import shop
import chat
import exchange
import background
import snd
import wndMgr
import constInfo
import ui
import uiCommon
import mouseModule
import localeInfo
import uiScriptLocale
import time
###################################################################################################
## Shop
class ShopDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = 0
		self.xShopStart = 0
		self.yShopStart = 0
		self.vid = 0
		self.questionDialog = None
		self.popup = None
		self.itemBuyQuestionDialog = None	
		if app.WJ_TRADABLE_ICON:
			self.interface = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __GetRealIndex(self, i):
		return self.tabIdx * shop.SHOP_SLOT_COUNT + i

	def Refresh(self):
		getItemID=shop.GetItemID
		getItemCount=shop.GetItemCount
		setItemID=self.itemSlotWindow.SetItemSlot
		for i in xrange(shop.SHOP_SLOT_COUNT):
			idx = self.__GetRealIndex(i)
			itemCount = getItemCount(idx)
			if itemCount <= 1:
				itemCount = 0
			setItemID(i, getItemID(idx), itemCount, 4)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				itemTransmutedVnum = shop.GetItemTransmutation(idx)
				if itemTransmutedVnum:
					self.itemSlotWindow.DisableCoverButton(i)
				else:
					self.itemSlotWindow.EnableCoverButton(i)
					
			if app.ENABLE_SOLD_ITEM_SYSTEM:
				if shop.GetItemIsSold(i) == 1:
					self.RegisterShopSold(i)
				else:
					if self.itemSlotWindow.HasCoverButton(i):
						self.itemSlotWindow.DeleteCoverButton(i)

		wndMgr.RefreshSlot(self.itemSlotWindow.GetWindowHandle())
			
	if app.ENABLE_SOLD_ITEM_SYSTEM:
		def RegisterShopSold(self, pos):
			if pos < 0 or not shop.GetItemID(pos):
				return

			item.SelectItem(int(shop.GetItemID(pos)))
			(itemWidth, itemHeight) = item.GetItemSize()

			targetSlot = self.itemSlotWindow
			imageName = "d:/ymir work/ui/sold_icon/slot_disabled_%d.tga" % itemHeight

			targetSlot.SetCoverButton(pos, imageName, imageName, imageName, "d:/ymir work/ui/game/belt_inventory/slot_disabled.tga", False, False)
			targetSlot.EnableSlot(pos)
			wndMgr.RefreshSlot(targetSlot.GetWindowHandle())

	def SetItemData(self, pos, itemID, itemCount, itemPrice):
		shop.SetItemData(pos, itemID, itemCount, itemPrice)

	def LoadDialog(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/shopdialog.py")
		except:
			import exception as exception
			exception.Abort("ShopDialog.LoadDialog.LoadObject")

		smallTab1 = None
		smallTab2 = None
		smallTab3 = None
		middleTab1 = None
		middleTab2 = None

		try:
			GetObject = self.GetChild
			self.board = GetObject("board")
			self.itemSlotWindow = GetObject("ItemSlot")
			self.btnBuy = GetObject("BuyButton")
			self.btnSell = GetObject("SellButton")
			self.btnClose = GetObject("CloseButton")
			self.titleBar = GetObject("TitleBar")
			self.titleName = GetObject('TitleName')
			middleTab1 = GetObject("MiddleTab1")
			middleTab2 = GetObject("MiddleTab2")
			smallTab1 = GetObject("SmallTab1")
			smallTab2 = GetObject("SmallTab2")
			smallTab3 = GetObject("SmallTab3")
			if (app.ENABLE_BATTLE_ZONE_SYSTEM):
				self.boardBattleShop = GetObject("BattleShopSubBoard")
				self.curPoints = GetObject("BattleShopSubInfo1")
				self.curLimitPoints = GetObject("BattleShopSubInfo2")
			###ENABLE_SHOP_SEEN###
			self.Bakannlar = self.GetChild("Bakanlar")
			self.Kackisibakmis = self.GetChild("Kackisibakmis")
			######################
		except:
			import exception as exception
			exception.Abort("ShopDialog.LoadDialog.BindObject")

		self.realWidth, self.realHeight = self.GetWidth(), self.GetHeight()
		self.realTitleBarX = self.titleBar.GetWidth()

		self.itemSlotWindow.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlotWindow.SAFE_SetButtonEvent("LEFT", "EMPTY", self.SelectEmptySlot)
		self.itemSlotWindow.SAFE_SetButtonEvent("LEFT", "EXIST", self.SelectItemSlot)
		self.itemSlotWindow.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
	
		self.itemSlotWindow.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlotWindow.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.btnBuy.SetToggleUpEvent(ui.__mem_func__(self.CancelShopping))
		self.btnBuy.SetToggleDownEvent(ui.__mem_func__(self.OnBuy))

		self.btnSell.SetToggleUpEvent(ui.__mem_func__(self.CancelShopping))
		self.btnSell.SetToggleDownEvent(ui.__mem_func__(self.OnSell))

		self.btnClose.SetEvent(ui.__mem_func__(self.AskClosePrivateShop))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		
		self.smallRadioButtonGroup = ui.RadioButtonGroup.Create([[smallTab1, lambda : self.OnClickTabButton(0), None], [smallTab2, lambda : self.OnClickTabButton(1), None], [smallTab3, lambda : self.OnClickTabButton(2), None]])
		self.middleRadioButtonGroup = ui.RadioButtonGroup.Create([[middleTab1, lambda : self.OnClickTabButton(0), None], [middleTab2, lambda : self.OnClickTabButton(1), None]])

		self.__HideMiddleTabs()
		self.__HideSmallTabs()
		self.tabIdx = 0
		self.coinType = shop.SHOP_COIN_TYPE_GOLD
		self.Refresh()

	def __ShowBuySellButton(self):
		self.btnBuy.Show()
		self.btnSell.Show()
		
	def __ShowMiddleTabs(self):
		self.middleRadioButtonGroup.Show()
	
	def __ShowSmallTabs(self):
		self.smallRadioButtonGroup.Show()
	
	def __HideBuySellButton(self):
		self.btnBuy.Hide()
		self.btnSell.Hide()
	
	def __HideMiddleTabs(self):
		self.middleRadioButtonGroup.Hide()
	
	def __HideSmallTabs(self):
		self.smallRadioButtonGroup.Hide()
		
	def __SetTabNames(self):
		if shop.GetTabCount() == 2:
			self.middleRadioButtonGroup.SetText(0, shop.GetTabName(0))
			self.middleRadioButtonGroup.SetText(1, shop.GetTabName(1))
		elif shop.GetTabCount() == 3:
			self.smallRadioButtonGroup.SetText(0, shop.GetTabName(0))
			self.smallRadioButtonGroup.SetText(1, shop.GetTabName(1))
			self.smallRadioButtonGroup.SetText(2, shop.GetTabName(2))

	def Destroy(self):
		if app.WJ_TRADABLE_ICON:
			self.Close(True)
			self.interface = None
		else:
			self.Close()
		self.ClearDictionary()

		self.tooltipItem = 0
		self.itemSlotWindow = 0
		self.btnBuy = 0
		self.btnSell = 0
		self.btnClose = 0
		self.titleBar = 0
		self.vid = 0
		self.questionDialog = None
		self.popup = None
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			self.boardBattleShop = 0
			self.curPoints = 0
			self.curLimitPoints = 0
			
	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		def SetCombatZoneBoard(self, flag):
			if flag:
				self.boardBattleShop.Show()
			else:
				self.boardBattleShop.Hide()
	
		def SetCombatZonePoints(self, points):
			self.curPoints.SetText(localeInfo.COMBAT_ZONE_CURRENT_REAL_POINTS % points)
			
		def SetLimitCombatZonePoints(self, curLimit, maxLimit):
			self.curLimitPoints.SetText(localeInfo.COMBAT_ZONE_LIMIT_SHOP_POINTS % (curLimit, maxLimit))

	def Open(self, vid, points=0, curLimit=0, maxLimit=0,count=0):
		self.vid=int(vid)
		isPrivateShop = False
		isMainPlayerPrivateShop = False
		myshop=False
		for i in xrange(len(constInfo.MyShops)):
			if int(constInfo.MyShops[i]["vid"]) == int(self.vid):
				myshop=True
				self.vid=int(constInfo.MyShops[i]["id"])
		chr.SelectInstance(self.vid)
		if (chr.GetRace() == 30000 or chr.GetRace() == 30001 or chr.GetRace() == 30002 or chr.GetRace() == 30003 or chr.GetRace() == 30004 or chr.GetRace() == 30005 or chr.GetRace() == 30006 or chr.GetRace() == 30007) or not chr.IsNPC(self.vid):
			isPrivateShop = True
		if player.IsMainCharacterIndex(self.vid):
			myshop=True
			self.vid=""
		if myshop == True:
			isMainPlayerPrivateShop = True

			self.btnBuy.Hide()
			self.btnSell.Hide()
			self.btnClose.Show()
			self.btnClose.SetPosition(0, 357)
			
			if vid != 0:
				self.titleName.SetText("%s'in D\xfckkan\xfd" % chr.GetNameByVID(vid))
			else:
				self.titleName.SetText(uiScriptLocale.SHOP_TITLE)
		else:

			isMainPlayerPrivateShop = False

			self.btnBuy.Show()
			self.btnSell.Show()
			self.btnClose.Hide()

			self.SetSize(self.realWidth, self.realHeight)	
			self.titleBar.SetWidth(self.realTitleBarX)
			if vid != 0:
				self.titleName.SetText("%s'in D\xfckkan\xfd" % chr.GetNameByVID(vid))
			else:
				self.titleName.SetText(uiScriptLocale.SHOP_TITLE)
		
		if vid == 0:
			myshop = False
			isPrivateShop = False
			isMainPlayerPrivateShop = False

		if isPrivateShop == True:
			self.Kackisibakmis.Show()
			self.Bakannlar.Show()
			self.Bakannlar.SetText(str(count) + localeInfo.SHOP_SEEN_TEXT)
			if isMainPlayerPrivateShop:
				self.board.SetSize(184, 385)
			else:
				self.board.SetSize(184, 358)
		else:
			self.Kackisibakmis.Hide()
			self.Bakannlar.Hide()
			self.board.SetSize(184, 360)
			self.SetSize(184, 360)
			self.btnBuy.SetPosition(21, 327)
			self.btnSell.SetPosition(104, 327)
			
		shop.Open(isPrivateShop, isMainPlayerPrivateShop)
	
		self.tabIdx = 0
	
		if shop.GetTabCount() == 1:
			self.__ShowBuySellButton()
			self.__HideMiddleTabs()
			self.__HideSmallTabs()
		elif shop.GetTabCount() == 2:
			self.__HideBuySellButton()
			self.__ShowMiddleTabs()
			self.__HideSmallTabs()
			self.__SetTabNames()
			self.middleRadioButtonGroup.OnClick(0)
		elif shop.GetTabCount() == 3:
			self.__HideBuySellButton()
			self.__HideMiddleTabs()
			self.__ShowSmallTabs()
			self.__SetTabNames()
			self.middleRadioButtonGroup.OnClick(1)
			
		if isMainPlayerPrivateShop == True or isPrivateShop == True:
			self.btnBuy.Hide()
			self.btnSell.Hide()
				
		self.Refresh()
		self.SetTop()
		
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			if shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
				self.SetCombatZonePoints(points)
				self.SetLimitCombatZonePoints(curLimit, maxLimit)
				self.SetCombatZoneBoard(True)
			else:
				self.SetCombatZoneBoard(False)
		
		self.Show()
	
		(self.xShopStart, self.yShopStart, z) = player.GetMainCharacterPosition()
		if app.WJ_TRADABLE_ICON:
			if not isPrivateShop:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
				self.interface.RefreshMarkInventoryBag()

	if app.WJ_TRADABLE_ICON:
		def Close(self, isDestroy=False):
			self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
			if not isDestroy:
				self.interface.RefreshMarkInventoryBag()

			if self.itemBuyQuestionDialog:
				self.itemBuyQuestionDialog.Close()
				self.itemBuyQuestionDialog = None
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
					
			self.OnCloseQuestionDialog()
			shop.Close()
			net.SendShopEndPacket()
			self.CancelShopping()
			self.tooltipItem.HideToolTip()
			self.Hide()
	else:
		def Close(self):
			if self.itemBuyQuestionDialog:
				self.itemBuyQuestionDialog.Close()
				self.itemBuyQuestionDialog = None
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

			self.OnCloseQuestionDialog()
			shop.Close()
			net.SendShopEndPacket()
			self.CancelShopping()
			self.tooltipItem.HideToolTip()
			self.Hide()

	def GetIndexFromSlotPos(self, slotPos):
		return self.tabIdx * shop.SHOP_SLOT_COUNT + slotPos
		
	def OnClickTabButton(self, idx):
		self.tabIdx = idx
		self.Refresh()

	def AskClosePrivateShop(self):
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.PRIVATE_SHOP_CLOSE_QUESTION)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnClosePrivateShop))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

		return True

	def OnClosePrivateShop(self):
		net.SendChatPacket("/close_shop "+str(self.vid))
		self.OnCloseQuestionDialog()
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnPressExitKey(self):
		self.Close()
		return True

	def OnBuy(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)
		app.SetCursor(app.BUY)
		self.btnSell.SetUp()

	def OnSell(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_SELL_INFO)
		app.SetCursor(app.SELL)
		self.btnBuy.SetUp()

	def CancelShopping(self):
		self.btnBuy.SetUp()
		self.btnSell.SetUp()
		app.SetCursor(app.NORMAL)

	def __OnClosePopupDialog(self):
		self.pop = None

	def SellAttachedItem(self):

		if shop.IsPrivateShop():
			mouseModule.mouseController.DeattachObject()
			return
			
		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedCount = mouseModule.mouseController.GetAttachedItemCount()
		attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
		if player.SLOT_TYPE_INVENTORY == attachedSlotType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType or player.SLOT_TYPE_SKILL_BOOK_INVENTORY == attachedSlotType or player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY == attachedSlotType or player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType:

			itemIndex = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemIndex)

			if item.IsAntiFlag(item.ANTIFLAG_SELL):
				popup = uiCommon.PopupDialog()
				popup.SetText(localeInfo.SHOP_CANNOT_SELL_ITEM)
				popup.SetAcceptEvent(self.__OnClosePopupDialog)
				popup.Open()
				self.popup = popup

			elif player.IsValuableItem(attachedSlotPos):

				itemPrice = player.GetISellItemPrice(attachedInvenType,attachedSlotPos)

				itemName = item.GetItemName()

				questionDialog = uiCommon.QuestionDialog()
				questionDialog.SetText(localeInfo.DO_YOU_SELL_ITEM(itemName, attachedCount, itemPrice))

				questionDialog.SetAcceptEvent(lambda arg1=attachedSlotPos, arg2=attachedCount, arg3=attachedInvenType: self.OnSellItem(arg1, arg2, arg3))
				questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
				questionDialog.Open()
				self.questionDialog = questionDialog

			else:
				self.OnSellItem(attachedSlotPos, attachedCount, attachedInvenType)

		else:
			snd.PlaySound("sound/ui/loginfail.wav")

		mouseModule.mouseController.DeattachObject()

	def OnSellItem(self, slotPos, count, inventype):
		net.SendShopSellPacketNew(slotPos, count, inventype)
		snd.PlaySound("sound/ui/money.wav")
		self.OnCloseQuestionDialog()

	def OnCloseQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None

	def SelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			self.SellAttachedItem()

	def UnselectItemSlot(self, selectedSlotPos):
		if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
			return
		if shop.IsPrivateShop():
			self.AskBuyItem(selectedSlotPos)
		else:
			if (app.ENABLE_BATTLE_ZONE_SYSTEM):
				if shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
					self.AskBuyItem(selectedSlotPos)
				else:
					net.SendShopBuyPacket(self.__GetRealIndex(selectedSlotPos))	
			else:
				net.SendShopBuyPacket(self.__GetRealIndex(selectedSlotPos))	

	def SelectItemSlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			self.SellAttachedItem()

		else:

			if True == shop.IsMainPlayerPrivateShop():
				return

			curCursorNum = app.GetCursor()
			if app.BUY == curCursorNum:
				self.AskBuyItem(selectedSlotPos)

			elif app.SELL == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_SELL_INFO)

			else:
				selectedItemID = shop.GetItemID(selectedSlotPos)
				itemCount = shop.GetItemCount(selectedSlotPos)

				type = player.SLOT_TYPE_SHOP
				if shop.IsPrivateShop():
					type = player.SLOT_TYPE_PRIVATE_SHOP

				mouseModule.mouseController.AttachObject(self, type, selectedSlotPos, selectedItemID, itemCount)
				mouseModule.mouseController.SetCallBack("INVENTORY", ui.__mem_func__(self.DropToInventory))
				snd.PlaySound("sound/ui/pick.wav")

	def DropToInventory(self):
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		self.AskBuyItem(attachedSlotPos)

	def AskBuyItem(self, slotPos):
		if app.ENABLE_SOLD_ITEM_SYSTEM:
			if shop.GetItemIsSold(slotPos) == 1:
				return
		#slotPos = self.__GetRealIndex(slotPos)
		itemIndex = shop.GetItemID(slotPos)
		itemPrice = shop.GetItemPrice(slotPos)
		if app.ENABLE_CHEQUE_SYSTEM:
			itemPriceCheque = shop.GetItemPriceCheque(slotPos)
		itemCount = shop.GetItemCount(slotPos)

		item.SelectItem(itemIndex)
		itemName = item.GetItemName()

		itemBuyQuestionDialog = uiCommon.QuestionDialog3()
		coinType = shop.GetTabCoinType(self.tabIdx)
		if app.ENABLE_CHEQUE_SYSTEM:
			if shop.IsPrivateShop():
				itemBuyQuestionDialog.SetText1(localeInfo.ASK_BUY_ITEM_TEXT % itemName)
				itemBuyQuestionDialog.SetText2(localeInfo.DO_YOU_BUY_ITEM_EX(itemCount, localeInfo.NumberToMoneyString(itemPrice), localeInfo.NumberToChequeString(itemPriceCheque)))
			else:
				requireditemVnum = shop.GetBuyWithItem(slotPos)
				if requireditemVnum != 0:
					item.SelectItem(requireditemVnum)
					requireditemName = item.GetItemName()
					itemBuyQuestionDialog = uiCommon.QuestionDialog()
					itemBuyQuestionDialog.SetText(localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice) + " adet " + requireditemName))
				else:
					if coinType == shop.SHOP_COIN_TYPE_YEAR_ROUND_COIN:
						itemBuyQuestionDialog.SetText1(localeInfo.ASK_BUY_ITEM_TEXT % itemName)
						itemBuyQuestionDialog.SetText2(localeInfo.DO_YOU_BUY_ITEM_EX(itemCount, localeInfo.NumberToYearRoundPointString(itemPrice), 0))
					elif app.ENABLE_BATTLE_ZONE_SYSTEM and shop.SHOP_COIN_TYPE_COMBAT_ZONE == shop.GetTabCoinType(self.tabIdx):
						itemBuyQuestionDialog.SetText1(localeInfo.ASK_BUY_ITEM_TEXT % itemName)
						itemBuyQuestionDialog.SetText2(localeInfo.DO_YOU_BUY_ITEM_EX(itemCount, localeInfo.NumberToCombatZoneCoinString(itemPrice), 0))
					elif coinType == shop.SHOP_COIN_TYPE_WORSHIP:
						itemBuyQuestionDialog.SetText1(localeInfo.ASK_BUY_ITEM_TEXT % itemName)
						itemBuyQuestionDialog.SetText2(localeInfo.DO_YOU_BUY_ITEM_EX(itemCount, localeInfo.NumberToWorshipPointString(itemPrice), 0))
					else:
						itemBuyQuestionDialog.SetText1(localeInfo.ASK_BUY_ITEM_TEXT % itemName)
						itemBuyQuestionDialog.SetText2(localeInfo.DO_YOU_BUY_ITEM_EX(itemCount, localeInfo.NumberToMoneyString(itemPrice), 0))
		else:
			itemBuyQuestionDialog.SetText1(localeInfo.ASK_BUY_ITEM_TEXT % itemName)
			itemBuyQuestionDialog.SetText(localeInfo.DO_YOU_BUY_ITEM_EX(itemCount, localeInfo.NumberToMoneyString(itemPrice), 0))
		itemBuyQuestionDialog.SetWidth(400)
		itemBuyQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.Open()
		itemBuyQuestionDialog.pos = slotPos
		self.itemBuyQuestionDialog = itemBuyQuestionDialog

	def AnswerBuyItem(self, flag):
		if flag:
			pos = self.itemBuyQuestionDialog.pos
			net.SendShopBuyPacket(pos)

		self.itemBuyQuestionDialog.Close()
		self.itemBuyQuestionDialog = None

		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def OverInItem(self, slotIndex):
		slotIndex = self.__GetRealIndex(slotIndex)
		if mouseModule.mouseController.isAttached():
			return

		coinType = shop.GetTabCoinType(self.tabIdx)
		if 0 != self.tooltipItem:
			self.tooltipItem.SetShopItem(slotIndex, coinType)

	def OverOutItem(self):
		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnUpdate(self):

		USE_SHOP_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xShopStart) > USE_SHOP_LIMIT_RANGE or abs(y - self.yShopStart) > USE_SHOP_LIMIT_RANGE:
			self.Close()
			
	if app.WJ_TRADABLE_ICON:
		def BindInterface(self, interface):
			self.interface = interface
			
		def CantSellInvenItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemIsAntiFlagBySlot = player.IsAntiFlagBySlot(player.INVENTORY, slotPos, item.ANTIFLAG_SELL)
				if itemIsAntiFlagBySlot:
					return True
			
			return False

		def OnTop(self):
			if not shop.IsPrivateShop():
				self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
				self.interface.RefreshMarkInventoryBag()


class MallPageDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()

	def Open(self):
		scriptLoader = ui.PythonScriptLoader()
		scriptLoader.LoadScriptFile(self, "UIScript/mallpagedialog.py")

		self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.Close))
		
		(x, y)=self.GetGlobalPosition()
		x+=10
		y+=30
		
		MALL_PAGE_WIDTH = 720
		MALL_PAGE_HEIGHT = 1120
		
		app.ShowWebPage(
			"http://metin2.co.kr/08_mall/game_mall/login_fail.htm", 
			(x, y, x+MALL_PAGE_WIDTH, y+MALL_PAGE_HEIGHT))

		self.Lock()
		self.Show()
		
	def Close(self):			
		app.HideWebPage()
		self.Unlock()
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
