import net
import app
import player
import chr
import item
import messenger
import guild
import nonplayer
import background
import ui
import wndMgr
import localeInfo
import constInfo
import uiToolTip
import uiCommon
import chrmgr
if app.ENABLE_TARGET_INFORMATION_SYSTEM:
	def HAS_FLAG(value, flag):
		return (value & flag) == flag
if app.ENABLE_VIEW_ELEMENT_SYSTEM:
	ELEMENT_IMAGE_DIC = {1: "elect", 2: "fire", 3: "ice", 4: "wind", 5: "earth", 6 : "dark"}
		
class TargetBoard(ui.ThinBoard):

	if app.ENABLE_TARGET_INFORMATION_SYSTEM:
		class InfoBoard(ui.ThinBoard):
			class ItemListBoxItem(ui.ListBoxExNew.Item):
				def __init__(self, width):
					ui.ListBoxExNew.Item.__init__(self)

					image = ui.ExpandedImageBox()
					image.SetParent(self)
					image.Show()
					self.image = image

					nameLine = ui.TextLine()
					nameLine.SetParent(self)
					nameLine.SetPosition(32 + 5, 0)
					nameLine.Show()
					self.nameLine = nameLine

					self.SetSize(width, 32 + 5)

				def LoadImage(self, image, name = None):
					self.image.LoadImage(image)
					self.SetSize(self.GetWidth(), self.image.GetHeight() + 5 * (self.image.GetHeight() / 32))
					if name != None:
						self.SetText(name)

				def SetText(self, text):
					self.nameLine.SetText(text)

				def RefreshHeight(self):
					ui.ListBoxExNew.Item.RefreshHeight(self)
					self.image.SetRenderingRect(0.0, 0.0 - float(self.removeTop) / float(self.GetHeight()), 0.0, 0.0 - float(self.removeBottom) / float(self.GetHeight()))
					self.image.SetPosition(0, - self.removeTop)

			EXP_BASE_LVDELTA = [
				1,  #  -15 0
				5,  #  -14 1
				10, #  -13 2
				20, #  -12 3
				30, #  -11 4
				50, #  -10 5
				70, #  -9  6
				80, #  -8  7
				85, #  -7  8
				90, #  -6  9
				92, #  -5  10
				94, #  -4  11
				96, #  -3  12
				98, #  -2  13
				100,	#  -1  14
				100,	#  0   15
				105,	#  1   16
				110,	#  2   17
				115,	#  3   18
				120,	#  4   19
				125,	#  5   20
				130,	#  6   21
				135,	#  7   22
				140,	#  8   23
				145,	#  9   24
				150,	#  10  25
				155,	#  11  26
				160,	#  12  27
				165,	#  13  28
				170,	#  14  29
				180,	#  15  30
			]

			RACE_FLAG_TO_NAME = {
				1 << 0  : localeInfo.TARGET_INFO_RACE_ANIMAL,
				1 << 1 	: localeInfo.TARGET_INFO_RACE_UNDEAD,
				1 << 2  : localeInfo.TARGET_INFO_RACE_DEVIL,
				1 << 3  : localeInfo.TARGET_INFO_RACE_HUMAN,
				1 << 4  : localeInfo.TARGET_INFO_RACE_ORC,
				1 << 5  : localeInfo.TARGET_INFO_RACE_MILGYO,
			}

			SUB_RACE_FLAG_TO_NAME = {
				1 << 11 : localeInfo.TARGET_INFO_RACE_ELEC,
				1 << 12 : localeInfo.TARGET_INFO_RACE_FIRE,
				1 << 13 : localeInfo.TARGET_INFO_RACE_ICE,
				1 << 14 : localeInfo.TARGET_INFO_RACE_WIND,
				1 << 15 : localeInfo.TARGET_INFO_RACE_EARTH,
				1 << 16 : localeInfo.TARGET_INFO_RACE_DARK,
			}

			STONE_START_VNUM = 28030
			STONE_LAST_VNUM = 28042

			BOARD_WIDTH = 250

			def __init__(self):
				ui.ThinBoard.__init__(self)

				self.HideCorners(self.LT)
				self.HideCorners(self.RT)
				self.HideLine(self.T)

				self.race = 0
				self.hasItems = False

				self.itemTooltip = uiToolTip.ItemToolTip()
				self.itemTooltip.HideToolTip()

				self.stoneImg = None
				self.stoneVnum = None
				self.lastStoneVnum = 0
				self.nextStoneIconChange = 0
				wndItem = ui.GridSlotWindow()
				wndItem.SetParent(self)
				wndItem.ArrangeSlot(0, 7, 5, 32, 32, 0, 0)
				wndItem.RefreshSlot()
				wndItem.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
				wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
				wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
				wndItem.Hide()
				self.wndItem = wndItem
				self.itemArray = [0 for i in xrange(35)]
				self.itemVnums = [0 for i in xrange(35)]

				self.SetSize(self.BOARD_WIDTH, 0)

			def __del__(self):
				ui.ThinBoard.__del__(self)

			def __UpdatePosition(self, targetBoard):
				self.SetPosition(targetBoard.GetLeft() + (targetBoard.GetWidth() - self.GetWidth()) / 2, targetBoard.GetBottom() - 17)

			def Open(self, targetBoard, race):
				self.__LoadInformation(race)

				self.SetSize(self.BOARD_WIDTH, self.yPos + 10)
				self.__UpdatePosition(targetBoard)

				self.Show()

			def Refresh(self):
				self.__LoadInformation(self.race)
				self.SetSize(self.BOARD_WIDTH, self.yPos + 10)

			def Close(self):
				self.itemTooltip.HideToolTip()
				self.Hide()

			def __LoadInformation(self, race):
				self.yPos = 7
				self.children = []
				self.race = race
				self.stoneImg = None
				self.stoneVnum = None
				self.nextStoneIconChange = 0

				for i in xrange(35):
					self.wndItem.ClearSlot(i)
				self.itemArray = [0 for i in xrange(35)]
				self.itemVnums = [0 for i in xrange(35)]
				self.__LoadInformation_Default(race)
				self.__LoadInformation_Race(race)
				self.__LoadInformation_Resists(race)
				self.__LoadInformation_Drops(race)

			def __LoadInformation_Default_GetHitRate(self, race):
				attacker_dx = nonplayer.GetMonsterDX(race)
				attacker_level = nonplayer.GetMonsterLevel(race)

				self_dx = player.GetStatus(player.DX)
				self_level = player.GetStatus(player.LEVEL)

				iARSrc = min(90, (attacker_dx * 4 + attacker_level * 2) / 6)
				iERSrc = min(90, (self_dx * 4 + self_level * 2) / 6)

				fAR = (float(iARSrc) + 210.0) / 300.0
				fER = (float(iERSrc) * 2 + 5) / (float(iERSrc) + 95) * 3.0 / 10.0

				return fAR - fER

			def __LoadInformation_Default(self, race):
				self.AppendSeperator()
				self.AppendTextLine(localeInfo.TARGET_INFO_MAX_HP % localeInfo.NumberToString(nonplayer.GetMonsterMaxHP(race)))

				# calc att damage
				monsterLevel = nonplayer.GetMonsterLevel(race)
				fHitRate = self.__LoadInformation_Default_GetHitRate(race)
				iDamMin, iDamMax = nonplayer.GetMonsterDamage(race)
				iDamMin = int((iDamMin + nonplayer.GetMonsterST(race)) * 2 * fHitRate) + monsterLevel * 2
				iDamMax = int((iDamMax + nonplayer.GetMonsterST(race)) * 2 * fHitRate) + monsterLevel * 2
				iDef = player.GetStatus(player.DEF_GRADE) * (100 + player.GetStatus(player.DEF_BONUS)) / 100
				fDamMulti = nonplayer.GetMonsterDamageMultiply(race)
				iDamMin = int(max(0, iDamMin - iDef) * fDamMulti)
				iDamMax = int(max(0, iDamMax - iDef) * fDamMulti)
				if iDamMin < 1:
					iDamMin = 1
				if iDamMax < 5:
					iDamMax = 5
				self.AppendTextLine(localeInfo.TARGET_INFO_DAMAGE % (str(iDamMin), str(iDamMax)))

				idx = min(len(self.EXP_BASE_LVDELTA) - 1, max(0, (monsterLevel + 15) - player.GetStatus(player.LEVEL)))
				iExp = nonplayer.GetMonsterExp(race) * self.EXP_BASE_LVDELTA[idx] / 100
				self.AppendTextLine(localeInfo.TARGET_INFO_EXP % localeInfo.NumberToString(iExp))

				# self.AppendTextLine(localeInfo.TARGET_INFO_GOLD_MIN_MAX % (localeInfo.NumberToString(nonplayer.GetMobGoldMin(race)), localeInfo.NumberToString(nonplayer.GetMobGoldMax(race))))
				self.AppendTextLine(localeInfo.TARGET_INFO_REGEN_INFO % (nonplayer.GetMobRegenPercent(race), nonplayer.GetMobRegenCycle(race)))

			def __LoadInformation_Race(self, race):
				dwRaceFlag = nonplayer.GetMonsterRaceFlag(race)
				self.AppendSeperator()

				mainrace = ""
				subrace = ""
				for i in xrange(17):
					curFlag = 1 << i
					if HAS_FLAG(dwRaceFlag, curFlag):
						if self.RACE_FLAG_TO_NAME.has_key(curFlag):
							mainrace += self.RACE_FLAG_TO_NAME[curFlag] + ", "
						elif self.SUB_RACE_FLAG_TO_NAME.has_key(curFlag):
							subrace += self.SUB_RACE_FLAG_TO_NAME[curFlag] + ", "
				if nonplayer.IsMonsterStone(race):
					mainrace += localeInfo.TARGET_INFO_RACE_METIN + ", "
				if mainrace == "":
					mainrace = localeInfo.TARGET_INFO_NO_RACE
				else:
					mainrace = mainrace[:-2]
				if subrace == "":
					subrace = localeInfo.TARGET_INFO_NO_RACE
				else:
					subrace = subrace[:-2]

				self.AppendTextLine(localeInfo.TARGET_INFO_MAINRACE % mainrace)
				self.AppendTextLine(localeInfo.TARGET_INFO_SUBRACE % subrace)

			def __LoadInformation_Resists(self, race):
				self.AppendSeperator()
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS)
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE0 % (nonplayer.GetResist(race, nonplayer.MOB_RESIST_SWORD), nonplayer.GetResist(race, nonplayer.MOB_RESIST_TWOHAND), nonplayer.GetResist(race, nonplayer.MOB_RESIST_BELL)))
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE1 % (nonplayer.GetResist(race, nonplayer.MOB_RESIST_DAGGER), nonplayer.GetResist(race, nonplayer.MOB_RESIST_FAN), nonplayer.GetResist(race, nonplayer.MOB_RESIST_BOW)))

			def SerachEmptySlot(self, size):
				
				for value in xrange(35):
					
					if 0 == self.itemArray[value]:	# ?????
					
						if 1 == size:
							return value
							
						emptySlotIndex	= value
						searchSucceed	= True
						
						for i in range(size - 1):
							emptySlotIndex = emptySlotIndex + 7
						
							if emptySlotIndex >= 35:
								searchSucceed = False
								continue
							
							if 1 == self.itemArray[emptySlotIndex]:
								searchSucceed = False
					
						if True == searchSucceed:
							return value
						
				return -1
				
			def __LoadInformation_Drops(self, race):
				self.AppendSeperator()

				if race in constInfo.MONSTER_INFO_DATA:
					if len(constInfo.MONSTER_INFO_DATA[race]["items"]) == 0:
						self.wndItem.Hide()
					else:
						self.wndItem.SetPosition(10, self.yPos - 7)
						self.yPos += 32*5
						self.SetSize(self.BOARD_WIDTH, self.yPos + 5)
						self.UpdateRect()
						self.wndItem.Show()

						for curItem in constInfo.MONSTER_INFO_DATA[race]["items"]:
							getItemID = 0
							if curItem.has_key("vnum_list"):
								getItemID = curItem["vnum_list"][0]
							else:
								getItemID = curItem["vnum"]

							getItemCount=curItem["count"]
							item.SelectItem(getItemID)
							itemSize = item.GetItemSize()

							emptySlotPos = self.SerachEmptySlot(itemSize[1])

							if -1 != emptySlotPos:
								self.itemArray[emptySlotPos] = 1

								if itemSize[1] == 2:
									self.itemArray[emptySlotPos + 7] = 1
								elif itemSize[1] == 3:
									self.itemArray[emptySlotPos + 7] = 1
									self.itemArray[emptySlotPos + 14] = 1

								self.wndItem.SetItemSlot(emptySlotPos, getItemID, getItemCount)
								self.itemVnums[emptySlotPos] = getItemID

						self.wndItem.RefreshSlot()
				else:
					self.wndItem.Hide()
			def OverInItem(self, slotIndex):
				vnum = self.itemVnums[slotIndex]
				if vnum != 0:
					self.OnShowItemTooltip(vnum)

			def OverOutItem(self):
				self.OnHideItemTooltip()
			def AppendTextLine(self, text):
				textLine = ui.TextLine()
				textLine.SetParent(self)
				textLine.SetWindowHorizontalAlignCenter()
				textLine.SetHorizontalAlignCenter()
				textLine.SetText(text)
				textLine.SetPosition(0, self.yPos)
				textLine.Show()

				self.children.append(textLine)
				self.yPos += 17

			def AppendSeperator(self):
				img = ui.ImageBox()
				img.LoadImage("d:/ymir work/ui/seperator.tga")
				self.AppendWindow(img)
				img.SetPosition(img.GetLeft(), img.GetTop() - 15)
				self.yPos -= 15

			def AppendItem(self, listBox, vnums, count):
				if type(vnums) == int:
					vnum = vnums
				else:
					vnum = vnums[0]

				item.SelectItem(vnum)
				itemName = item.GetItemName()
				if type(vnums) != int and len(vnums) > 1:
					vnums = sorted(vnums)
					realName = itemName[:itemName.find("+")]
					if item.GetItemType() == item.ITEM_TYPE_METIN:
						realName = localeInfo.TARGET_INFO_STONE_NAME
						itemName = realName + "+0 - +4"
					else:
						itemName = realName + "+" + str(vnums[0] % 10) + " - +" + str(vnums[len(vnums) - 1] % 10)
					vnum = vnums[len(vnums) - 1]

				myItem = self.ItemListBoxItem(listBox.GetWidth())
				myItem.LoadImage(item.GetIconImageFileName())
				if count <= 1:
					myItem.SetText(itemName)
				else:
					myItem.SetText("%dx %s" % (count, itemName))
				myItem.SAFE_SetOverInEvent(self.OnShowItemTooltip, vnum)
				myItem.SAFE_SetOverOutEvent(self.OnHideItemTooltip)
				listBox.AppendItem(myItem)

				if item.GetItemType() == item.ITEM_TYPE_METIN:
					self.stoneImg = myItem
					self.stoneVnum = vnums
					self.lastStoneVnum = self.STONE_LAST_VNUM + vnums[len(vnums) - 1] % 1000 / 100 * 100

				return myItem.GetHeight()

			def OnShowItemTooltip(self, vnum):
				item.SelectItem(vnum)
				if item.GetItemType() != item.ITEM_TYPE_METIN and vnum != 50300:
					self.itemTooltip.SetItemToolTip(vnum)

			def OnHideItemTooltip(self):
				self.itemTooltip.HideToolTip()

			def AppendWindow(self, wnd, x = 0, width = 0, height = 0):
				if width == 0:
					width = wnd.GetWidth()
				if height == 0:
					height = wnd.GetHeight()

				wnd.SetParent(self)
				if x == 0:
					wnd.SetPosition((self.GetWidth() - width) / 2, self.yPos)
				else:
					wnd.SetPosition(x, self.yPos)
				wnd.Show()

				self.children.append(wnd)
				self.yPos += height + 5

			def OnUpdate(self):
				if self.stoneImg != None and self.stoneVnum != None and app.GetTime() >= self.nextStoneIconChange:
					nextImg = self.lastStoneVnum + 1
					if nextImg % 100 > self.STONE_LAST_VNUM % 100:
						nextImg -= (self.STONE_LAST_VNUM - self.STONE_START_VNUM) + 1
					self.lastStoneVnum = nextImg
					self.nextStoneIconChange = app.GetTime() + 2.5

					item.SelectItem(nextImg)
					itemName = item.GetItemName()
					realName = itemName[:itemName.find("+")]
					realName = realName + "+0 - +4"
					self.stoneImg.LoadImage(item.GetIconImageFileName(), realName)

					if self.itemTooltip.IsShow() and self.itemTooltip.isStone:
						self.itemTooltip.SetItemToolTip(nextImg)

	BUTTON_NAME_LIST = ( 
		localeInfo.TARGET_BUTTON_WHISPER, 
		localeInfo.TARGET_BUTTON_EXCHANGE, 
		localeInfo.TARGET_BUTTON_FIGHT, 
		localeInfo.TARGET_BUTTON_ACCEPT_FIGHT, 
		localeInfo.TARGET_BUTTON_AVENGE, 
		localeInfo.TARGET_BUTTON_FRIEND, 
		localeInfo.TARGET_BUTTON_INVITE_PARTY, 
		localeInfo.TARGET_BUTTON_LEAVE_PARTY, 
		localeInfo.TARGET_BUTTON_EXCLUDE, 
		localeInfo.TARGET_BUTTON_INVITE_GUILD,
		localeInfo.TARGET_BUTTON_DISMOUNT,
		localeInfo.TARGET_BUTTON_EXIT_OBSERVER,
		localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT,
		localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY,
		localeInfo.TARGET_BUTTON_BUILDING_DESTROY,
		localeInfo.TARGET_BUTTON_EMOTION_ALLOW,
		localeInfo.TARGET_BUTTON_BLOCK,
		localeInfo.TARGET_BUTTON_UNBLOCK,
		"VOTE_BLOCK_CHAT",
		localeInfo.TARGET_BUTTON_REVIVE,
	)

	GRADE_NAME =	{
						nonplayer.PAWN : localeInfo.TARGET_LEVEL_PAWN,
						nonplayer.S_PAWN : localeInfo.TARGET_LEVEL_S_PAWN,
						nonplayer.KNIGHT : localeInfo.TARGET_LEVEL_KNIGHT,
						nonplayer.S_KNIGHT : localeInfo.TARGET_LEVEL_S_KNIGHT,
						nonplayer.BOSS : localeInfo.TARGET_LEVEL_BOSS,
						nonplayer.KING : localeInfo.TARGET_LEVEL_KING,
					}
	EXCHANGE_LIMIT_RANGE = 3000

	def __init__(self):
		ui.ThinBoard.__init__(self)
		
		name = ui.TextLine()
		name.SetParent(self)
		name.SetDefaultFontName()
		name.SetOutline()
		name.Show()
		
		if app.ENABLE_ATTENDANCE_EVENT:
			hitCount = ui.TextLine()
			hitCount.SetParent(name)
			hitCount.SetDefaultFontName()
			hitCount.SetOutline()
			hitCount.Show()
			self.hitCount = hitCount
		
		hpGaugeDMG = ui.Gauge()
		hpGaugeDMG.SetParent(self)
		hpGaugeDMG.MakeGauge(130, "yellow")
		hpGaugeDMG.Hide()

		hpGauge = ui.GaugeWithoutBorder()
		hpGauge.SetParent(self)
		hpGauge.MakeGauge(130, "red")
		hpGauge.Hide()
		
		hpGaugePoison = ui.GaugeWithoutBorder()
		hpGaugePoison.SetParent(self)
		hpGaugePoison.MakeGauge(130, "lime")
		hpGaugePoison.Hide()
		
		if app.ENABLE_VIEW_TARGET_MONSTER_HP:
			hpDecimal = ui.TextLine()
			hpDecimal.SetParent(hpGauge)
			hpDecimal.SetDefaultFontName()
			hpDecimal.SetPosition(5, 5)
			hpDecimal.SetOutline()
			hpDecimal.Hide()

			hpDecimalPoison = ui.TextLine()
			hpDecimalPoison.SetParent(hpGaugePoison)
			hpDecimalPoison.SetDefaultFontName()
			hpDecimalPoison.SetPosition(5, 5)
			hpDecimalPoison.SetOutline()
			hpDecimalPoison.Hide()

		closeButton = ui.Button()
		closeButton.SetParent(self)
		closeButton.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		closeButton.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		closeButton.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		closeButton.SetPosition(30, 13)


		hpGauge.SetPosition(175, 17)
		hpGauge.SetWindowHorizontalAlignRight()
		hpGaugePoison.SetPosition(175, 17)
		hpGaugePoison.SetWindowHorizontalAlignRight()
		closeButton.SetWindowHorizontalAlignRight()
		hpGaugeDMG.SetPosition(175, 17)
		hpGaugeDMG.SetWindowHorizontalAlignRight()

		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			infoButton = ui.Button()
			infoButton.SetParent(self)
			infoButton.SetUpVisual("d:/ymir work/ui/game/mark/question_mark_1.tga")
			infoButton.SetOverVisual("d:/ymir work/ui/game/mark/question_mark_2.tga")
			infoButton.SetDownVisual("d:/ymir work/ui/game/mark/question_mark_3.tga")
			infoButton.SetEvent(ui.__mem_func__(self.OnPressedInfoButton))
			infoButton.Hide()

			infoBoard = self.InfoBoard()
			infoBoard.Hide()
			infoButton.showWnd = infoBoard
			
		closeButton.SetEvent(ui.__mem_func__(self.OnPressedCloseButton))
		closeButton.Show()

		self.buttonDict = {}
		self.showingButtonList = []
		for buttonName in self.BUTTON_NAME_LIST:
			button = ui.Button()
			button.SetParent(self)
		

			button.SetUpVisual("d:/ymir work/ui/public/small_thin_button_01.sub")
			button.SetOverVisual("d:/ymir work/ui/public/small_thin_button_02.sub")
			button.SetDownVisual("d:/ymir work/ui/public/small_thin_button_03.sub")
			
			button.SetWindowHorizontalAlignCenter()
			button.SetText(buttonName)
			button.Hide()
			self.buttonDict[buttonName] = button
			self.showingButtonList.append(button)

		self.buttonDict[localeInfo.TARGET_BUTTON_WHISPER].SetEvent(ui.__mem_func__(self.OnWhisper))
		if app.ENABLE_MESSENGER_BLOCK:
			self.buttonDict[localeInfo.TARGET_BUTTON_BLOCK].SetEvent(ui.__mem_func__(self.OnAppendToBlockMessenger))
			self.buttonDict[localeInfo.TARGET_BUTTON_UNBLOCK].SetEvent(ui.__mem_func__(self.OnRemoveToBlockMessenger))
		self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE].SetEvent(ui.__mem_func__(self.OnExchange))
		self.buttonDict[localeInfo.TARGET_BUTTON_FIGHT].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[localeInfo.TARGET_BUTTON_ACCEPT_FIGHT].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[localeInfo.TARGET_BUTTON_AVENGE].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[localeInfo.TARGET_BUTTON_FRIEND].SetEvent(ui.__mem_func__(self.OnAppendToMessenger))
		self.buttonDict[localeInfo.TARGET_BUTTON_FRIEND].SetEvent(ui.__mem_func__(self.OnAppendToMessenger))
		self.buttonDict[localeInfo.TARGET_BUTTON_INVITE_PARTY].SetEvent(ui.__mem_func__(self.OnPartyInvite))
		self.buttonDict[localeInfo.TARGET_BUTTON_LEAVE_PARTY].SetEvent(ui.__mem_func__(self.OnPartyExit))
		self.buttonDict[localeInfo.TARGET_BUTTON_EXCLUDE].SetEvent(ui.__mem_func__(self.OnPartyRemove))

		self.buttonDict[localeInfo.TARGET_BUTTON_INVITE_GUILD].SAFE_SetEvent(self.__OnGuildAddMember)
		self.buttonDict[localeInfo.TARGET_BUTTON_DISMOUNT].SAFE_SetEvent(self.__OnDismount)
		self.buttonDict[localeInfo.TARGET_BUTTON_EXIT_OBSERVER].SAFE_SetEvent(self.__OnExitObserver)
		self.buttonDict[localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT].SAFE_SetEvent(self.__OnViewEquipment)
		self.buttonDict[localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY].SAFE_SetEvent(self.__OnRequestParty)
		self.buttonDict[localeInfo.TARGET_BUTTON_BUILDING_DESTROY].SAFE_SetEvent(self.__OnDestroyBuilding)
		self.buttonDict[localeInfo.TARGET_BUTTON_EMOTION_ALLOW].SAFE_SetEvent(self.__OnEmotionAllow)
		
		if app.ENABLE_12ZI:
			self.buttonDict[localeInfo.TARGET_BUTTON_REVIVE].SAFE_SetEvent(self.__OnReviveQustionDialog)
		
		self.buttonDict["VOTE_BLOCK_CHAT"].SetEvent(ui.__mem_func__(self.__OnVoteBlockChat))

		self.name = name
		self.hpGauge = hpGauge
		self.hpGaugeDMG = hpGaugeDMG
		self.hpGaugePoison = hpGaugePoison
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.infoButton = infoButton
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.vnum = 0
		if app.ENABLE_VIEW_TARGET_MONSTER_HP:
			self.hpDecimal = hpDecimal
			self.hpDecimalPoison = hpDecimalPoison
		self.closeButton = closeButton
		self.nameString = 0
		self.nameLength = 0
		self.vid = 0
		self.eventWhisper = None
		self.isShowButton = False
		if app.ENABLE_VIEW_ELEMENT_SYSTEM:
			self.elementImage = None
		self.actual_hp = 0
		self.max_hp = 100
		self.time = 0
		self.max_hp_dmg = 100
		self.timedmg = 0
		if app.ENABLE_12ZI:
			self.questionDialog = None

		self.__Initialize()
		self.ResetTargetBoard()

	def __del__(self):
		ui.ThinBoard.__del__(self)

		print "===================================================== DESTROYED TARGET BOARD"

	def __Initialize(self):
		self.nameString = ""
		self.nameLength = 0	
		self.vid = 0
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.vnum = 0
		if app.ENABLE_VIEW_ELEMENT_SYSTEM:
			self.elementImage = None
		self.lastHPPercentage=-1
		self.isShowButton = False

	def Destroy(self):
		self.eventWhisper = None
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.infoButton = None
		self.closeButton = None
		self.showingButtonList = None
		self.buttonDict = None
		self.name = None
		if app.ENABLE_ATTENDANCE_EVENT:
			self.hitCount = None
		self.hpGauge = None
		if app.ENABLE_VIEW_TARGET_MONSTER_HP:
			self.hpDecimal = None
			self.hpDecimalPoison = None
		self.hpGaugeDMG = None
		self.hpGaugePoison = None
		self.__Initialize()

	if app.ENABLE_TARGET_INFORMATION_SYSTEM:
		def RefreshMonsterInfoBoard(self):
			if not self.infoButton.showWnd.IsShow():
				return

			self.infoButton.showWnd.Refresh()

		def OnPressedInfoButton(self):
			net.SendTargetInfoLoad(player.GetTargetVID())
			if self.infoButton.showWnd.IsShow():
				self.infoButton.showWnd.Close()
			elif self.vnum != 0:
				self.infoButton.showWnd.Open(self, self.vnum)

	def OnPressedCloseButton(self):
		player.ClearTarget()
		self.Close()

	def Close(self):
		self.__Initialize()
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.infoButton.showWnd.Close()

		self.Hide()

	def Open(self, vid, name):
		if app.ENABLE_ATTENDANCE_EVENT:
			if self.hitCount:
				self.hitCount.Hide()
		if vid:
			if not constInfo.GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD():
				if not player.IsSameEmpire(vid):
					self.Hide()
					return

			if vid != self.GetTargetVID():
				self.ResetTargetBoard()
				self.SetTargetVID(vid)
				self.SetTargetName(name)

			if player.IsMainCharacterIndex(vid):
				self.__ShowMainCharacterMenu()		
			elif chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(self.vid):
				self.Hide()
			else:
				self.RefreshButton()
				self.Show()
		else:
			self.HideAllButton()
			self.__ShowButton(localeInfo.TARGET_BUTTON_WHISPER)
			self.__ShowButton("VOTE_BLOCK_CHAT")
			self.__ArrangeButtonPosition()
			self.SetTargetName(name)
			self.Show()
			
	def Refresh(self):
		if self.IsShow():
			if self.IsShowButton():			
				self.RefreshButton()		

	def RefreshByVID(self, vid):
		if vid == self.GetTargetVID():			
			self.Refresh()
			
	def RefreshByName(self, name):
		if name == self.GetTargetName():
			self.Refresh()

	def __ShowMainCharacterMenu(self):
		canShow=0

		self.HideAllButton()

		if player.IsMountingHorse():
			self.__ShowButton(localeInfo.TARGET_BUTTON_DISMOUNT)
			canShow=1

		if player.IsObserverMode():
			self.__ShowButton(localeInfo.TARGET_BUTTON_EXIT_OBSERVER)
			canShow=1

		if canShow:
			self.__ArrangeButtonPosition()
			self.Show()
		else:
			self.Hide()

	def __ShowNameOnlyMenu(self):
		self.HideAllButton()

	def SetWhisperEvent(self, event):
		self.eventWhisper = event

	def UpdatePosition(self):
		self.SetPosition(wndMgr.GetScreenWidth() / 2 - self.GetWidth()/2, 10)

	def ResetTargetBoard(self):

		for btn in self.buttonDict.values():
			btn.Hide()

		self.__Initialize()

		self.name.SetPosition(0, 13)
		self.name.SetHorizontalAlignCenter()
		self.name.SetWindowHorizontalAlignCenter()
		if app.ENABLE_ATTENDANCE_EVENT:
			if self.hitCount:
				self.hitCount.SetPosition(0, 30)
				self.hitCount.SetHorizontalAlignLeft()
				self.hitCount.SetWindowHorizontalAlignCenter()
		self.hpGauge.Hide()
		self.hpGaugeDMG.Hide()
		self.hpGaugePoison.Hide()
		if app.ENABLE_VIEW_ELEMENT_SYSTEM and self.elementImage:
			self.elementImage = None
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.infoButton.Hide()
			self.infoButton.showWnd.Close()
		if app.ENABLE_VIEW_TARGET_MONSTER_HP:
			self.hpDecimal.Hide()
			self.hpDecimalPoison.Hide()
		self.SetSize(250, 40)
		
	if app.ENABLE_VIEW_ELEMENT_SYSTEM:
		def SetElementImage(self,elementId):
			try:
				if elementId > 0 and elementId in ELEMENT_IMAGE_DIC.keys():
					self.elementImage = ui.ImageBox()
					self.elementImage.SetParent(self)
					self.elementImage.SetPosition(-40, 0)
					self.elementImage.SetWindowHorizontalAlignLeft()
					self.elementImage.LoadImage("d:/ymir work/ui/game/12zi/element/%s.sub" % (ELEMENT_IMAGE_DIC[elementId]))
					self.elementImage.Show()
			except:
				pass
				
	def SetTargetVID(self, vid):
		self.vid = vid
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			self.vnum = 0
			
	def SetEnemyVID(self, vid):
		self.SetTargetVID(vid)

		name = chr.GetNameByVID(vid)
		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			vnum = nonplayer.GetRaceNumByVID(vid)

		level = nonplayer.GetLevelByVID(vid)
		grade = nonplayer.GetGradeByVID(vid)

		nameFront = ""
		if -1 != level:
			nameFront += "Lv." + str(level) + " "
		if self.GRADE_NAME.has_key(grade):
			nameFront += "(" + self.GRADE_NAME[grade] + ") "

		self.SetTargetName(nameFront + name)

		if app.ENABLE_TARGET_INFORMATION_SYSTEM:
			(textWidth, textHeight) = self.name.GetTextSize()

			self.infoButton.SetPosition(textWidth + 25, 12)
			self.infoButton.SetWindowHorizontalAlignLeft()

			self.vnum = vnum
			self.infoButton.Show()
			
		if app.ENABLE_ATTENDANCE_EVENT:
			countHits = player.GetHitCountByVID(vid)
			if countHits:
				self.hitCount.SetText(localeInfo.TARGET_TOOLTIP_ATTACK_COUNT % countHits)
				self.hitCount.Show()
			else:
				self.hitCount.Hide()
			
	def GetTargetVID(self):
		return self.vid

	def GetTargetName(self):
		return self.nameString
	
	def SetTargetName(self, name):
		self.nameString = name
		self.nameLength = len(name)
		self.name.SetText(name)

	if app.ENABLE_VIEW_TARGET_MONSTER_HP:
		def SetHP(self, hpPercentage, iMinHP, iMaxHP):
			if not self.hpGauge.IsShow():
				if app.ENABLE_VIEW_TARGET_MONSTER_HP:
					showingButtonCount = len(self.showingButtonList)
					if showingButtonCount > 0:
						if chr.GetInstanceType(self.vid) == chr.INSTANCE_TYPE_PLAYER:
							self.SetSize(max(150 + 75 * 3, showingButtonCount * 75), self.GetHeight())
						else:
							self.SetSize(200 + 7*self.nameLength, self.GetHeight())
					else:
						self.SetSize(200 + 7*self.nameLength, self.GetHeight())
				else:
					self.SetSize(200 + 7*self.nameLength, self.GetHeight())
				
				self.name.SetPosition(23, 13)
				
				self.name.SetWindowHorizontalAlignLeft()
				self.name.SetHorizontalAlignLeft()
				self.hpGauge.Show()
				self.hpGaugeDMG.Show()
				self.UpdatePosition()
			
			self.actual_hp = hpPercentage
			if self.lastHPPercentage == -1:
				self.lastHPPercentage=hpPercentage
			self.hpGaugeDMG.SetPercentage(self.lastHPPercentage, 100)
			self.lastHPPercentage=hpPercentage
			self.hpGauge.SetPercentage(hpPercentage, 100)
			self.hpGaugePoison.SetPercentage(hpPercentage, 100)
			if app.ENABLE_VIEW_TARGET_MONSTER_HP:
				iMinHPText = '.'.join([i - 3 < 0 and str(iMinHP)[:i] or str(iMinHP)[i-3:i] for i in range(len(str(iMinHP)) % 3, len(str(iMinHP))+1, 3) if i])
				iMaxHPText = '.'.join([i - 3 < 0 and str(iMaxHP)[:i] or str(iMaxHP)[i-3:i] for i in range(len(str(iMaxHP)) % 3, len(str(iMaxHP))+1, 3) if i])
				self.hpDecimal.SetText(str(iMinHPText) + "/" + str(iMaxHPText))
				self.hpDecimalPoison.SetText(str(iMinHPText) + "/" + str(iMaxHPText))
				(textWidth, textHeight)=self.hpDecimal.GetTextSize()
				self.hpDecimal.SetPosition(130 / 2 - textWidth / 2, -13)
				self.hpDecimalPoison.SetPosition(130 / 2 - textWidth / 2, -13)
				self.hpDecimal.Show()
	else:
		def SetHP(self, hpPercentage):
			if not self.hpGauge.IsShow():
				if app.ENABLE_VIEW_TARGET_MONSTER_HP:
					showingButtonCount = len(self.showingButtonList)
					if showingButtonCount > 0:
						if chr.GetInstanceType(self.GetTargetVID) != chr.INSTANCE_TYPE_PLAYER:
							if showingButtonCount != 1:
								self.SetSize(max(150, showingButtonCount * 75), self.GetHeight())
							else:
								self.SetSize(max(150, 2 * 75), self.GetHeight())
						else:
							self.SetSize(200 + 7*self.nameLength, self.GetHeight())
					else:
						self.SetSize(200 + 7*self.nameLength, self.GetHeight())
				else:
					self.SetSize(200 + 7*self.nameLength, self.GetHeight())

				self.name.SetPosition(23, 13)

				self.name.SetWindowHorizontalAlignLeft()
				self.name.SetHorizontalAlignLeft()
				self.hpGauge.Show()
				self.UpdatePosition()

	def ShowDefaultButton(self):

		self.isShowButton = True
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_WHISPER])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_FIGHT])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_EMOTION_ALLOW])
		for button in self.showingButtonList:
			button.Show()

	def HideAllButton(self):
		self.isShowButton = False
		for button in self.showingButtonList:
			button.Hide()
		self.showingButtonList = []

	def __ShowButton(self, name):

		if not self.buttonDict.has_key(name):
			return

		self.buttonDict[name].Show()
		self.showingButtonList.append(self.buttonDict[name])

	def __HideButton(self, name):

		if not self.buttonDict.has_key(name):
			return

		button = self.buttonDict[name]
		button.Hide()

		for btnInList in self.showingButtonList:
			if btnInList == button:
				self.showingButtonList.remove(button)
				break

	def OnWhisper(self):
		if None != self.eventWhisper:
			self.eventWhisper(self.nameString)

	def OnExchange(self):
		net.SendExchangeStartPacket(self.vid)

	def OnPVP(self):
		net.SendChatPacket("/pvp %d" % (self.vid))

	def OnAppendToMessenger(self):
		net.SendMessengerAddByVIDPacket(self.vid)
		
	if app.ENABLE_MESSENGER_BLOCK:
		def OnAppendToBlockMessenger(self):
			net.SendMessengerAddBlockByVIDPacket(self.vid)
			
		def OnRemoveToBlockMessenger(self):
			messenger.RemoveBlock(constInfo.ME_KEY)
			net.SendMessengerRemoveBlockPacket(constInfo.ME_KEY, chr.GetNameByVID(self.vid))

	def OnPartyInvite(self):
		net.SendPartyInvitePacket(self.vid)

	def OnPartyExit(self):
		net.SendPartyExitPacket()

	def OnPartyRemove(self):
		net.SendPartyRemovePacket(player.PartyMemberVIDToPID(self.vid))

	def __OnGuildAddMember(self):
		net.SendGuildAddMemberPacket(self.vid)

	def __OnDismount(self):
		net.SendChatPacket("/unmount")

	def __OnExitObserver(self):
		net.SendChatPacket("/observer_exit")

	def __OnViewEquipment(self):
		net.SendChatPacket("/view_equip " + str(self.vid))

	def __OnRequestParty(self):
		net.SendChatPacket("/party_request " + str(self.vid))

	def __OnDestroyBuilding(self):
		net.SendChatPacket("/build d %d" % (self.vid))

	def __OnEmotionAllow(self):
		net.SendChatPacket("/emotion_allow %d" % (self.vid))
		
	def __OnVoteBlockChat(self):
		cmd = "/vote_block_chat %s" % (self.nameString)
		net.SendChatPacket(cmd)

	def OnPressEscapeKey(self):
		self.OnPressedCloseButton()
		return True

	def IsShowButton(self):
		return self.isShowButton

	def RefreshButton(self):

		self.HideAllButton()

		if app.ENABLE_12ZI:
			if chrmgr.IsPC(self.vid) and chrmgr.IsDead(self.vid):
				if str(background.GetCurrentMapName()) in "metin2_12zi_stage":
					self.__ShowButton(localeInfo.TARGET_BUTTON_REVIVE)
					self.__ArrangeButtonPosition()
				return

		if chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(self.vid):
			#self.__ShowButton(localeInfo.TARGET_BUTTON_BUILDING_DESTROY)
			#self.__ArrangeButtonPosition()
			return
		
		if player.IsPVPInstance(self.vid) or player.IsObserverMode():
			# PVP_INFO_SIZE_BUG_FIX
			self.SetSize(200 + 7*self.nameLength, 40)
			self.UpdatePosition()
			# END_OF_PVP_INFO_SIZE_BUG_FIX			
			return	

		self.ShowDefaultButton()

		if guild.MainPlayerHasAuthority(guild.AUTH_ADD_MEMBER):
			if not guild.IsMemberByName(self.nameString):
				if 0 == chr.GetGuildID(self.vid):
					self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_GUILD)

		if not messenger.IsFriendByName(self.nameString):
			self.__ShowButton(localeInfo.TARGET_BUTTON_FRIEND)
			
		if app.ENABLE_MESSENGER_BLOCK and not str(self.nameString)[0] == "[":
			if not messenger.IsBlockByName(self.nameString):
				self.__ShowButton(localeInfo.TARGET_BUTTON_BLOCK)
				self.__HideButton(localeInfo.TARGET_BUTTON_UNBLOCK)
			else:
				self.__ShowButton(localeInfo.TARGET_BUTTON_UNBLOCK)
				self.__HideButton(localeInfo.TARGET_BUTTON_BLOCK)
				
		if player.IsPartyMember(self.vid):

			self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)

			if player.IsPartyLeader(self.vid):
				self.__ShowButton(localeInfo.TARGET_BUTTON_LEAVE_PARTY)
			elif player.IsPartyLeader(player.GetMainCharacterIndex()):
				self.__ShowButton(localeInfo.TARGET_BUTTON_EXCLUDE)

		else:
			if player.IsPartyMember(player.GetMainCharacterIndex()):
				if player.IsPartyLeader(player.GetMainCharacterIndex()):
					self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)
			else:
				if chr.IsPartyMember(self.vid):
					self.__ShowButton(localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY)
				else:
					self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)

			if player.IsRevengeInstance(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)
				self.__ShowButton(localeInfo.TARGET_BUTTON_AVENGE)
			elif player.IsChallengeInstance(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)
				self.__ShowButton(localeInfo.TARGET_BUTTON_ACCEPT_FIGHT)
			elif player.IsCantFightInstance(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)

			if not player.IsSameEmpire(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)
				self.__HideButton(localeInfo.TARGET_BUTTON_FRIEND)
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)

		distance = player.GetCharacterDistance(self.vid)
		if distance > self.EXCHANGE_LIMIT_RANGE:
			self.__HideButton(localeInfo.TARGET_BUTTON_EXCHANGE)
			self.__ArrangeButtonPosition()

		self.__ArrangeButtonPosition()

	def __ArrangeButtonPosition(self):
		showingButtonCount = len(self.showingButtonList)
		pos = -(showingButtonCount / 2) * 68
		if 0 == showingButtonCount % 2:
			pos += 34

		for button in self.showingButtonList:
			button.SetPosition(pos, 33)
			pos += 68
		
		if app.ENABLE_VIEW_TARGET_MONSTER_HP:
			if showingButtonCount <= 2:
				self.SetSize(max(150 + 125, showingButtonCount * 75), 65)
			else:
				self.SetSize(max(150, showingButtonCount * 75), 65)
		else:
			self.SetSize(max(150, showingButtonCount * 75), 65)
		
		self.UpdatePosition()

	if app.ENABLE_ATTENDANCE_EVENT:
		def RefreshHitCount(self, vid):
			countHits = player.GetHitCountByVID(vid)
			if countHits:
				self.hitCount.SetText(localeInfo.TARGET_TOOLTIP_ATTACK_COUNT % countHits)
				self.hitCount.Show()
			else:
				self.hitCount.Hide()
				
	if app.ENABLE_12ZI:
		def __OnReviveQustionDialog(self):
			net.SendChatPacket("/revivedialog %d" % (self.vid))

		def OpenReviveDialog(self, vid, itemcount):
			self.questionDialog = uiCommon.QuestionDialog()
			if player.IsMainCharacterIndex(vid):
				self.questionDialog.SetText(localeInfo.REVIVE_SELF_QUESTION % (itemcount))
			else:
				self.questionDialog.SetText(localeInfo.REVIVE_QUESTION % (chr.GetNameByVID(vid), itemcount))

			self.questionDialog.SetAcceptEvent(lambda arg=vid: self.OnRivive(arg))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnQuestionDialogClose))
			self.questionDialog.Open()

		def OnRivive(self, virId):
			net.SendChatPacket("/revive %d" % (virId))
			self.OnQuestionDialogClose()

		def OnQuestionDialogClose(self):
			self.questionDialog.Close()
			self.questionDialog = None
			return True

	def OnUpdate(self):
		if not chrmgr.IsPC(self.vid) and not chr.IsStone(self.vid):
			if self.max_hp > self.actual_hp:
				if app.GetTime() > self.time:
					self.max_hp -= 1
					self.hpGauge.SetPercentage(self.max_hp, 100)
					self.hpGaugePoison.SetPercentage(self.max_hp, 100)
					self.time = app.GetTime() + 0.005
			if self.max_hp_dmg > self.actual_hp:
				if app.GetTime() > self.timedmg:
					self.max_hp_dmg = -1
					self.hpGaugeDMG.SetPercentage(self.max_hp_dmg, 100)
					self.timedmg = app.GetTime() + 0.005
			if app.ENABLE_POISON_GAUGE_SYSTEM:
				if chr.IsPoisoned(self.vid):
					self.hpGauge.Hide()
					self.hpGaugePoison.Show()
					self.hpDecimal.Hide()
					self.hpDecimalPoison.Show()
				else:
					self.hpGaugePoison.Hide()
					self.hpGauge.Show()
					self.hpDecimalPoison.Hide()
					self.hpDecimal.Show()

		if self.isShowButton:
			exchangeButton = self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE]
			distance = player.GetCharacterDistance(self.vid)

			if distance < 0:
				return

			if exchangeButton.IsShow():
				if distance > self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()

			else:
				if distance < self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()

			if exchangeButton.IsShow():
				if distance > self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()

			else:
				if distance < self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()
					
			if guild.MainPlayerHasAuthority(guild.AUTH_ADD_MEMBER):
				if not guild.IsMemberByName(self.nameString):
					if 0 == chr.GetGuildID(self.vid):
						self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_GUILD)
		if self.isShowButton:
			exchangeButton = self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE]
			distance = player.GetCharacterDistance(self.vid)
			if distance < 0:
				return
