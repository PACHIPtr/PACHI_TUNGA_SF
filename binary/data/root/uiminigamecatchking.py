import ui
import uiScriptLocale
import wndMgr
import player
import localeInfo
import net
import app
import constInfo
import event
import uiCommon
import grpImage
import grp
import chat
import item
import uiToolTip
import dbg

from _weakref import proxy
from collections import deque

SHOW_LINE_COUNT_MAX = 15
SHOW_LINE_HEIGHT = 12
SHOW_LINE_GAP = 4

DEFAULT_DESC_Y	= 7

CARD_SET_ITEM_VNUM = 79604
CARD_SET_BET_MAX_NUM = 5

LOW_SCORE_VNUM = 50930
MID_SCORE_VNUM = 50929
HIGH_SCORE_VNUM = 50928

def LoadScript(self, fileName):
	try:
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, fileName)
	except:
		import exception as exception
		exception.Abort("MiniGameCatchKing.LoadScript")
	
class CatchKingWaitingPage(ui.ScriptWindow):
	class DescriptionBox(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.descIndex = -1
		def __del__(self):
			ui.Window.__del__(self)
		def SetIndex(self, index):
			self.descIndex = index
		def OnRender(self):
			event.RenderEventSet( self.descIndex )
	
	def __init__(self, wndMiniGameCatchKing):
		import exception as exception

		if not wndMiniGameCatchKing:
			exception.Abort("wndMiniGameCatchKing parameter must be set to MiniGameCatchKing")
			return	
	
		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.SetWindowName("CatchKingEventGameWaitingPage")
		self.wndMiniGameCatchKing = wndMiniGameCatchKing

		self.startButton = None
		self.prevButton = None
		self.nextButton = None
		
		self.descBoard = None
		self.descriptionBox = None
		self.descIndex = -1
		self.descY = 0
		
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
	
	def __LoadWindow(self):
		if self.isLoaded == 1:
			return
		self.isLoaded = 1
		
		try:
			LoadScript(self, "UIScript/minigamecatchkingwaitingpage.py")
		except:
			import exception as exception
			exception.Abort("MiniGameCatchKingWaitingPage.LoadWindow.LoadObject")
			
		try:
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))
			
			self.startButton = self.GetChild("game_start_button")
			self.prevButton = self.GetChild("prev_button")
			self.nextButton = self.GetChild("next_button")

			self.descBoard = self.GetChild("desc_board")
		except:
			import exception as exception
			exception.Abort("MiniGameCatchKingWaitingPage.LoadWindow.BindObject")
			
		try:
			self.startButton.SetEvent(ui.__mem_func__(self.__ClickStartButton))
			self.prevButton.SetEvent(ui.__mem_func__(self.PrevDescriptionPage))
			self.nextButton.SetEvent(ui.__mem_func__(self.NextDescriptionPage))
			
			self.descriptionBox = self.DescriptionBox()
			self.descriptionBox.SetParent(self.descBoard)
			self.descriptionBox.Show()
		except:
			import exception as exception
			exception.Abort("MiniGameCatchKingWaitingPage.LoadWindow.BindEvent")	
		
		self.Hide()
		
	def Close(self):
		self.Hide()

		event.ClearEventSet(self.descIndex)
		self.descIndex = -1
		
		if self.descriptionBox:
			self.descriptionBox.Hide()
			
		self.descY = 0

	def Destroy(self):
		self.isLoaded = 0
		
		self.startButton = None
		self.prevButton = None
		self.nextButton = None
		
		self.descBoard = None
		self.descriptionBox = None
		self.descIndex = -1
		self.descY = 0

	def OnPressEscapeKey(self):
		self.Close()
		return True	
			
	def __ClickStartButton(self):
		if self.wndMiniGameCatchKing:
			if self.wndMiniGameCatchKing.wndSimpleWaitingPage:
				self.wndMiniGameCatchKing.wndSimpleWaitingPage.Show()
				
		self.Close()
		
	def OnUpdate(self):
		(xposEventSet, yposEventSet) = self.descBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet + 7, -(yposEventSet + 7 + self.descY))
		self.descriptionBox.SetIndex(self.descIndex)
				
	def Show(self):
		ui.ScriptWindow.Show(self)
		
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet( uiScriptLocale.CATCH_KING_DESC )
		event.SetFontColor(self.descIndex, 0.7843, 0.7843, 0.7843, 1.0)
		#aici
		event.SetVisibleLineCount(self.descIndex, SHOW_LINE_COUNT_MAX)
		event.SetRestrictedCount(self.descIndex, 70)
		event.Skip(self.descIndex)

		if self.descriptionBox:
			self.descriptionBox.Show()
			
	def PrevDescriptionPage(self):
		curStartLine = event.GetVisibleStartLine(self.descIndex)
		
		if curStartLine - SHOW_LINE_COUNT_MAX < 0:
			return

		event.SetVisibleStartLine(self.descIndex, curStartLine - SHOW_LINE_COUNT_MAX)
		self.descY += ((SHOW_LINE_HEIGHT + SHOW_LINE_GAP) * SHOW_LINE_COUNT_MAX)
			
	def NextDescriptionPage(self):
		curStartLine = event.GetVisibleStartLine(self.descIndex)
		increaseCount = SHOW_LINE_COUNT_MAX
		
		if curStartLine + increaseCount >= 88:
			increaseCount = 88 - curStartLine

		if increaseCount < 0 or curStartLine + increaseCount >= 88:
			return
		
		event.SetVisibleStartLine(self.descIndex, curStartLine + increaseCount)
		self.descY -= ((SHOW_LINE_HEIGHT + SHOW_LINE_GAP) * increaseCount)
	
class CatchKingSimpleWaitingPage(ui.ScriptWindow):
	class DescriptionBox(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.descIndex = -1
		def __del__(self):
			ui.Window.__del__(self)
		def SetIndex(self, index):
			self.descIndex = index
		def OnRender(self):
			event.RenderEventSet( self.descIndex )
	
	def __init__(self, wndMiniGameCatchKing):
		import exception as exception

		if not wndMiniGameCatchKing:
			exception.Abort("wndMiniGameCatchKing parameter must be set to MiniGameCatchKing")
			return	
	
		ui.ScriptWindow.__init__(self)

		self.isLoaded = 0
		self.SetWindowName("CatchKingEventGameSimpleWaitingPage")
		self.wndMiniGameCatchKing = wndMiniGameCatchKing

		self.startButton = None
		self.helpButton = None
		self.itemSlot = None
		
		self.challengeWindow = None
		self.upArrowButton = None
		self.downArrowButton = None
		self.challengeCount = None
		
		self.descBoard = None
		self.descriptionBox = None
		self.descIndex = -1
		self.descY = 0
		
		self.startQuestionDialog = None
		self.betNumber = 1
		
		self.toolTip = None
		self.isShowToolTip = False
		
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
	
	def __LoadWindow(self):
		if self.isLoaded == 1:
			return
		self.isLoaded = 1
		
		try:
			LoadScript(self, "UIScript/minigamecatchkingsimplewaitingpage.py")
		except:
			import exception as exception
			exception.Abort("MiniGameCatchKingSimpleWaitingPage.LoadWindow.LoadObject")
			
		try:
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))
			
			self.startButton = self.GetChild("game_start_button")
			self.helpButton = self.GetChild("help_button")
			self.itemSlot = self.GetChild("ChallengeItemSlot")
			
			self.challengeWindow = self.GetChild("challenge_text_window")
			self.upArrowButton = self.GetChild("up_arrow_button")
			self.downArrowButton = self.GetChild("down_arrow_button")
			self.challengeCount = self.GetChild("challenge_count_text")

			self.descBoard = self.GetChild("desc_board")
			
			self.GetChild("prev_button").Hide()
			self.GetChild("next_button").Hide()
		except:
			import exception as exception
			exception.Abort("MiniGameCatchKingSimpleWaitingPage.LoadWindow.BindObject")
			
		try:
			self.startButton.SetEvent(ui.__mem_func__(self.__ClickStartButton))
			self.helpButton.SetEvent(ui.__mem_func__(self.__ClickHelpButton))
			
			self.toolTip = uiToolTip.ToolTip()
			self.toolTip.ClearToolTip()
			
			self.challengeWindow.SetOverInEvent(ui.__mem_func__(self.__OverInChallengeText))
			self.challengeWindow.SetOverOutEvent(ui.__mem_func__(self.__OverOutChallengeText))
			#aici
			
			self.upArrowButton.SetEvent(ui.__mem_func__(self.__ClickUpArrowButton))
			self.downArrowButton.SetEvent(ui.__mem_func__(self.__ClickDownArrowButton))
			
			self.descriptionBox = self.DescriptionBox()
			self.descriptionBox.SetParent(self.descBoard)
			self.descriptionBox.Show()
		except:
			import exception as exception
			exception.Abort("MiniGameCatchKingSimpleWaitingPage.LoadWindow.BindEvent")
		
		self.Hide()
		
	def Close(self):
		self.Hide()
		self.CloseStartDlg()
		
		self.isShowToolTip = False

		event.ClearEventSet(self.descIndex)
		self.descIndex = -1
		
		if self.descriptionBox:
			self.descriptionBox.Hide()
			
		self.descY = 0

	def Destroy(self):
		self.isLoaded = 0
		
		self.startButton = None
		self.helpButton = None
		self.itemSlot = None
		
		self.challengeWindow = None
		self.upArrowButton = None
		self.downArrowButton = None	
		self.challengeCount = None
		
		self.descBoard = None
		self.descriptionBox = None
		self.descIndex = -1
		self.descY = 0
		
		self.startQuestionDialog = None
		self.betNumber = 1
		
		self.toolTip = None
		self.isShowToolTip = False

	def OnPressEscapeKey(self):
		self.Close()
		return True	
			
	def __ClickStartButton(self):
		if None == self.startQuestionDialog:
			self.startQuestionDialog = uiCommon.QuestionDialog2()
			self.startQuestionDialog.SetAcceptEvent(ui.__mem_func__(self.__StartAccept))
			self.startQuestionDialog.SetCancelEvent(ui.__mem_func__(self.__StartCancel))
			
		item.SelectItem(CARD_SET_ITEM_VNUM)
		startText = localeInfo.MINI_GAME_CATCHKING_START_QUESTION.split("\\n")
		
		self.startQuestionDialog.SetText1(startText[0] % (30000 * self.betNumber, item.GetItemName(), self.betNumber))
		self.startQuestionDialog.SetText2(startText[1])
		self.startQuestionDialog.AutoResize()
		#aici
		self.startQuestionDialog.Open()
		
	def CloseStartDlg(self):
		if self.startQuestionDialog:
			self.startQuestionDialog.Close() 
			self.startQuestionDialog = None
	
	def __StartAccept(self):
		net.SendMiniGameCatchKing(0, self.betNumber)
	
	def __StartCancel(self):
		if self.startQuestionDialog:
			self.startQuestionDialog.Close()
		
	def __ClickHelpButton(self):
		if self.wndMiniGameCatchKing:
			if self.wndMiniGameCatchKing.wndWaitingPage:
				self.wndMiniGameCatchKing.wndWaitingPage.Show()
				
		self.Close()
		
	def OnUpdate(self):
		(xposEventSet, yposEventSet) = self.descBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet + 7, -(yposEventSet + 7 + self.descY))
		self.descriptionBox.SetIndex(self.descIndex)
		
		if self.isShowToolTip:
			if self.toolTip:
				pos_x, pos_y = wndMgr.GetMousePosition()
				self.toolTip.SetToolTipPosition(pos_x, pos_y + 100)
				
	def Show(self):
		ui.ScriptWindow.Show(self)
		
		self.itemSlot.SetItemSlot(0, CARD_SET_ITEM_VNUM, 0)
		
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet( uiScriptLocale.CATCH_KING_SIMPLE )
		event.SetFontColor(self.descIndex, 0.7843, 0.7843, 0.7843, 1.0)
		#aici
		event.SetVisibleLineCount(self.descIndex, SHOW_LINE_COUNT_MAX)
		event.SetRestrictedCount(self.descIndex, 70)
		event.Skip(self.descIndex)

		if self.descriptionBox:
			self.descriptionBox.Show()

	def __OverInChallengeText(self):
		if self.toolTip:
			pos_x, pos_y = wndMgr.GetMousePosition()
			
			item.SelectItem(CARD_SET_ITEM_VNUM)
			
			self.toolTip.ClearToolTip()
			self.toolTip.SetThinBoardSize(10 * 50)
			self.toolTip.SetToolTipPosition(pos_x, pos_y + 100)
			self.toolTip.AppendTextLine(localeInfo.MINI_GAME_CATCHKING_CHALLENGE_TOOLTIP1 % item.GetItemName())
			self.toolTip.AppendTextLine(localeInfo.MINI_GAME_CATCHKING_CHALLENGE_TOOLTIP2 % CARD_SET_BET_MAX_NUM)
			self.toolTip.Show()
		
			self.isShowToolTip = True
		
	def __OverOutChallengeText(self):
		if self.toolTip:
			self.toolTip.Hide()
			
		self.isShowToolTip = False
		
	def __ClickUpArrowButton(self):
		if self.betNumber == CARD_SET_BET_MAX_NUM:
			return
			
		tempBet = self.betNumber + 1
		
		if tempBet > constInfo.COUNT_SPECIFY_ITEM(CARD_SET_ITEM_VNUM):
			#aici
			return
			
		self.betNumber = tempBet
		self.challengeCount.SetText(str(self.betNumber))
		
	def __ClickDownArrowButton(self):
		if self.betNumber == 1:
			return
			
		self.betNumber = self.betNumber - 1
		self.challengeCount.SetText(str(self.betNumber))
		
class CatchKingCard:
	def __init__(self, miniGame, cardType, parent, x, y, clickFunc, clickArg):
		self.pos = (x, y)
		self.posGlobal = parent.GetLocalPosition()

		self.backgroundImage = None
		self.backgroundOverImage = None
		self.backgroundRowImage = None
		self.fiveNearEffect = None
		self.arrowImage = None
		self.cardCountImage = None
		self.cardCountText = None
		self.destroyCardEffect = None
		self.miniGame = miniGame
		self.cardNumber = 0
		self.cardCount = 0
		self.isTheEnd = False

		self.__CreateBackgroundImg(parent, cardType, clickFunc, clickArg)
		
		if cardType == 0:
			self.__CreateBackgroundOverImg(parent)
			self.__CreateCardDestroyEffect(parent)
			self.__CreateFiveNearEffect(parent)
			self.__CreateBackgroundRowImg(parent)
		
		self.__CreateArrowImg(parent, cardType)
		
		if cardType == 1:
			self.__CreateCardCount(parent)
			
	def __del__(self):
		self.pos = (0, 0)
		self.posGlobal = (0, 0)

		self.backgroundImage = None
		self.backgroundOverImage = None
		self.backgroundRowImage = None
		self.arrowImage = None
		self.fiveNearEffect = None
		self.cardCountImage = None
		self.cardCountText = None
		self.destroyCardEffect = None
		self.miniGame = None
		self.cardNumber = 0
		self.cardCount = 0
		self.isTheEnd = False
		
	def SetCardNumber(self, cardNumber, isEmpty = True, cardType = 0, overInFunc = None, overOutFunc = None):
		self.cardNumber = cardNumber

		if self.backgroundImage:
			if cardType == 1 and overInFunc and overOutFunc:
				self.backgroundImage.SetEvent(ui.__mem_func__(overInFunc), "MOUSE_OVER_IN", self.cardNumber)
				self.backgroundImage.SetEvent(ui.__mem_func__(overOutFunc), "MOUSE_OVER_OUT")
			
			if isEmpty:
				self.isTheEnd = False
				
				if self.backgroundRowImage:
					self.backgroundRowImage.Hide()
					
				self.backgroundImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_k.sub")
				
				if self.destroyCardEffect:
					self.destroyCardEffect.Hide()
			else:
				self.backgroundImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_%d.sub" % cardNumber)
				
	def SetEndCardNumber(self, cardNumber):
		self.isTheEnd = True
		if self.backgroundImage:
			self.backgroundImage.LoadImage("d:/ymir work/ui/minigame/catchking/end_card_number_%d.sub" % cardNumber)
				
	def ShowRowBackground(self):
		if self.backgroundRowImage:
			self.backgroundRowImage.Show()
				
	def GetCardNumber(self):
		return self.cardNumber
		
	def __CreateCardDestroyEffect(self, parent):
		self.destroyCardEffect = ui.AniImageBox()
		self.destroyCardEffect.SetParent(proxy(parent))
		self.destroyCardEffect.SetEndFrameEvent(ui.__mem_func__(self.SetCardNumber), 0, True)
		self.destroyCardEffect.SetDelay(6)
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/1.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/2.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/3.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/4.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/5.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/6.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/7.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/8.sub")
		self.destroyCardEffect.Hide()
		self.destroyCardEffect.AddFlag("not_pick")
		self.destroyCardEffect.AddFlag("float")
		
	def __CreateFiveNearEffect(self, parent):
		(x, y) = self.pos
		self.fiveNearEffect = ui.AniImageBox()
		self.fiveNearEffect.SetParent(proxy(parent))
		self.fiveNearEffect.SetEndFrameEvent(ui.__mem_func__(self.HideFiveNearEffect))
		self.fiveNearEffect.SetDelay(6)
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/1.sub")
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/1.sub")
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/2.sub")
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/3.sub")
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/4.sub")
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/5.sub")
		self.fiveNearEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/over5/6.sub")
		self.fiveNearEffect.SetPosition(x, y)
		self.fiveNearEffect.Hide()
		self.fiveNearEffect.AddFlag("not_pick")
		self.fiveNearEffect.AddFlag("float")
		
	def HideFiveNearEffect(self):
		if self.fiveNearEffect:
			self.fiveNearEffect.Hide()
		
	def ShowFiveNearEffect(self, endFunc, *args):
		if self.fiveNearEffect:
			if endFunc:
				self.fiveNearEffect.SetEndFrameEvent(ui.__mem_func__(endFunc), *args)
			self.fiveNearEffect.SetTop()
			self.fiveNearEffect.ResetFrame()
			self.fiveNearEffect.Show()
		
	def ShowCardDestroyEffect(self):
		if self.destroyCardEffect:
			(x, y) = self.pos
			self.destroyCardEffect.SetPosition(x - 39, y - 50)
			self.destroyCardEffect.SetTop()
			self.destroyCardEffect.ResetFrame()
			self.destroyCardEffect.Show()
		
	def __CreateBackgroundImg(self, parent, cardType, clickFunc, clickArg):
		(x, y) = self.pos
		self.backgroundImage = ui.ImageBox()
		self.backgroundImage.SetParent(proxy(parent))
		self.backgroundImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_k.sub")
		
		if cardType == 0:
			self.backgroundImage.SetEvent(ui.__mem_func__(self.__OverInFunc), "MOUSE_OVER_IN")
			self.backgroundImage.SetEvent(ui.__mem_func__(self.__OverOutFunc), "MOUSE_OVER_OUT")
		
		if clickFunc:
			self.backgroundImage.SAFE_SetMouseClickEvent(clickFunc, clickArg)
		
		self.backgroundImage.SetPosition(x, y)
		self.backgroundImage.Show()
		self.backgroundImage.AddFlag("float")
		
	def __CreateBackgroundOverImg(self, parent):
		(x, y) = self.pos
		self.backgroundOverImage = ui.ImageBox()
		self.backgroundOverImage.SetParent(proxy(parent))
		self.backgroundOverImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_over_img.sub")
		self.backgroundOverImage.SetPosition(x, y)
		self.backgroundOverImage.Hide()
		self.backgroundOverImage.AddFlag("not_pick")
		self.backgroundOverImage.AddFlag("float")
		
	def __CreateBackgroundRowImg(self, parent):
		(x, y) = self.pos
		self.backgroundRowImage = ui.ImageBox()
		self.backgroundRowImage.SetParent(proxy(parent))
		self.backgroundRowImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_cover_bingo.sub")
		self.backgroundRowImage.SetPosition(x, y)
		self.backgroundRowImage.Hide()
		self.backgroundRowImage.AddFlag("not_pick")
		self.backgroundRowImage.AddFlag("float")

	def __CreateArrowImg(self, parent, cardType):
		(x, y) = self.pos
		self.arrowImage = ui.AniImageBox()
		self.arrowImage.SetParent(proxy(parent))
		self.arrowImage.SetDelay(10)
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/1.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/2.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/3.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/4.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/5.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/4.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/3.sub")
		self.arrowImage.AppendImage("D:/Ymir Work/UI/minigame/yutnori/move_arrow/2.sub")
		
		if cardType == 0:
			self.arrowImage.SetPosition(x + 17, y - 10)
		elif cardType == 1:
			self.arrowImage.SetPosition(x + 17, y - 34)
			
		self.arrowImage.Hide()
		self.arrowImage.AddFlag("not_pick")
		self.arrowImage.AddFlag("float")
		
	def __CreateCardCount(self, parent):
		(x, y) = self.pos
		self.cardCountImage = ui.ImageBox()
		self.cardCountImage.SetParent(proxy(parent))
		self.cardCountImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_count_bg.sub")
		self.cardCountImage.SetPosition(x + 29, y - 6)
		self.cardCountImage.Hide()
		self.cardCountImage.AddFlag("not_pick")
		self.cardCountImage.AddFlag("float")
		
		self.cardCountText = ui.TextLine()
		self.cardCountText.SetParent(self.cardCountImage)
		self.cardCountText.SetWindowHorizontalAlignCenter()
		self.cardCountText.SetWindowVerticalAlignCenter()
		self.cardCountText.SetVerticalAlignCenter()
		self.cardCountText.SetHorizontalAlignCenter()
		self.cardCountText.Show()
		
	def SetCardCount(self, cardCount):
		if self.cardCountImage:
			if not self.cardCountImage.IsShow():
				self.cardCountImage.Show()
						
		if not cardCount:
			if self.cardCountImage:
				self.cardCountImage.Hide()
				
			if self.backgroundImage:
				self.backgroundImage.Hide()
		else:
			if self.cardCountImage:
				self.cardCountImage.Show()
				
			if self.backgroundImage:
				self.backgroundImage.Show()
				
		self.cardCount = cardCount
				
		if self.cardCountText:
			self.cardCountText.SetText("X%d" % cardCount)
			
	def GetCardCount(self):
		return self.cardCount
		
	def GetPos(self):
		return self.pos
		
	def GetGlobalPos(self):
		return self.posGlobal
		
	def GetLocalPosition(self):
		if self.backgroundImage:
			return self.backgroundImage.GetLocalPosition()
			
		return (0,0)
		
	def SetPosition(self, x, y):
		if self.backgroundImage:
			self.backgroundImage.SetPosition(x, y)
			
		if self.arrowImage:
			self.arrowImage.SetPosition(x + 7, y - 34)

	def SetTop(self):
		if self.backgroundImage:
			self.backgroundImage.SetTop()
			
		if self.backgroundOverImage:
			self.backgroundOverImage.SetTop()
			
		if self.arrowImage:
			self.arrowImage.SetTop()
			
	def __OverInFunc(self):
		if self.cardNumber:
			return
			
		if self.isTheEnd:
			return
			
		if self.backgroundOverImage:
			self.backgroundOverImage.Show()
			
		if self.miniGame:	
			if self.miniGame.HaveCardInHand():
				self.ArrowImgShow()
		
	def __OverOutFunc(self):
		if self.backgroundOverImage:
			self.backgroundOverImage.Hide()
			
		self.ArrowImgHide()
		
	def IsShowArrow(self):
		if self.arrowImage:
			return self.arrowImage.IsShow()
			
		return False
		
	def ArrowImgShow(self):
		if self.arrowImage:
			self.arrowImage.SetTop()
			self.arrowImage.ResetFrame()
			self.arrowImage.Show()
		
	def ArrowImgHide(self):
		if self.arrowImage:
			self.arrowImage.Hide()

class CatchKingGamePage(ui.ScriptWindow):
	def __init__(self, wndMiniGameCatchKing):
		import exception as exception

		if not wndMiniGameCatchKing:
			exception.Abort("wndMiniGameCatchKing parameter must be set to MiniGameCatchKing")
			return	
			
		ui.ScriptWindow.__init__(self)
		
		self.isLoaded = 0
		self.SetWindowName("CatchKingEventGamePageWindow")
		self.wndMiniGameCatchKing = wndMiniGameCatchKing
		
		self.fieldBackground = None
	
		self.myHandCardImage = None
		self.selectedCardImage = None

		self.gameCardList = []
		self.handCardList = []
		self.scoreInfo = []
		
		self.handCardNumber = 0

		self.infoTooltip = None
		
		self.confirmWindowCheckButton = None
		self.checkImage = None
		
		self.bigScoreText = None
		self.scoreWnd = None
		self.scoreText = None
		
		self.rewardButton = None
		
		self.myHandCardBg = None
		self.selectedCardBg = None
		
		self.destroyCardEffect = None
		self.popupResult = None
		
		self.scoreTextEffect = None
		self.scoreEffect1 = None
		self.scoreEffect2 = None
		self.scoreEffect3 = None
		
		self.isLocked = False
		
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
			LoadScript(self, "UIScript/minigamecatchkinggamepage.py")
		except:
			import exception as exception
			exception.Abort("CatchKingGamePage.LoadWindow.LoadObject")
			
		try:
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))
			self.fieldBackground = self.GetChild("field_bg")
			
			self.confirmWindowCheckButton = self.GetChild("confirm_check_button")
			#self.confirmWindowCheckButton.SetEvent(ui.__mem_func__(self.__ClickConfirmCheckButton), "MOUSE_LEFT_BUTTON_UP", 0)
			
			self.checkImage = self.GetChild("check_image")
			self.checkImage.Show()
			
			self.bigScoreText = self.GetChild("high_score_text")
			self.scoreText = self.GetChild("score_text")
			self.scoreWnd = self.GetChild("score_window")
			self.myHandCardBg = self.GetChild("my_number_card_bg")
			self.selectedCardBg = self.GetChild("selection_number_card_bg")

			self.rewardButton = self.GetChild("reward_button")
			self.rewardButton.SetEvent(ui.__mem_func__(self.__ClickRewardButton))
			
			## End Succes Effect
			self.scoreTextEffect = self.GetChild("success_text_effect")
			self.scoreTextEffect.SetEndFrameEvent(ui.__mem_func__(self.__ScoreTextEffectEndFrameEvent))
			self.scoreTextEffect.Hide()
			
			self.scoreEffect1 = self.GetChild("success_effect1")
			self.scoreEffect2 = self.GetChild("success_effect2")
			self.scoreEffect3 = self.GetChild("success_effect3")
			self.scoreEffect1.Hide()
			self.scoreEffect2.Hide()
			self.scoreEffect3.Hide()
			self.scoreEffect1.SetEndFrameEvent(ui.__mem_func__(self.__ScoreEffectEndFrameEvent1))
			self.scoreEffect2.SetEndFrameEvent(ui.__mem_func__(self.__ScoreEffectEndFrameEvent2))
			self.scoreEffect3.SetEndFrameEvent(ui.__mem_func__(self.__ScoreEffectEndFrameEvent3))
			
			self.scoreEffect1.SetKeyFrameEvent(ui.__mem_func__(self.__ScoreEffectKeyFrameEvent1))
			self.scoreEffect2.SetKeyFrameEvent(ui.__mem_func__(self.__ScoreEffectKeyFrameEvent2))
			#aici
			
			self.__ClearScoreCompletionEffect()
			## End Succes Effect
			
			self.infoTooltip = uiToolTip.ToolTip()
			self.infoTooltip.ClearToolTip()
			
			self.scoreWnd.SetOverInEvent(ui.__mem_func__(self.OverInScoreToolTip))
			self.scoreWnd.SetOverOutEvent(ui.__mem_func__(self.OverOutToolTip))

			for i in range(25):
				self.gameCardList.append(CatchKingCard(self, 0, self.fieldBackground, 8 + ((i % 5) * 51), 8 + ((i / 5) * 37), self.ClickCard, i))
				self.gameCardList[i].SetCardNumber(0, True)

			for k in range(6):
				self.handCardList.append(CatchKingCard(self, 1, self.GetChild("hand_card_bg%d" % int(k + 1)), 4, 4, self.ClickHandCard, k))
				self.handCardList[k].SetCardNumber(k + 1, False, 1, self.OverInToolTip, self.OverOutToolTip)
			
			self.myHandCardImage = ui.ImageBox()
			self.myHandCardImage.SetParent(self.myHandCardBg)
			self.myHandCardImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_k.sub")
			self.myHandCardImage.SetPosition(8, 8)
			self.myHandCardImage.AddFlag("float")
			self.myHandCardImage.Hide()
			
			self.selectedCardImage = ui.ImageBox()
			self.selectedCardImage.SetParent(self.selectedCardBg)
			self.selectedCardImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_k.sub")
			self.selectedCardImage.SetPosition(8, 8)
			self.selectedCardImage.AddFlag("float")
			self.selectedCardImage.Hide()
			
			self.CreateCardDestroyEffect()
		except:
			import exception as exception
			exception.Abort("CatchKingGamePage.LoadWindow.BindObject")

		self.CreateScoreTooltip()
		
		self.Hide()
		
	def __ClickRewardButton(self):
		if self.HaveCardInHand():
			return
			
		if self.isLocked:
			return

		net.SendMiniGameCatchKing(3, 0)
		
	def __ScoreTextEffectEndFrameEvent(self):
		if self.scoreTextEffect: 
			self.scoreTextEffect.Hide()
	
	def __ScoreEffectKeyFrameEvent1(self, cur_frame):
		if cur_frame == 2:
			if self.scoreTextEffect:
				self.scoreTextEffect.Show()
				
			if self.scoreEffect2:
				self.scoreEffect2.Show()
			
	def __ScoreEffectKeyFrameEvent2(self, cur_frame):
		if cur_frame == 1:
			if self.scoreEffect3:
				self.scoreEffect3.Show()
	
	def __ScoreEffectEndFrameEvent1(self):
		if self.scoreEffect1:
			self.scoreEffect1.Hide()
		
	def __ScoreEffectEndFrameEvent2(self):
		if self.scoreEffect2:
			self.scoreEffect2.Hide()
		
	def __ScoreEffectEndFrameEvent3(self):
		if self.scoreEffect3:
			self.scoreEffect3.Hide()
			
	def __ClearScoreCompletionEffect(self):
		if self.scoreTextEffect:
			self.scoreTextEffect.Hide()
			self.scoreTextEffect.ResetFrame()
			self.scoreTextEffect.SetDelay(6)
		
		if self.scoreEffect1:
			self.scoreEffect1.Hide()
			self.scoreEffect1.ResetFrame()
			self.scoreEffect1.SetDelay(6)
		
		if self.scoreEffect2:
			self.scoreEffect2.Hide()
			self.scoreEffect2.ResetFrame()
			self.scoreEffect2.SetDelay(6)
		
		if self.scoreEffect3:
			self.scoreEffect3.Hide()
			self.scoreEffect3.ResetFrame()
			self.scoreEffect3.SetDelay(6)
		
	def CreateScoreTooltip(self):
		item.SelectItem(LOW_SCORE_VNUM)
		self.scoreInfo.append(localeInfo.MINI_GAME_CATCHKING_TOOLTIP_SCORE3 % item.GetItemName())
		
		item.SelectItem(MID_SCORE_VNUM)
		self.scoreInfo.append(localeInfo.MINI_GAME_CATCHKING_TOOLTIP_SCORE2 % item.GetItemName())
		
		item.SelectItem(HIGH_SCORE_VNUM)
		self.scoreInfo.append(localeInfo.MINI_GAME_CATCHKING_TOOLTIP_SCORE1 % item.GetItemName())
		
		self.scoreInfo.append(localeInfo.MINI_GAME_CATCHKING_TOOLTIP_SCORE4)
		
	def CreateCardDestroyEffect(self):
		self.destroyCardEffect = ui.AniImageBox()
		self.destroyCardEffect.SetParent(self.myHandCardBg)
		self.destroyCardEffect.SetDelay(6)
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/1.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/2.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/3.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/4.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/5.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/6.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/7.sub")
		self.destroyCardEffect.AppendImage("D:/Ymir Work/UI/minigame/catchking/effect/explosion/8.sub")
		self.destroyCardEffect.Hide()
		self.destroyCardEffect.AddFlag("not_pick")
		self.destroyCardEffect.AddFlag("float")
		
	def SetEndEffectCardEvent(self, event, keepFieldCard, destroyHandCard, cardValue, isFiveNear):
		if self.destroyCardEffect:
			self.destroyCardEffect.SetEndFrameEvent(ui.__mem_func__(event), keepFieldCard, destroyHandCard, cardValue, isFiveNear)
		
	def ShowCardDestroyEffect(self):
		if not self.myHandCardImage:
			return
			
		if self.destroyCardEffect:
			self.destroyCardEffect.SetPosition(-32, -42)
			self.destroyCardEffect.SetTop()
			self.destroyCardEffect.ResetFrame()
			self.destroyCardEffect.Show()
		
	def IsCheckShowPopUp(self):
		if self.checkImage:
			if self.checkImage.IsShow():
				return True
			else:
				return False
				
		return False
	
	def __ClickConfirmCheckButton(self):
		if self.checkImage:
			if self.checkImage.IsShow():
				self.checkImage.Hide()
			else:
				self.checkImage.Show()
		
	def ClickCard(self, cardNumber):
		if not self.HaveCardInHand():
			return

		if self.isLocked:
			return
			
		net.SendMiniGameCatchKing(2, cardNumber)
			
	def ClickHandCard(self, cardNumber):
		if self.HaveCardInHand():
			return
			
		if not self.handCardList[cardNumber].IsShowArrow():
			return
			
		if self.isLocked:
			return
			
		net.SendMiniGameCatchKing(1, 0)
		
	def SetEndCard(self, cardPos, cardNumber):
		if cardPos < 0 or cardPos > 24:
			return
			
		if cardNumber < 1 or cardNumber > 6:
			return
			
		self.gameCardList[cardPos].SetEndCardNumber(cardNumber)
	
	def SetHandCard(self, cardNumber):
		if cardNumber < 1 or cardNumber > 6:
			return
			
		cardPos = cardNumber - 1
			
		self.handCardList[cardPos].ArrowImgHide()
		self.handCardList[cardPos].SetCardCount(self.handCardList[cardPos].GetCardCount() - 1)
		
		(x, y) = self.handCardList[cardPos].GetGlobalPos()

		self.handCardNumber = cardNumber
		
		if self.myHandCardImage:
			self.myHandCardImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_%d.sub" % self.handCardNumber)
			self.myHandCardImage.Show()
			
	def ShowPopupDialog(self, handCard, fieldCard, isFiveNear):
		if not self.popupResult:
			self.popupResult = uiCommon.PopupDialogNew()
			self.popupResult.SetWidth(390)
			
		startText = [ "", "" ]
		
		if handCard == 5 and isFiveNear:
			startText = localeInfo.MINI_GAME_CATCHKING_HELP_MSG4.split("\\n")
			self.popupResult.SetText1(startText[0], True)
			self.popupResult.SetText2(startText[1], True)
			self.popupResult.Open()
			return
			
		if handCard == 6 and fieldCard != 6:
			startText = localeInfo.MINI_GAME_CATCHKING_POPUP_CATCH_FAIL.split("\\n")
			self.popupResult.SetText1(startText[0], True)
			self.popupResult.SetText2(startText[1], True)
			self.popupResult.Open()
			return
		
		if handCard < fieldCard:
			startText = localeInfo.MINI_GAME_CATCHKING_HELP_MSG1.split("\\n")
		elif handCard == fieldCard:
			startText = localeInfo.MINI_GAME_CATCHKING_HELP_MSG2.split("\\n")
		elif handCard > fieldCard:
			startText = localeInfo.MINI_GAME_CATCHKING_HELP_MSG3.split("\\n")
			
		self.popupResult.SetText1(startText[0], True)
		self.popupResult.SetText2(startText[1], True)
		self.popupResult.Open()
			
	def ResultEffectHandCard(self, keepFieldCard, destroyHandCard, cardValue, isFiveNear):
		if self.IsCheckShowPopUp():
			self.ShowPopupDialog(self.handCardNumber, cardValue, isFiveNear)

		if destroyHandCard:
			if self.myHandCardImage:
				self.myHandCardImage.Hide()
				
			for card in self.handCardList:
				if card.GetCardCount() > 0:
					if self.handCardNumber == card.GetCardNumber():
						net.SendMiniGameCatchKing(1, 0)
						self.handCardNumber = 0
					else:
						card.ArrowImgShow()
					break
					
			self.handCardNumber = 0
								
		if self.selectedCardImage:
			self.selectedCardImage.Hide()
				
		if self.destroyCardEffect:
			self.destroyCardEffect.Hide()
			
		self.isLocked = False
			
	def ShowFiveNearEffect(self, cardPos, endFunc, rowType, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
		checkPos = []
		checkPos.append(cardPos - 5)
		checkPos.append(cardPos + 5)
		
		if cardPos % 10 != 4 and cardPos % 10 != 9:
			checkPos.append(cardPos + 1)
			checkPos.append(cardPos - 5 + 1)
			checkPos.append(cardPos + 5 + 1)
		
		if cardPos % 10 != 0 and cardPos % 10 != 5:
			checkPos.append(cardPos - 1)
			checkPos.append(cardPos - 5 - 1)
			checkPos.append(cardPos + 5 - 1)
			
		#alreadySet = True
		for i in xrange(len(self.gameCardList)):
			if i in checkPos:
				self.gameCardList[i].ShowFiveNearEffect(endFunc, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)

	def ResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
		if cardPos < 0 or cardPos > 24:
			return
			
		self.isLocked = True
			
		self.SetScore(score)

		self.gameCardList[cardPos].ArrowImgHide()
		self.gameCardList[cardPos].SetCardNumber(cardValue, False)
		
		if self.selectedCardImage:
			self.selectedCardImage.LoadImage("d:/ymir work/ui/minigame/catchking/card_number_%d.sub" % cardValue)
			self.selectedCardImage.Show()
		
		if isFiveNear:
			self.ShowFiveNearEffect(cardPos, self.EndFiveNearEffect, rowType, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MINI_GAME_CATCHKING_SEARCH_NUMBER5)
		else:
			self.EndFiveNearEffect(rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)
			
	def SetRow(self, cardPos, rowType):
		if rowType == 0:
			return
			
		rowStart = 5 * (cardPos / 5);
		rowEnd = 4 + (5 * (cardPos / 5));
		colStart = cardPos - (5 * (cardPos / 5));
		colEnd = cardPos + 20 - (5 * (cardPos / 5));
		
		if rowType == 1 or rowType == 3: 
			while rowStart <= rowEnd:
				self.gameCardList[rowStart].ShowRowBackground()
				rowStart += 1
				
		if rowType == 2 or rowType == 3: 
			while colStart <= colEnd:
				self.gameCardList[colStart].ShowRowBackground()
				colStart += 5
				
	def EndFiveNearEffect(self, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
		if self.handCardNumber == 6:
			self.gameCardList[cardPos].SetEndCardNumber(cardValue)
			
			if cardValue == 6:
				self.__ClearScoreCompletionEffect()
				self.scoreEffect1.Show()
		else:
			if not keepFieldCard:
				self.gameCardList[cardPos].ShowCardDestroyEffect()
	
		self.SetEndEffectCardEvent(self.ResultEffectHandCard, keepFieldCard, destroyHandCard, cardValue, isFiveNear)
		self.ShowCardDestroyEffect()

		for card in self.gameCardList:
			card.HideFiveNearEffect()
			
		self.SetRow(cardPos, rowType)

	def HaveCardInHand(self):
		return self.handCardNumber
		
	def OverInToolTip(self, eventType, arg):
		if arg < 1 or arg > 6:
			return
		
		(x, y) = self.GetGlobalPosition()
		self.infoTooltip.ClearToolTip()
		self.infoTooltip.SetThinBoardSize(395)
		self.infoTooltip.AppendTextLine(localeInfo.GetVariableValue("MINI_GAME_CATCHKING_TOOLTIP_CARD%d" % arg))
		#aici
		self.infoTooltip.SetToolTipPosition(x + (self.GetWidth() / 2), y + self.GetHeight() + self.infoTooltip.GetHeight() - 10)
		self.infoTooltip.Show()
		
	def OverInScoreToolTip(self):	
		self.infoTooltip.ClearToolTip()
		self.infoTooltip.SetThinBoardSize(395)
		self.infoTooltip.SetToolTipPosition(-1, -1)
		for text in self.scoreInfo:
			self.infoTooltip.AppendTextLine(text)
		self.infoTooltip.Show()

	def OverOutToolTip(self):
		self.infoTooltip.Hide()
		
	def SetBigScore(self, bigScore):
		if self.bigScoreText:
			self.bigScoreText.SetText(str(bigScore))
			
	def SetScore(self, score):
		if self.scoreText:
			if score < 400:
				self.scoreText.SetPackedFontColor(grp.GenerateColor(0.78, 0.78, 0.78, 1.0))
			elif score < 550:
				self.scoreText.SetPackedFontColor(grp.GenerateColor(1.0, 0.85, 0.39, 1.0))
			else:
				self.scoreText.SetPackedFontColor(grp.GenerateColor(1.0, 0.0, 0.0, 1.0))

			self.scoreText.SetText(str(score))
			
	def SetReward(self, rewardCode):
		if not self.popupResult:
			self.popupResult = uiCommon.PopupDialog()
			self.popupResult.SetWidth(390)
			
		if rewardCode == 0:
			if self.rewardButton:
				self.rewardButton.SetEvent(ui.__mem_func__(self.__ClickRetryButton))
				self.rewardButton.SetText(uiScriptLocale.MINI_GAME_CATCHKING_RETRY)
				
		elif rewardCode == 1:
			self.popupResult.SetText(localeInfo.MINI_GAME_CATCHKING_POPUP_REWARD_FAIL)
			self.popupResult.Open()
			
			if self.rewardButton:
				self.rewardButton.SetEvent(ui.__mem_func__(self.__ClickRetryButton))
				self.rewardButton.SetText(uiScriptLocale.MINI_GAME_CATCHKING_RETRY)
				
	def __ClickRetryButton(self):
		if self.wndMiniGameCatchKing:
			self.wndMiniGameCatchKing.StartAgain()
			
	def OnPressEscapeKey(self):
		self.Hide()
		return True
		
	def Close(self):
		if self.popupResult:
			self.popupResult.Close()
			
		if self.infoTooltip:
			self.infoTooltip.HideToolTip()
			
		self.isLocked = False
			
		self.Hide()
		
	def Clear(self):
		self.handCardList[0].SetCardCount(5)
		self.handCardList[1].SetCardCount(2)
		self.handCardList[2].SetCardCount(2)
		self.handCardList[3].SetCardCount(1)
		self.handCardList[4].SetCardCount(1)
		self.handCardList[5].SetCardCount(1)
		self.handCardList[0].ArrowImgShow()
		
		self.SetScore(0)
		
		for card in self.gameCardList:
			card.SetCardNumber(0, True)
			
		self.handCardNumber = 0

		if self.rewardButton:
			self.rewardButton.SetEvent(ui.__mem_func__(self.__ClickRewardButton))
			self.rewardButton.SetText(uiScriptLocale.MINI_GAME_CATCHKING_REWARD)
		
	def Destroy(self):
		self.isLoaded = 0
		
		self.fieldBackground = None
		
		self.myHandCardImage = None
		self.selectedCardImage = None

		self.gameCardList = []
		self.handCardList = []
		self.scoreInfo = []
		
		self.handCardNumber = 0

		self.infoTooltip = None
		
		self.confirmWindowCheckButton = None
		self.checkImage = None
		
		self.bigScoreText = None
		self.scoreWnd = None
		self.scoreText = None
		
		self.rewardButton = None
		
		self.myHandCardBg = None
		self.selectedCardBg = None
		
		self.destroyCardEffect = None
		self.popupResult = None
		
		self.scoreTextEffect = None
		self.scoreEffect1 = None
		self.scoreEffect2 = None
		self.scoreEffect3 = None
		
		self.isLocked = False
		
	def Show(self):
		ui.ScriptWindow.Show(self)
		
		if self.checkImage:
			self.checkImage.Show()	
			
class MiniGameCatchKing(ui.Window):
	wndWaitingPage		= None
	wndGamePage			= None
	
	def __init__(self):
		ui.Window.__init__(self)
		self.SetWindowName("MiniGameCatchKingWindow")
		self.wndWaitingPage = CatchKingWaitingPage(self)
		self.wndSimpleWaitingPage = CatchKingSimpleWaitingPage(self)
		self.wndGamePage = CatchKingGamePage(self)
		
		self.isPlaying = False
		
	def __del__(self):
		ui.Window.__del__(self)
		
	def Destroy(self):
		if self.wndWaitingPage:
			self.wndWaitingPage.Destroy()
			self.wndWaitingPage = None
			
		if self.wndSimpleWaitingPage:
			self.wndSimpleWaitingPage.Destroy()
			self.wndSimpleWaitingPage = None
			
		if self.wndGamePage:
			self.wndGamePage.Destroy()
			self.wndGamePage = None
			
		self.isPlaying = False
		
	def Open(self):
		if self.wndGamePage:
			if self.wndGamePage.IsShow():
				return
				
		if self.isPlaying:
			if self.wndGamePage:
				self.wndGamePage.Show()
		else:
			if self.wndWaitingPage:
				self.wndWaitingPage.Show()
		
	def Close(self):
		if self.wndWaitingPage:
			if self.wndWaitingPage.IsShow():
				self.wndWaitingPage.Close()
				
		if self.wndSimpleWaitingPage:
			if self.wndSimpleWaitingPage.IsShow():
				self.wndSimpleWaitingPage.Close()
				
		if self.wndGamePage:
			if self.wndGamePage.IsShow():
				self.wndGamePage.Close()
		        		
	def GameStart(self, bigScore):
		if self.wndWaitingPage:
			if self.wndWaitingPage.IsShow():
				self.wndWaitingPage.Close()
				
		if self.wndSimpleWaitingPage:
			if self.wndSimpleWaitingPage.IsShow():
				self.wndSimpleWaitingPage.Close()
		
		if self.wndGamePage:
			self.wndGamePage.Clear()
		
			self.isPlaying = True
				
			self.wndGamePage.SetBigScore(bigScore)
			self.wndGamePage.Show()
			
	def StartAgain(self):
		self.isPlaying = False
		
		if self.wndGamePage:
			if self.wndGamePage.IsShow():
				self.wndGamePage.Close()
				
		if self.wndSimpleWaitingPage:
			self.wndSimpleWaitingPage.Show()
			
	def CatchKingSetHandCard(self, cardNumber):
		if self.wndGamePage:
			self.wndGamePage.SetHandCard(cardNumber)
			
	def CatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
		if self.wndGamePage:
			self.wndGamePage.ResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)
			
	def CatchKingSetEndCard(self, cardPos, cardNumber):
		if self.wndGamePage:
			self.wndGamePage.SetEndCard(cardPos, cardNumber)
		
	def CatchKingReward(self, rewardCode):
		if self.wndGamePage:
			self.wndGamePage.SetReward(rewardCode)
		
	def GameEnd(self):
		if self.wndGamePage:
			self.isPlaying = False
			
			self.wndGamePage.Clear()
			self.wndGamePage.Close()
		
	def SetItemToolTip(self, tooltip):
		if self.wndGamePage:
			self.wndGamePage.SetItemToolTip(tooltip)	
		
