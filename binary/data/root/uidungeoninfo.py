import app
import ui
import uiToolTip
import grp
import item
import player
import event
import constInfo
import localeInfo
import uiScriptLocale
import uiCommon

def FormatTime(seconds):
	if seconds == 0:
		return localeInfo.DUNGEON_INFO_NONE

	m, s = divmod(seconds, 60)
	h, m = divmod(m, 60)

	return "%d:%02d:%02d" % (h, m, s)
	
	
def SecondToHM(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24

	if hour <= 0:
		return "Soðuma: 00:%d:%02d" % (minute, second)
	else:
		return "Soðuma: 0%d:%02d:%02d" % (hour, minute,second)

class DungeonInfo(ui.ScriptWindow):
	TOOLTIP_NORMAL_COLOR = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)
	TOOLTIP_SPECIAL_COLOR = grp.GenerateColor(1.0, 0.7843, 0.0, 1.0)
	MIN_SCROLLBAR_LIST = 10

	DUNGEON_TYPE = {
		0 : localeInfo.DUNGEON_INFO_TYPE0,
		1 : localeInfo.DUNGEON_INFO_TYPE1,
		2 : localeInfo.DUNGEON_INFO_TYPE2
	}

	DUNGEON_ORGANIZATION = {
		0 : localeInfo.DUNGEON_INFO_ORGANIZATION0,
		1 : localeInfo.DUNGEON_INFO_ORGANIZATION1,
		2 : localeInfo.DUNGEON_INFO_ORGANIZATION2
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.dungeonIndex = 0
		self.dungeonButton = {}
		self.dungeonImage = {}
		self.dungeonImageIcon = {}
		self.dungeonName = {}
		self.dungeonAvailable = {}
		self.questionDialog = None
		self.dungeonRankingIndex = None

		self.isLoaded = False

	def __del__(self):
		ui.ScriptWindow.__del__(self)

		self.dungeonIndex = 0
		self.dungeonButton = {}
		self.dungeonImage = {}
		self.dungeonImageIcon = {}
		self.dungeonName = {}
		self.dungeonAvailable = {}
		self.questionDialog = None
		self.dungeonRankingIndex = None

		self.isLoaded = False

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/dungeoninfowindow.py")
		except:
			import exception
			exception.Abort("DungeonInfo.LoadDialog.LoadScript")

		try:
			self.dungeonBoard = self.GetChild("DungeonBoard")
			self.dungeonBoardTitleBar = self.GetChild("DungeonBoardTitleBar")

			self.dungeonButtonBoard = self.GetChild("DungeonButtonBoard")
			self.dungeonButtonThinBoard = self.GetChild("DungeonButtonThinBoard")

			self.dungeonInfoItem = self.GetChild("DungeonInfoItem")
			self.dungeonInfoItemSlot = self.GetChild("DungeonInfoItemSlot")

			self.dungeonScrollBar = self.GetChild("ScrollBar")
			self.dungeonInfoBoard = self.GetChild("DungeonInfoBoard")

			self.dungeonInfoName = self.GetChild("DungeonInfoName")
			self.dungeonInfoType = self.GetChild("DungeonInfoType")
			self.dungeonInfoOrganization = self.GetChild("DungeonInfoOrganization")
			self.dungeonInfoLevelLimit = self.GetChild("DungeonInfoLevelLimit")
			self.dungeonInfoPartyMembers = self.GetChild("DungeonInfoPartyMembers")
			self.dungeonInfoCooldown = self.GetChild("DungeonInfoCooldown")
			self.dungeonInfoDuration = self.GetChild("DungeonInfoDuration")
			self.dungeonInfoEntrance = self.GetChild("DungeonInfoEntrance")
			self.dungeonInfoStrengthBonus = self.GetChild("DungeonInfoStrengthBonus")
			self.dungeonInfoResistanceBonus = self.GetChild("DungeonInfoResistanceBonus")
			# self.dungeonInfoTotalFinished = self.GetChild("DungeonInfoTotalFinished")
			# self.dungeonInfoFastestTime = self.GetChild("DungeonInfoFastestTime")
			# self.dungeonInfoHighestDamage = self.GetChild("DungeonInfoHighestDamage")

			self.dungeonInfoTeleportButton = self.GetChild("DungeonInfoTeleportButton")
			self.closeDungeonBoard = self.GetChild("CloseDungeonBoard")

			# self.dungeonRank1Button = self.GetChild("DungeonRank1Button")
			# self.dungeonRank2Button = self.GetChild("DungeonRank2Button")
			# self.dungeonRank3Button = self.GetChild("DungeonRank3Button")

		except:
			import exception
			exception.Abort("DungeonInfo.LoadDialog.GetChild")

		self.dungeonBoardTitleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.closeDungeonBoard.SetEvent(ui.__mem_func__(self.Close))
		self.dungeonInfoTeleportButton.SetEvent(self.TeleportDungeon)

		# self.dungeonRank1Button.SetEvent(lambda arg = 1: self.OpenRankingBoard(arg))
		# self.dungeonRank2Button.SetEvent(lambda arg = 2: self.OpenRankingBoard(arg))
		# self.dungeonRank3Button.SetEvent(lambda arg = 3: self.OpenRankingBoard(arg))

		self.toolTip = uiToolTip.ToolTip()

		self.LoadDungeonButtons()
		self.LoadDungeonInfoBoard(self.dungeonIndex)

		self.isLoaded = True

	def Close(self):
		if self.toolTip:
			self.toolTip = None

		self.isLoaded = False
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Open(self):
		if not self.isLoaded:
			self.LoadDialog()

		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def LoadDungeonButtons(self):
		if not constInfo.dungeonInfo:
			return

		for index in xrange(min(self.MIN_SCROLLBAR_LIST, len(constInfo.dungeonInfo))):
			self.AppendDungeonButton(
				index,\
				self.dungeonButtonBoard,\
				3, 3 + (38 * index)
			)

		if len(constInfo.dungeonInfo) <= self.MIN_SCROLLBAR_LIST:
			self.dungeonScrollBar.SetMiddleBarSize(float(len(constInfo.dungeonInfo)) / float(len(constInfo.dungeonInfo)))
			self.dungeonScrollBar.Show()
		else:
			self.dungeonScrollBar.SetMiddleBarSize(float(self.MIN_SCROLLBAR_LIST) / float(len(constInfo.dungeonInfo)))
			self.dungeonScrollBar.Show()

		self.dungeonScrollBar.SetScrollEvent(self.OnScroll)

	def OnScroll(self):
		button_count = len(self.dungeonButton)
		pos = int(self.dungeonScrollBar.GetPos() * (len(constInfo.dungeonInfo) - button_count))

		self.dungeonButton = {}
		self.dungeonImage = {}
		self.dungeonImageIcon = {}
		self.dungeonName = {}
		self.dungeonAvailable = {}

		for idx in xrange(min(self.MIN_SCROLLBAR_LIST, button_count)):
			realPos = idx + pos

			self.AppendDungeonButton(
				realPos,\
				self.dungeonButtonBoard,\
				3, 3 + (38 * idx)
			)

			if realPos != self.dungeonIndex:
				self.dungeonButton[realPos].SetUpVisual("d:/ymir work/ui/game/mailbox/post_default.sub")
				self.dungeonButton[realPos].SetOverVisual("d:/ymir work/ui/game/mailbox/post_over.sub")
				self.dungeonButton[realPos].SetDownVisual("d:/ymir work/ui/game/mailbox/post_select.sub")

	def AppendDungeonButton(self, index, parent, x, y):
		self.dungeonButton[index] = ui.Button()
		self.dungeonButton[index].SetParent(parent)
		self.dungeonButton[index].SetUpVisual("d:/ymir work/ui/game/mailbox/post_select.sub")
		self.dungeonButton[index].SetOverVisual("d:/ymir work/ui/game/mailbox/post_select.sub")
		self.dungeonButton[index].SetDownVisual("d:/ymir work/ui/game/mailbox/post_select.sub")
		self.dungeonButton[index].SetPosition(x, y)
		self.dungeonButton[index].SetEvent(lambda: self.LoadDungeonInfoBoard(index))
		#self.dungeonButton[index].SetText("%d" % index)
		self.dungeonButton[index].Show()

		self.dungeonImage[index] = ui.ImageBox()
		self.dungeonImage[index].SetParent(self.dungeonButton[index])
		self.dungeonImage[index].LoadImage("d:/ymir work/ui/game/mailbox/mailbox_icon_empty.sub")
		self.dungeonImage[index].SetPosition(1, 2)
		self.dungeonImage[index].Show()

		self.dungeonImageIcon[index] = ui.Button()
		self.dungeonImageIcon[index].SetParent(self.dungeonImage[index])

		#imageIcon = constInfo.dungeonInfo[index]['item_vnum']
		mapIndex = constInfo.dungeonInfo[index]['map_index']
		#if imageIcon > 0:
		self.dungeonImageIcon[index].SetUpVisual("d:/ymir work/ui/game/dungeon_info/icons/%d.tga" % mapIndex)
		self.dungeonImageIcon[index].SetOverVisual("d:/ymir work/ui/game/dungeon_info/icons/%d.tga" % mapIndex)
		self.dungeonImageIcon[index].SetDownVisual("d:/ymir work/ui/game/dungeon_info/icons/%d.tga" % mapIndex)
		self.dungeonImageIcon[index].SetEvent(lambda: self.LoadDungeonInfoBoard(index))
		#else:
		#	self.dungeonImageIcon[index].SetUpVisual("d:/ymir work/ui/pet/skill_button/skill_enable_button.sub")
		#	self.dungeonImageIcon[index].SetOverVisual("d:/ymir work/ui/pet/skill_button/skill_enable_button.sub")
		#	self.dungeonImageIcon[index].SetDownVisual("d:/ymir work/ui/pet/skill_button/skill_enable_button.sub")

		self.dungeonImageIcon[index].SetPosition(0, 0)
		self.dungeonImageIcon[index].Show()

		self.dungeonName[index] = ui.TextLine()
		self.dungeonName[index].SetParent(self.dungeonButton[index])
		self.dungeonName[index].SetPosition(40, 10)
		self.dungeonName[index].SetText("%s" % constInfo.dungeonInfo[index]['map'])
		self.dungeonName[index].Show()

		self.dungeonAvailable[index] = ui.TextLine()
		self.dungeonAvailable[index].SetParent(self.dungeonButton[index])
		self.dungeonAvailable[index].SetPosition(200, 10)
		self.dungeonAvailable[index].SetText(localeInfo.DUNGEON_INFO_UNAVAILABLE)
		self.dungeonAvailable[index].Show()

		self.dungeonAvailable[index].SetText("data_loading")

		# format_time = constInfo.dungeonInfo[index]['reptime']-app.GetGlobalTimeStamp()

		# if player.GetStatus(player.LEVEL) >= constInfo.dungeonInfo[index]['level_limit']:
			# self.dungeonAvailable[index].SetText(localeInfo.DUNGEON_INFO_AVAILABLE)
		# if (format_time < 0):
			# self.dungeonAvailable[index].SetText(localeInfo.DUNGEON_INFO_AVAILABLE)
		# else:
			# self.dungeonAvailable[index].SetText("|cFFF5F5DC%s|r" % SecondToHM(format_time))
			# self.dungeonAvailable[index].SetPosition(200-5, 10)

	def LoadDungeonInfoBoard(self, index):
		self.dungeonIndex = index
		mapIndex = constInfo.dungeonInfo[self.dungeonIndex]['map_index']
		if (self.dungeonIndex == 0):
			self.dungeonInfoBoard.LoadImage("d:/ymir work/ui/game/dungeon_info/background/66.tga")
		else:
			self.dungeonInfoBoard.LoadImage("d:/ymir work/ui/game/dungeon_info/background/%d.tga" % int(mapIndex))


		self.dungeonButton[self.dungeonIndex].SetUpVisual("d:/ymir work/ui/game/mailbox/post_select.sub")
		self.dungeonButton[self.dungeonIndex].SetOverVisual("d:/ymir work/ui/game/mailbox/post_select.sub")
		self.dungeonButton[self.dungeonIndex].SetDownVisual("d:/ymir work/ui/game/mailbox/post_select.sub")

		pos = int(self.dungeonScrollBar.GetPos() * (len(constInfo.dungeonInfo) - len(self.dungeonButton)))
		for idx in xrange(len(self.dungeonButton)):
			realPos = idx + pos
			if realPos != self.dungeonIndex:
				self.dungeonButton[realPos].SetUpVisual("d:/ymir work/ui/game/mailbox/post_default.sub")
				self.dungeonButton[realPos].SetOverVisual("d:/ymir work/ui/game/mailbox/post_over.sub")
				self.dungeonButton[realPos].SetDownVisual("d:/ymir work/ui/game/mailbox/post_select.sub")

		dungeonMap = str(constInfo.dungeonInfo[self.dungeonIndex]['map'])
		dungeonType = constInfo.dungeonInfo[self.dungeonIndex]['type']
		dungeonOrganization = constInfo.dungeonInfo[self.dungeonIndex]['organization']
		dungeonLevelLimit = constInfo.dungeonInfo[self.dungeonIndex]['level_limit']
		dungeonPartyMembers = constInfo.dungeonInfo[self.dungeonIndex]['party_members']
		dungeonCooldown = constInfo.dungeonInfo[self.dungeonIndex]['cooldown']
		dungeonDuration = constInfo.dungeonInfo[self.dungeonIndex]['duration']
		dungeonEntranceMap = str(constInfo.dungeonInfo[self.dungeonIndex]['entrance_map'])
		dungeonStrengthBonus = str(constInfo.dungeonInfo[self.dungeonIndex]['strength_bonus'])
		dungeonResistanceBonus = str(constInfo.dungeonInfo[self.dungeonIndex]['resistance_bonus'])
		dungeonItemVnum = int(constInfo.dungeonInfo[self.dungeonIndex]['item_vnum'])

		self.dungeonInfoName.SetText("%s" % dungeonMap)
		self.dungeonInfoType.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_TYPE, self.DUNGEON_TYPE[dungeonType]))
		self.dungeonInfoOrganization.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_ORGANIZATION, self.DUNGEON_ORGANIZATION[dungeonOrganization]))
		self.dungeonInfoLevelLimit.SetText("%s : %d - 120" % (uiScriptLocale.DUNGEON_INFO_LEVEL_LIMIT, dungeonLevelLimit))
		self.dungeonInfoPartyMembers.SetText("%s : %d" % (uiScriptLocale.DUNGEON_INFO_PARTY_MEMBERS, dungeonPartyMembers))
		self.dungeonInfoCooldown.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_COOLDOWN, FormatTime(dungeonCooldown)))
		self.dungeonInfoDuration.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_DURATION, FormatTime(dungeonDuration)))
		self.dungeonInfoEntrance.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_ENTRANCE, dungeonEntranceMap))
		self.dungeonInfoStrengthBonus.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_STRENGTH, dungeonStrengthBonus))
		self.dungeonInfoResistanceBonus.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_RESISTANCE, dungeonResistanceBonus))
		# self.dungeonInfoTotalFinished.SetText("%s : %d" % (uiScriptLocale.DUNGEON_INFO_TOTAL_FINISHED, dungeonFinished))
		# self.dungeonInfoFastestTime.SetText("%s : %s" % (uiScriptLocale.DUNGEON_INFO_FASTEST_TIME, FormatTime(dungeonFastestTime)))
		# self.dungeonInfoHighestDamage.SetText("%s : %d" % (uiScriptLocale.DUNGEON_INFO_HIGHEST_DAMAGE, dungeonHighestDamage))

		if dungeonItemVnum > 0:
			self.dungeonInfoItemSlot.LoadImage("icon/item/%d.tga" % dungeonItemVnum)
		else:
			self.dungeonInfoItemSlot.LoadImage("d:/ymir work/ui/pet/skill_button/skill_enable_button.sub")

	def TeleportDungeon(self):
		if player.GetStatus(player.LEVEL) < constInfo.dungeonInfo[self.dungeonIndex]['level_limit']:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DUNGEON_INFO_LEVEL_ERROR)
			return

		dungeonTeleportQuestionDialog = uiCommon.QuestionDialogWithTimeLimit3()
		dungeonTeleportQuestionDialog.SetText1(localeInfo.DUNGEON_INFO_TELEPORT_QUESTION % str(constInfo.dungeonInfo[self.dungeonIndex]['map']))
		# dungeonTeleportQuestionDialog.SetTimeOverMsg("iptal edildi.")
		dungeonTeleportQuestionDialog.SetTimeOverEvent(self.AnswerTeleport, False)
		dungeonTeleportQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerTeleport(arg))
		dungeonTeleportQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerTeleport(arg))
		dungeonTeleportQuestionDialog.Open(10)
		self.dungeonTeleportQuestionDialog = dungeonTeleportQuestionDialog

	def AnswerTeleport(self, answer):
		if not self.dungeonTeleportQuestionDialog:
			return

		if answer == True:

			self.Close()
			dungeonMapCoordX = int(constInfo.dungeonInfo[self.dungeonIndex]['map_coord_x'])
			dungeonMapCoordY = int(constInfo.dungeonInfo[self.dungeonIndex]['map_coord_y'])

			constInfo.dungeonData["quest_cmd"] = "WARP#%d#%d" % (dungeonMapCoordX, dungeonMapCoordY)
			event.QuestButtonClick(int(constInfo.dungeonData["quest_index"]))

		self.dungeonTeleportQuestionDialog.Close()
		self.dungeonTeleportQuestionDialog = None

	def OnUpdate(self):
		pos = int(self.dungeonScrollBar.GetPos() * (len(constInfo.dungeonInfo) - len(self.dungeonButton)))

		for idx in xrange(min(self.MIN_SCROLLBAR_LIST, len(self.dungeonButton))):
			realPos = idx + pos
			format_time = constInfo.dungeonInfo[realPos]['reptime']-app.GetGlobalTimeStamp()
	
			if player.GetStatus(player.LEVEL) >= constInfo.dungeonInfo[realPos]['level_limit']:
				self.dungeonAvailable[realPos].SetText(localeInfo.DUNGEON_INFO_AVAILABLE)
				self.dungeonAvailable[realPos].SetPosition(200, 10)
				if (format_time < 0):
					self.dungeonAvailable[realPos].SetText(localeInfo.DUNGEON_INFO_AVAILABLE)
					self.dungeonAvailable[realPos].SetPosition(200, 10)
				else:
					self.dungeonAvailable[realPos].SetText("|cffc9c89a%s|r" % SecondToHM(format_time))
					self.dungeonAvailable[realPos].SetPosition(200-8, 10)
			elif player.GetStatus(player.LEVEL) <= constInfo.dungeonInfo[realPos]['level_limit']:
				self.dungeonAvailable[realPos].SetText(localeInfo.DUNGEON_INFO_UNAVAILABLE)
				self.dungeonAvailable[realPos].SetPosition(200, 10)

			
		if self.toolTip:
			if self.dungeonInfoItemSlot.IsIn():
				self.toolTip.ClearToolTip()

				dungeonItemVnum = constInfo.dungeonInfo[self.dungeonIndex]['item_vnum']
				if dungeonItemVnum > 0:
					item.SelectItem(dungeonItemVnum)

					self.toolTip.AppendTextLine(item.GetItemName(), self.TOOLTIP_SPECIAL_COLOR)
					self.toolTip.AppendDescription(item.GetItemDescription(), 26)

					self.toolTip.AlignHorizonalCenter()
					self.toolTip.ShowToolTip()

			else:
				self.toolTip.HideToolTip()
				
