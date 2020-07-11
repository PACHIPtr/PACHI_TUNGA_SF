import app
import net
import chr
import event
import systemSetting

import ui
import grp
import snd
import math
import wndMgr
import uiToolTip
import constInfo
import musicInfo
import localeInfo
import networkModule
import uiScriptLocale
import playersettingmodule

MAN = 0
WOMAN = 1
SHAPE0 = 0
SHAPE1 = 1
PAGE_COUNT = 2
SLOT_COUNT = 5
BASE_CHR_ID	= 3
LOCALE_PATH = "uiscript/"+uiScriptLocale.CODEPAGE+"_"

class CreateCharacterWindow(ui.Window):
	CREATE_STAT_POINT = 0
	
	STAT_CON = 0
	STAT_INT = 1
	STAT_STR = 2
	STAT_DEX = 3
	START_STAT = (
					[4, 3, 6, 3,],
					[3, 3, 4, 6,],
					[3, 5, 5, 3,],
					[4, 6, 3, 3,],
					[6, 2, 6, 2,],
				)
	
	EMPIRE_NAME = {
		net.EMPIRE_A : localeInfo.EMPIRE_A,
		net.EMPIRE_B : localeInfo.EMPIRE_B,
		net.EMPIRE_C : localeInfo.EMPIRE_C
	}
	
	STAT_DESCRIPTION = {
							STAT_CON : localeInfo.STAT_TOOLTIP_CON,
							STAT_INT : localeInfo.STAT_TOOLTIP_INT,
							STAT_STR : localeInfo.STAT_TOOLTIP_STR,
							STAT_DEX : localeInfo.STAT_TOOLTIP_DEX,
						}
	
	DESCRIPTION_FILE_NAME =	(
								uiScriptLocale.JOBDESC_WARRIOR_PATH,
								uiScriptLocale.JOBDESC_ASSASSIN_PATH,
								uiScriptLocale.JOBDESC_SURA_PATH,
								uiScriptLocale.JOBDESC_SHAMAN_PATH,
								uiScriptLocale.JOBDESC_WOLFMAN_PATH,
							)
	
	RACE_DECT = {
					0 : playersettingmodule.RACE_WARRIOR_M,
					1 : playersettingmodule.RACE_ASSASSIN_M,
					2 : playersettingmodule.RACE_SURA_M,
					3 : playersettingmodule.RACE_SHAMAN_M,
					4 : playersettingmodule.RACE_WOLFMAN_M,
					5 : playersettingmodule.RACE_WARRIOR_W,
					6 : playersettingmodule.RACE_ASSASSIN_W,
					7 : playersettingmodule.RACE_SURA_W,
					8 : playersettingmodule.RACE_SHAMAN_W,
	}
	
	FACE_IMAGE_DICT = {
						0 : "icon/face/warrior_m.tga",
						1 : "icon/face/assassin_m.tga",
						2 : "icon/face/sura_m.tga",
						3 : "icon/face/shaman_m.tga",
						4 : "icon/face/wolfman_m.tga",
						5 : "icon/face/warrior_w.tga",
						6 : "icon/face/assassin_w.tga",
						7 : "icon/face/sura_w.tga",
						8 : "icon/face/shaman_w.tga",
						9 : "icon/face/wolfman_m.tga",
					}
	
	RACE_NAME = {
					0 : localeInfo.JOB_WARRIOR,
					1 : localeInfo.JOB_ASSASSIN,
					2 : localeInfo.JOB_SURA,
					3 : localeInfo.JOB_SHAMAN,
					4 : localeInfo.JOB_WOLFMAN,
					5 : localeInfo.JOB_WARRIOR,
					6 : localeInfo.JOB_ASSASSIN,
					7 : localeInfo.JOB_SURA,
					8 : localeInfo.JOB_SHAMAN,
					9 : localeInfo.JOB_WOLFMAN,
	}

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

	class CharacterRenderer(ui.Window):
		def OnRender(self):
			grp.ClearDepthBuffer()
			grp.SetGameRenderState()
			grp.PushState()
			grp.SetOmniLight()
			
			screenWidth = wndMgr.GetScreenWidth()
			screenHeight = wndMgr.GetScreenHeight()
			newScreenWidth = float(screenWidth - 270)
			newScreenHeight = float(screenHeight)
			grp.SetViewport(270.0/screenWidth, 0.0, newScreenWidth/screenWidth, newScreenHeight/screenHeight)
			
			app.SetCenterPosition(0.0, 0.0, 0.0)
			app.SetCamera(1550.0, 15.0, 180.0, 95.0)
			grp.SetPerspective(10.0, newScreenWidth/newScreenHeight, 1000.0, 3000.0)
			
			(x, y) = app.GetCursorPosition()
			grp.SetCursorPosition(x, y)
			
			chr.Deform()
			chr.Render()
			
			grp.RestoreViewport()
			grp.PopState()
			grp.SetInterfaceRenderState()

	def __init__(self, stream):
		print "NEW CREATE WINDOW ----------------------------------------------------------------------------"
		
		ui.Window.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_CREATE, self)
		self.stream=stream

	def __del__(self):
		print "---------------------------------------------------------------------------- DELETE CREATE WINDOW"
		
		net.SetPhaseWindow(net.PHASE_WINDOW_CREATE, 0)
		ui.Window.__del__(self)

	def Open(self):
		print "OPEN CREATE WINDOW ----------------------------------------------------------------------------"
		
		playersettingmodule.LoadGameData("INIT")
		self.reservingRaceIndex = -1
		self.reservingShapeIndex = -1
		self.reservingStartTime = 0
		self.stat = [0, 0, 0, 0]
		self.slot = 0
		self.gender = 0
		self.shapeList = [[0, 0, 0, 0, 0],[0, 0, 0, 0, 0]]
		self.descIndex = 0
		try:
			dlgBoard = ui.ScriptWindow()
			pythonScriptLoader = ui.PythonScriptLoader()
			pythonScriptLoader.LoadScriptFile(dlgBoard, "UIScript/createcharacterwindow.py")
		except:
			import exception
			exception.Abort("CreateCharacterWindow.Open.LoadObject")
		
		try:
			getChild = dlgBoard.GetChild
			self.backGroundImg1 = getChild("BackGround")
			self.backGroundImg2 = getChild("BackGround2")
			self.backGroundImg2.Hide()
			self.backGroundImg3 = getChild("BackGround3")
			self.backGroundImg3.Hide()
			
			self.empireName = getChild("EmpireName")
			self.EmpireFlagA = getChild("EmpireFlag_A")
			self.EmpireFlagB = getChild("EmpireFlag_B")
			self.EmpireFlagC = getChild("EmpireFlag_C")
			
			self.CharacterSlot_0 = getChild("WARRIOR")
			self.CharacterFace_0 = getChild("CharacterFace_0")
			self.CharacterSlot_1 = getChild("ASSASSIN")
			self.CharacterFace_1 = getChild("CharacterFace_1")
			self.CharacterSlot_2 = getChild("SURA")
			self.CharacterFace_2 = getChild("CharacterFace_2")
			self.CharacterSlot_3 = getChild("SHAMAN")
			self.CharacterFace_3 = getChild("CharacterFace_3")
			self.CharacterSlot_4 = getChild("WOLFMAN")
			self.CharacterFace_4 = getChild("CharacterFace_4")
			self.DiscFace = getChild("DiscFace")
			self.raceName = getChild("raceName_Text")
			
			self.NameList = []
			self.NameList.append(getChild("name_warrior"))
			self.NameList.append(getChild("name_assassin"))
			self.NameList.append(getChild("name_sura"))
			self.NameList.append(getChild("name_shaman"))
			self.NameList.append(getChild("name_wolfman"))
			
			self.GaugeList = []
			self.GaugeList.append(getChild("hth_gauge"))
			self.GaugeList.append(getChild("int_gauge"))
			self.GaugeList.append(getChild("str_gauge"))
			self.GaugeList.append(getChild("dex_gauge"))
			
			self.btnCreate = getChild("create_button")
			self.btnCancel = getChild("cancel_button")
			self.btnPrev = getChild("prev_button")
			self.btnNext = getChild("next_button")
			self.textBoard = getChild("text_board")
			
			self.genderButtonList = []
			self.genderButtonList.append(getChild("gender_button_01"))
			self.genderButtonList.append(getChild("gender_button_02"))
			
			self.shapeButtonList = []
			self.shapeButtonList.append(getChild("shape_button_01"))
			self.shapeButtonList.append(getChild("shape_button_02"))
			
			self.editCharacterName = getChild("character_name_value")
			
			self.statValue = []
			self.statValue.append(getChild("hth_value"))
			self.statValue.append(getChild("int_value"))
			self.statValue.append(getChild("str_value"))
			self.statValue.append(getChild("dex_value"))
			
			getChild("hth_slot").ShowToolTip = lambda arg=self.STAT_CON: self.OverInStatButton(arg)
			getChild("hth_slot").HideToolTip = lambda arg=self.STAT_CON: self.OverOutStatButton()
			getChild("int_slot").ShowToolTip = lambda arg=self.STAT_INT: self.OverInStatButton(arg)
			getChild("int_slot").HideToolTip = lambda arg=self.STAT_INT: self.OverOutStatButton()
			getChild("str_slot").ShowToolTip = lambda arg=self.STAT_STR: self.OverInStatButton(arg)
			getChild("str_slot").HideToolTip = lambda arg=self.STAT_STR: self.OverOutStatButton()
			getChild("dex_slot").ShowToolTip = lambda arg=self.STAT_DEX: self.OverInStatButton(arg)
			getChild("dex_slot").HideToolTip = lambda arg=self.STAT_DEX: self.OverOutStatButton()
			getChild("hth_slot").Hide()
			getChild("int_slot").Hide()
			getChild("str_slot").Hide()
			getChild("dex_slot").Hide()
			
			self.genderButtonList[0].ShowToolTip = lambda arg=1: self.OverInButton(arg)
			self.genderButtonList[0].HideToolTip = lambda arg=1: self.OverOutButton()
			self.genderButtonList[1].ShowToolTip = lambda arg=2: self.OverInButton(arg)
			self.genderButtonList[1].HideToolTip = lambda arg=2: self.OverOutButton()
			self.shapeButtonList[0].ShowToolTip = lambda arg=3: self.OverInButton(arg)
			self.shapeButtonList[0].HideToolTip = lambda arg=3: self.OverOutButton()
			self.shapeButtonList[1].ShowToolTip = lambda arg=4: self.OverInButton(arg)
			self.shapeButtonList[1].HideToolTip = lambda arg=4: self.OverOutButton()
			getChild("prev_button").ShowToolTip = lambda arg=5: self.OverInButton(arg)
			getChild("prev_button").HideToolTip = lambda arg=5: self.OverOutButton()
			getChild("next_button").ShowToolTip = lambda arg=6: self.OverInButton(arg)
			getChild("next_button").HideToolTip = lambda arg=6: self.OverOutButton()
			getChild("create_button").ShowToolTip = lambda arg=7: self.OverInButton(arg)
			getChild("create_button").HideToolTip = lambda arg=7: self.OverOutButton()
			getChild("cancel_button").ShowToolTip = lambda arg=8: self.OverInButton(arg)
			getChild("cancel_button").HideToolTip = lambda arg=8: self.OverOutButton()
			getChild("WARRIOR").ShowToolTip = lambda arg=9: self.OverInButton(arg)
			getChild("WARRIOR").HideToolTip = lambda arg=9: self.OverOutButton()
			getChild("ASSASSIN").ShowToolTip = lambda arg=10: self.OverInButton(arg)
			getChild("ASSASSIN").HideToolTip = lambda arg=10: self.OverOutButton()
			getChild("SURA").ShowToolTip = lambda arg=11: self.OverInButton(arg)
			getChild("SURA").HideToolTip = lambda arg=11: self.OverOutButton()
			getChild("SHAMAN").ShowToolTip = lambda arg=12: self.OverInButton(arg)
			getChild("SHAMAN").HideToolTip = lambda arg=12: self.OverOutButton()
			getChild("WOLFMAN").ShowToolTip = lambda arg=13: self.OverInButton(arg)
			getChild("WOLFMAN").HideToolTip = lambda arg=13: self.OverOutButton()
		except:
			import exception
			exception.Abort("CreateCharacterWindow.Open.BindObject")
		
		self.CharacterSlot_0.SAFE_SetEvent(self.__SelectSlot, 0)
		self.CharacterSlot_1.SAFE_SetEvent(self.__SelectSlot, 1)
		self.CharacterSlot_2.SAFE_SetEvent(self.__SelectSlot, 2)
		self.CharacterSlot_3.SAFE_SetEvent(self.__SelectSlot, 3)
		self.CharacterSlot_4.SAFE_SetEvent(self.__SelectSlot, 4)
		self.CharacterSlot_4.Hide()
		if constInfo.WOLF_MAN != "DISABLED" and constInfo.WOLF_MAN == "ENABLED":
			self.CharacterSlot_4.Show()
		elif constInfo.WOLF_WOMEN != "DISABLED" and constInfo.WOLF_WOMEN == "ENABLED":
			self.CharacterSlot_4.Show()
		
		self.btnCreate.SetEvent(ui.__mem_func__(self.CreateCharacter))
		self.btnCancel.SetEvent(ui.__mem_func__(self.CancelCreate))
		self.btnPrev.SetEvent(ui.__mem_func__(self.PrevDescriptionPage))
		self.btnNext.SetEvent(ui.__mem_func__(self.NextDescriptionPage))
		
		self.genderButtonList[0].SetEvent(ui.__mem_func__(self.__SelectGender), MAN)
		self.genderButtonList[1].SetEvent(ui.__mem_func__(self.__SelectGender), WOMAN)
		
		self.shapeButtonList[0].SetEvent(ui.__mem_func__(self.__SelectShape), SHAPE0)
		self.shapeButtonList[1].SetEvent(ui.__mem_func__(self.__SelectShape), SHAPE1)
		self.editCharacterName.SetReturnEvent(ui.__mem_func__(self.CreateCharacter))
		self.editCharacterName.SetEscapeEvent(ui.__mem_func__(self.CancelCreate))
		self.dlgBoard = dlgBoard
		
		self.curNameAlpha = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
		self.destNameAlpha = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
		self.curGauge = [0.0, 0.0, 0.0, 0.0]
		self.destGauge = [0.0, 0.0, 0.0, 0.0]
		
		self.descriptionBox = self.DescriptionBox()
		self.descriptionBox.Show()
		
		self.toolTip = uiToolTip.ToolTip()
		self.toolTip.ClearToolTip()
		
		self.editCharacterName.SetText("")
		
		self.EnableWindow()
		
		app.SetCamera(500.0, 10.0, 180.0, 95.0)
		
		self.__SelectSlot(0)
		
		self.dlgBoard.Show()
		self.Show()
		
		if musicInfo.createMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/"+musicInfo.createMusic)
		
		self.SetEmpire(net.GetEmpireID())
		app.ShowCursor()

	def Close(self):
		print "---------------------------------------------------------------------------- CLOSE CREATE WINDOW"
		
		self.editCharacterName.Enable()
		self.dlgBoard.ClearDictionary()
		self.stream = 0
		self.shapeButtonList = []
		self.genderButtonList = []
		self.btnCreate = 0
		self.btnCancel = 0
		self.btnPrev = 0
		self.btnNext = 0
		self.textBoard = 0
		self.editCharacterName = 0
		self.backGroundImg1 = None
		self.backGroundImg2 = None
		self.backGroundImg3 = None
		self.EmpireFlagA = None
		self.EmpireFlagB = None
		self.EmpireFlagC = None
		if musicInfo.createMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.createMusic)
		
		for id in xrange(BASE_CHR_ID + SLOT_COUNT * PAGE_COUNT):
			chr.DeleteInstance(id)
		
		self.dlgBoard.Hide()
		self.Hide()
		self.descriptionBox.Hide()
		
		app.HideCursor()
		event.Destroy()

	def SetEmpire(self, id):
		self.empireName.SetText(self.EMPIRE_NAME.get(id, ""))
		if id == 1:
			self.empireName.SetFontColor(1.0, 0, 0)
			self.EmpireFlagA.Show()
			self.EmpireFlagB.Hide()
			self.EmpireFlagC.Hide()
			self.BackGround = self.backGroundImg1
			self.backGroundImg1.Show()
			self.backGroundImg2.Hide()
			self.backGroundImg3.Hide()
			
			self.chrRenderer = self.CharacterRenderer()
			self.chrRenderer.SetParent(self.backGroundImg1)
			self.chrRenderer.Show()
		elif id == 2:
			self.empireName.SetFontColor(1.0, 1.0, 0.0)
			self.EmpireFlagA.Hide()
			self.EmpireFlagB.Show()
			self.EmpireFlagC.Hide()
			self.BackGround = self.backGroundImg2
			self.backGroundImg1.Hide()
			self.backGroundImg2.Show()
			self.backGroundImg3.Hide()
			
			self.chrRenderer = self.CharacterRenderer()
			self.chrRenderer.SetParent(self.backGroundImg2)
			self.chrRenderer.Show()
		elif id == 3:
			self.empireName.SetFontColor(0.0, 0, 1.0)
			self.EmpireFlagA.Hide()
			self.EmpireFlagB.Hide()
			self.EmpireFlagC.Show()
			self.BackGround = self.backGroundImg3
			self.backGroundImg1.Hide()
			self.backGroundImg2.Hide()
			self.backGroundImg3.Show()
			
			self.chrRenderer = self.CharacterRenderer()
			self.chrRenderer.SetParent(self.backGroundImg3)
			self.chrRenderer.Show()

	def EnableWindow(self):
		self.reservingRaceIndex = -1
		self.reservingShapeIndex = -1
		self.btnCreate.Enable()
		self.btnCancel.Enable()
		self.btnPrev.Enable()
		self.btnNext.Enable()
		self.editCharacterName.SetFocus()
		self.editCharacterName.Enable()
		chr_id = self.__GetSlotChrID(self.gender, self.slot)
		chr.SelectInstance(chr_id)
		chr.BlendLoopMotion(chr.MOTION_INTRO_WAIT, 0.1)

	def DisableWindow(self):
		self.btnCreate.Disable()
		self.btnCancel.Disable()
		self.btnPrev.Disable()
		self.btnNext.Disable()
		self.editCharacterName.Disable()
		self.btnCreate.SetUp()

	def __GetSlotChrID(self, gender, slot):
		return self.RACE_DECT[gender + slot]

	def __MakeCharacter(self, race):
		chr_id = self.__GetSlotChrID(self.gender, self.slot)
		chr.CreateInstance(chr_id)
		chr.SelectInstance(chr_id)
		chr.SetVirtualID(chr_id)
		chr.SetRace(chr_id)
		chr.SetArmor(0)
		chr.SetHair(0)
		chr.Refresh()
		chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)
		chr.SetRotation(0.0)
		
		distance = 50.0
		rotRadian = 82.0 * (math.pi*2) / 360.0
		x = distance*math.sin(rotRadian) + distance*math.cos(rotRadian)
		y = distance*math.cos(rotRadian) - distance*math.sin(rotRadian)
		chr.SetPixelPosition(int(x), int(y), 30)
		chr.Hide()

	def __SelectGender(self, gender):
		slot = self.slot
		if constInfo.WOLF_WOMEN != "ENABLED" and constInfo.WOLF_WOMEN == "DISABLED" and slot == 4 and gender == 1:
			for button in self.genderButtonList:
				button.Down()
			
			self.genderButtonList[gender].SetUp()
			return
		elif constInfo.WOLF_MAN != "ENABLED" and constInfo.WOLF_MAN == "DISABLED" and slot == 4 and gender == 0:
			for button in self.genderButtonList:
				button.Down()
			
			self.genderButtonList[gender].SetUp()
			return
		
		for button in self.genderButtonList:
			button.SetUp()
		
		self.genderButtonList[gender].Down()
		self.gender = gender
		if gender == 0 and self.slot > 4:
			slot = self.slot - 4
		elif gender == 1 and self.slot < 4:
			slot = self.slot + 4
			if gender == 1 and self.slot > 8:
				slot = self.slot - 4
		
		for i in xrange(9):
			chr.DeleteInstance(i)
		
		chr_id = self.__GetSlotChrID(self.gender, slot)
		chr.CreateInstance(chr_id)
		chr.SelectInstance(chr_id)
		chr.SetVirtualID(chr_id)
		chr.SetRace(chr_id)
		chr.SetArmor(0)
		chr.SetHair(0)
		chr.Refresh()
		chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)
		chr.SetRotation(0.0)
		
		distance = 50.0
		rotRadian = 82.0 * (math.pi*2) / 360.0
		x = distance*math.sin(rotRadian) + distance*math.cos(rotRadian)
		y = distance*math.cos(rotRadian) - distance*math.sin(rotRadian)
		chr.SetPixelPosition(int(x), int(y), 30)
		chr.Show()

	def __SelectShape(self, shape):
		self.shapeList[self.gender][self.slot] = shape
		for button in self.shapeButtonList:
			button.SetUp()
		
		self.shapeButtonList[shape].Down()
		chr_id = self.__GetSlotChrID(self.gender, self.slot)
		chr.SelectInstance(chr_id)
		chr.ChangeShape(shape)
		chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)

	def GetSlotIndex(self):
		return self.slot

	def RefreshStat(self):
		statSummary = self.stat[0] + self.stat[1] + self.stat[2] + self.stat[3]
		self.destGauge = (
							float(self.stat[0])/float(statSummary),
							float(self.stat[1])/float(statSummary),
							float(self.stat[2])/float(statSummary),
							float(self.stat[3])/float(statSummary),
						)
		
		for i in xrange(4):
			self.statValue[i].SetText(str(self.stat[i]))

	def __SelectSlot(self, slot):
		self.slot = slot
		self.ResetStat()
		
		for i in xrange(9):
			chr.DeleteInstance(i)
		
		self.genderButtonList[0].Show()
		self.genderButtonList[1].Show()
		self.DiscFace.LoadImage(self.FACE_IMAGE_DICT[slot])
		self.DiscFace.Show()
		self.raceName.SetText(self.RACE_NAME[slot])
		self.raceName.Show()
		if slot == 0:
			self.CharacterSlot_0.Down()
			self.CharacterSlot_1.SetUp()
			self.CharacterSlot_2.SetUp()
			self.CharacterSlot_3.SetUp()
			self.CharacterSlot_4.SetUp()
		elif slot == 1:
			self.CharacterSlot_1.Down()
			self.CharacterSlot_0.SetUp()
			self.CharacterSlot_2.SetUp()
			self.CharacterSlot_3.SetUp()
			self.CharacterSlot_4.SetUp()
		elif slot == 2:
			self.CharacterSlot_2.Down()
			self.CharacterSlot_0.SetUp()
			self.CharacterSlot_1.SetUp()
			self.CharacterSlot_3.SetUp()
			self.CharacterSlot_4.SetUp()
		elif slot == 3:
			self.CharacterSlot_3.Down()
			self.CharacterSlot_0.SetUp()
			self.CharacterSlot_1.SetUp()
			self.CharacterSlot_2.SetUp()
			self.CharacterSlot_4.SetUp()
		elif slot == 4:
			self.CharacterSlot_4.Down()
			self.CharacterSlot_0.SetUp()
			self.CharacterSlot_1.SetUp()
			self.CharacterSlot_2.SetUp()
			self.CharacterSlot_3.SetUp()
			if constInfo.WOLF_MAN != "DISABLED" and constInfo.WOLF_MAN == "ENABLED":
				self.genderButtonList[0].Show()
			else:
				self.genderButtonList[0].Hide()
			
			if constInfo.WOLF_WOMEN != "DISABLED" and constInfo.WOLF_WOMEN == "ENABLED":
				self.genderButtonList[1].Show()
			else:
				self.genderButtonList[1].Hide()
		
		self.__MakeCharacter(slot)
		self.__SelectGender(0)
		self.__SelectShape(0)
		
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet(self.DESCRIPTION_FILE_NAME[self.slot])

		chr_id = self.__GetSlotChrID(self.gender, slot)
		if chr.HasInstance(chr_id):
			chr.SelectInstance(chr_id)
			self.__SelectShape(self.shapeList[self.gender][slot])

	def CreateCharacter(self):
		if -1 != self.reservingRaceIndex:
			return
		
		textName = self.editCharacterName.GetText()
		if self.__CheckCreateCharacter(textName) == False:
			return
		
		if musicInfo.selectMusic != "":
			snd.FadeLimitOutMusic("BGM/"+musicInfo.selectMusic, systemSetting.GetMusicVolume()*0.05)
		
		self.DisableWindow()
		chr_id = self.__GetSlotChrID(self.gender, self.slot)
		chr.SelectInstance(chr_id)
		self.reservingRaceIndex = chr.GetRace()
		
		print("========================================================")
		print("Race : %d" % (self.reservingRaceIndex))
		print("========================================================")
		
		self.reservingShapeIndex = self.shapeList[self.gender][self.slot]
		self.reservingStartTime = app.GetTime()
		for eachSlot in xrange(SLOT_COUNT):
			sel_id = self.__GetSlotChrID(self.gender, eachSlot)
			chr.SelectInstance(sel_id)
			if eachSlot == self.slot:
				chr.PushOnceMotion(chr.MOTION_INTRO_SELECTED)
			else:
				chr.PushOnceMotion(chr.MOTION_INTRO_NOT_SELECTED)
		self.toolTip.Hide()

	def CancelCreate(self):
		self.stream.SetSelectCharacterPhase()

	def PrevDescriptionPage(self):
		if event.IsWait(self.descIndex) == True:
			if event.GetVisibleStartLine(self.descIndex)-14 >= 0:
				event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex)-14)
				event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)

	def NextDescriptionPage(self):
		if True == event.IsWait(self.descIndex):
			event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex)+14)
			event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)

	def __CheckCreateCharacter(self, name):
		if len(name) == 0:
			self.PopupMessage(localeInfo.CREATE_INPUT_NAME, self.EnableWindow)
			return False
		elif name.find(localeInfo.CREATE_GM_NAME)!=-1:
			self.PopupMessage(localeInfo.CREATE_ERROR_GM_NAME, self.EnableWindow)
			return False
		elif net.IsInsultIn(name):
			self.PopupMessage(localeInfo.CREATE_ERROR_INSULT_NAME, self.EnableWindow)
			return False
		
		return True

	def ResetStat(self):
		for i in xrange(4):
			self.stat[i] = self.START_STAT[self.slot][i]
		
		self.lastStatPoint = self.CREATE_STAT_POINT
		self.RefreshStat()

	def OnCreateSuccess(self):
		self.stream.SetSelectCharacterPhase()

	def OnCreateFailure(self, type):
		if type == 1:
			self.PopupMessage(localeInfo.CREATE_EXIST_SAME_NAME, self.EnableWindow)
		else:
			self.PopupMessage(localeInfo.CREATE_FAILURE, self.EnableWindow)

	def OnUpdate(self):
		chr.Update()
		(xposEventSet, yposEventSet) = self.textBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet+7, -(yposEventSet+7))
		self.descriptionBox.SetIndex(self.descIndex)
		SLOT_COUNT_TWO = 4
		if constInfo.WOLF_WOMEN != "DISABLED" and constInfo.WOLF_WOMEN == "ENABLED":
			SLOT_COUNT_TWO += 1
		else:
			if constInfo.WOLF_MAN != "DISABLED" and constInfo.WOLF_MAN == "ENABLED":
				SLOT_COUNT_TWO += 1
		
		if SLOT_COUNT_TWO > 5:
			SLOT_COUNT_TWO = 5
		
		for page in xrange(PAGE_COUNT):
			SLOT_COUNT_RES = 5
			for i in xrange(SLOT_COUNT_RES):
				self.curNameAlpha[i] += (self.destNameAlpha[i] - self.curNameAlpha[i]) / 10.0
				self.NameList[i].SetAlpha(self.curNameAlpha[i])
			
			for i in xrange(4):
				self.curGauge[i] += (self.destGauge[i] - self.curGauge[i]) / 10.0
				if abs(self.curGauge[i] - self.destGauge[i]) < 0.005:
					self.curGauge[i] = self.destGauge[i]
				self.GaugeList[i].SetPercentage(self.curGauge[i], 1.0)
		
		if -1 != self.reservingRaceIndex:
			if app.GetTime() - self.reservingStartTime >= 1.5:
				chrSlot=self.stream.GetCharacterSlot()
				textName = self.editCharacterName.GetText()
				raceIndex = self.reservingRaceIndex
				shapeIndex = self.reservingShapeIndex
				
				startStat = self.START_STAT[self.slot]
				statCon = self.stat[0] - startStat[0]
				statInt = self.stat[1] - startStat[1]
				statStr = self.stat[2] - startStat[2]
				statDex = self.stat[3] - startStat[3]
				
				net.SendCreateCharacterPacket(chrSlot, textName, raceIndex, shapeIndex, statCon, statInt, statStr, statDex)
				self.reservingRaceIndex = -1

	def EmptyFunc(self):
		pass

	def PopupMessage(self, msg, func=0):
		if not func:
			func=self.EmptyFunc
		
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

	def OnPressExitKey(self):
		self.CancelCreate()
		return True

	def OverInStatButton(self, stat):
		if not self.STAT_DESCRIPTION.has_key(stat):
			return
		
		self.toolTip.ClearToolTip()
		self.toolTip.AppendTextLine(self.STAT_DESCRIPTION[stat])
		self.toolTip.Show()

	def OverOutStatButton(self):
		self.toolTip.Hide()

	def OverInButton(self, stat):
		if stat == 1:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.CHARACTER_CREATE_MALE, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 2:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.CHARACTER_CREATE_FEMALE, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 3:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.CHARACTER_CREATE_APPEARANCE1, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 4:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.CHARACTER_CREATE_APPEARANCE2, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 5:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(uiScriptLocale.CREATE_PREV, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 6:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(uiScriptLocale.CREATE_NEXT, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 7:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(uiScriptLocale.CREATE_CREATE, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 8:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(uiScriptLocale.CANCEL, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 9:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.JOB_WARRIOR, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 10:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.JOB_ASSASSIN, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 11:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.JOB_SURA, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 12:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.JOB_SHAMAN, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()
		elif stat == 13:
			self.toolTip.ClearToolTip()
			self.toolTip.AlignHorizonalCenter()
			self.toolTip.AutoAppendNewTextLine(localeInfo.JOB_WOLFMAN, grp.GenerateColor(1.0, 1.0, 0.0, 1.0))
			self.toolTip.Show()

	def OverOutButton(self):
		self.toolTip.Hide()
		
