import uiCommon
import snd
import chat
import app
import net
import player
import item
import wndMgr
import mouseModule
import localeInfo
import constInfo
import ui

class RedGemShopWindow(ui.ScriptWindow):
	GEM_SHOP_ADD_ITEM_VNUM = 39064
	GEM_SHOP_REFRESH_ITEM_VNUM = 39063
	GEM_SHOP_SLOT_MAX = 9
	GEM_SHOP_WINDOW_LIMIT_RANGE = 500

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.pageNumber = 1
		self.tooltipItem = 0
		self.xGemShopStart = 0
		self.yGemShopStart = 0
		self.questionDialog = None
		self.lastUpdate = 0
		
		self.priceDict = {}
		
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/redgemshopwindow.py")
		except:
			import exception
			exception.Abort("GemShopWindow.__LoadWindow.LoadObject")

		try:
			self.titleBar = self.GetChild("TitleBar")
			self.itemSlot = self.GetChild("SellItemSlot")
			self.nextPage = self.GetChild("nextPage1")
			self.pageText = self.GetChild("pageText1")
			self.backPage = self.GetChild("backPage1")
			for i in xrange(9):
				self.priceDict["slot_%d_price" % i] = self.GetChild("slot_%s_price" % str(i+1))
		except:
			import exception
			exception.Abort("RedGemShopWindow.__LoadWindow.BindObject")


		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
			

		# self.backPage.SetEvent(ui.__mem_func__(self.PageNextandBack(0)))
		# self.nextPage.SetEvent(ui.__mem_func__(self.PageNextandBack(1)))
		
		self.backPage.SAFE_SetEvent(self.PageNextandBack,0)
		self.nextPage.SAFE_SetEvent(self.PageNextandBack,1)

		self.itemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.itemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
			
	def Close(self):
		self.Hide()
		
	def Destroy(self):
		self.ClearDictionary()
		
		self.questionDialog = None
		
	def Open(self):
		self.pageNumber = 1
		self.pageText.SetText("Sayfa " + str(self.pageNumber) + " / 3")
		self.RefreshItemSlot()
		self.SetTop()
		self.Show()

		(self.xGemShopStart, self.yGemShopStart, z) = player.GetMainCharacterPosition()
	


			
	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip
			
	def OverInItem(self, slotIndex):
		if mouseModule.mouseController.isAttached():
			return

		if 0 != self.tooltipItem:
		
			if int(self.pageNumber == 1):
				self.tooltipItem.SetItemToolTip(player.GetGemShopItemVnum(slotIndex, True))
			elif int(self.pageNumber == 2):
				self.tooltipItem.SetItemToolTip(player.GetGemShopItemVnum(slotIndex+9, True))
			elif int(self.pageNumber == 3):
				self.tooltipItem.SetItemToolTip(player.GetGemShopItemVnum(slotIndex+18, True))
		
		
			

	def OverOutItem(self):
		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()
						
	def UseItemSlot(self, slotIndex):
		ADD_ITEM_NEED_COUNT = [0, 0, 0, 1, 2, 4, 8, 8, 8]

		if int(self.pageNumber == 1):
			if player.GetItemCountByVnum(50927) < player.GetGemShopItemPrice(slotIndex, True):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GEM_SYSTEM_NOT_ENOUGH_HP_GEM)
			else:
				self.RedGemShopSlotBuy(slotIndex)
		elif int(self.pageNumber == 2):
			if player.GetItemCountByVnum(50927) < player.GetGemShopItemPrice(slotIndex+9, True):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GEM_SYSTEM_NOT_ENOUGH_HP_GEM)
			else:
				self.RedGemShopSlotBuy(slotIndex+9)
		elif int(self.pageNumber == 3):
			if player.GetItemCountByVnum(50927) < player.GetGemShopItemPrice(slotIndex+18, True):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GEM_SYSTEM_NOT_ENOUGH_HP_GEM)
			else:
				self.RedGemShopSlotBuy(slotIndex+18)

		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def PageNextandBack(self, arg):
		if (int(arg) == 1 and int(self.pageNumber) < 3):
			self.pageNumber +=1
		elif (int(arg) == 0 and int(self.pageNumber) >= 1):
			if int(self.pageNumber != 1):
				self.pageNumber -=1
			
		self.pageText.SetText("Sayfa " + str(self.pageNumber) + " / 3")
		self.RefreshGemShop()
			
	def RefreshGemShop(self):
		self.RefreshItemSlot()
		
	def RefreshItemSlot(self):
		for i in xrange(self.GEM_SHOP_SLOT_MAX):
			if int(self.pageNumber == 1):
				itemCount = player.GetGemShopItemCount(i, True)
				itemPrice = player.GetGemShopItemPrice(i, True)
			elif int(self.pageNumber == 2):
				itemCount = player.GetGemShopItemCount(i+9, True)
				itemPrice = player.GetGemShopItemPrice(i+9, True)
			elif int(self.pageNumber == 3):
				itemCount = player.GetGemShopItemCount(i+18, True)
				itemPrice = player.GetGemShopItemPrice(i+18, True)

			if itemCount <= 1:
				itemCount = 0
			
			if int(self.pageNumber == 1):
				self.itemSlot.SetItemSlot(i, player.GetGemShopItemVnum(i, True), itemCount)
			elif int(self.pageNumber == 2):
				self.itemSlot.SetItemSlot(i, player.GetGemShopItemVnum(i+9, True), itemCount)
			elif int(self.pageNumber == 3):
				self.itemSlot.SetItemSlot(i, player.GetGemShopItemVnum(i+18, True), itemCount)


			
			self.priceDict["slot_%d_price" % i].SetText(str(itemPrice))
			
			self.itemSlot.EnableSlot(i)

		wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())
		
			
	def RedGemShopSlotBuy(self, slotIndex):
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.GEM_SYSTEM_BUY_ITEM)
		questionDialog.SetAcceptEvent(lambda arg = int(slotIndex): ui.__mem_func__(self.SendBuyAccept)(arg))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.SlotAddQuestionCancel))
		questionDialog.Open()
		self.questionDialog = questionDialog
		
	def SlotAddQuestionCancel(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None
		
	def SendBuyAccept(self, slotIndex):
		self.SlotAddQuestionCancel()
		net.SendGemShopBuy(slotIndex)
		
		
	def OnUpdate(self):
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xGemShopStart) > self.GEM_SHOP_WINDOW_LIMIT_RANGE or abs(y - self.yGemShopStart) > self.GEM_SHOP_WINDOW_LIMIT_RANGE:
			self.Close()
