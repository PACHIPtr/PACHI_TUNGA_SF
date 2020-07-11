import net
import player
import item
import event
import chat

import ui
import mouseModule
import snd
import localeInfo
import grp
import uiScriptLocale
import uiToolTip
import uiCommon

class CardsInfoWindow(ui.ScriptWindow):
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
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.descIndex=0
		self.scrollPos = 0
		self.safemode = 1
		self.questionDialog = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/minigamerumiwaitingpage.py")
		except:
			import exception
			exception.Abort("minigamerumiwaitingpage.LoadDialog.LoadScript")

		try:
			GetObject=self.GetChild
			self.titleBar = GetObject("titlebar")
			self.textBoard = GetObject("desc_board")
			self.scrollBar = GetObject("scrollbar")
			self.checkButton = GetObject("check_image")
			self.checkButton2 = GetObject("confirm_check_button")
			self.startButton = GetObject("game_start_button")

		except:
			import exception
			exception.Abort("CubeWindow.LoadDialog.BindObject")

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.__OnCloseButtonClick))
		self.scrollBar.SetPos(0.0)
		self.scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.checkButton.SetEvent(ui.__mem_func__(self.__OnSafeMode))
		self.checkButton2.SetEvent(ui.__mem_func__(self.__OnSafeMode))
		self.startButton.SetEvent(ui.__mem_func__(self.__OnStartQuestion))

	def Destroy(self):
		self.ClearDictionary()
		self.titleBar = None
		self.textBoard = None
		self.descriptionBox = None
		self.scrollPos = None
		self.safemode = None
		self.questionDialog = None
	
	def Open(self):
		self.__SetDescriptionEvent()
		self.__CreateDescriptionBox()
		self.scrollBar.SetPos(0.0)
		self.Show()

	def Close(self):
		event.ClearEventSet(self.descIndex)
		self.descIndex = 0
		self.Hide()
		
	def __OnStartQuestion(self):
		questionDialog = uiCommon.QuestionDialog2()
		questionDialog.SetText1(localeInfo.MINI_GAME_RUMI_START_QUESTION % (1000000, 1))
		questionDialog.SetText2(localeInfo.MINI_GAME_RUMI_START_QUESTION2)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.__OnStart))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog
		
	def OnCloseQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None
		
	def __OnStart(self):
		self.OnCloseQuestionDialog()
		self.Close()
		net.SendChatPacket("/cards o "+ str(self.safemode))
		
	def __OnSafeMode(self):
		if self.safemode == 0:
			self.safemode = 1
			self.checkButton.Show()
		else:
			self.checkButton.Hide()
			self.safemode = 0
		
	def OnUpdate(self):
		(xposEventSet, yposEventSet) = self.textBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet+7, -(yposEventSet+7-(int(self.scrollPos) * 16)))
		self.descriptionBox.SetIndex(self.descIndex)
		
	def OnScroll(self):
		import math
		pos = self.scrollBar.GetPos()
		self.scrollPos = math.floor(float(pos) / float(float(1) / float(event.GetLineCount(self.descIndex) - 18)) + 0.001)
		event.SetVisibleStartLine(self.descIndex, int(self.scrollPos))
		event.Skip(self.descIndex)

	def __SetDescriptionEvent(self):
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet(uiScriptLocale.CARDS_DESC)
		event.SetRestrictedCount(self.descIndex, 100)
		event.SetVisibleLineCount(self.descIndex, 18)

	def __CreateDescriptionBox(self):
		self.descriptionBox = self.DescriptionBox()
		self.descriptionBox.Show()

	def __OnCloseButtonClick(self):
		self.Close()

	def OnPressEscapeKey(self):
		if 0 != self.eventClose:
			self.eventClose()
		return True
		
class CardsWindow(ui.ScriptWindow):
	CARDS_ICONS = {	
		1 : "d:/ymir work/ui/minigame/rumi/card/card_blue_%d.sub",
		2 : "d:/ymir work/ui/minigame/rumi/card/card_red_%d.sub",
		3 : "d:/ymir work/ui/minigame/rumi/card/card_yellow_%d.sub", }
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.safemode = 0
		self.questionDialog = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/minigamerumigamepage.py")
		except:
			import exception
			exception.Abort("minigamerumigamepage.LoadDialog.LoadScript")

		try:
			GetObject=self.GetChild
			self.titleBar = GetObject("titlebar")
			self.handSlot = GetObject("HandCardSlot")
			self.fieldSlot = GetObject("FieldCardSlot")
			self.deckSlot = GetObject("DeckCardSlot")
			self.score_completion_effect1 = GetObject("score_completion_effect1")
			self.score_completion_effect2 = GetObject("score_completion_effect2")
			self.score_completion_effect3 = GetObject("score_completion_effect3")
			self.score_completion_text_effect = GetObject("score_completion_text_effect")
			self.deck_flush_effect = GetObject("deck_flush_effect")
			self.cards_count = GetObject("card_cnt_text")
			self.total_points = GetObject("total_score")
			self.field_points = GetObject("score_number_text")
			self.exitButton = GetObject("game_exit_button")

		except:
			import exception
			exception.Abort("CubeWindow.LoadDialog.BindObject")

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.__OnCloseButtonClick))
		self.handSlot.SAFE_SetButtonEvent("LEFT", "EXIST", self.SetSelectItemSlotEvent)
		self.handSlot.SAFE_SetButtonEvent("RIGHT", "EXIST", self.SetUnselectItemSlotEvent)
		self.fieldSlot.SAFE_SetButtonEvent("LEFT", "EXIST", self.SetSelectItemSlotEvent2)
		self.score_completion_effect1.SetOnEndFrame(ui.__mem_func__(self.__OnEndFrame))
		self.deckSlot.SAFE_SetButtonEvent("LEFT", "ALWAYS", self.SetPickCardFromDeck)
		self.exitButton.SetEvent(ui.__mem_func__(self.__EndGame))
		self.HideEffects()
		
		
	def SetPickCardFromDeck(self, slotIndex):
		if int(self.cards_count.GetText()) < 1:
			return
		net.SendChatPacket("/cards p")
	def SetSelectItemSlotEvent(self, slotIndex):
		net.SendChatPacket("/cards a " + str(slotIndex))
	def SetSelectItemSlotEvent2(self, slotIndex):
		net.SendChatPacket("/cards r " + str(slotIndex))
	def SetUnselectItemSlotEvent(self, slotIndex):
		if self.safemode == 1:
			questionDialog = uiCommon.QuestionDialog()
			questionDialog.SetText(localeInfo.MINI_GAME_RUMI_DISCARD_QUESTION)
			questionDialog.SetAcceptEvent(lambda arg=True: self.DestroyCard(slotIndex))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
			questionDialog.Open()
			self.questionDialog = questionDialog
		else:
			self.DestroyCard(slotIndex)
			
	def __EndGame(self):
		questionDialog = uiCommon.QuestionDialog2()
		questionDialog.SetText1(localeInfo.MINI_GAME_RUMI_EXIT_QUESTION)
		questionDialog.SetText2(localeInfo.MINI_GAME_RUMI_EXIT_QUESTION2)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnEndGame))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog
		
	def OnEndGame(self):
		net.SendChatPacket("/cards e")
		self.OnCloseQuestionDialog()
		self.Close()
	def DestroyCard(self, index):
		net.SendChatPacket("/cards d " + str(index))
		self.OnCloseQuestionDialog()
		
	def OnCloseQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None
		
	def __OnEndFrame(self):
		self.HideEffects()
		
	def UpdateCardsInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
		if (hand_1 == 0):
			self.handSlot.ClearSlot(0)
		elif (hand_1 != 0):
			self.handSlot.SetCardSlot(0, 10, (self.CARDS_ICONS[hand_1] % (int(hand_1_v))))
		if (hand_2 == 0):
			self.handSlot.ClearSlot(1)
		elif (hand_2 != 0):
			self.handSlot.SetCardSlot(1, 10, (self.CARDS_ICONS[hand_2] % (int(hand_2_v))))
		if (hand_3 == 0):
			self.handSlot.ClearSlot(2)
		elif (hand_3 != 0):
			self.handSlot.SetCardSlot(2, 10, (self.CARDS_ICONS[hand_3] % (int(hand_3_v))))
		if (hand_4 == 0):
			self.handSlot.ClearSlot(3)
		elif (hand_4 != 0):
			self.handSlot.SetCardSlot(3, 10, (self.CARDS_ICONS[hand_4] % (int(hand_4_v))))
		if (hand_5 == 0):
			self.handSlot.ClearSlot(4)
		elif (hand_5 != 0):
			self.handSlot.SetCardSlot(4, 10, (self.CARDS_ICONS[hand_5] % (int(hand_5_v))))
		self.cards_count.SetText(str(cards_left))
		self.total_points.SetText(str(points))
		self.UpdateDeckSlot()
		
	def UpdateCardsFieldInfo(self, slot1, slot1_v, slot2, slot2_v, slot3, slot3_v, points):
		if (slot1 == 0):
			self.fieldSlot.ClearSlot(0)
		elif (slot1 != 0):
			self.fieldSlot.SetCardSlot(0, 10, (self.CARDS_ICONS[slot1] % (int(slot1_v))))
		if (slot2 == 0):
			self.fieldSlot.ClearSlot(1)
		elif (slot2 != 0):
			self.fieldSlot.SetCardSlot(1, 10, (self.CARDS_ICONS[slot2] % (int(slot2_v))))
		if (slot3 == 0):
			self.fieldSlot.ClearSlot(2)
		elif (slot3 != 0):
			self.fieldSlot.SetCardSlot(2, 10, (self.CARDS_ICONS[slot3] % (int(slot3_v))))
		self.field_points.SetText(str(points))
		
	def UpdateDeckSlot(self):
		if int(self.cards_count.GetText()) > 16:
			self.deckSlot.SetCardSlot(2, 10, "d:/ymir work/ui/minigame/rumi/deck/deck3.sub")
			self.deckSlot.SetCardSlot(1, 10, "d:/ymir work/ui/minigame/rumi/deck/deck2.sub")
		elif int(self.cards_count.GetText()) <= 16 and int(self.cards_count.GetText()) > 8:
			self.deckSlot.ClearSlot(2)
			self.deckSlot.SetCardSlot(1, 10, "d:/ymir work/ui/minigame/rumi/deck/deck2.sub")
		elif int(self.cards_count.GetText()) <= 8 and int(self.cards_count.GetText()) > 0:
			self.deckSlot.ClearSlot(2)
			self.deckSlot.ClearSlot(1)
			self.deckSlot.SetCardSlot(0, 10, "d:/ymir work/ui/minigame/rumi/deck/deck1.sub")
		else:
			self.deckSlot.ClearSlot(0)
			self.deckSlot.ClearSlot(1)
			self.deckSlot.ClearSlot(2)
		
	def CardsPutReward(self, slot1, slot1_v, slot2, slot2_v, slot3, slot3_v, points):
		self.score_completion_effect1.ResetFrame()
		self.score_completion_effect1.Show()
		self.score_completion_effect2.ResetFrame()
		self.score_completion_effect2.Show()
		self.score_completion_effect3.ResetFrame()
		self.score_completion_effect3.Show()
		self.score_completion_text_effect.ResetFrame()
		self.score_completion_text_effect.Show()

	def HideEffects(self):
		self.score_completion_effect1.Hide()
		self.score_completion_effect2.Hide()
		self.score_completion_effect3.Hide()
		self.score_completion_text_effect.Hide()
		self.deck_flush_effect.Hide()
		self.field_points.SetText("0")

	def Destroy(self):
		self.ClearDictionary()
		self.titleBar = None
		self.handSlot = None
		self.fieldSlot = None
		self.deckSlot = None
		self.score_completion_effect1 = None
		self.score_completion_effect2 = None
		self.score_completion_effect3 = None
		self.score_completion_text_effect = None
		self.deck_flush_effect = None
		self.cards_count = None
		self.total_points = None
		self.field_points = None
		self.safemode = None
		self.questionDialog = None

	def Open(self, safemode):
		self.safemode = safemode
		self.Show()

	def Close(self):
		self.Hide()

	def __OnCloseButtonClick(self):
		self.Close()

	def OnPressEscapeKey(self):
		if 0 != self.eventClose:
			self.eventClose()
		return True
		
class IngameWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.icon = 0
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/minigamewindow.py")
		except:
			import exception
			exception.Abort("minigamewindow.LoadDialog.LoadScript")

		try:
			GetObject=self.GetChild
			self.gameButton = GetObject("minigame_rumi_button")

		except:
			import exception
			exception.Abort("CubeWindow.LoadDialog.BindObject")

		self.gameButton.SetEvent(ui.__mem_func__(self.__OnClickGame))
	def __OnClickGame(self):
		self.window = CardsInfoWindow()
		self.window.Open()
		
	def Open(self):
		pass
		#self.Show()
		
	def Destroy(self):
		self.ClearDictionary()
