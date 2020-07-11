import app
import player
import chr
import net
import chrmgr
import event
import item
import constInfo, mouseModule, uiScriptLocale, localeInfo, uiCommon, grpImage, wndMgr, grp, ui

SPECIAL_TYPE = 1
NORMAL_TYPE = 2
FISH_EVENT_TYPE_MAX = 3

ITEM_FISH_EVENT_BOX = 25106
ITEM_FISH_EVENT_BOX_SPECIAL = 25107
	
def LoadScript(self, fileName):
	try:
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, fileName)
	except:
		import exception as exception
		exception.Abort("MiniGameFish.LoadScript")
		
class FishEventGameWaitingPage(ui.ScriptWindow):
	class DescriptionBox(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.descIndex = 0
		def __del__(self):
			ui.Window.__del__(self)
		def SetIndex(self, index):
			self.descIndex = index
		def OnRender(self):
			event.RenderEventSet(self.descIndex)
			
	def __init__(self, wndMiniGameFish):
		import exception as exception

		if not wndMiniGameFish:
			exception.Abort("wndMiniGameFish parameter must be set to MiniGameFish")
			return	
			
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.SetWindowName("FishEventGameWaitingPage")
		self.wndMiniGameFish = wndMiniGameFish
		
		self.descIndex = 0
		
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
			
	def __LoadWindow(self):
		if self.isLoaded == 1:
			return
		self.isLoaded = 1
		
		try:
			LoadScript(self, "UIScript/minigamefisheventwaitingpage.py")
		except:
			import exception as exception
			exception.Abort("FishEventGameWaitingPage.__LoadWindow.LoadObject")
			
		try:				
			self.titleBar = self.GetChild("titlebar")
			self.gameStartButton = self.GetChild("game_start_button")
			self.descBoard = self.GetChild("desc_board")
			self.prevButton = self.GetChild("prev_button")
			self.nextButton = self.GetChild("next_button")
		except:
			import exception as exception
			exception.Abort("FishEventGameWaitingPage.__LoadWindow.BindObject")
			
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.gameStartButton.SetEvent(ui.__mem_func__(self.__ClickStartButton))

		self.descriptionBox = self.DescriptionBox()
		self.descriptionBox.SetParent(self.descBoard)
		self.descriptionBox.Show()
		
		self.prevButton.SetEvent(ui.__mem_func__(self.PrevDescriptionPage))
		self.nextButton.SetEvent(ui.__mem_func__(self.NextDescriptionPage))
	
	def Show(self):
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet("locale/tr/fish_event_desc.txt")
		event.SetRestrictedCount(self.descIndex, 65)
		event.SetVisibleLineCount(self.descIndex, 8)
		
		if self.descriptionBox:
			self.descriptionBox.Show()
	
		ui.ScriptWindow.Show(self)
			
	def Close(self):
		event.ClearEventSet(self.descIndex)
		self.descIndex = 0
		
		if self.descriptionBox:
			self.descriptionBox.Hide()
			
		ui.ScriptWindow.Hide(self)
		
	def Destroy(self):
		self.isLoaded = 0
		self.ClearDictionary()
		
		self.descIndex = None
		self.titleBar = None
		self.descriptionBox = None
		
	def __ClickStartButton(self):
		if self.wndMiniGameFish:
			if self.wndMiniGameFish.wndGamePage:
				self.wndMiniGameFish.wndGamePage.Show()
				self.Close()
				
	def PrevDescriptionPage(self):
		if True == event.IsWait(self.descIndex):
			if event.GetVisibleStartLine(self.descIndex) - 8 >= 0:
				event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) - 8)
				event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)
	
	def NextDescriptionPage(self):
		if True == event.IsWait(self.descIndex):
			event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) + 8)
			event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)
		
	def OnUpdate(self):		
		(xposEventSet, yposEventSet) = self.descBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet + 7, -(yposEventSet + 7))
		self.descriptionBox.SetIndex(self.descIndex)

	def OnPressEscapeKey(self):
		self.Close()
		return True
			
class FishEventGamePage(ui.ScriptWindow):
	def __init__(self, wndMiniGameFish):
		import exception as exception

		if not wndMiniGameFish:
			exception.Abort("wndMiniGameFish parameter must be set to MiniGameFish")
			return	
			
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.SetWindowName("FishEventGamePageWindow")
		self.wndMiniGameFish = wndMiniGameFish
					
		self.tooltipItem = None
		
		self.fishBoxSlot = [ 0 for col in range(0, FISH_EVENT_TYPE_MAX)]
		self.fishBoxSlotPos = [-1 for col in range(0, FISH_EVENT_TYPE_MAX)]
		
		self.gridSlot = None
		self.currentShape = 0
		
		self.questionDialog = None
		
		self.useCountText = None
		self.useCount = 0
		
		self.scoreTextEffect = None
		self.scoreEffects = [ None for effect in xrange(3)]
		
		self.popupReward = None
		self.rewardVnum = 0

		self.fishPieceDict = {
			app.FISH_EVENT_SHAPE_1	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_1.sub", (0, 0), (1, 3)],
			app.FISH_EVENT_SHAPE_2	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_2.sub", (0, 0), (1, 1)],
			app.FISH_EVENT_SHAPE_3	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_3.sub", (-16, -16), (2, 2)],
			app.FISH_EVENT_SHAPE_4	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_4.sub", (-16, -16), (2, 2)],
			app.FISH_EVENT_SHAPE_5	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_5.sub", (-16, -16), (2, 2)],
			app.FISH_EVENT_SHAPE_6	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_6.sub", (-8, -16), (3, 2)],
			app.FISH_EVENT_SHAPE_7	: [ "D:/Ymir Work/UI/minigame/fish_event/fish_7.sub", (-8, -16), (3, 2)],
		}
		
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
				
	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip
		
	def __LoadWindow(self):
		if self.isLoaded == 1:
			return
		self.isLoaded = 1
		
		try:
			LoadScript(self, "UIScript/minigamefishgamepage.py")
		except:
			import exception as exception
			exception.Abort("FishEventGamePage.__LoadWindow.LoadObject")

		try:				
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.Close))
			self.GetChild("help_button").SetEvent(ui.__mem_func__(self.__ClickHelpButton))
			
			self.gridSlot = self.GetChild("GameSlot")
			self.gridSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__SelectEmptyGameSlot))
			self.gridSlot.SetUnselectEmptySlotEvent(ui.__mem_func__(self.__UnselectEmptyGameSlot))
			self.gridSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__UnselectItemGameSlotEvent))
			
			self.useCountText = self.GetChild("use_count_text")
			self.useCountText.SetText(str(self.useCount))
			
			for i in xrange(len(self.scoreEffects)):
				self.scoreEffects[i] = self.GetChild("score_completion_effect%d" % int(i + 1))
				self.scoreEffects[i].SetScale(1.2, 1.2)
				self.scoreEffects[i].Hide()
				self.scoreEffects[i].SetEndFrameEvent(lambda arg = i : ui.__mem_func__(self.__ScoreEffectEndFrameEvent)(arg))

			self.scoreTextEffect = self.GetChild("score_completion_text_effect")
			self.scoreTextEffect.SetEndFrameEvent(ui.__mem_func__(self.__ScoreTextEffectEndFrameEvent))
			self.scoreTextEffect.Hide()
			
			specialItemSlot = self.GetChild("SpecialItemSlot")
			specialItemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__SelectEmptySlot), 1)
			specialItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			specialItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			specialItemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot), 1)
			specialItemSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__UnselectItemSlot), 1)
			self.fishBoxSlot[1] = specialItemSlot
			
			normalItemSlot = self.GetChild("NormalItemSlot")
			normalItemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__SelectEmptySlot), 2)
			normalItemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			normalItemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			normalItemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot), 2)
			normalItemSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__UnselectItemSlot), 2)
			self.fishBoxSlot[2] = normalItemSlot
		except:
			import exception as exception
			exception.Abort("FishEventGamePage.__LoadWindow.BindObject")
			
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def Show(self):
		ui.ScriptWindow.Show(self)
		self.SetTop()
		mouseModule.mouseController.DeattachObjectPostProcess()

		for i in xrange(player.INVENTORY_PAGE_SIZE * player.INVENTORY_PAGE_COUNT):
			if player.GetItemIndex(i) == ITEM_FISH_EVENT_BOX_SPECIAL:
				self.__AddFishEventSlot(1, i)
				break
				
		for i in xrange(player.INVENTORY_PAGE_SIZE * player.INVENTORY_PAGE_COUNT):
			if player.GetItemIndex(i) == ITEM_FISH_EVENT_BOX:
				self.__AddFishEventSlot(2, i)
				break

	def Close(self):
		if player.SLOT_TYPE_FISH_EVENT == mouseModule.mouseController.GetAttachedType():
			return
							
		self.ClearFishBoxSlot()
		
		app.ShowCursor()
		
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()	
			
		if self.popupReward:
			self.popupReward.Close()
			
		ui.ScriptWindow.Hide(self)
		
	def Destroy(self):
		self.ClearFishBoxSlot()
		
		if player.SLOT_TYPE_FISH_EVENT == mouseModule.mouseController.GetAttachedType():
			self.__DropAccept()
			
		self.isLoaded = 0
		self.wndMiniGameFish = None
		
		if self.fishBoxSlot:
			del self.fishBoxSlot[:]
			
		if self.fishBoxSlotPos:
			del self.fishBoxSlotPos[:]
			
		if self.scoreEffects:
			del self.scoreEffects[:]
						
		self.tooltipItem = None	
		
		self.gridSlot = None
		self.currentShape = 0
		self.popupReward = None
		self.useCountText = None
		self.scoreTextEffect = None
		self.rewardVnum = 0

		self.ClearDictionary()
	
	def __ClickHelpButton(self):
		if player.SLOT_TYPE_FISH_EVENT == mouseModule.mouseController.GetAttachedType():
			return
			
		if self.wndMiniGameFish:
			if self.wndMiniGameFish.wndWaitingPage:
				self.wndMiniGameFish.wndWaitingPage.Show()
				self.Close()
		
	def OverInItem(self, slotIndex):
		if self.tooltipItem:
			self.tooltipItem.SetInventoryItem(self.fishBoxSlotPos[1])
				
	def	OverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
	def __UnselectItemSlot(self, slotIndex, type):
		if mouseModule.mouseController.isAttached():
			return
			
		if self.questionDialog:
			if self.questionDialog.IsShow():
				return
			
		self.ClearFishBoxSlot(type)
			
	def __UnselectEmptyGameSlot(self, slotIndex):
		if not mouseModule.mouseController.isAttached():
			return
			
		self.DropQuestionDialog()

	def	__UnselectItemGameSlotEvent(self, slotIndex):
		if not mouseModule.mouseController.isAttached():
			return
			
		self.DropQuestionDialog()
		
	def __AddFishEventSlot(self, type, pos):
		if -1 != self.fishBoxSlotPos[type]:
			return

		self.fishBoxSlotPos[type]	= pos
		self.fishBoxSlot[type].SetItemSlot(0, player.GetItemIndex(player.INVENTORY, pos), player.GetItemCount(player.INVENTORY, pos))
		self.fishBoxSlot[type].ActivateSlot(0)
		self.fishBoxSlot[type].RefreshSlot()
		
	def __SelectEmptySlot(self, slotIndex, type):
		if not mouseModule.mouseController.isAttached():
			return
		
		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
		attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
		
		if player.SLOT_TYPE_INVENTORY != attachedSlotType:
			return
			
		mouseModule.mouseController.DeattachObject()
		if attachedSlotPos >= player.INVENTORY_PAGE_SIZE * player.INVENTORY_PAGE_COUNT:
			return
			
		if 2 == type:
			if ITEM_FISH_EVENT_BOX != attachedItemIndex:
				return
		elif 1 == type:
			if ITEM_FISH_EVENT_BOX_SPECIAL != attachedItemIndex:
				return
		else:
			return

		self.fishBoxSlotPos[type] = attachedSlotPos
		self.fishBoxSlot[type].SetItemSlot(0, attachedItemIndex, player.GetItemCount(player.INVENTORY, attachedSlotPos))
		self.fishBoxSlot[type].ActivateSlot(0)
		self.fishBoxSlot[type].RefreshSlot()
		
	def __SelectEmptyGameSlot(self,	slotIndex):
		if not mouseModule.mouseController.isAttached():
			return
			
		if 0 == self.currentShape:
			return
		
		attachedSlotType = mouseModule.mouseController.GetAttachedType()
		if player.SLOT_TYPE_FISH_EVENT != attachedSlotType:
			return
			
		if self.questionDialog:
			self.questionDialog.Close()
			del self.questionDialog
			
		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetAcceptEvent(lambda arg = slotIndex : ui.__mem_func__(self.__AddAccept)(arg))
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__QuestionCancel))
		self.questionDialog.SetText(localeInfo.MINIGAME_FISH_EVENT_ADD_ACCEPT)
		self.questionDialog.Open()
		mouseModule.mouseController.SetAttachedIconRender(False)
		
		app.ShowCursor()
		
		if self.gridSlot:
			self.gridSlot.SetPickedAreaRender(False)
			
	def __AddAccept(self, slotIndex):
		self.__QuestionCancel()
		net.SendAddFishBox(slotIndex)
		self.currentShape = 0
		self.DeattachObject()
		
	def MiniGameFishAdd(self, pos, shape):
		shapeImage = self.fishPieceDict[shape][0]
		
		positionInfo = {
			app.FISH_EVENT_SHAPE_1 : [0, 6, 12],
			app.FISH_EVENT_SHAPE_2 : [0],
			app.FISH_EVENT_SHAPE_3 : [0, 6, 7],
			app.FISH_EVENT_SHAPE_4 : [0, 1, 7],
			app.FISH_EVENT_SHAPE_5 : [0, 1, 6, 7],
			app.FISH_EVENT_SHAPE_6 : [0, 1, 7, 8],
			app.FISH_EVENT_SHAPE_7 : [0, 1, 2, 6, 7, 8],
		}
		
		if shape in positionInfo:
			self.gridSlot.SetCoverButton(pos, shapeImage, shapeImage, shapeImage, shapeImage, False, False)
			self.gridSlot.SetAlwaysRenderCoverButton(pos)
			for i in xrange(len(positionInfo[shape])):
				self.gridSlot.SetSlot(pos + positionInfo[shape][i], 1, 1, 1, 0)
				
			self.gridSlot.RefreshSlot()
			
	def MiniGameFishReward(self, vnum):
		self.rewardVnum = vnum
		self.__ClearCompletionEffect()
		
		for scoreEffect in self.scoreEffects:
			if scoreEffect:
				scoreEffect.Show()
			
		if self.scoreTextEffect:
			self.scoreTextEffect.Show()
			
	def ClearFishBoxSlot(self, type = FISH_EVENT_TYPE_MAX):
		if FISH_EVENT_TYPE_MAX == type:
			for index in range(1, FISH_EVENT_TYPE_MAX):
				if not self.fishBoxSlot[index]:
					continue
				
				if self.fishBoxSlotPos[index] == -1:
					continue
	
				self.fishBoxSlot[index].SetItemSlot(0, 0)
				self.fishBoxSlot[index].DeactivateSlot(0)
				self.fishBoxSlot[index].RefreshSlot()
				self.fishBoxSlotPos[index] = -1
			
		else:	
			if not self.fishBoxSlot[type]:
				return
			
			if self.fishBoxSlotPos[type] == -1:
				return
				
			self.fishBoxSlot[type].SetItemSlot(0, 0)
			self.fishBoxSlot[type].DeactivateSlot(0)
			self.fishBoxSlot[type].RefreshSlot()
			self.fishBoxSlotPos[type] = -1
			
				
	def SelectItemSlot(self, slotIndex, type):
		if 0 != self.currentShape:
			return
			
		mouseModule.mouseController.DeattachObject()
			
		if self.questionDialog:
			self.questionDialog.Close()
			del self.questionDialog
			
		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetAcceptEvent(lambda arg = type : ui.__mem_func__(self.__UseAccept)(arg))
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseAcceptCancel))
		self.questionDialog.SetText(localeInfo.MINIGAME_FISH_EVENT_USE_ACCEPT)
		self.questionDialog.Open()
		
	def __UseAccept(self, type):
		self.__QuestionCancel()
		if self.fishBoxSlotPos[type] != -1:
			net.SendUseFishBox(player.INVENTORY, self.fishBoxSlotPos[type])
			
	def __UseAcceptCancel(self):
		if self.questionDialog:
			self.questionDialog.Close()
			self.questionDialog = None
			
	def __QuestionCancel(self):
		if self.questionDialog:
			self.questionDialog.Close()
			self.questionDialog = None
			
		if self.gridSlot:
			self.gridSlot.SetPickedAreaRender(True)
			
		mouseModule.mouseController.SetAttachedIconRender(True)
		app.HideCursor()
		
	def DropQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()
			del self.questionDialog
			
		self.questionDialog = uiCommon.QuestionDialog()
		self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__DropAccept))
		self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__QuestionCancel))
		self.questionDialog.SetText(localeInfo.MINIGAME_FISH_EVENT_DROP_ACCEPT)
		self.questionDialog.Open()
		self.questionDialog.SetTop()
		
		mouseModule.mouseController.SetAttachedIconRender(False)
		app.ShowCursor()
		
		if self.gridSlot:
			self.gridSlot.SetPickedAreaRender(False)
		
	def __DropAccept(self):
		self.__QuestionCancel()
		self.currentShape = 0
		app.ShowCursor()
		wndMgr.SetDisableDeattach(False)
		self.DeattachObject()
		
	def MiniGameFishCount(self, useCount):
		self.useCount = useCount
		if self.useCountText:
			self.useCountText.SetText(str(self.useCount))
		
	def MiniGameFishUse(self, shape, useCount):
		self.currentShape = shape
		
		for index in range(1, FISH_EVENT_TYPE_MAX):
			item_count = player.GetItemCount(player.INVENTORY, self.fishBoxSlotPos[index])
			item_vnum  = player.GetItemIndex(player.INVENTORY, self.fishBoxSlotPos[index])
			
			if 0 == item_count:
				self.ClearFishBoxSlot(index)
			else:
				self.fishBoxSlot[index].SetItemSlot(0, item_vnum, item_count)
				self.fishBoxSlot[index].RefreshSlot()
		
		app.HideCursor()
				
		(adjust_x, adjust_y) = self.fishPieceDict[shape][1]
		(width, height) = self.fishPieceDict[shape][2]
		
		if shape == app.FISH_EVENT_SHAPE_1:
			mouseModule.mouseController.AttachFishPiece(self, 3000, grpImage.Generate(self.fishPieceDict[shape][0]), adjust_x, adjust_y, width, height)
		elif shape == app.FISH_EVENT_SHAPE_2:
			mouseModule.mouseController.AttachFishPiece(self, 71084, grpImage.Generate(self.fishPieceDict[shape][0]), adjust_x, adjust_y, width, height)
		else:
			mouseModule.mouseController.AttachFishPiece(self, shape, grpImage.Generate(self.fishPieceDict[shape][0]), adjust_x, adjust_y, width, height)
		
		for type in range(1, FISH_EVENT_TYPE_MAX):
			self.fishBoxSlot[type].DeactivateSlot(0)
			
		wndMgr.SetDisableDeattach(True)
		
		self.useCount = useCount
		if self.useCountText:
			self.useCountText.SetText(str(self.useCount))
		
	def DeattachObject(self):
		mouseModule.mouseController.DeattachObjectPostProcess()
		
		for type in range(1, FISH_EVENT_TYPE_MAX):
			item_count = player.GetItemCount(player.INVENTORY, self.fishBoxSlotPos[type])
			if item_count:
				self.fishBoxSlot[type].ActivateSlot(0)
			else:
				self.fishBoxSlot[type].DeactivateSlot(0)
				
	def __ScoreEffectEndFrameEvent(self, type):
		if self.scoreEffects[type]:
			self.scoreEffects[type].Hide()
		
	def __ScoreTextEffectEndFrameEvent(self):
		if self.scoreTextEffect: 
			self.scoreTextEffect.Hide()
			
		self.useCount = 0
		if self.useCountText:
			self.useCountText.SetText(str(self.useCount))
		
		for slotPos in xrange(self.gridSlot.GetSlotCount()):
			self.gridSlot.DeleteCoverButton(slotPos)
			self.gridSlot.ClearSlot(slotPos)			
		self.gridSlot.RefreshSlot()
		
		if not self.popupReward:
			self.popupReward = uiCommon.PopupDialog()
			
		item.SelectItem(self.rewardVnum)
		self.popupReward.SetText(localeInfo.MINIGAME_FISH_EVENT_REWARD_MSG % item.GetItemName())
		self.popupReward.Open()
		
		self.rewardVnum = 0
		
	def __ClearCompletionEffect(self):
		if self.scoreTextEffect:
			self.scoreTextEffect.Hide()
			self.scoreTextEffect.ResetFrame()
			self.scoreTextEffect.SetDelay(6)
			
		for scoreEffect in self.scoreEffects:
			if scoreEffect:
				scoreEffect.Hide()
				scoreEffect.ResetFrame()
				scoreEffect.SetDelay(6)
		
class MiniGameFish(ui.Window):
	wndWaitingPage		= None
	wndGamePage			= None
	
	def __init__(self):
		ui.Window.__init__(self)
		self.SetWindowName("MiniGameFishWindow")
		self.wndWaitingPage	= FishEventGameWaitingPage(self)
		self.wndGamePage = FishEventGamePage(self)

	def __del__(self):
		ui.Window.__del__(self)
		
	def Destroy(self):
		self.isLoaded = 0
		
		if self.wndWaitingPage:
			self.wndWaitingPage.Destroy()
			self.wndWaitingPage = None
			
		if self.wndGamePage:
			self.wndGamePage.Destroy()
			self.wndGamePage = None
			
	def Open(self):
		if self.wndGamePage:
			if self.wndGamePage.IsShow():
				return
				
		if self.wndWaitingPage:
			self.wndWaitingPage.Show()
		
	def Close(self):
		if self.wndWaitingPage:
			if self.wndWaitingPage.IsShow():
				self.wndWaitingPage.Close()
				
		if self.wndGamePage:
			if self.wndGamePage.IsShow():
				self.wndGamePage.Close()

	def MiniGameFishUse(self, shape, useCount):
		if self.wndGamePage:
			self.wndGamePage.MiniGameFishUse(shape, useCount)
			
	def MiniGameFishAdd(self, pos, shape):
		if self.wndGamePage:
			self.wndGamePage.MiniGameFishAdd(pos, shape)
		
	def MiniGameFishReward(self, vnum):
		if self.wndGamePage:
			self.wndGamePage.MiniGameFishReward(vnum)
			
	def MiniGameFishCount(self, count):
		if self.wndGamePage:
			self.wndGamePage.MiniGameFishCount(count)	
	
	def CantFishEventSlot(self, InvenSlot):
		ItemVnum = player.GetItemIndex(InvenSlot)
		if ItemVnum in [ITEM_FISH_EVENT_BOX, ITEM_FISH_EVENT_BOX_SPECIAL]:
			return False
				
		return True
		
	def SetItemToolTip(self, tooltip):
		if self.wndGamePage:
			self.wndGamePage.SetItemToolTip(tooltip)	
		