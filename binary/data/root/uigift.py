import app
import net
import player
import item
import chat
import ui
import uiScriptLocale
import time
import snd
import localeInfo
import mouseModule
import constInfo
import grp
import dbg
import uiToolTip
import uiCommon
import uiWhisper
import wndMgr


##Should be disabled becouse this is only for my client UI ##
POSITION_FIX=False
#############################################################

class GiftDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.UI = {}
		self.pop=None
		self.pages = []
		self.pageCur = 1
		self.pageNum = 0
		self.search=""
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()	
		self.LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		self.UI["board"]=ui.BoardWithTitleBar()
		self.UI["board"].SetSize(500, 320)		
		self.UI["board"].SetTitleName(uiScriptLocale.GIFT_WINDOW_NAME)
		self.UI["board"].SetCloseEvent(ui.__mem_func__(self.Close))
		self.UI["board"].AddFlag("movable")
		self.UI["board"].Hide()
		
		self.UI["slots"] = ui.GridSlotWindow()
		self.UI["slots"].SetParent(self.UI["board"])
		self.UI["slots"].SetPosition(10,30)
		self.UI["slots"].SetSize(480, 280)
		self.UI["slots"].ArrangeSlot(0, 15,8, 32, 32, 0, 0)
		self.UI["slots"].RefreshSlot()
		self.UI["slots"].SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
		self.UI["slots"].SAFE_SetButtonEvent("LEFT", "EXIST", self.OnSelectItemSlot)
		self.UI["slots"].SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
		self.UI["slots"].SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.UI["slots"].SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		
		self.UI["slots"].Show()
		Y=self.UI["board"].GetHeight()-30
		if POSITION_FIX:
			Y=self.UI["board"].GetHeight()-43
		self.UI["get_button"] = ui.MakeButton(self.UI["board"],250, Y,"","d:/ymir work/ui/public/","large_Button_01.sub","large_Button_02.sub","large_Button_03.sub")
		self.UI["get_button"].SetText(uiScriptLocale.GIFT_GET_ALL)
		self.UI["get_button"].SetEvent(lambda : self.GetAll())
		self.UI["get_button"].Show()
		
		self.UI["pageSlotImage"] = ui.MakeImageBox(self.UI["board"], "d:/ymir work/ui/public/Parameter_Slot_00.sub", self.UI["board"].GetWidth()-110, Y)
		self.UI["pageSlot"]= ui.MakeTextLine(self.UI["pageSlotImage"])
		self.UI["pageSlot"].SetWindowHorizontalAlignCenter()
		self.UI["pageSlot"].SetHorizontalAlignCenter()
		self.UI["pageSlot"].SetText(str(self.pageCur)+" - "+str(self.pageNum+1))
		self.UI["pageSlot"].Show()
		
		## Prev Button
		self.UI["prevButton"] = ui.MakeButton(self.UI["board"], self.UI["board"].GetWidth()-160, Y ,uiScriptLocale.GIFT_PREV_PAGE, "d:/ymir work/ui/public/", "small_button_01.sub", "small_button_02.sub", "small_button_03.sub")
		self.UI["prevButton"].SetText("<<")
		self.UI["prevButton"].SetEvent(ui.__mem_func__(self.PrepPage), -1)
		
		## Next Button
		self.UI["nextButton"] = ui.MakeButton(self.UI["board"], self.UI["board"].GetWidth()-65, Y , uiScriptLocale.GIFT_NEXT_PAGE, "d:/ymir work/ui/public/", "small_button_01.sub", "small_button_02.sub", "small_button_03.sub")
		self.UI["nextButton"].SetText(">>")
		self.UI["nextButton"].SetEvent(ui.__mem_func__(self.PrepPage), 1)
		
		self.AddFlag("movable")
		self.AddFlag("float")
		self.SetCenterPosition()
		self.Show()

	def Open(self):
		self.SetCenterPosition()
		self.Show()	
		self.UI["board"].Show()
		self.UI["board"].SetCenterPosition()
		self.LoadPage(self.pageCur)

	def LoadPage(self,page):
		self.pageCur=page
		net.SendChatPacket("/gift_refresh "+str(page))

	def PrepPage(self,page):
		if page==1 and self.pageCur==self.pageNum or self.pageCur == 1 and page<1:
			return
		page=self.pageCur+page
		self.LoadPage(page)

	def OnUpdate(self):
		self.UI["pageSlot"].SetText(str(self.pageCur)+" - "+str(self.pageNum))
		
	def Close(self):
		if None != self.tooltipItem:
			self.tooltipItem.HideToolTip()
		self.Hide()
		self.UI["board"].Hide()
		return True

	def Clear(self):
		constInfo.gift_items={}
		self.Refresh()

	def GetItemCount(self,slot):
		try:
			return int(constInfo.gift_items[int(slot)]["count"])
		except KeyError:
			return 0
		
	def GetItemID(self,slot):
		try:
			return int(constInfo.gift_items[int(slot)]["vnum"])
		except KeyError:
			return 0
	
	def Refresh(self):
		for i in xrange(self.UI["slots"].GetSlotCount()):
			vnum=self.GetItemID(i)
			itemCount = self.GetItemCount(i)
			if vnum == 1 or itemCount==0:
				itemCount=0
			self.UI["slots"].SetItemSlot(i, vnum, itemCount)
		wndMgr.RefreshSlot(self.UI["slots"].GetWindowHandle())
		self.UI["slots"].RefreshSlot()

	def __ShowToolTip(self, slot):
		if self.tooltipItem:
			self.tooltipItem.ClearToolTip()
			self.tooltipItem.SetCannotUseItemForceSetDisableColor(0)
			if int(slot) in constInfo.gift_items.keys():
				it=constInfo.gift_items[int(slot)]
				#self.tooltipItem.AppendTextLine("|cff18BB00|h[Item ID: %d]|h|r" % (it["id"]))
				#self.tooltipItem.AppendSpace(5)
				if it["vnum"]==1:
					self.tooltipItem.ClearToolTip()
					self.tooltipItem.SetTitle(localeInfo.NumberToMoneyString(it["count"]))
					self.tooltipItem.AppendDescription(uiScriptLocale.GIFT_MONEY_DESC, 26)
					self.tooltipItem.AppendDescription(item.GetItemSummary(), 26, 0xffBEB47D)
					self.tooltipItem.ShowToolTip()
				else:
					self.tooltipItem.AddItemData(int(it["vnum"]),it["sockets"],it["attrs"],0,0,0,player.INVENTORY, -1, int(it["transmutation"]))
				self.tooltipItem.AppendTextLine("|cff18BB00|h[%s %s]|h|r" % (uiScriptLocale.GIFT_FROM,it["give"]))
				self.tooltipItem.AppendSpace(5)
				self.tooltipItem.AppendTextLine("|cff18BB00|h[%s %s]|h|r" % (uiScriptLocale.GIFT_REASON,it["reason"]))
				self.tooltipItem.AppendSpace(5)
			else:
				self.tooltipItem.ClearToolTip()
				self.tooltipItem.HideToolTip()
			
	def OverInItem(self, slotIndex):
		#slotIndex = self.__GetRealIndex(slotIndex)
		self.UI["slots"].SetUsableItem(False)
		self.__ShowToolTip(slotIndex)

	def OverOutItem(self):
		self.UI["slots"].SetUsableItem(False)
		if self.tooltipItem:
			self.tooltipItem.ClearToolTip()
			self.tooltipItem.HideToolTip()

	def UnselectItemSlot(self,selectedSlotPos):
		self.__OnClosePopupDialog()

	def OnSelectItemSlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()
			self.UI["board"].Hide()
		else:
			if not int(selectedSlotPos) in constInfo.gift_items.keys():
				self.UI["board"].Hide()
				return

			snd.PlaySound("sound/ui/drop.wav")
			self.Get(selectedSlotPos)

	def __OnClosePopupDialog(self):
		if self.pop != None:
			if self.pop.IsShow():
				self.pop.Hide()
		self.pop = None

	def PopupMessage(self,text):
		pop = uiCommon.PopupDialog()
		pop.SetText(text)
		pop.SetAcceptEvent(self.__OnClosePopupDialog)
		pop.Open()
		self.pop = pop
		self.pop.SetTop()

	def Get(self,slot):
		if int(slot) in constInfo.gift_items.keys():
			it=constInfo.gift_items[int(slot)]
			item.SelectItem(int(it["vnum"]))
			self.pop = uiCommon.QuestionDialog()
			self.pop.SetText(uiScriptLocale.GIFT_GET_QUEST%(item.GetItemName()))
			self.pop.SetAcceptEvent(lambda arg1=str(it["id"]): self.OnGet(arg1))
			self.pop.SetCancelEvent(ui.__mem_func__(self.__OnClosePopupDialog))
			self.pop.Open()
		
	def GetAll(self):
		self.pop = uiCommon.QuestionDialog()
		self.pop.SetText(uiScriptLocale.GIFT_GET_QUEST_ALL)
		self.pop.SetAcceptEvent(lambda arg1="": self.OnGet(arg1))
		self.pop.SetCancelEvent(ui.__mem_func__(self.__OnClosePopupDialog))
		self.pop.Open()
			
	def OnGet(self,id):
		net.SendChatPacket("/gift_get "+str(id))	
		self.__OnClosePopupDialog()
		if len(constInfo.gift_items.keys())-1<=0:
			self.Close()
		
class Edit2(ui.EditLine):
	def __init__(self,parent,text,x,y,width,height,number=False,slot=2,max = 12):
		ui.EditLine.__init__(self)
		self.imageSlot=ui.MakeImageBox(parent, "d:/ymir work/ui/public/Parameter_Slot_0"+str(slot)+".sub", x,y)
		self.SetText(text)
		self.main = text
		self.SetMax(max)
		self.SetUserMax(max)
		self.SetParent(self.imageSlot)
		if number:
			self.SetNumberMode()
		self.SetSize(width,height)
		self.SetPosition(5,2)
		self.Show()

	def GetText(self):
		res = ui.EditLine.GetText(self)
		if res == "":
			return ""
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