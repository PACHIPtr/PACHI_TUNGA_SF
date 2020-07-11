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

import uiCommon, ui, time, playersettingmodule, localeInfo, mouseModule, constInfo, uiScriptLocale, interfaceModule, dbg, wndMgr, snd, uiToolTip

class RefineRarityWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.DefaultSettings()
		self.needitemcount = 0
		self.needmoney = 0
		self.percentage = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/rarityrefinewindow.py")
		GetObject=self.GetChild
		self.board = GetObject("board")
		self.refineRaritySlot = GetObject("RefineR_Slot")
		self.refineRaritySlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectRaritySlotEmpty))
		self.refineRaritySlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectRaritySlot))
		self.refineRaritySlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInRaritySlotItem))
		self.refineRaritySlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutRaritySlotItem))
		self.closebutton = GetObject("CloseButton")
		self.closebutton.SetEvent(ui.__mem_func__(self.Close))
		self.refinebutton = GetObject("RefineR_Change_Button")
		self.refinebutton.SetEvent(ui.__mem_func__(self.RefineR_Change))
		self.okbutton = GetObject("RefineR_OK_Button")
		self.okbutton.SetEvent(ui.__mem_func__(self.RefineR_OK))
		self.bilgi = GetObject("bilgi")
		self.bilgi.SetToolTipWindow(self.__bilgiYazisi(localeInfo.TOOLTIP_RARITYREFINE_INFO))
		self.otbilgitext = GetObject("otBilgiText")
		self.sonuctext = GetObject("AttrChangeResultText")

	def Destroy(self):
		self.ClearDictionary()
		self.board = None

	def Open(self):
		self.otbilgitext.SetText("Gerekli Yang: %s" % (25000000))
		self.sonuctext.SetText("Evrim yükseltme şansı: %%%d" % (int(35)))
		self.Show()
		self.SetCenterPosition()

	def DefaultSettings(self):
		self.slotPositionsDict = {}
		self.refineRaritySlot.ClearSlot(0)
		self.refineRaritySlot.ClearSlot(1)
		self.refineRaritySlot.ClearSlot(2)
		self.refineRaritySlot.ClearSlot(3)
		self.okbutton.Hide()
		self.refinebutton.Show()

	def Close(self):
		self.DefaultSettings()
		self.Hide()
		
	def __bilgiYazisi(self, title):
		toolTip = uiToolTip.ToolTip()
		toolTip.SetTitle(title)
		toolTip.AppendSpace(3)
		toolTip.AutoAppendTextLine(localeInfo.TOOLTIP_RARITYREFINE_INFO1)
		toolTip.AutoAppendTextLine(localeInfo.TOOLTIP_RARITYREFINE_INFO2)

		toolTip.AlignHorizonalCenter()
		return toolTip
		
	def SuccessMessage(self):
		self.Close()

	def Failed(self):
		self.Close()
		
	def RefineR_Change(self):
		if (self.slotPositionsDict.has_key(0) == True and self.slotPositionsDict.has_key(1) == True):
			refineItemPos = self.slotPositionsDict[0][1]
			if self.slotPositionsDict.has_key(2) == True:
				import dbg
				dbg.TraceError("PAKET DOGRU")
				dbg.TraceError("{0}".format(int(self.slotPositionsDict[2][1])))
				net.SendChatPacket("/refine_evo %d %d %d" % (refineItemPos, self.slotPositionsDict[1][1], self.slotPositionsDict[2][1]))
			else:
				net.SendChatPacket("/refine_evo %d %d" % (refineItemPos, self.slotPositionsDict[1][1]))
			
	def RefineR_OK(self):
		self.Close()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
			
	def __OnOverInRaritySlotItem(self, slotIndex):
		if (not slotIndex in self.slotPositionsDict):
			return
		(invenType, invenPos) = self.slotPositionsDict[slotIndex]
		self.tooltipItem.SetInventoryItem(invenPos, invenType)
		
	def __OnOverOutRaritySlotItem(self):
		self.tooltipItem.Hide()
		
	def IS_ERITME_ITEM(self, vnum):
		if vnum >= 81705 and vnum <= 81711:
			return True
			
		return False
		
	def __OnSelectRaritySlotEmpty(self, selectedSlotPos):
		
		if (mouseModule.mouseController.isAttached() == False):
			return
			
		if (selectedSlotPos == 3):
			mouseModule.mouseController.DeattachObject()
			return
			
		attachedSlotType		= mouseModule.mouseController.GetAttachedType()
		attachedSlotPos			= mouseModule.mouseController.GetAttachedSlotNumber()
		mouseModule.mouseController.DeattachObject()
		
		attachedInvenType		= player.SlotTypeToInvenType(attachedSlotType)
		itemIndex				= player.GetItemIndex(attachedInvenType, attachedSlotPos)
		itemCount				= player.GetItemCountByVnum(itemIndex)
		
		if (selectedSlotPos == 1 and not self.IS_ERITME_ITEM(itemIndex)):
			return
		if (selectedSlotPos == 2 and itemIndex != 81712):
			return
		
		if (selectedSlotPos == 2):
			oncekiYazi = self.sonuctext.GetText()
			self.sonuctext.SetText("%s + |cff10c100%%%d" % (oncekiYazi, int(65)))
		self.slotPositionsDict[selectedSlotPos] = (attachedInvenType, attachedSlotPos)
		self.refineRaritySlot.SetItemSlot(selectedSlotPos, itemIndex, itemCount)
		if (selectedSlotPos == 0):
			(invenType, invenPos) = self.slotPositionsDict[0]
			self.slotPositionsDict[3] = (invenType, invenPos)
			self.refineRaritySlot.SetItemSlot(3, player.GetItemIndex(invenType, invenPos), 0)
			self.refineRaritySlot.ActivateSlot(3)
		
	def __OnSelectRaritySlot(self, selectedSlotPos):
		
		if (mouseModule.mouseController.isAttached() == True or selectedSlotPos == 3):
			return
			
		if (selectedSlotPos == 2 and self.slotPositionsDict.has_key(2)):
			self.sonuctext.SetText("Evrim yükseltme şansı: %%%s" % (self.percentage))
		
		del self.slotPositionsDict[selectedSlotPos]
		self.refineRaritySlot.ClearSlot(selectedSlotPos)

	def OnPressEscapeKey(self):
		self.Close()
		return True

