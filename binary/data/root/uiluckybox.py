import app
import chr
import chrmgr
import player
import net
import pack
import background
import chat
import textTail
import event
import effect
import systemSetting
import quest
import guild
import skill
import messenger
import exchange
import ime
import item
import renderTarget
import nonplayer

import uiCommon, ui, time, playersettingmodule, localeInfo, mouseModule, constInfo, uiScriptLocale, interfaceModule, dbg, wndMgr, snd, uiToolTip

class LuckyBoxWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.itemVnum = 0
		self.itemCount = 0
		self.needMoneyMem = 0
		self.tooltipItem = 0
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/luckyboxwindow.py")
		except:
			import exception
			exception.Abort("luckyboxwindow.__LoadWindow.LoadScriptFile")
			
		try:
			self.RewardItemSlot = self.GetChild("RewardItemSlot")
			self.RetryButton	= self.GetChild("RetryButton")
			self.RecvButton		= self.GetChild("RecvButton")
			self.NeedMoney		= self.GetChild("NeedMoney")
		except:
			import exception as exception
			exception.Abort("LuckyBoxWindow.LoadDialog.BindObject")

		self.RewardItemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.RewardItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.RewardItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		
		self.RetryButton.SetEvent(ui.__mem_func__(self.RetryButtonFunc))
		self.RecvButton.SetEvent(ui.__mem_func__(self.RecvButtonFunc))
		
	def Open(self):
		self.Show()
		
	def Close(self):
		self.tooltipItem.HideToolTip()
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		
	def Destroy(self):
		self.itemVnum = 0
		self.itemCount = 0
		self.needMoneyMem = 0
		self.tooltipItem = 0
		self.RewardItemSlot = 0
		self.RetryButton = 0
		self.RecvButton = 0
		self.NeedMoney = 0
		self.ClearDictionary()
		
	def SetData(self, itemVnum, itemCount, needMoney):
		self.itemVnum = itemVnum
		self.itemCount = itemCount
		self.needMoneyMem = needMoney
		
		## Real Part
		self.RewardItemSlot.SetItemSlot(0, itemVnum, itemCount)
		self.NeedMoney.SetText(localeInfo.NumberToMoneyString(int(needMoney)))
	
	def RetryButtonFunc(self):
		net.SendChatPacket("/change_special_gacha")
		
	def RecvButtonFunc(self):
		net.SendChatPacket("/get_special_gacha_item")

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def OverInItem(self, slotIndex):
		slotIndex = self.__GetRealIndex(slotIndex)
		if mouseModule.mouseController.isAttached():
			return

		coinType = shop.GetTabCoinType(self.tabIdx)
		if 0 != self.tooltipItem:
			self.tooltipItem.SetItemToolTip(self.itemVnum)
			
	def OverOutItem(self):
		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
