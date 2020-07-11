import ui
import uiScriptLocale
import wndMgr
import player
import localeInfo
import net
import app
import event
import uiCommon
import item

class MiniGameAttendance(ui.ScriptWindow):
	class DescriptionBox(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.descIndex = 0
		def __del__(self):
			ui.Window.__del__(self)
		def SetIndex(self, index):
			self.descIndex = index
		def OnRender(self):
			event.RenderEventSet( self.descIndex )
			
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.curPage = 0
		self.numberButton = [None]
		self.numberEffect = None
		self.numberBoard = None
		self.popupDialog = None
		self.prevButton	= None
		self.nextButton = None
		
		self.curAttendanceNumber = 0
		self.curEffectNumber = 0
		self.missionClear = 0
		self.getReward = 0
		self.rewardDict = {}
		self.showMax = 14
		
		self.descBoard = None
		self.descriptionBox = None
		self.descIndex = 0

		self.rewardQuestionDialog = None

		self.tooltipItem = None
		self.showTooltip = False
		
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.isLoaded = 0
		self.curPage = 0
		self.numberButton = [None]
		self.numberEffect = None
		self.numberBoard = None
		self.popupDialog = None
		self.prevButton = None
		self.nextButton = None
		
		self.curAttendanceNumber = 0
		self.curEffectNumber = 0
		self.missionClear = 0
		self.getReward = 0
		self.rewardDict = {}
		self.showMax = 14

		self.descBoard = None
		self.descriptionBox	= None
		self.descIndex = 0

		self.tooltipItem = None
		self.showTooltip = False
		
		self.Close()
		
	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip
		
	def __LoadWindow(self):
		if self.isLoaded == 1:
			return
		
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/MiniGameAttendance.py")
		except:
			import exception as exception
			exception.Abort("MiniGameAttendance.LoadWindow")
		
		try:
			self.GetChild("titlebar").SetCloseEvent( ui.__mem_func__(self.Close) )
			self.numberBoard = self.GetChild("number_board_window")
			self.numberEffect = self.GetChild("number_effect")
			self.numberEffect.Hide()

			for number in range(1, 8 * 2 + 1):
				if number > self.showMax:
					self.numberButton.append( None )
				else:
					button = self.GetChild("disable_number_button" + str(number))
					button.SetDisableVisual("d:/ymir work/ui/minigame/attendance/attendance_disable_number" + str(number) + ".sub")
					button.SAFE_SetEvent(self.__OnClickNumberButton, number)
					button.Hide()
					self.numberButton.append(button)

			self.prevButton = self.GetChild("prev_button")
			self.prevButton.SAFE_SetEvent(self.__OnClickPrevButton)
			self.prevButton.Hide()

			self.nextButton = self.GetChild("next_button")
			self.nextButton.SAFE_SetEvent(self.__OnClickNextButton)
			self.nextButton.Hide()
			
			self.descPrevButton = self.GetChild("desc_prev_button")
			self.descPrevButton.SAFE_SetEvent(self.__OnClickDescPrevButton)
			
			self.descNextButton = self.GetChild("desc_next_button")
			self.descNextButton.SAFE_SetEvent(self.__OnClickDescNextButton)
			
			self.descBoard = self.GetChild("desc_board")
			self.descriptionBox = self.DescriptionBox()
			self.descriptionBox.SetParent(self.descBoard)
			self.descriptionBox.Show()

			self.RefreshAttendanceBoard()
			self.__NumberFlash(self.curEffectNumber)
		except:
			import exception as exception
			exception.Abort("MiniGameAttendance.LoadWindow.BindObject")
			
		self.rewardDict = player.GetAttendanceRewardList()

		self.Hide()
		self.isLoaded = 1
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
			
	def Open(self):
		if self.IsShow():
			self.Close()
		else:
			self.Show()
			self.SetTop()
			
	def Close(self):
		self.Hide()
		
		event.ClearEventSet(self.descIndex)
		self.descIndex = -1
		
		if self.descriptionBox:
			self.descriptionBox.Hide()

		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			
	def Show(self):
		self.__LoadWindow()
		
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet( uiScriptLocale.ATTENDANCE_DESC )
		event.SetFontColor(self.descIndex, 0.7843, 0.7843, 0.7843, 1.0)
		event.SetVisibleLineCount(self.descIndex, 7)
		event.SetRestrictedCount(self.descIndex, 70)
		event.Skip(self.descIndex)
		
		if self.descriptionBox:
			self.descriptionBox.Show()		
		
		ui.ScriptWindow.Show(self)
 
	def OnUpdate(self):
		(xposEventSet, yposEventSet) = self.descBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet + 7, -(yposEventSet + 7))
		self.descriptionBox.SetIndex(self.descIndex)

		self.NumberButtonOverCheck()
		
	def NumberButtonOverCheck(self):
		if not self.tooltipItem:
			return
			
		if not self.rewardDict:
			return
			
		if self.rewardQuestionDialog:
			if self.rewardQuestionDialog.IsShow():
				self.tooltipItem.HideToolTip()
				self.showTooltip = False
				return
			
		if not self.numberBoard.IsIn():
			if self.showTooltip:
				self.tooltipItem.HideToolTip()
				self.showTooltip = False
							
		isin = False
		
		for number in range(1, 8 * 2 + 1):
			if number > self.showMax:
				break

			if self.numberButton[number].IsIn():
				if self.tooltipItem:
					if self.rewardDict.has_key(number):
						(itemVnum, itemCount) = self.rewardDict[number]
						self.tooltipItem.SetAttendanceRewardItem(itemVnum, itemCount)
						self.showTooltip = True
						isin = True
						break
				
		if self.numberBoard.IsIn() and False == isin:
			self.tooltipItem.HideToolTip()
			self.showTooltip = False

	def MiniGameAttendanceSetDay(self, day):
		self.curAttendanceNumber = day + 1

	def MiniGameAttendanceSetMissionClear(self, clear):
		self.missionClear = clear

	def MiniGameAttendanceSetReward(self, reward):
		self.getReward = reward

	def RefreshAttendanceBoard(self):
		for number in range(1, 8 * 2 + 1):
			if number > self.showMax:
				break
			else:
				if number < self.curAttendanceNumber:
					strNumber = str(number)
					fullPath = "d:/ymir work/ui/minigame/attendance/attendance_close_button.sub"
					self.numberButton[number].SetUpVisual(fullPath)
					self.numberButton[number].SetOverVisual(fullPath)
					self.numberButton[number].SetDownVisual(fullPath)
					self.numberButton[number].Enable()
				elif number == self.curAttendanceNumber:
					if self.missionClear and self.getReward:
						strNumber = str(number)
						fullPath = "d:/ymir work/ui/minigame/attendance/attendance_close_button.sub"
						self.numberButton[number].SetUpVisual(fullPath)
						self.numberButton[number].SetOverVisual(fullPath)
						self.numberButton[number].SetDownVisual(fullPath)
						self.numberButton[number].Enable()
					else:
						strNumber = str(number)
						fullPath = "d:/ymir work/ui/minigame/attendance/attendance_enable_number" + strNumber + ".sub"
						self.numberButton[number].SetUpVisual(fullPath)
						self.numberButton[number].SetOverVisual(fullPath)
						self.numberButton[number].SetDownVisual(fullPath)
						self.numberButton[number].Enable()
				else:
					self.numberButton[number].Disable()
					
		if self.curAttendanceNumber > 8:
			self.curPage = 1

		self.__HideAllNumber()
		self.__ShowCurPageNumber()

		if self.missionClear and not self.getReward:
			self.__NumberFlash(self.curAttendanceNumber)
		else:
			self.__NumberFlash(0)

		if self.prevButton and self.nextButton:
			self.prevButton.Show()
			self.nextButton.Show()
		
	def __HideAllNumber(self):
		for number in range(1, 8 * 2 + 1):
			if number > self.showMax:
				break
			else:
				self.numberButton[number].Hide()
				
	def __ShowCurPageNumber(self):
		min_range = self.curPage * 8 + 1
		max_range = (self.curPage + 1) * 8 + 1
		
		for number in range(min_range, max_range):
			if number > self.showMax:
				continue
			else:
				self.numberButton[number].Show()
				
	def __NumberFlash(self, number):
		if not self.numberEffect:
			return
			
		self.numberEffect.Hide()
	
		if number > self.showMax or number < 1:
			return
		
		min_range = self.curPage * 8 + 1
		max_range = (self.curPage + 1) * 8 + 1
		
		if number < min_range or number > max_range:
			return
			
		(x, y) = self.numberButton[number].GetLocalPosition()
		
		self.numberEffect.SetPosition(x,y)
		self.numberEffect.ResetFrame()
		self.numberEffect.Show()
	
	def __OnClickPrevButton(self):
		temp_page = self.curPage
		self.curPage -= 1
		if self.curPage < 0:
			self.curPage = 0
			
		if temp_page == self.curPage:
			return
	
		self.__HideAllNumber()
		self.__ShowCurPageNumber()
		if self.missionClear and not self.getReward:
			self.__NumberFlash(self.curAttendanceNumber)
		else:
			self.__NumberFlash(0)
		
	def __OnClickNextButton(self):
		temp_page = self.curPage
		self.curPage += 1
		if self.curPage >= 2:
			self.curPage = 2 - 1
			
		if temp_page == self.curPage:
			return
			
		self.__HideAllNumber()
		self.__ShowCurPageNumber()
		if self.missionClear and not self.getReward:
			self.__NumberFlash(self.curAttendanceNumber)
		else:
			self.__NumberFlash(0)
	
	def __OnClickNumberButton(self, index):
		if self.curAttendanceNumber != index:
			return
			
		if self.getReward:
			return
			
		if not self.missionClear:
			return
		
		if None == self.rewardQuestionDialog:
			self.rewardQuestionDialog = uiCommon.QuestionDialog()
			self.rewardQuestionDialog.SetText(localeInfo.ATTENDANCE_REWARD_QUESTION)
			self.rewardQuestionDialog.SetAcceptEvent(ui.__mem_func__(self.__RewardAccept))
			self.rewardQuestionDialog.SetCancelEvent(ui.__mem_func__(self.__RewardCancel))
			self.rewardQuestionDialog.AutoResize(60)
			self.rewardQuestionDialog.Open()
		
	def __RewardAccept(self):
		net.SendAttendanceGetReward()
		self.__RewardCancel()
		
	def __RewardCancel(self):
		if self.rewardQuestionDialog:
			self.rewardQuestionDialog.Close()
			self.rewardQuestionDialog = None
		
	def __OnClickDescPrevButton(self):
		if True == event.IsWait(self.descIndex):
			if event.GetVisibleStartLine(self.descIndex) - 7 >= 0:
				event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) - 7)
				event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)
		
	def __OnClickDescNextButton(self):
		if True == event.IsWait(self.descIndex):
			event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) + 7)
			event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)
		
	def OpenPopupDialog(self, msg):
		if not self.popupDialog:
			self.popupDialog = uiCommon.PopupDialog()

		self.popupDialog.SetText(msg)
		self.popupDialog.Open()
		
		
		