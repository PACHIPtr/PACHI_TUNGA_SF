import app
import net
import chr
import event
import systemSetting
import grp
import wndMgr
import snd
import musicInfo
import localeInfo

import ui
import uiToolTip
import uiScriptLocale
import networkModule
import playersettingmodule

MAN			= 0
WOMAN		= 1
			  
class CreateCharacterWindow(ui.Window):
	M2STATPOINT = ( ## CON INT STR DEX ## 
		( 4, 3, 6, 3 ), ## WARRIOR ##
		( 3, 3, 4, 6 ), ## ASSASSIN ##
		( 3, 5, 5, 3 ), ## SURA ##
		( 4, 6, 3, 3 ), ## SHAMAN ##
		( 6, 2, 2, 6 ), ## WOLFMAN ##
	)
	M2JOBLIST = {
		0	:	localeInfo.JOB_WARRIOR,	
		1	:	localeInfo.JOB_ASSASSIN,	
		2	:	localeInfo.JOB_SURA,	
		3	:	localeInfo.JOB_SHAMAN,		 
		4	:	localeInfo.JOB_WOLFMAN,	
	}
	##M2_CONST_ID[JOB][SEX]##	
	M2_CONST_ID = 	(	
		(playersettingmodule.RACE_WARRIOR_M, playersettingmodule.RACE_WARRIOR_W),	
		(playersettingmodule.RACE_ASSASSIN_M, playersettingmodule.RACE_ASSASSIN_W),
		(playersettingmodule.RACE_SURA_M, playersettingmodule.RACE_SURA_W),
		(playersettingmodule.RACE_SHAMAN_M, playersettingmodule.RACE_SHAMAN_W),
		(playersettingmodule.RACE_WOLFMAN_M, -1),
	)
	LEN_JOBLIST = len(M2JOBLIST) #5
	LEN_STATPOINT = len(M2STATPOINT[0]) #4
	M2_INIT_VALUE = -1	
	EMPIRE_NAME = { 
		net.EMPIRE_A : localeInfo.EMPIRE_A, 
		net.EMPIRE_B : localeInfo.EMPIRE_B, 
		net.EMPIRE_C : localeInfo.EMPIRE_C 
	}
	EMPIRE_NAME_COLOR = { 
		net.EMPIRE_A : (0.7450, 0, 0), 
		net.EMPIRE_B : (0.8666, 0.6156, 0.1843), 
		net.EMPIRE_C : (0.2235, 0.2549, 0.7490) 
	}
	## Job Description ## 
	DESCRIPTION_FILE_NAME =	(
			uiScriptLocale.JOBDESC_WARRIOR_PATH,
			uiScriptLocale.JOBDESC_ASSASSIN_PATH,
			uiScriptLocale.JOBDESC_SURA_PATH,
			uiScriptLocale.JOBDESC_SHAMAN_PATH,
			uiScriptLocale.JOBDESC_WOLFMAN_PATH,
		)
	RACE_FACE_PATH = {
		playersettingmodule.RACE_WARRIOR_M		:	"D:/ymir work/ui/intro/public_intro/face/face_warrior_m_0",
		playersettingmodule.RACE_ASSASSIN_W		:	"D:/ymir work/ui/intro/public_intro/face/face_assassin_w_0",
		playersettingmodule.RACE_SURA_M			:	"D:/ymir work/ui/intro/public_intro/face/face_sura_m_0",
		playersettingmodule.RACE_SHAMAN_W		:	"D:/ymir work/ui/intro/public_intro/face/face_shaman_w_0",
		playersettingmodule.RACE_WARRIOR_W		:	"D:/ymir work/ui/intro/public_intro/face/face_warrior_w_0",
		playersettingmodule.RACE_ASSASSIN_M		:	"D:/ymir work/ui/intro/public_intro/face/face_assassin_m_0",
		playersettingmodule.RACE_SURA_W			:	"D:/ymir work/ui/intro/public_intro/face/face_sura_w_0",
		playersettingmodule.RACE_SHAMAN_M		:	"D:/ymir work/ui/intro/public_intro/face/face_shaman_m_0",
		playersettingmodule.RACE_WOLFMAN_M		:	"D:/ymir work/ui/intro/public_intro/face/face_wolfman_m_0",
	}
	DISC_FACE_PATH = {
		playersettingmodule.RACE_WARRIOR_M		:"icon/face/warrior_m.tga",
		playersettingmodule.RACE_ASSASSIN_W		:"icon/face/assassin_w.tga",
		playersettingmodule.RACE_SURA_M			:"icon/face/sura_m.tga",
		playersettingmodule.RACE_SHAMAN_W		:"icon/face/shaman_w.tga",
		playersettingmodule.RACE_WARRIOR_W		:"icon/face/warrior_w.tga",
		playersettingmodule.RACE_ASSASSIN_M		:"icon/face/assassin_m.tga",
		playersettingmodule.RACE_SURA_W			:"icon/face/sura_w.tga",
		playersettingmodule.RACE_SHAMAN_M		:"icon/face/shaman_m.tga",
		playersettingmodule.RACE_WOLFMAN_M		:"icon/face/wolfman_m.tga",
	}		
	## Job Description ## 
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
			newScreenWidth = float(screenWidth)
			newScreenHeight = float(screenHeight)

			grp.SetViewport(0.0, 0.0, newScreenWidth/screenWidth, newScreenHeight/screenHeight)
	
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

	## class CreateCharacterWindow Function ## 			
	def __init__(self, stream):
		ui.Window.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_CREATE, self)
		self.stream = stream

		##Init Value##
		self.ShowToolTip = False
		self.createSuccess = False
		self.MotionStart = False
		self.MotionTime = 0.0
		self.gender = self.M2_INIT_VALUE	
		self.job_id = self.M2_INIT_VALUE	
		self.shape = self.M2_INIT_VALUE	
		self.descIndex = 0
		self.statpoint = [0, 0, 0, 0]
		self.curGauge  = [0.0, 0.0, 0.0, 0.0]
		
	def __del__(self):
		net.SetPhaseWindow(net.PHASE_WINDOW_CREATE, 0)
		ui.Window.__del__(self)
		
	def Open(self):
		#print "##---------------------------------------- NEW INTRO CREATE OPEN"
		playersettingmodule.LoadGameData("INIT")
				
		## Func Open UI Loading START ##
		dlgBoard = ui.ScriptWindow()
		self.dlgBoard = dlgBoard
		pythonScriptLoader = ui.PythonScriptLoader()#uiScriptLocale.LOCALE_UISCRIPT_PATH = locale/ymir_ui/
		pythonScriptLoader.LoadScriptFile( self.dlgBoard, "UIScript/New_CreateCharacterWindow.py" ) 

		getChild = self.dlgBoard.GetChild
		
		## HanMoon ##
		self.NameList = []
		self.NameList.append(getChild("name_warrior"))
		self.NameList.append(getChild("name_assassin"))
		self.NameList.append(getChild("name_sura"))
		self.NameList.append(getChild("name_shaman"))
		self.NameList.append(getChild("name_wolfman"))
		
		## Text ##
		self.textBoard = getChild("text_board")
		self.btnPrev = getChild("prev_button")
		self.btnNext = getChild("next_button")
				
		## Select Job Button List Append ##
		self.JobList = []
		self.JobList.append(getChild("WARRIOR"))
		self.JobList.append(getChild("ASSASSIN"))
		self.JobList.append(getChild("SURA"))
		self.JobList.append(getChild("SHAMAN"))
		self.JobList.append(getChild("WOLFMAN"))


		## Image Click Name & Face(Left Button) ##
		self.SelectBtnNameList = []
		self.SelectBtnNameList.append(getChild("name_warrior_small"))
		self.SelectBtnNameList.append(getChild("name_assassin_small"))
		self.SelectBtnNameList.append(getChild("name_sura_small"))
		self.SelectBtnNameList.append(getChild("name_shaman_small"))
		self.SelectBtnNameList.append(getChild("name_wolfman_small"))
		
		self.SelectBtnFaceList = []
		self.SelectBtnFaceList.append(getChild("CharacterFace_0"))
		self.SelectBtnFaceList.append(getChild("CharacterFace_1"))
		self.SelectBtnFaceList.append(getChild("CharacterFace_2"))
		self.SelectBtnFaceList.append(getChild("CharacterFace_3"))
		self.SelectBtnFaceList.append(getChild("CharacterFace_4"))
			
		## Select SEX Button List Append ##
		self.genderButtonList = []
		self.genderButtonList.append(getChild("gender_button_01"))
		self.genderButtonList.append(getChild("gender_button_02"))
		
		## Select Shape Button List Append ##
		self.shapeButtonList = []
		self.shapeButtonList.append(getChild("shape_button_01"))
		self.shapeButtonList.append(getChild("shape_button_02"))
		
		## StatPoint Value ##
		self.statValue = []
		self.statValue.append(getChild("hth_value"))
		self.statValue.append(getChild("int_value"))
		self.statValue.append(getChild("str_value"))
		self.statValue.append(getChild("dex_value"))
		
		## Gauge UI ##
		self.GaugeList = []
		self.GaugeList.append(getChild("hth_gauge"))
		self.GaugeList.append(getChild("int_gauge"))
		self.GaugeList.append(getChild("str_gauge"))
		self.GaugeList.append(getChild("dex_gauge"))
		
		## Create or Cancel ##
		self.btnCreate = getChild("create_button")
		self.btnCancel = getChild("cancel_button")
		
		## Empire Flag ##
		self.empireName = getChild("EmpireName")
		self.flagDict = {
			net.EMPIRE_B : "d:/ymir work/ui/intro/empire/empireflag_b.sub",
			net.EMPIRE_C : "d:/ymir work/ui/intro/empire/empireflag_c.sub",
		}
		self.flag = getChild("EmpireFlag")

		self.discFace = getChild("DiscFace")
		self.raceNameText = getChild("raceName_Text")
			
		## ID Character Name ##
		self.editCharacterName = getChild("character_name_value")
	
		## Background ## Note : Default : Sinsu
		self.backGroundDict = {
			net.EMPIRE_B : "d:/ymir work/ui/intro/empire/background/empire_chunjo.sub",
			net.EMPIRE_C : "d:/ymir work/ui/intro/empire/background/empire_jinno.sub",
		}
		self.backGround = getChild("BackGround")

		
		## INIT Job Sex ##
		self.JobSex = {}
		self.JobSex[0] = app.GetRandom(MAN, WOMAN) ## WARRIOR ##
		self.JobSex[1] = app.GetRandom(MAN, WOMAN) ## ASSASSIN ##
		self.JobSex[2] = app.GetRandom(MAN, WOMAN) ## SURA	##
		self.JobSex[3] = app.GetRandom(MAN, WOMAN) ## SHAMAN ##
		self.JobSex[4] = MAN ## WOLFMAN ##
		
		##Tool Tip##
		self.toolTip = uiToolTip.ToolTip()
		self.toolTip.ClearToolTip()
		
		self.editCharacterName.SetText("")
		
		self.EnableWindow()
		
		## Select Job Button List Start Event ##
		self.JobList[0].SetEvent(ui.__mem_func__(self.SelectJob), 0)
		self.JobList[1].SetEvent(ui.__mem_func__(self.SelectJob), 1)
		self.JobList[2].SetEvent(ui.__mem_func__(self.SelectJob), 2)
		self.JobList[3].SetEvent(ui.__mem_func__(self.SelectJob), 3)
		self.JobList[4].SetEvent(ui.__mem_func__(self.SelectJob), 4)
		
		## Select Sex Button List Start Event ##
		self.genderButtonList[MAN].SetEvent(ui.__mem_func__(self.SelectGender), MAN)
		self.genderButtonList[WOMAN].SetEvent(ui.__mem_func__(self.SelectGender), WOMAN)
		
		## Select Shape Button List Start Event ##
		self.shapeButtonList[0].SetEvent(ui.__mem_func__(self.SelectShape), 0)
		self.shapeButtonList[1].SetEvent(ui.__mem_func__(self.SelectShape), 1)
		
		## Create or Cancel ##
		self.btnCreate.SetEvent(ui.__mem_func__(self.CreateCharacterButton))
		self.btnCancel.SetEvent(ui.__mem_func__(self.CancelButton))
	
		## Job Description ##
		self.btnPrev.SetEvent(ui.__mem_func__(self.PrevDescriptionPage))
		self.btnNext.SetEvent(ui.__mem_func__(self.NextDescriptionPage))
		
		self.editCharacterName.SetReturnEvent(ui.__mem_func__(self.CreateCharacterButton))
		self.editCharacterName.SetEscapeEvent(ui.__mem_func__(self.CancelButton))
		
		## Image Click Face & Name ##
		self.SelectBtnNameList[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 0)
		self.SelectBtnNameList[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 1)
		self.SelectBtnNameList[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 2)
		self.SelectBtnNameList[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 3)
		self.SelectBtnNameList[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 4)
		
		self.SelectBtnNameList[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 0)
		self.SelectBtnNameList[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 1)
		self.SelectBtnNameList[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 2)
		self.SelectBtnNameList[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 3)
		self.SelectBtnNameList[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 4)
		
		self.SelectBtnNameList[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 0)
		self.SelectBtnNameList[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 1)
		self.SelectBtnNameList[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 2)
		self.SelectBtnNameList[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 3)
		self.SelectBtnNameList[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 4)
		
		self.SelectBtnFaceList[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 0)
		self.SelectBtnFaceList[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 1)
		self.SelectBtnFaceList[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 2)
		self.SelectBtnFaceList[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 3)
		self.SelectBtnFaceList[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 4)
		
		self.SelectBtnFaceList[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 0)
		self.SelectBtnFaceList[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 1)
		self.SelectBtnFaceList[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 2)
		self.SelectBtnFaceList[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 3)
		self.SelectBtnFaceList[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 4)
		
		self.SelectBtnFaceList[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 0)
		self.SelectBtnFaceList[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 1)
		self.SelectBtnFaceList[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 2)
		self.SelectBtnFaceList[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 3)
		self.SelectBtnFaceList[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 4)
		
		## Tooltip ##
		getChild("WARRIOR").ShowToolTip	 	= lambda arg = self.M2JOBLIST[0] : self.OverInToolTip(arg)
		getChild("WARRIOR").HideToolTip	 	= lambda : self.OverOutToolTip()
		getChild("ASSASSIN").ShowToolTip 	= lambda arg = self.M2JOBLIST[1] : self.OverInToolTip(arg)
		getChild("ASSASSIN").HideToolTip 	= lambda : self.OverOutToolTip()
		getChild("SURA").ShowToolTip 		= lambda arg = self.M2JOBLIST[2] : self.OverInToolTip(arg)
		getChild("SURA").HideToolTip 		= lambda : self.OverOutToolTip()
		getChild("SHAMAN").ShowToolTip 		= lambda arg = self.M2JOBLIST[3] : self.OverInToolTip(arg)
		getChild("SHAMAN").HideToolTip 		= lambda : self.OverOutToolTip()
		getChild("WOLFMAN").ShowToolTip 	= lambda arg = self.M2JOBLIST[4] : self.OverInToolTip(arg)
		getChild("WOLFMAN").HideToolTip 	= lambda : self.OverOutToolTip()
		
		getChild("gender_button_01").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_MAN : self.OverInToolTip(arg)
		getChild("gender_button_01").HideToolTip 	= lambda : self.OverOutToolTip()
		getChild("gender_button_02").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_WOMAN : self.OverInToolTip(arg)
		getChild("gender_button_02").HideToolTip 	= lambda : self.OverOutToolTip()
		
		getChild("shape_button_01").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_SHAPE + "1": self.OverInToolTip(arg)
		getChild("shape_button_01").HideToolTip 	= lambda : self.OverOutToolTip()
		getChild("shape_button_02").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_SHAPE + "2" : self.OverInToolTip(arg)
		getChild("shape_button_02").HideToolTip 	= lambda : self.OverOutToolTip()
		
		getChild("prev_button").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_PREV : self.OverInToolTip(arg)
		getChild("prev_button").HideToolTip 	= lambda : self.OverOutToolTip()
		getChild("next_button").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_NEXT : self.OverInToolTip(arg)
		getChild("next_button").HideToolTip 	= lambda : self.OverOutToolTip()
		getChild("create_button").ShowToolTip 	= lambda arg = uiScriptLocale.CREATE_CREATE : self.OverInToolTip(arg)
		getChild("create_button").HideToolTip 	= lambda : self.OverOutToolTip()
		getChild("cancel_button").ShowToolTip 	= lambda arg = uiScriptLocale.CANCEL : self.OverInToolTip(arg)
		getChild("cancel_button").HideToolTip 	= lambda : self.OverOutToolTip()
		
		## Func Open UI Loading END##		
		## Func Open Make Character & Render START##	
	
		## Flag Setting ##
		my_empire = net.GetEmpireID()
		self.SetEmpire(my_empire)
		
		#Job Description Box
		self.descriptionBox = self.DescriptionBox()
		self.descriptionBox.Show()
				
		##Character Render##
		self.chrRenderer = self.CharacterRenderer()
		self.chrRenderer.SetParent(self.backGround)
		self.chrRenderer.Show()
									
		## Make Character ##
		self.MakeCharacter(playersettingmodule.RACE_WARRIOR_M)	
		self.MakeCharacter(playersettingmodule.RACE_ASSASSIN_M)	
		self.MakeCharacter(playersettingmodule.RACE_SURA_M)		
		self.MakeCharacter(playersettingmodule.RACE_SHAMAN_M)		

		self.MakeCharacter(playersettingmodule.RACE_WARRIOR_W)	
		self.MakeCharacter(playersettingmodule.RACE_ASSASSIN_W)	
		self.MakeCharacter(playersettingmodule.RACE_SURA_W)		
		self.MakeCharacter(playersettingmodule.RACE_SHAMAN_W)		
		
		self.MakeCharacter(playersettingmodule.RACE_WOLFMAN_M)	
		
		self.dlgBoard.Show()
		self.Show() 	
		app.ShowCursor()
		
		if musicInfo.createMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/"+musicInfo.createMusic)
		
		##Default Setting##
		self.SelectJob(app.GetRandom(0, self.LEN_JOBLIST - 1))

	def Close(self):
		#print "##---------------------------------------- NEW INTRO CREATE CLOSE"
		##Init Value##
		self.ShowToolTip = None
		self.createSuccess = None
		self.MotionStart = None
		self.MotionTime = None
		self.gender = None
		self.job_id = None
		self.shape = None
		self.descIndex = None
		self.statpoint = None
		self.curGauge  = None
			
		for i in xrange(self.LEN_JOBLIST * 2 - 1):
			chr.DeleteInstance(i)
			
		self.stream = None
		self.M2STATPOINT = None
		self.M2JOBLIST = None
		self.M2_CONST_ID = None
		self.LEN_JOBLIST = None
		self.LEN_STATPOINT = None
		self.M2_INIT_VALUE = None
		self.EMPIRE_NAME = None
		self.EMPIRE_NAME_COLOR = None
		self.DESCRIPTION_FILE_NAME = None
		self.RACE_FACE_PATH = None
		self.DISC_FACE_PATH = None
		self.NameList = None
		self.JobList = None
		self.SelectBtnNameList = None
		self.SelectBtnFaceList = None
		self.genderButtonList = None
		self.shapeButtonList = None
		self.statValue = None
		self.GaugeList = None
		self.flag = None
		self.flagDict = None
		self.raceNameText = None
		
		self.textBoard = None
		self.btnPrev = None
		self.btnNext = None
		self.btnCreate = None
		self.btnCancel = None
		self.empireName = None
		self.editCharacterName = None
		self.backGround = None
		self.backGroundDict = None
		self.toolTip = None
		self.JobSex = None
		self.dlgBoard.ClearDictionary()
		
		if musicInfo.createMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.createMusic)
	
		self.dlgBoard.Hide()
		self.Hide()
		self.descriptionBox.Hide()

		app.HideCursor()
		event.Destroy()
	
	##[Event]Create Success - Phase Change##
	def OnCreateSuccess(self):
		self.createSuccess = True
	
	##[Event]Create Fail - ##EnableWindow
	def OnCreateFailure(self, type):
		self.MotionStart = False
		chr.BlendLoopMotion(chr.MOTION_INTRO_WAIT, 0.1)
		if 1 == type:
			self.PopupMessage(localeInfo.CREATE_EXIST_SAME_NAME, self.EnableWindow)
		elif 2 == type:
			self.PopupMessage(localeInfo.CREATE_ERROR_LIMIT_LEVEL, self.EnableWindow)
		elif 3 == type:
			self.PopupMessage(localeInfo.CREATE_ERROR_TIME_LIMIT, self.EnableWindow)
		elif 4 == type:
			self.PopupMessage(localeInfo.CREATE_ERROR_WRONG_STRING, self.EnableWindow)
		else:
			self.PopupMessage(localeInfo.CREATE_FAILURE, self.EnableWindow)			
		
	def EnableWindow(self):
		self.btnPrev.Enable()
		self.btnNext.Enable()
		self.btnCreate.Enable()
		self.btnCancel.Enable()
		self.editCharacterName.SetFocus()
		self.editCharacterName.Enable()
		
		for i in xrange(2) :
			self.genderButtonList[i].Enable()
			self.shapeButtonList[i].Enable()
			
	def DisableWindow(self):
		self.btnPrev.Disable()
		self.btnNext.Disable()
		self.btnCreate.Disable()
		self.btnCancel.Disable()
		self.editCharacterName.Disable()
		
		for i in xrange(2) :
			self.genderButtonList[i].Disable()
			self.shapeButtonList[i].Disable()
				
	def MakeCharacter(self, race):	
		chr_id = race;
		
		chr.CreateInstance(chr_id)
		chr.SelectInstance(chr_id)
		chr.SetVirtualID(chr_id)
		chr.SetNameString(str(race))
		
		chr.SetRace(race)
		chr.SetArmor(0) 
		chr.SetHair(0)

		chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)

		chr.SetRotation(-7.0) #Degree value	
		chr.Hide()
			
	def SelectJob(self, job_id):		
		if self.MotionStart :
			self.JobList[job_id].SetUp()
			return
			
		for button in self.JobList:
			button.SetUp()
					
		##Default Setting##
		self.job_id = job_id
		self.JobList[self.job_id].Down()
		self.SelectGender(self.JobSex[job_id])
		
		##Job Descirption##
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet(self.DESCRIPTION_FILE_NAME[self.job_id])
		event.SetFontColor(self.descIndex, 0.7843, 0.7843, 0.7843)
		total_line = event.GetTotalLineCount(self.descIndex)
		
		self.raceNameText.SetText(self.M2JOBLIST[job_id])
		

		event.SetRestrictedCount(self.descIndex, 35)

		if event.BOX_VISIBLE_LINE_COUNT >= total_line :
			self.btnPrev.Hide()
			self.btnNext.Hide()
		else :
			self.btnPrev.Show()
			self.btnNext.Show()
			
		##Stat Point##
		self.ResetStat()
			
		if self.M2_CONST_ID[self.job_id][MAN] == playersettingmodule.RACE_WOLFMAN_M	:
			self.genderButtonList[WOMAN].Hide()
		else:
			self.genderButtonList[WOMAN].Show()
						
	def SelectGender(self, gender):
		for button in self.genderButtonList:
			button.SetUp()
		
		self.gender = gender
		self.genderButtonList[self.gender].Down()

		if self.M2_INIT_VALUE	 == self.job_id	:
			return
			
		self.JobSex[self.job_id] = self.gender
		self.race = self.M2_CONST_ID[self.job_id][self.gender]

		for i in xrange(self.LEN_JOBLIST) :
			if self.job_id == i :
				self.SelectBtnFaceList[i].LoadImage(self.RACE_FACE_PATH[self.race] + "1.sub")
			else :
				self.SelectBtnFaceList[i].LoadImage( self.RACE_FACE_PATH[self.M2_CONST_ID[i][self.JobSex[i]]] + "2.sub")

		self.discFace.LoadImage(self.DISC_FACE_PATH[self.race])
		
		if self.M2_INIT_VALUE == self.shape :
			self.shape = 0
			
		self.SelectShape(self.shape)
		
	def SelectShape(self, shape):
		for button in self.shapeButtonList:
			button.SetUp()

		self.shape = shape
		self.shapeButtonList[self.shape].Down()
		
		#print "job = %s, race =%s, shape =%s" %(self.job_id, self.race, self.shape)
		
		if self.M2_INIT_VALUE	 == self.job_id	:
			return
		
		chr.Hide()
		chr.SelectInstance(self.race)
		chr.ChangeShape(self.shape)
		chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)
		chr.Show()
		
	# def GetSlotIndex(self):	
	def RefreshStat(self):
		statSummary = 8.0 
		self.curGauge =	[
			float(self.statpoint[0])/statSummary,
			float(self.statpoint[1])/statSummary,
			float(self.statpoint[2])/statSummary,
			float(self.statpoint[3])/statSummary,
		]
							
		for i in xrange(self.LEN_STATPOINT):
			self.statValue[i].SetText(str(self.statpoint[i]))
	
	def ResetStat(self):
		for i in xrange(self.LEN_STATPOINT) :
			self.statpoint[i] = self.M2STATPOINT[self.job_id][i]
		self.RefreshStat()
		
	##Create Character Button##
	def CreateCharacterButton(self):
		if self.job_id == self.M2_INIT_VALUE or self.MotionStart :
			return 
		
		textName = self.editCharacterName.GetText()
		
		if False == self.CheckCreateCharacterName(textName): 
			return
		
		self.DisableWindow()
		
		chrSlot=self.stream.GetCharacterSlot()
		raceIndex = self.M2_CONST_ID[self.job_id][self.gender]
		shapeIndex = self.shape
		statCon = self.M2STATPOINT[self.job_id][0]
		statInt = self.M2STATPOINT[self.job_id][1]
		statStr = self.M2STATPOINT[self.job_id][2]
		statDex = self.M2STATPOINT[self.job_id][3]

		chr.PushOnceMotion(chr.MOTION_INTRO_SELECTED)
		net.SendCreateCharacterPacket(chrSlot, textName, raceIndex, shapeIndex, statCon, statInt, statStr, statDex)
		
		self.MotionStart = True
		self.MotionTime = app.GetTime()
	
	##Cancel Button##
	def CancelButton(self):
		self.stream.SetSelectCharacterPhase()
		self.Hide()
		self.descriptionBox.Hide()

	def EmptyFunc(self):
		pass

	def PopupMessage(self, msg, func = 0):
		if not func :
			func = self.EmptyFunc
		
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

	def OnPressExitKey(self):		
		self.CancelButton()
		return True
		
	def CheckCreateCharacterName(self, name):
		if len(name) == 0:
			self.PopupMessage(localeInfo.CREATE_INPUT_NAME, self.EnableWindow)
			return False

		if name.find(localeInfo.CREATE_GM_NAME)!=-1:
			self.PopupMessage(localeInfo.CREATE_ERROR_GM_NAME, self.EnableWindow)
			return False

		if net.IsInsultIn(name):
			self.PopupMessage(localeInfo.CREATE_ERROR_INSULT_NAME, self.EnableWindow)
			return False

		return True	
	
	def SetEmpire(self, empire_id):
		self.empireName.SetText(self.EMPIRE_NAME.get(empire_id, ""))
		rgb = self.EMPIRE_NAME_COLOR[empire_id]
		self.empireName.SetFontColor(rgb[0], rgb[1], rgb[2])
		if empire_id != net.EMPIRE_A :
			self.flag.LoadImage(self.flagDict[empire_id])
			self.flag.SetScale(0.45, 0.45)
			self.backGround.LoadImage(self.backGroundDict[empire_id])
			self.backGround.SetScale(float(wndMgr.GetScreenWidth()) / 1024.0, float(wndMgr.GetScreenHeight()) / 768.0)

	def PrevDescriptionPage(self):
		if True == event.IsWait(self.descIndex):
			if event.GetVisibleStartLine(self.descIndex) - event.BOX_VISIBLE_LINE_COUNT >= 0:
				event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) - event.BOX_VISIBLE_LINE_COUNT)
				event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)
	
	def NextDescriptionPage(self):
		if True == event.IsWait(self.descIndex):
			event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) + event.BOX_VISIBLE_LINE_COUNT)
			event.Skip(self.descIndex)
		else:
			event.Skip(self.descIndex)
			
	def EventProgress(self, event_type, slot) :
		#print "EventProcess %s, %s" %(event_type, slot)
		if "mouse_click" == event_type :
			if slot == self.job_id :
				return
				
			snd.PlaySound("sound/ui/click.wav")
			self.SelectJob(slot)
		elif "mouse_over_in" == event_type :
			for button in self.JobList :
				button.SetUp()
			
			self.JobList[slot].Over()
			self.JobList[self.job_id].Down()
			self.OverInToolTip(self.M2JOBLIST[slot])
		elif "mouse_over_out" == event_type :
			for button in self.JobList :
				button.SetUp()
				
			self.JobList[self.job_id].Down()
			self.OverOutToolTip()
		else :
			print " New_introSelect.py ::EventProgress : False"
	
	def OverInToolTip(self, arg) :	
		arglen = len(str(arg))
		pos_x, pos_y = wndMgr.GetMousePosition()
		
		self.toolTip.ClearToolTip()
		self.toolTip.SetThinBoardSize(11 * arglen)
		self.toolTip.SetToolTipPosition(pos_x + 50, pos_y + 50)
		self.toolTip.AppendTextLine(arg, 0xffffff00)
		self.toolTip.Show()
		self.ShowToolTip = True
	
	def OverOutToolTip(self) :
		self.toolTip.Hide()
		self.ShowToolTip = False
		
	def ToolTipProgress(self) :
		if self.ShowToolTip :
			pos_x, pos_y = wndMgr.GetMousePosition()
			self.toolTip.SetToolTipPosition(pos_x + 50, pos_y + 50)

	def OnUpdate(self):
		chr.Update()
		self.ToolTipProgress()

		(xposEventSet, yposEventSet) = self.textBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet+7, -(yposEventSet+7))
		self.descriptionBox.SetIndex(self.descIndex)
		
		for i in xrange(len(self.NameList)):
			if self.job_id == i	: 
				self.NameList[i].SetAlpha(1)
			else :
				self.NameList[i].SetAlpha(0)
				
		for i in xrange(self.LEN_STATPOINT):
			self.GaugeList[i].SetPercentage(self.curGauge[i], 1.0)		
		
		if self.MotionStart and self.createSuccess and app.GetTime() - self.MotionTime >= 2.0 :
			self.MotionStart = False
			self.stream.SetSelectCharacterPhase()
			self.Hide()
			self.descriptionBox.Hide()
		
