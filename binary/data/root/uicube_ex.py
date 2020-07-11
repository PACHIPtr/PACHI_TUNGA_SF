import ui
import uiCommon
import uiToolTip
import mouseModule
import localeInfo
import snd
import app
import chat
import player
import net
import item

CUBE_SLOT_MAX 				= 5
CUBE_SLOT_RIGHT				= 6
CUBE_SLOT_LEFT				= 0
IS_IN_MATERIAL_VNUM_RANGE	= lambda item_vnum : (item_vnum == 71123 or item_vnum == 71129 or item_vnum == 81600)
IS_FIT_FOR_LEFT_SLOT		= lambda item_type : (item_type == item.ITEM_TYPE_WEAPON or item_type == item.ITEM_TYPE_ARMOR)

class CubeWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		
		self.tooltipItem 		= None
		self.slotPositionsDict	= {}
		self.xCubeStart			= 0
		self.yCubeStart			= 0
		self.rewardItemsList	= []
		self.rewardItemVnum		= 0
		self.craftItemsDict		= {}
		
		self.showUpgradeItemsTime		= True
		self.showUpgradeItems			= False
		self.showUpgradeItemsVnumDict	= {}
		
		self.showList					= False
		self.showListItemsDict			= {}
		self.showListCurrentIndex		= -1
		
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/cubewindowex.py")
		except:
			import exception
			exception.Abort("CubeWindow.__LoadWindow.UIScript/cubewindowex.py")
			
		try:
			self.wndLeftSlot		= self.GetChild("LeftSlot")
			self.wndRightSlot		= self.GetChild("RightSlot")
			self.wndMaterialSlots 	= [self.GetChild("MaterialSlot%d" % int(i+1)) for i in xrange(CUBE_SLOT_MAX)]
			self.wndMoneyText		= self.GetChild("MoneyText")
			self.btnAccept			= self.GetChild("LButton")
			self.btnCancel			= self.GetChild("RButton")
			self.btnToolTip			= self.GetChild("ToolTipButton")
			self.btnInform			= self.GetChild("InformButton")
			self.btnOpenList		= self.GetChild("OpenListButton")
			self.btnInform.Hide()
		except:
			import exception
			exception.Abort("CubeWindow.__LoadWindow.BindObject")
			
		try:
			# Left slot configuration
			self.wndLeftSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
			self.wndLeftSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
			
			self.wndLeftSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectLeftItemSlot))
			self.wndLeftSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectLeftEmptySlot))
			
			# Right slot configuration
			self.wndRightSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
			self.wndRightSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
			
			# Material slot configuration
			for materialSlot in self.wndMaterialSlots:
				materialSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
				materialSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
				materialSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectMaterialItemSlot))
				materialSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectMaterialEmptySlot))				
				
			self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.__OnClose))
			self.btnAccept.SetEvent(ui.__mem_func__(self.__OnClickLButton))
			self.btnCancel.SetEvent(ui.__mem_func__(self.__OnClickRButton))
			self.btnToolTip.SetToolTipWindow(self.__CreateToolTip(localeInfo.CUBE_EX_WINDOW_TOOLTIP_TITLE, [localeInfo.CUBE_EX_WINDOW_TOOLTIP1, localeInfo.CUBE_EX_WINDOW_TOOLTIP2, localeInfo.CUBE_EX_WINDOW_TOOLTIP3]))						
			self.btnInform.SetEvent(ui.__mem_func__(self.__OnClickInformButton))
			
			self.btnOpenList.SetEvent(ui.__mem_func__(self.__OnClickOpenList))
		except:
			import exception
			exception.Abort("CubeWindow.__LoadWindow.SetEvent")
			
	def AddCubeResultItem(self, itemVnum, itemCount):
		self.rewardItemsList.append((itemVnum, itemCount))
		
	def AddMaterialInfo(self, itemIndex, orderIndex, itemVnum, itemCount):
		if (not itemIndex in self.craftItemsDict):
			self.craftItemsDict[itemIndex] = []
			
		self.craftItemsDict[itemIndex].append((itemVnum, itemCount))
		
	def UpdateInfo(self, gold, itemVnum, itemCount):
		if (self.wndMoneyText):
			self.wndMoneyText.SetText(localeInfo.NumberToMoneyString(gold))
		
	def ClearSlots(self):
		self.slotPositionsDict	= {}
		self.wndLeftSlot.ClearSlot(CUBE_SLOT_LEFT)
		self.wndRightSlot.ClearSlot(CUBE_SLOT_RIGHT)
		
		for i in xrange(CUBE_SLOT_MAX):
			self.wndMaterialSlots[i].ClearSlot(i + 1)
			
		self.wndMoneyText.SetText("")
		
	def __OnClickOpenList(self):
		self.showList = not self.showList
		
		if (self.showList):
			self.btnAccept.SetUpVisual("d:/ymir work/ui/game/cube_ex/cube_list_button_03.tga")
			self.btnAccept.SetOverVisual("d:/ymir work/ui/game/cube_ex/cube_list_button_04.tga")
			self.btnAccept.SetDownVisual("d:/ymir work/ui/game/cube_ex/cube_list_button_03.tga")
			self.btnAccept.SetPosition(75, 380)
			
			self.btnCancel.SetUpVisual("d:/ymir work/ui/game/cube_ex/cube_list_button_01.tga")
			self.btnCancel.SetOverVisual("d:/ymir work/ui/game/cube_ex/cube_list_button_02.tga")
			self.btnCancel.SetDownVisual("d:/ymir work/ui/game/cube_ex/cube_list_button_01.tga")
			
			# self.popupDlg = uiCommon.PopupDialog2()
			# self.popupDlg.SetText1(localeInfo.CUBE_POP_DLG1)
			# self.popupDlg.SetText2(localeInfo.CUBE_POP_DLG2)
			# self.popupDlg.Open()
		else:
			self.btnAccept.SetUpVisual("d:/ymir work/ui/public/AcceptButton00.sub")
			self.btnAccept.SetOverVisual("d:/ymir work/ui/public/AcceptButton01.sub")
			self.btnAccept.SetDownVisual("d:/ymir work/ui/public/AcceptButton02.sub")
			self.btnAccept.SetPosition(55, 380)
			
			self.btnCancel.SetUpVisual("d:/ymir work/ui/public/CancleButton00.sub")
			self.btnCancel.SetOverVisual("d:/ymir work/ui/public/CancleButton01.sub")
			self.btnCancel.SetDownVisual("d:/ymir work/ui/public/CancleButton02.sub")
			
			self.showListCurrentIndex 	= -1
			self.showListItemsDict 		= {}
			self.ClearSlots()
		
	def __OnClickInformButton(self):
		if (self.slotPositionsDict.has_key(CUBE_SLOT_LEFT) == False):
			return
			
		(invenType, invenPos)	= self.slotPositionsDict[CUBE_SLOT_LEFT]
		leftItemIndex 			= player.GetItemIndex(invenType, invenPos)

		mineMaterialInfo		= None
		for itemIndex, materialInfo in self.craftItemsDict.iteritems():
			for itemVnum, itemCount in materialInfo:				
				if (itemVnum == leftItemIndex):
					mineMaterialInfo = materialInfo
					break
			
			if (mineMaterialInfo is not None):
				break
					
		if (mineMaterialInfo is None):
			return
			
		i = 0
		for itemVnum, itemCount in mineMaterialInfo:
			if (itemVnum == leftItemIndex):
				continue
				
			self.wndMaterialSlots[i].SetItemSlot((i+1), itemVnum, itemCount)
			self.showUpgradeItemsVnumDict[i+1] = itemVnum
			i += 1
		
		self.showUpgradeItems 		= True
		self.showUpgradeItemsTime	= app.GetTime()
		
	def __CreateToolTip(self, title, descList):
		toolTip = uiToolTip.ToolTip()
		toolTip.SetTitle(title)
		toolTip.AppendSpace(5)

		for desc in descList:
			toolTip.AutoAppendTextLine(desc)

		toolTip.AlignHorizonalCenter()
		return toolTip
		
	def __ShowCurrentList(self):
		
		if (self.showUpgradeItems):
			self.showUpgradeItems 			= False
			self.showUpgradeItemsTime 		= 0		
	
		self.ClearSlots()
		itemDict = self.craftItemsDict[self.showListCurrentIndex]
		
		i = 0
		j = 0
		for itemInfo in itemDict:
			(itemVnum, itemCount) = itemInfo
			if (i == CUBE_SLOT_LEFT):
				self.wndLeftSlot.SetItemSlot(CUBE_SLOT_LEFT, itemVnum, itemCount)
				self.showListItemsDict[CUBE_SLOT_LEFT] = (itemVnum, itemCount)
				i += 1
				continue
				
			self.wndMaterialSlots[j].SetItemSlot(j + 1, itemVnum, itemCount)
			self.showListItemsDict[j + 1] = (itemVnum, itemCount)
			j += 1
			i += 1
			
		(itemVnum, itemCount) = self.rewardItemsList[self.showListCurrentIndex]
		self.wndRightSlot.SetItemSlot(CUBE_SLOT_RIGHT, itemVnum, itemCount)		
		self.showListItemsDict[CUBE_SLOT_RIGHT] = (itemVnum, itemCount)
		
	def __OnClickLButton(self):
	
		if (self.showList):
			if (self.showListCurrentIndex == -1):
				self.showListCurrentIndex = len(self.craftItemsDict) - 1
			else:
				self.showListCurrentIndex -= 1
				if (self.showListCurrentIndex < 0):
					self.showListCurrentIndex = len(self.craftItemsDict) - 1
			self.__ShowCurrentList()
			return
			
		if (len(self.slotPositionsDict) == 0):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CUBE_WINDOW_EMPTY)
			return
			
		if (self.rewardItemVnum == -1):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CUBE_WRONG_COMBINATION)
			return
			
		self.questionDlg = uiCommon.QuestionDialog()
		self.questionDlg.SetText(localeInfo.CUBE_QUESTION_DLG)
		self.questionDlg.SetAcceptEvent(ui.__mem_func__(self.__OnAccept))
		self.questionDlg.SetCancelEvent(ui.__mem_func__(self.__OnCancel))
		self.questionDlg.Open()
		
	def __OnClickRButton(self):
		if (self.showList):
			self.showListCurrentIndex += 1
			if (self.showListCurrentIndex >= len(self.craftItemsDict)):
				self.showListCurrentIndex = 0
			self.__ShowCurrentList()
			return
		
		self.__OnClose()
		
	def __OnCancel(self):
		self.questionDlg.Close()
		self.questionDlg = None
		
	def __OnAccept(self):
		self.__OnCancel()
		net.SendChatPacket("/cube make")
	
	def __IsItemExist(self, itemVnum, itemCount):
		for slotPos, invenPosInfo in self.slotPositionsDict.iteritems():
			(invenType, invenPos) = invenPosInfo
			if (player.GetItemIndex(invenType, invenPos) == itemVnum and player.GetItemCount(invenType, invenPos) >= itemCount):
				return True
				
		return False
	
	def __GetRewardItemInfo(self):
		if (len(self.craftItemsDict) == 0 or len(self.slotPositionsDict) == 0):
			return (-1, -1)
			
		for itemIndex, materialInfo in self.craftItemsDict.iteritems():
			tmpMaterialInfoLen = 0
			for itemVnum, itemCount in materialInfo:
				if (self.__IsItemExist(itemVnum, itemCount) == False):
					continue
				
				tmpMaterialInfoLen += 1
				
			if (tmpMaterialInfoLen == len(materialInfo)):
				return self.rewardItemsList[itemIndex]
				
		return (-1, -1)
		
	def Refresh(self):
		
		for slotPos in xrange(CUBE_SLOT_MAX + 1):			
			if (slotPos == 0):
				if (not slotPos in self.slotPositionsDict):
					self.wndLeftSlot.ClearSlot(slotPos)
					continue
					
				(invenType, invenPos)	= self.slotPositionsDict[slotPos]
				itemIndex				= player.GetItemIndex(invenType, invenPos)
				itemCount				= player.GetItemCount(invenType, invenPos)
				
				if (itemCount > 0):
					self.wndLeftSlot.SetItemSlot(slotPos, itemIndex, itemCount)
				else:
					del self.slotPositionsDict[slotPos]
					self.wndLeftSlot.ClearSlot(slotPos)			
			else:
				realSlotPos = slotPos - 1
				
				if (not slotPos in self.slotPositionsDict):
					self.wndMaterialSlots[realSlotPos].ClearSlot(slotPos)
					continue
					
				(invenType, invenPos)	= self.slotPositionsDict[slotPos]
				itemIndex				= player.GetItemIndex(invenType, invenPos)
				itemCount				= player.GetItemCount(invenType, invenPos)
				
				if (itemCount > 0):
					self.wndMaterialSlots[realSlotPos].SetItemSlot(slotPos, itemIndex, itemCount)
				else:
					del self.slotPositionsDict[slotPos - 1]
					self.wndMaterialSlots[realSlotPos].ClearSlot(slotPos + 1)
					
		try:
			(self.rewardItemVnum, itemCount) = self.__GetRewardItemInfo()
			if (self.rewardItemVnum != -1):
				self.wndRightSlot.SetItemSlot(CUBE_SLOT_RIGHT, self.rewardItemVnum, itemCount)
			else:
				self.wndRightSlot.ClearSlot(CUBE_SLOT_RIGHT)
		except TypeError:
			self.rewardItemVnum = 0
			self.wndRightSlot.ClearSlot(CUBE_SLOT_RIGHT)
			
		if (self.slotPositionsDict.has_key(CUBE_SLOT_LEFT)):
			self.btnInform.Show()
		else:
			self.btnInform.Hide()
		
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
		
	def Open(self):	
		self.rewardItemsList 	= []
		self.rewardItemVnum 	= 0
		self.craftItemsDict 	= {}
		self.slotPositionsDict 	= {}
		self.wndMoneyText.SetText("")
		
		self.Refresh()
		self.SetCenterPosition()
		self.Show()
		self.SetTop()
		
		(self.xCubeStart, self.yCubeStart, z) = player.GetMainCharacterPosition()
		
	def __OnClose(self):
		if (self.tooltipItem is not None):
			self.tooltipItem.Hide()
		
		if (self.showList):
			self.btnAccept.SetUpVisual("d:/ymir work/ui/public/AcceptButton00.sub")
			self.btnAccept.SetOverVisual("d:/ymir work/ui/public/AcceptButton01.sub")
			self.btnAccept.SetDownVisual("d:/ymir work/ui/public/AcceptButton02.sub")
			self.btnAccept.SetPosition(55, 380)
			
			self.btnCancel.SetUpVisual("d:/ymir work/ui/public/CancleButton00.sub")
			self.btnCancel.SetOverVisual("d:/ymir work/ui/public/CancleButton01.sub")
			self.btnCancel.SetDownVisual("d:/ymir work/ui/public/CancleButton02.sub")
			
			self.showList = False
			self.showListCurrentIndex = -1
		
		net.SendChatPacket("/cube close")
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.__OnClose()
		return True
		
	def __OnSelectLeftEmptySlot(self, selectedSlotPos):
		if (mouseModule.mouseController.isAttached() == False):
			return
			
		attachedSlotType	= mouseModule.mouseController.GetAttachedType()
		attachedSlotPos		= mouseModule.mouseController.GetAttachedSlotNumber()
		mouseModule.mouseController.DeattachObject()
		
		attachedInvenType	= player.SlotTypeToInvenType(attachedSlotType)
		
		itemIndex			= player.GetItemIndex(attachedInvenType, attachedSlotPos)
		itemCount			= player.GetItemCount(attachedInvenType, attachedSlotPos)
		if (itemCount <= 1):
			itemCount		= 0
		
		if (IS_FIT_FOR_LEFT_SLOT(player.GetItemTypeBySlot(attachedInvenType, attachedSlotPos)) == False):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CUBE_IS_NOT_FIT_ITEM)
			return
		
		self.slotPositionsDict[selectedSlotPos] = (attachedInvenType, attachedSlotPos)		
		net.SendChatPacket("/cube add %d %d %d" % (selectedSlotPos, attachedSlotPos, attachedInvenType))
		self.wndLeftSlot.SetItemSlot(selectedSlotPos, itemIndex, itemCount)
		self.Refresh()
		
	def __OnSelectLeftItemSlot(self, selectedSlotPos):
		
		if (mouseModule.mouseController.isAttached()):
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()
			return
			
		if (not selectedSlotPos in self.slotPositionsDict):
			return
			
		snd.PlaySound("sound/ui/drop.wav")
		net.SendChatPacket("/cube del %d" % (selectedSlotPos))
		del self.slotPositionsDict[selectedSlotPos]
		self.Refresh()
		
	def __OnSelectMaterialEmptySlot(self, selectedSlotPos):
		if (mouseModule.mouseController.isAttached() == False):
			return
			
		attachedSlotType	= mouseModule.mouseController.GetAttachedType()
		attachedSlotPos		= mouseModule.mouseController.GetAttachedSlotNumber()
		mouseModule.mouseController.DeattachObject()
		
		attachedInvenType	= player.SlotTypeToInvenType(attachedSlotType)
		
		itemIndex			= player.GetItemIndex(attachedInvenType, attachedSlotPos)
		itemCount			= player.GetItemCount(attachedInvenType, attachedSlotPos)
		if (itemCount <= 1):
			itemCount		= 0
			
		if (IS_IN_MATERIAL_VNUM_RANGE(itemIndex) == False):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CUBE_IS_NOT_FIT_ITEM)
			return
			
		for slotPos, invenPosInfo in self.slotPositionsDict.items():
			(invenType, invenPos) = invenPosInfo
			if (invenPos == attachedSlotPos):
				del self.slotPositionsDict[slotPos]
			
		self.slotPositionsDict[selectedSlotPos] = (attachedInvenType, attachedSlotPos)
		net.SendChatPacket("/cube add %d %d %d" % (selectedSlotPos, attachedSlotPos, attachedInvenType))
		self.wndMaterialSlots[selectedSlotPos - 1].SetItemSlot(selectedSlotPos, itemIndex, itemCount)
		self.Refresh()
		
	def __OnSelectMaterialItemSlot(self, selectedSlotPos):
		if (mouseModule.mouseController.isAttached()):
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()
			return
			
		if (not selectedSlotPos in self.slotPositionsDict):
			return
			
		snd.PlaySound("sound/ui/drop.wav")
		net.SendChatPacket("/cube del %d" % (selectedSlotPos))
		del self.slotPositionsDict[selectedSlotPos]
		self.Refresh()
		
	def __OnOverInItem(self, slotIndex):
	
		if (self.showList and self.showListItemsDict.has_key(slotIndex)):
			itemVnum = self.showListItemsDict[slotIndex][0]
			self.tooltipItem.SetItemToolTip(itemVnum)
			return
		
		if (slotIndex == CUBE_SLOT_RIGHT and self.slotPositionsDict.has_key(CUBE_SLOT_LEFT)):
			self.tooltipItem.ClearToolTip()
			(invenType, invenPos) 	= self.slotPositionsDict[CUBE_SLOT_LEFT]
			metinSlot				= [player.GetItemMetinSocket(invenType, invenPos, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot				= [player.GetItemAttribute(invenType, invenPos, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			self.tooltipItem.AddItemData(self.rewardItemVnum, metinSlot, attrSlot, 0, 0, 0, 0, 0, -1)
			self.tooltipItem.ShowToolTip()
			return
	
		if (self.showUpgradeItems and self.showUpgradeItemsVnumDict.has_key(slotIndex)):
			self.tooltipItem.SetItemToolTip(self.showUpgradeItemsVnumDict[slotIndex])
			return
		
		if (not slotIndex in self.slotPositionsDict):
			return
			
		(invenType, invenPos) = self.slotPositionsDict[slotIndex]
		self.tooltipItem.SetInventoryItem(invenPos, invenType)
		
	def __OnOverOutItem(self):
		if (self.tooltipItem is not None):
			self.tooltipItem.Hide()
		
	def OnUpdate(self):
		
		if (self.showUpgradeItems and app.GetTime() > self.showUpgradeItemsTime + 10):
			self.showUpgradeItems 			= False
			self.showUpgradeItemsTime 		= 0
			self.showUpgradeItemsVnumDict	= {}
			for i in xrange(CUBE_SLOT_MAX):
				self.wndMaterialSlots[i].ClearSlot(i + 1)
	
		USE_CUBE_WINDOW_LIMIT_RANGE		= 1000
		(x, y, z)						= player.GetMainCharacterPosition()
		if (abs(x - self.xCubeStart) > USE_CUBE_WINDOW_LIMIT_RANGE or abs(y - self.yCubeStart) > USE_CUBE_WINDOW_LIMIT_RANGE):
			self.__OnClose()
			
