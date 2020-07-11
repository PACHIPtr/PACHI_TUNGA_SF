import chr
import gameEvents
import chat
import player
import app
import ui
import uiScriptLocale
import uiCommon
import constInfo
if app.ENABLE_FISH_JIGSAW_EVENT:
	import uiMiniGameFishEvent
if app.ENABLE_ATTENDANCE_EVENT:
	import uiMiniGameAttendance
if app.ENABLE_MINI_GAME_CATCH_KING:
	import uiMiniGameCatchKing
import dbg

button_gap		= 10
button_height	= 25

class EventWindow(ui.ScriptWindow):

	def __init__(self, interface):
		ui.ScriptWindow.__init__(self)
		self.interface = interface
		if app.ENABLE_FISH_JIGSAW_EVENT:
			self.fishGame = None
			self.tooltipItem = None
			self.wndInventory = None
		if app.ENABLE_ATTENDANCE_EVENT:
			self.attendanceGame = None
		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.catchKingGame = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def BindInventory(self, inventory):
		self.wndInventory = inventory
			
	def SetItemToolTip(self, toolTip):
		self.tooltipItem = toolTip
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/eventwindow.py")
			self.gameButton = self.GetChild("game_event_button")
		except:
			import exception
			exception.Abort("eventwindow.LoadDialog.LoadScript")

		try:
			self.eventinfoDialog = EventInfoDialog()
			self.eventinfoDialog.Hide()
		except:
			import exception
			exception.Abort("eventwindow.LoadDialog.BindObject")
			
		self.gameButton.SetEvent(ui.__mem_func__(self.OpenEventInfoDialog))
			
	def OpenEventInfoDialog(self):
		self.eventinfoDialog.Show()
		
	def __OpenOkeyCardDialog(self):
		if self.interface:
			self.interface.OpenCardsInfoWindow()
			
	def __OpenEventItemInformationMoonDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_MOON_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(50011, uiScriptLocale.GAME_EVENTS_MOON_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_MOON_DROP_2, "", gameEvents.GetEventTime(0))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationHexagonDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_HEXAGON_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(50037, uiScriptLocale.GAME_EVENTS_HEXAGON_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_HEXAGON_DROP_2, "", gameEvents.GetEventTime(1))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationCarvingStoneDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_CARVING_STONE_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(30178, uiScriptLocale.GAME_EVENTS_CARVING_STONE_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_CARVING_STONE_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_CARVING_STONE_DROP_3, gameEvents.GetEventTime(2))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationRamadan(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_RAMADAN_1)
		informationDialog.height = 32
		informationDialog.Open(30315, uiScriptLocale.GAME_EVENTS_RAMADAN, uiScriptLocale.GAME_EVENTS_INFORMATION_RAMADAN_2, uiScriptLocale.GAME_EVENTS_INFORMATION_RAMADAN_3, gameEvents.GetEventTime(3))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationXmas(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_XMAS_1)
		informationDialog.height = 32
		informationDialog.Open(50010, uiScriptLocale.GAME_EVENTS_XMAS, uiScriptLocale.GAME_EVENTS_INFORMATION_XMAS_2, uiScriptLocale.GAME_EVENTS_INFORMATION_XMAS_3, gameEvents.GetEventTime(4))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationFootBallDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_FOOTBALL_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(50096, uiScriptLocale.GAME_EVENTS_FOOTBALL_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_FOOTBALL_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_FOOTBALL_DROP_3, gameEvents.GetEventTime(6))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationEaster(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_XMAS_1)
		informationDialog.height = 32
		informationDialog.Open(71150, uiScriptLocale.GAME_EVENTS_EASTER, uiScriptLocale.GAME_EVENTS_INFORMATION_EASTER_2, uiScriptLocale.GAME_EVENTS_INFORMATION_EASTER_3, gameEvents.GetEventTime(5))
		self.informationDialog = informationDialog		
		
	def __OpenEventItemInformationValentineDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_VALENTINE_DROP_1)
		informationDialog.height = 32
		if chr.RaceToSex(chr.GetRace()) == 0:
			informationDialog.Open(71146, uiScriptLocale.GAME_EVENTS_VALENTINE_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_VALENTINE_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_VALENTINE_DROP_3, gameEvents.GetEventTime(7))
		else:
			informationDialog.Open(71147, uiScriptLocale.GAME_EVENTS_VALENTINE_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_VALENTINE_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_VALENTINE_DROP_3, gameEvents.GetEventTime(7))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationPetDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_PET_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(53001, uiScriptLocale.GAME_EVENTS_PET_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_PET_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_PET_DROP_3, gameEvents.GetEventTime(8))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationKidsDayDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_KIDS_DAY_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(50034, uiScriptLocale.GAME_EVENTS_KIDS_DAY_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_KIDS_DAY_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_KIDS_DAY_DROP_3, gameEvents.GetEventTime(9))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationHalloween(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_HALLOWEEN_1)
		informationDialog.height = 32
		informationDialog.Open(30336, uiScriptLocale.GAME_EVENTS_HALLOWEEN, uiScriptLocale.GAME_EVENTS_INFORMATION_HALLOWEEN_2, uiScriptLocale.GAME_EVENTS_INFORMATION_HALLOWEEN_3, gameEvents.GetEventTime(10))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationOlympicBadge(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_OLYMPIC_BADGE_1)
		informationDialog.height = 32
		informationDialog.Open(30265, uiScriptLocale.GAME_EVENTS_OLYMPIC_BADGE, uiScriptLocale.GAME_EVENTS_INFORMATION_OLYMPIC_BADGE_2, uiScriptLocale.GAME_EVENTS_INFORMATION_OLYMPIC_BADGE_3, gameEvents.GetEventTime(11))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationMountDrop(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_MOUNT_DROP_1)
		informationDialog.height = 32
		informationDialog.Open(52701, uiScriptLocale.GAME_EVENTS_MOUNT_DROP, uiScriptLocale.GAME_EVENTS_INFORMATION_MOUNT_DROP_2, uiScriptLocale.GAME_EVENTS_INFORMATION_MOUNT_DROP_3, gameEvents.GetEventTime(13))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationAlignment(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_ALIGNMENT_1)
		informationDialog.height = 32
		informationDialog.Open(1, uiScriptLocale.GAME_EVENTS_ALIGNMENT, uiScriptLocale.GAME_EVENTS_INFORMATION_ALIGNMENT_2, "", gameEvents.GetEventTime(14))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationYearRound(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_YEAR_ROUND_1)
		informationDialog.height = 32
		informationDialog.Open(25105, uiScriptLocale.GAME_EVENTS_YEAR_ROUND, uiScriptLocale.GAME_EVENTS_INFORMATION_YEAR_ROUND_2, uiScriptLocale.GAME_EVENTS_INFORMATION_YEAR_ROUND_3, gameEvents.GetEventTime(15))
		self.informationDialog = informationDialog
		
		
	def __OpenEventItemInformationMining(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_MINING_1)
		informationDialog.height = 32
		informationDialog.Open(50613, uiScriptLocale.GAME_EVENTS_MINING, uiScriptLocale.GAME_EVENTS_INFORMATION_MINING_2, uiScriptLocale.GAME_EVENTS_INFORMATION_MINING_3, gameEvents.GetEventTime(18))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationHarvestFestival(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_HARVEST_FESTIVAL_1)
		informationDialog.height = 32
		informationDialog.Open(1, uiScriptLocale.GAME_EVENTS_HARVEST_FESTIVAL, uiScriptLocale.GAME_EVENTS_INFORMATION_HARVEST_FESTIVAL_2, uiScriptLocale.GAME_EVENTS_INFORMATION_HARVEST_FESTIVAL_3, gameEvents.GetEventTime(19))
		self.informationDialog = informationDialog
		
	def __OpenEventItemInformationMakeWonso(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_MAKE_WONSO_1)
		informationDialog.height = 32
		informationDialog.Open(50021, uiScriptLocale.GAME_EVENTS_MAKE_WONSO, uiScriptLocale.GAME_EVENTS_INFORMATION_MAKE_WONSO_2, uiScriptLocale.GAME_EVENTS_INFORMATION_MAKE_WONSO_3, gameEvents.GetEventTime(20))
		self.informationDialog = informationDialog
		
	def __OpenGoldenRainEventInformation(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_GOLDEN_RAIN_1)
		informationDialog.height = 32
		informationDialog.Open(1, uiScriptLocale.GAME_EVENTS_GOLDEN_RAIN, uiScriptLocale.GAME_EVENTS_INFORMATION_GOLDEN_RAIN_2, uiScriptLocale.GAME_EVENTS_INFORMATION_GOLDEN_RAIN_3, gameEvents.GetEventTime(22))
		self.informationDialog = informationDialog
		
	def __OpenSuperMetinEventInformation(self):
		if constInfo.IS_OPEN_EVENT_INFORMATION == 1:
			return
		informationDialog = uiCommon.EventInformationDialog()
		informationDialog.SetText(uiScriptLocale.GAME_EVENTS_INFORMATION_SUPER_METIN_1)
		informationDialog.height = 32
		informationDialog.Open(1, uiScriptLocale.GAME_EVENTS_SUPER_METIN, uiScriptLocale.GAME_EVENTS_INFORMATION_SUPER_METIN_2, uiScriptLocale.GAME_EVENTS_INFORMATION_SUPER_METIN_3, gameEvents.GetEventTime(23))
		self.informationDialog = informationDialog
	
	def Refresh(self):
		activate_event_count = 0	## ActivateEventCount
		if self.eventinfoDialog == None:
			return
			
		self.eventinfoDialog.DeleteAllButton()	## Delete All Buttons
		
		for i in xrange(gameEvents.EVENT_MAX_NUM):
			isActivate = gameEvents.IsActivateEvent(i)
			if isActivate == True:
				activate_event_count += 1
				if gameEvents.EVENT_MOON_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_MOON_DROP, self.__OpenEventItemInformationMoonDrop)
				elif gameEvents.EVENT_HEXAGON_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_HEXAGON_DROP, self.__OpenEventItemInformationHexagonDrop)
				elif gameEvents.EVENT_CARVING_STONE_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_CARVING_STONE_DROP, self.__OpenEventItemInformationCarvingStoneDrop)
				elif gameEvents.EVENT_RAMADAN == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_RAMADAN, self.__OpenEventItemInformationRamadan)
				elif gameEvents.EVENT_XMAS == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_XMAS, self.__OpenEventItemInformationXmas)
				elif gameEvents.EVENT_EASTER == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_EASTER, self.__OpenEventItemInformationEaster)
				elif gameEvents.EVENT_FOOTBALL_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_FOOTBALL_DROP, self.__OpenEventItemInformationFootBallDrop)
				elif gameEvents.EVENT_VALENTINE_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_VALENTINE_DROP, self.__OpenEventItemInformationValentineDrop)
				elif gameEvents.EVENT_PET_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_PET_DROP, self.__OpenEventItemInformationPetDrop)
				elif gameEvents.EVENT_KIDS_DAY_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_KIDS_DAY_DROP, self.__OpenEventItemInformationKidsDayDrop)
				elif gameEvents.EVENT_HALLOWEEN == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_HALLOWEEN, self.__OpenEventItemInformationHalloween)
				elif gameEvents.EVENT_OLYMPIC_BADGE == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_OLYMPIC_BADGE, self.__OpenEventItemInformationOlympicBadge)
				elif gameEvents.EVENT_OKEY_CARD == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_OKEY_CARD, self.__OpenOkeyCardDialog)
				elif gameEvents.EVENT_MOUNT_DROP == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_MOUNT_DROP, self.__OpenEventItemInformationMountDrop)
				elif gameEvents.EVENT_ALIGNMENT == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_ALIGNMENT, self.__OpenEventItemInformationAlignment)
				elif gameEvents.EVENT_YEAR_ROUND == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_YEAR_ROUND, self.__OpenEventItemInformationYearRound)
				elif gameEvents.EVENT_BOSS_HUNTERS == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_BOSS_HUNTERS, self.__OpenBossHuntersDialog)
				elif gameEvents.EVENT_NEW_FISH == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_FISH_EVENT, self.__ClickFishEventButton)
				elif gameEvents.EVENT_MINING == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_MINING, self.__OpenEventItemInformationMining)
				elif gameEvents.EVENT_HARVEST_FESTIVAL == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_HARVEST_FESTIVAL, self.__OpenEventItemInformationHarvestFestival)
				elif gameEvents.EVENT_MAKE_WONSO == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_MAKE_WONSO, self.__OpenEventItemInformationMakeWonso)
				elif gameEvents.EVENT_CATCH_KING == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_CATCH_KING, self.__OpenCatchKingDialog)
				elif gameEvents.EVENT_GOLDEN_RAIN == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_GOLDEN_RAIN, self.__OpenGoldenRainEventInformation)
					self.gameButton.SetUpVisual("d:/ymir work/ui/event_golden_rain.tga")
					self.gameButton.SetOverVisual("d:/ymir work/ui/event_golden_rain.tga")
					self.gameButton.SetDownVisual("d:/ymir work/ui/event_golden_rain.tga")
				elif gameEvents.EVENT_SUPER_METIN == i:
					self.eventinfoDialog.AppendButton(uiScriptLocale.GAME_EVENTS_SUPER_METIN, self.__OpenSuperMetinEventInformation)
			else:
				self.eventinfoDialog.DeleteButton(i)
				
		if activate_event_count > 0:	## Has Activate Event Show Icon
			self.Show()
		else:
			self.Hide()
			
		self.eventinfoDialog.RefreshDialog()	## Refresh Event Dialog
		
	def IntegrationMiniGame(self):				
		if app.ENABLE_FISH_JIGSAW_EVENT:
			if self.fishGame:
				self.fishGame.Destroy()
				self.fishGame = None
				
		if app.ENABLE_ATTENDANCE_EVENT:
			if self.attendanceGame:
				self.attendanceGame.Destroy()
				self.attendanceGame = None
				
		if app.ENABLE_MINI_GAME_CATCH_KING:
			if self.catchKingGame:
				self.catchKingGame.Destroy()
				self.catchKingGame = None

		if app.ENABLE_FISH_JIGSAW_EVENT:
			if not self.fishGame:
				self.fishGame = uiMiniGameFishEvent.MiniGameFish()
				
				if self.tooltipItem:
					self.fishGame.SetItemToolTip(self.tooltipItem)
					
		if app.ENABLE_ATTENDANCE_EVENT:
			if not self.attendanceGame:
				self.attendanceGame = uiMiniGameAttendance.MiniGameAttendance()
				
				if self.tooltipItem:
					self.attendanceGame.SetItemToolTip(self.tooltipItem)
					
		if app.ENABLE_MINI_GAME_CATCH_KING:
			if not self.catchKingGame:
				self.catchKingGame = uiMiniGameCatchKing.MiniGameCatchKing()
				
				if self.tooltipItem:
					self.catchKingGame.SetItemToolTip(self.tooltipItem)
		self.Refresh()
		
	if app.ENABLE_FISH_JIGSAW_EVENT:
		def MiniGameFishUse(self, shape, useCount):			
			if self.fishGame:
				self.fishGame.MiniGameFishUse(shape, useCount)
				
		def MiniGameFishAdd(self, pos, shape):			
			if self.fishGame:
				self.fishGame.MiniGameFishAdd(pos, shape)
				
		def MiniGameFishReward(self, vnum):			
			if self.fishGame:
				self.fishGame.MiniGameFishReward(vnum)
				
		def MiniGameFishCount(self, count):			
			if self.fishGame:
				self.fishGame.MiniGameFishCount(count)
			
		def __ClickFishEventButton(self):
			if self.fishGame:
				self.fishGame.Open()
				
	if app.ENABLE_MINI_GAME_CATCH_KING:		
		def MiniGameCatchKingEventStart(self, bigScore):
			if self.catchKingGame:
				self.catchKingGame.GameStart(bigScore)
				# dbg.TraceError("MiniGameCatchKingEventStart {0}".format(int(bigScore)))
				
		def MiniGameCatchKingSetHandCard(self, cardNumber):
			if self.catchKingGame:
				self.catchKingGame.CatchKingSetHandCard(cardNumber)
				# dbg.TraceError("MiniGameCatchKingSetHandCard {0}".format(int(cardNumber)))
				
		def MiniGameCatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
			if self.catchKingGame:
				self.catchKingGame.CatchKingResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)
				# dbg.TraceError("MiniGameCatchKingResultField {0} {1} {2} {3} {4} {5} {6} {7}".format(int(score), int(rowType), int(cardPos), int(cardValue), int(keepFieldCard), int(destroyHandCard), int(getReward), int(isFiveNear)))
				
		def MiniGameCatchKingSetEndCard(self, cardPos, cardNumber):
			if self.catchKingGame:
				self.catchKingGame.CatchKingSetEndCard(cardPos, cardNumber)
				# dbg.TraceError("MiniGameCatchKingSetEndCard {0} {1}".format(int(cardPos), int(cardNumber)))
				
		def MiniGameCatchKingReward(self, rewardCode):
			if self.catchKingGame:
				self.catchKingGame.CatchKingReward(rewardCode)
				# dbg.TraceError("MiniGameCatchKingReward {0}".format(int(rewardCode)))
				
		def __OpenCatchKingDialog(self):
			if self.catchKingGame:
				self.catchKingGame.Open()
				
	if app.ENABLE_ATTENDANCE_EVENT:
		def SetItemToolTip(self, tooltip):
			self.tooltipItem = tooltip
				
		def MiniGameAttendanceSetData(self, type, value):
			if not self.attendanceGame:
				self.attendanceGame = uiMiniGameAttendance.MiniGameAttendance()
				
			if type == 1:
				self.attendanceGame.MiniGameAttendanceSetDay(value)
			elif type == 2:
				self.attendanceGame.MiniGameAttendanceSetMissionClear(value)
			elif type == 3:
				self.attendanceGame.MiniGameAttendanceSetReward(value)
								
			self.attendanceGame.RefreshAttendanceBoard()
			
		def __OpenBossHuntersDialog(self):
			if self.attendanceGame:
				self.attendanceGame.Open()
			
	def Destroy(self):
		if app.ENABLE_FISH_JIGSAW_EVENT:
			if self.fishGame:
				self.fishGame.Destroy()
				self.fishGame = None
		if app.ENABLE_ATTENDANCE_EVENT:
			if self.attendanceGame:
				self.attendanceGame.Destroy()
				self.attendanceGame = None
		if app.ENABLE_MINI_GAME_CATCH_KING:
			if self.catchKingGame:
				self.catchKingGame.Destroy()
				self.catchKingGame = None
		self.ClearDictionary()
		
class EventInfoDialog(ui.ScriptWindow):		
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.board			= None
		self.close_button	= None
		self.button_dict	= {}
		self.LoadWindow()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		self.Destroy()
		
	def LoadWindow(self):
		if self.isLoaded == 1:
			return
	
		self.isLoaded = 1
	
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/eventinfodialog.py")
		except:
			import exception
			exception.Abort("eventinfodialog.LoadDialog.LoadScript")
			
		try:
			GetObject = self.GetChild
			self.board = GetObject("board")
			self.close_button = GetObject("close_button")
		except:
			import exception
			exception.Abort("eventinfodialog.LoadDialog.BindObject")
			
		self.close_button.SetEvent(ui.__mem_func__(self.Close))
		
	def AppendButton(self, name, func):
	
		if self.button_dict.has_key(name):
			return
	
		button = ui.Button()
		button.SetParent( self.board )
		button_count = len(self.button_dict)
		pos_y = (button_gap * (button_count + 1)) + button_count * button_height
		button.SetPosition( 10, pos_y )
		button.SetUpVisual( "d:/ymir work/ui/public/XLarge_Button_01.sub" )
		button.SetOverVisual( "d:/ymir work/ui/public/XLarge_Button_02.sub" )
		button.SetDownVisual( "d:/ymir work/ui/public/XLarge_Button_03.sub" )
		
		if name:
			button.SetText(name)
		
		if func: 
			button.SetEvent( ui.__mem_func__(func) )
			
		button.Show()
		self.button_dict[name] = button
		
	def DeleteButton(self, name):
	
		if not self.button_dict.has_key(name):
			return
			
		self.button_dict[name].Hide()
		del self.button_dict[name]
		
	def DeleteAllButton(self):
		
		for button in self.button_dict.values():
			button.Hide()
			del button
			
		self.button_dict.clear()
		
	def Show(self):
		self.LoadWindow()
		ui.ScriptWindow.Show(self)
		
	def Destroy(self):
		self.isLoaded = 0
		self.board			= None
		self.close_button	= None
		self.button_dict	= {}		

	def Close(self):
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True

	def RefreshDialog(self):
		## board 의 height 값 계산
		## self.button_dict 에는 close 버튼이 포함되어 있辭 않기 때문에 + 1 해斜다.
		total_len = len(self.button_dict) + 1
		board_height = (button_height * total_len) + (button_gap * (total_len + 1))
		self.board.SetSize(200, board_height)
		self.SetSize(200, board_height)
		
		## close 버튼의 위치 갱신
		dict_len = len(self.button_dict)
		pos_y = (button_gap * (dict_len + 1)) + dict_len * button_height
		self.close_button.SetPosition( 10, pos_y )
		
