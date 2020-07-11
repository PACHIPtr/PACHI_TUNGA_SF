import app
import net
import chr
import systemSetting
import event
import chat
import player
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

####################################
# 빠른 실행을 위한 모듈 로딩 분담
####################################
import uiCommon                    
import uiMapNameShower             
import uiAffectShower              
import uiPlayerGauge               
import uiCharacter                 
import uiTarget                    
import consoleModule               
import interfaceModule
import uiTaskbar                   
import uiInventory
import constInfo
	
###################################

M2_INIT_VALUE = -1

JOB_WARRIOR		= 0
JOB_ASSASSIN	= 1
JOB_SURA		= 2
JOB_SHAMAN		= 3
JOB_WOLFMAN		= 4
	
class MyCharacters :
	class MyUnit :

		def __init__(self, const_id, name, level, race, playtime, guildname, form, hair, sash, stat_str, stat_dex, stat_hth, stat_int, change_name):
			self.UnitDataDic = {
				"ID" 	: 	const_id,
				"NAME"	:	name,
				"LEVEL"	:	level,
				"RACE"	:	race,
				"PLAYTIME"	:	playtime,
				"GUILDNAME"	:	guildname,
				"FORM"	:	form,
				"HAIR"	:	hair,
				"SASH" : sash,
				"STR"	:	stat_str,
				"DEX"	:	stat_dex,
				"HTH"	:	stat_hth,
				"INT"	:	stat_int,
				"CHANGENAME"	:	change_name,
			}

		def __del__(self) :
			#print self.UnitDataDic["NAME"]
			self.UnitDataDic = None
		
		def GetUnitData(self) :
			return self.UnitDataDic

	def __init__(self, stream) :
		self.MainStream = stream
		self.PriorityData = []
		self.myUnitDic = {}
		self.HowManyChar = 0
		self.EmptySlot	=  []
		self.Race 		= [None, None, None, None, None]
		self.Job		= [None, None, None, None, None]
		self.Guild_Name = [None, None, None, None, None]
		self.Play_Time 	= [None, None, None, None, None]
		self.Change_Name= [None, None, None, None, None]
		self.Stat_Point = { 0 : None, 1 : None, 2 : None, 3 : None, 4 : None }
	
	def __del__(self) :
		self.MainStream = None 
		
		for i in xrange(self.HowManyChar) :
			chr.DeleteInstance(i)
			
		self.PriorityData = None
		self.myUnitDic = None
		self.HowManyChar = None
		self.EmptySlot	= None
		self.Race = None
		self.Job = None 		
		self.Guild_Name = None
		self.Play_Time = None
		self.Change_Name = None
		self.Stat_Point = None
			
	def LoadCharacterData(self) :
		self.RefreshData()
		self.MainStream.All_ButtonInfoHide()
		PrepareLastPlay = 0
		Character_PlayTime = { 0 : None, 1 : None, 2 : None, 3 : None, 4 : None }
		for i in xrange(5) :
			pid 			= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_ID) 
			
			if not pid :
				self.EmptySlot.append(i)
				continue
				
			name 			= net.GetAccountCharacterSlotDataString(i, net.ACCOUNT_CHARACTER_SLOT_NAME)
			level 			= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_LEVEL)
			race 			= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_RACE)
			playtime 		= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_PLAYTIME)
			guildname 		= net.GetAccountCharacterSlotDataString(i, net.ACCOUNT_CHARACTER_SLOT_GUILD_NAME)
			form 			= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_FORM)
			hair 			= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_HAIR)
			stat_str 		= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_STR)
			stat_dex		= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_DEX)
			stat_hth		= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_HTH)
			stat_int		= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_INT)
			last_playtime	= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_LAST_PLAYTIME)
			change_name		= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG)
			sash 			= net.GetAccountCharacterSlotDataInteger(i, net.ACCOUNT_CHARACTER_SLOT_SASH)
			
			if last_playtime <= 0:
				PrepareLastPlay += 1
				Character_PlayTime[i] = PrepareLastPlay
				self.SetPriorityData(PrepareLastPlay)
				self.myUnitDic[PrepareLastPlay] = self.MyUnit(i, name, level, race, playtime, guildname, form, hair, sash, stat_str, stat_dex, stat_hth, stat_int, change_name)
			else:
				Character_PlayTime[i] = last_playtime
				for j in xrange(5):
					if Character_PlayTime[j] == Character_PlayTime[i]:
						last_playtime += 3600
				self.SetPriorityData(last_playtime)
				self.myUnitDic[last_playtime] = self.MyUnit(i, name, level, race, playtime, guildname, form, hair, sash, stat_str, stat_dex, stat_hth, stat_int, change_name)
		self.PriorityData.sort(reverse = True)
		#sorted(self.PriorityData.items(), key=lambda t: t[::-1])

		for index, data in enumerate(self.PriorityData):
			unitDicData = self.myUnitDic[data].GetUnitData()
			self.SetSortingData(index, unitDicData["RACE"], unitDicData["GUILDNAME"],
										unitDicData["PLAYTIME"], unitDicData["STR"],
										unitDicData["DEX"],unitDicData["HTH"], unitDicData["INT"], unitDicData["CHANGENAME"])
			self.MakeCharacter(index, unitDicData["NAME"], unitDicData["RACE"], unitDicData["FORM"], unitDicData["HAIR"], unitDicData["SASH"])
			self.MainStream.InitDataSet(index, unitDicData["NAME"], unitDicData["LEVEL"], unitDicData["ID"])
		
		## Default Setting ##
		if self.HowManyChar :
			self.MainStream.SelectButton(0)
			
		return self.HowManyChar;
		
	def SetPriorityData(self, val):
		self.PriorityData.append(val)
	
	def MakeCharacter(self, slot, name, race, form, hair, sash):
		chr.CreateInstance(slot)
		chr.SelectInstance(slot)
		chr.SetVirtualID(slot)
		chr.SetNameString(name)

		chr.SetRace(race)
		chr.SetArmor(form)
		chr.SetHair(hair)
		chr.SetSash(sash)
	
		chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)
		
		## 수인족만, 스케일 조금 축소.
		if chr.RaceToJob(race) == JOB_WOLFMAN:
			chr.SetScale(0.95,0.95,0.95)
			
		chr.SetRotation(0.0)
		chr.Hide()
		
	def SetSortingData(self, slot, race, guildname, playtime, pStr, pDex, pHth, pInt, changename) :	
		self.HowManyChar += 1
		self.Race[slot] = race
		self.Job[slot] = chr.RaceToJob(race)
		self.Guild_Name[slot] = guildname
		self.Play_Time[slot] = playtime
		self.Change_Name[slot] = changename
		self.Stat_Point[slot] = [pHth, pInt, pStr, pDex]
	
	def GetRace(self, slot) :
		return self.Race[slot]
	
	def GetJob(self, slot) :
		return self.Job[slot]
		
	def GetMyCharacterCount(self) :
		return self.HowManyChar
		
	def GetEmptySlot(self) :
		if not len(self.EmptySlot) :
			return M2_INIT_VALUE
		
		#print "GetEmptySlot %s" % self.EmptySlot[0]
		return self.EmptySlot[0]
		
	def GetStatPoint(self, slot) :			
		return self.Stat_Point[slot]
	
	def GetGuildNamePlayTime(self, slot) :
		return self.Guild_Name[slot], self.Play_Time[slot]
		
	def GetChangeName(self, slot) :
		return self.Change_Name[slot]
		
	def SetChangeNameSuccess(self, slot) :
		self.Change_Name[slot] = 0
		
	def RefreshData(self) :
		self.HowManyChar = 0
		self.EmptySlot	=  []
		self.PriorityData = []
		self.Race 		= [None, None, None, None, None]
		self.Guild_Name = [None, None, None, None, None]
		self.Play_Time 	= [None, None, None, None, None]
		self.Change_Name= [None, None, None, None, None]
		self.Stat_Point = { 0 : None, 1 : None, 2 : None, 3 : None, 4 : None }
		
class SelectCharacterWindow(ui.Window) :	
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
	##Job Description## 
	DESCRIPTION_FILE_NAME =	(
		uiScriptLocale.JOBDESC_WARRIOR_PATH,
		uiScriptLocale.JOBDESC_ASSASSIN_PATH,
		uiScriptLocale.JOBDESC_SURA_PATH,
		uiScriptLocale.JOBDESC_SHAMAN_PATH,
		uiScriptLocale.JOBDESC_WOLFMAN_PATH,
	)
	
	##Job List##
	JOB_LIST = { 	
		0	:	localeInfo.JOB_WARRIOR,	
		1	:	localeInfo.JOB_ASSASSIN,	
		2	:	localeInfo.JOB_SURA,
		3	:	localeInfo.JOB_SHAMAN,		 
		4	:	localeInfo.JOB_WOLFMAN,	 
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
			newScreenWidth = float(screenWidth)
			newScreenHeight = float(screenHeight)

			grp.SetViewport(0.0, 0.0, newScreenWidth/screenWidth, newScreenHeight/screenHeight)

			app.SetCenterPosition(0.0, 0.0, 0.0) #X, Z, Y ( X+ RIGHT, Z+ ??, Y+ DOWN ) 좀 더 분석해 볼 것 
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
		ui.Window.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_SELECT, self)
		self.stream = stream
		
		##Init Value##
		self.SelectSlot = M2_INIT_VALUE
		self.SelectEmpire = False
		self.ShowToolTip = False
		self.select_job = M2_INIT_VALUE
		self.select_race = M2_INIT_VALUE
		self.LEN_STATPOINT = 4
		self.descIndex = 0
		self.statpoint = [0, 0, 0, 0]
		self.curGauge  = [0.0, 0.0, 0.0, 0.0]
		self.Name_FontColor_Def	 = grp.GenerateColor(0.7215, 0.7215, 0.7215, 1.0)
		self.Name_FontColor		 = grp.GenerateColor(197.0/255.0, 134.0/255.0, 101.0/255.0, 1.0)
		self.Level_FontColor 	 = grp.GenerateColor(250.0/255.0, 211.0/255.0, 136.0/255.0, 1.0)
		self.Not_SelectMotion = False
		self.MotionStart = False
		self.MotionTime = 0.0
		self.RealSlot = []
		self.Disable = False

	def __del__(self):
		ui.Window.__del__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_SELECT, 0)
		
	def Open(self):
		#print "##---------------------------------------- NEW INTRO SELECT OPEN"
		playersettingmodule.LoadGameData("INIT")
		
		dlgBoard = ui.ScriptWindow()
		self.dlgBoard = dlgBoard
		pythonScriptLoader = ui.PythonScriptLoader()#uiScriptLocale.LOCALE_UISCRIPT_PATH = locale/ymir_ui/
		pythonScriptLoader.LoadScriptFile( self.dlgBoard, "UIScript/New_SelectCharacterWindow.py" ) 

		getChild = self.dlgBoard.GetChild
		
		##Background##
		self.backGroundDict = {
			net.EMPIRE_B : "d:/ymir work/ui/intro/empire/background/empire_chunjo.sub",
			net.EMPIRE_C : "d:/ymir work/ui/intro/empire/background/empire_jinno.sub",
		}
		self.backGround = getChild("BackGround")
	
		##한문##
		self.NameList = []
		self.NameList.append(getChild("name_warrior"))
		self.NameList.append(getChild("name_assassin"))
		self.NameList.append(getChild("name_sura"))
		self.NameList.append(getChild("name_shaman"))
		self.NameList.append(getChild("name_wolfman"))
		
		##Empire Flag##
		self.empireName = getChild("EmpireName")
		self.flagDict = {
			net.EMPIRE_B : "d:/ymir work/ui/intro/empire/empireflag_b.sub",
			net.EMPIRE_C : "d:/ymir work/ui/intro/empire/empireflag_c.sub",
		}
		self.flag = getChild("EmpireFlag")

		##Button List##
		self.btnStart		= getChild("start_button")
		self.btnCreate		= getChild("create_button")
		self.btnDelete		= getChild("delete_button")
		self.btnExit		= getChild("exit_button")
		
		##Face Image##
		self.FaceImage = []
		self.FaceImage.append(getChild("CharacterFace_0"))
		self.FaceImage.append(getChild("CharacterFace_1"))
		self.FaceImage.append(getChild("CharacterFace_2"))
		self.FaceImage.append(getChild("CharacterFace_3"))
		self.FaceImage.append(getChild("CharacterFace_4"))
		
		##Select Character List##
		self.CharacterButtonList = []
		self.CharacterButtonList.append(getChild("CharacterSlot_0"))
		self.CharacterButtonList.append(getChild("CharacterSlot_1"))
		self.CharacterButtonList.append(getChild("CharacterSlot_2"))
		self.CharacterButtonList.append(getChild("CharacterSlot_3"))
		self.CharacterButtonList.append(getChild("CharacterSlot_4"))
		
		##ToolTip : GuildName, PlayTime##
		getChild("CharacterSlot_0").ShowToolTip = lambda arg = 0 : self.OverInToolTip(arg)
		getChild("CharacterSlot_0").HideToolTip = lambda : self.OverOutToolTip()
		getChild("CharacterSlot_1").ShowToolTip = lambda arg = 1 : self.OverInToolTip(arg)
		getChild("CharacterSlot_1").HideToolTip = lambda : self.OverOutToolTip()
		getChild("CharacterSlot_2").ShowToolTip = lambda arg = 2 : self.OverInToolTip(arg)
		getChild("CharacterSlot_2").HideToolTip = lambda : self.OverOutToolTip()
		getChild("CharacterSlot_3").ShowToolTip = lambda arg = 3 : self.OverInToolTip(arg)
		getChild("CharacterSlot_3").HideToolTip = lambda : self.OverOutToolTip()
		getChild("CharacterSlot_4").ShowToolTip = lambda arg = 4 : self.OverInToolTip(arg)
		getChild("CharacterSlot_4").HideToolTip = lambda : self.OverOutToolTip()
		
		## ToolTip etc : Create, Delete, Start, Exit, Prev, Next ##
		getChild("create_button").ShowToolTip = lambda arg = uiScriptLocale.SELECT_CREATE : self.OverInToolTipETC(arg)
		getChild("create_button").HideToolTip = lambda : self.OverOutToolTip()
		getChild("delete_button").ShowToolTip = lambda arg = uiScriptLocale.SELECT_DELETE : self.OverInToolTipETC(arg)
		getChild("delete_button").HideToolTip = lambda : self.OverOutToolTip()
		getChild("start_button").ShowToolTip = lambda arg = uiScriptLocale.SELECT_SELECT : self.OverInToolTipETC(arg)
		getChild("start_button").HideToolTip = lambda : self.OverOutToolTip()
		getChild("exit_button").ShowToolTip = lambda arg = uiScriptLocale.SELECT_EXIT : self.OverInToolTipETC(arg)
		getChild("exit_button").HideToolTip = lambda : self.OverOutToolTip()
		getChild("prev_button").ShowToolTip = lambda arg = uiScriptLocale.CREATE_PREV : self.OverInToolTipETC(arg)
		getChild("prev_button").HideToolTip = lambda : self.OverOutToolTip()
		getChild("next_button").ShowToolTip = lambda arg = uiScriptLocale.CREATE_NEXT : self.OverInToolTipETC(arg)
		getChild("next_button").HideToolTip = lambda : self.OverOutToolTip()
		
		
		##StatPoint Value##
		self.statValue = []
		self.statValue.append(getChild("hth_value"))
		self.statValue.append(getChild("int_value"))
		self.statValue.append(getChild("str_value"))
		self.statValue.append(getChild("dex_value"))
		
		##Gauge UI##
		self.GaugeList = []
		self.GaugeList.append(getChild("hth_gauge"))
		self.GaugeList.append(getChild("int_gauge"))
		self.GaugeList.append(getChild("str_gauge"))
		self.GaugeList.append(getChild("dex_gauge"))
		
		##Text##
		self.textBoard = getChild("text_board")
		self.btnPrev = getChild("prev_button")
		self.btnNext = getChild("next_button")
		
		##DescFace##
		self.discFace = getChild("DiscFace")
		self.raceNameText = getChild("raceName_Text")
		
		##MyID##
		#self.descPhaseText = getChild("desc_phase_text")
		self.myID = getChild("my_id")
		self.myID.SetText(str(constInfo.ACCOUNT_NAME))
		
		##Button Event##
		self.btnStart.SetEvent(ui.__mem_func__(self.StartGameButton))
		self.btnCreate.SetEvent(ui.__mem_func__(self.CreateCharacterButton))
		self.btnExit.SetEvent(ui.__mem_func__(self.ExitButton))
		self.btnDelete.SetEvent(ui.__mem_func__(self.InputPrivateCode))		
		
		##Select MyCharacter##
		self.CharacterButtonList[0].SetEvent(ui.__mem_func__(self.SelectButton), 0)
		self.CharacterButtonList[1].SetEvent(ui.__mem_func__(self.SelectButton), 1)
		self.CharacterButtonList[2].SetEvent(ui.__mem_func__(self.SelectButton), 2)
		self.CharacterButtonList[3].SetEvent(ui.__mem_func__(self.SelectButton), 3)
		self.CharacterButtonList[4].SetEvent(ui.__mem_func__(self.SelectButton), 4)
		
		self.FaceImage[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 0)
		self.FaceImage[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 1)
		self.FaceImage[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 2)
		self.FaceImage[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 3)
		self.FaceImage[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_click", 4)
		
		self.FaceImage[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 0)
		self.FaceImage[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 1)
		self.FaceImage[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 2)
		self.FaceImage[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 3)
		self.FaceImage[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_in", 4)
		
		self.FaceImage[0].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 0)
		self.FaceImage[1].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 1)
		self.FaceImage[2].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 2)
		self.FaceImage[3].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 3)
		self.FaceImage[4].SetEvent(ui.__mem_func__(self.EventProgress), "mouse_over_out", 4)
				
		##Job Description##
		self.btnPrev.SetEvent(ui.__mem_func__(self.PrevDescriptionPage))
		self.btnNext.SetEvent(ui.__mem_func__(self.NextDescriptionPage))
		
		##MyCharacter CLASS##
		self.mycharacters = MyCharacters(self);
		self.mycharacters.LoadCharacterData()
				
		if not self.mycharacters.GetMyCharacterCount() :
			self.stream.SetCharacterSlot(self.mycharacters.GetEmptySlot())
			self.SelectEmpire = True
		
		##Job Description Box##
		self.descriptionBox = self.DescriptionBox()
		self.descriptionBox.Show()
		
		##Tool Tip(Guild Name, PlayTime)##
		self.toolTip = uiToolTip.ToolTip()
		self.toolTip.ClearToolTip()
		
		self.dlgBoard.Show()
		self.Show()
		
		##Empire Flag & Background Setting##
		my_empire = net.GetEmpireID()
		self.SetEmpire(my_empire)
	
		app.ShowCursor()
		
		if musicInfo.selectMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/"+musicInfo.selectMusic)
		
		##Character Render##
		self.chrRenderer = self.CharacterRenderer()
		self.chrRenderer.SetParent(self.backGround)
		self.chrRenderer.Show()
		
		##Default Setting##
	def EventProgress(self, event_type, slot) :
		if self.Disable :
			return

		if "mouse_click" == event_type :
			if slot == self.SelectSlot :
				return 
				
			snd.PlaySound("sound/ui/click.wav")
			self.SelectButton(slot)
		elif "mouse_over_in" == event_type :
			for button in self.CharacterButtonList :
				button.SetUp()
		
			self.CharacterButtonList[slot].Over()
			self.CharacterButtonList[self.SelectSlot].Down()
			self.OverInToolTip(slot)
		elif "mouse_over_out" == event_type :
			for button in self.CharacterButtonList :
				button.SetUp()
			
			self.CharacterButtonList[self.SelectSlot].Down()
			self.OverOutToolTip()
		else :
			print " New_introSelect.py ::EventProgress : False"
		
	def SelectButton(self, slot):		
		#print "self.RealSlot = %s" % self.RealSlot
		#slot 0 ~ 4
		if slot >= self.mycharacters.GetMyCharacterCount() or slot == self.SelectSlot :
			return
			
		if self.Not_SelectMotion or self.MotionTime != 0.0 :
			self.CharacterButtonList[slot].SetUp()
			self.CharacterButtonList[slot].Over()
			return
		
		for button in self.CharacterButtonList:
			button.SetUp()
					
		self.SelectSlot = slot
		self.CharacterButtonList[self.SelectSlot].Down()
		self.stream.SetCharacterSlot(self.RealSlot[self.SelectSlot])
		
		self.select_job = self.mycharacters.GetJob(self.SelectSlot)
		
		##Job Descirption##
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet(self.DESCRIPTION_FILE_NAME[self.select_job])
		event.SetFontColor(self.descIndex, 0.7843, 0.7843, 0.7843)

		event.SetRestrictedCount(self.descIndex, 35)

		if event.BOX_VISIBLE_LINE_COUNT >= event.GetTotalLineCount(self.descIndex) :
			self.btnPrev.Hide()
			self.btnNext.Hide()
		else :
			self.btnPrev.Show()
			self.btnNext.Show()
		
		self.ResetStat()
		
		## 한문 Setting ##
		for i in xrange(len(self.NameList)):
			if self.select_job == i	: 
				self.NameList[i].SetAlpha(1)
			else :
				self.NameList[i].SetAlpha(0)
		
		## Face Setting & Font Color Setting ##
		self.select_race = self.mycharacters.GetRace(self.SelectSlot)
		#print "self.mycharacters.GetMyCharacterCount() = %s" % self.mycharacters.GetMyCharacterCount()
		for i in xrange(self.mycharacters.GetMyCharacterCount()) :
			if slot == i :
				self.FaceImage[slot].LoadImage(self.RACE_FACE_PATH[self.select_race] + "1.sub")
				self.CharacterButtonList[slot].SetAppendTextColor(0, self.Name_FontColor)
			else :
				self.FaceImage[i].LoadImage(self.RACE_FACE_PATH[self.mycharacters.GetRace(i)] + "2.sub")
				self.CharacterButtonList[i].SetAppendTextColor(0, self.Name_FontColor_Def)
		
		## Desc Face & raceText Setting ##
		self.discFace.LoadImage(self.DISC_FACE_PATH[self.select_race])
		self.raceNameText.SetText(self.JOB_LIST[self.select_job])
		
		chr.Hide()
		chr.SelectInstance(self.SelectSlot)
		chr.Show()
		
	def Close(self):
		#print "##---------------------------------------- NEW INTRO SELECT CLOSE"
		del self.mycharacters
		self.EMPIRE_NAME = None
		self.EMPIRE_NAME_COLOR = None
		self.RACE_FACE_PATH = None
		self.DISC_FACE_PATH = None
		self.DESCRIPTION_FILE_NAME = None
		self.JOB_LIST = None
		
		##Default Value##
		self.SelectSlot = None
		self.SelectEmpire = None
		self.ShowToolTip = None
		self.LEN_STATPOINT = None
		self.descIndex = None
		self.statpoint = None#[]
		self.curGauge  = None#[]
		self.Name_FontColor_Def	 = None
		self.Name_FontColor		 = None
		self.Level_FontColor 	 = None
		self.Not_SelectMotion = None
		self.MotionStart = None
		self.MotionTime = None
		self.RealSlot = None
	
		self.select_job = None
		self.select_race = None
		
		##Open Func##
		self.dlgBoard = None
		self.backGround = None
		self.backGroundDict = None
		self.NameList = None#[]
		self.empireName = None
		self.flag = None
		self.flagDict = None#{}
		self.btnStart = None	
		self.btnCreate = None	
		self.btnDelete = None	
		self.btnExit = None	
		self.FaceImage = None#[]
		self.CharacterButtonList = None#[]
		self.statValue = None#[]
		self.GaugeList = None#[]
		self.textBoard = None
		self.btnPrev = None
		self.btnNext = None
		self.raceNameText = None
		#self.descPhaseText = None
		self.myID = None
		
		self.toolTip = None
		self.Disable = None
		
		if musicInfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.selectMusic)
		
		self.Hide()
		self.descriptionBox.Hide()
		self.KillFocus()
		app.HideCursor()
		event.Destroy()
		
	def SetEmpire(self, empire_id):
		self.empireName.SetText(self.EMPIRE_NAME.get(empire_id, ""))
		rgb = self.EMPIRE_NAME_COLOR[empire_id]
		self.empireName.SetFontColor(rgb[0], rgb[1], rgb[2])
		if empire_id != net.EMPIRE_A :
			self.flag.LoadImage(self.flagDict[empire_id])
			self.flag.SetScale(0.45, 0.45)
			self.backGround.LoadImage(self.backGroundDict[empire_id])
			self.backGround.SetScale(float(wndMgr.GetScreenWidth()) / 1024.0, float(wndMgr.GetScreenHeight()) / 768.0)
						
	def CreateCharacterButton(self):
		#빈 슬롯의 번호를 가져와서 케릭터를 만든다.
		slotNumber = self.mycharacters.GetEmptySlot()
			
		if slotNumber == M2_INIT_VALUE :
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.CREATE_FULL, 0, localeInfo.UI_OK)
			return 
		
		pid = self.GetCharacterSlotPID(slotNumber)
		
		if not pid:
			self.stream.SetCharacterSlot(slotNumber)

			if not self.mycharacters.GetMyCharacterCount() :
				self.SelectEmpire = True
			else :
				self.stream.SetCreateCharacterPhase()
				self.Hide()
				self.descriptionBox.Hide()
		
	def ExitButton(self):
		self.stream.SetLoginPhase()		
		self.Hide()
		self.descriptionBox.Hide()
		
	def StartGameButton(self):
		if not self.mycharacters.GetMyCharacterCount() or self.MotionTime != 0.0 :
			return
		
		self.DisableWindow()
		
		IsChangeName = self.mycharacters.GetChangeName(self.SelectSlot)
		if IsChangeName :
			self.OpenChangeNameDialog()
			return
			
		chr.PushOnceMotion(chr.MOTION_INTRO_SELECTED)
		self.MotionStart = True
		self.MotionTime = app.GetTime()
				
	def OnUpdate(self):
		chr.Update()
		self.ToolTipProgress()
		
		if self.SelectEmpire :
			self.SelectEmpire = False
			self.stream.SetReselectEmpirePhase()
			self.Hide()
			self.descriptionBox.Hide()
			
		if self.MotionStart and app.GetTime() - self.MotionTime >= 2.0 :
			self.MotionStart = False
			##기존 사용 그대로 들구옴##
			#print " Start Game "
			chrSlot = self.stream.GetCharacterSlot()
			
			#print "chrSlot = %s" % chrSlot
			if musicInfo.selectMusic != "":
				snd.FadeLimitOutMusic("BGM/"+musicInfo.selectMusic, systemSetting.GetMusicVolume()*0.05)
			
			net.DirectEnter(chrSlot)
			playTime = net.GetAccountCharacterSlotDataInteger(chrSlot, net.ACCOUNT_CHARACTER_SLOT_PLAYTIME)

			player.SetPlayTime(playTime)
			chat.Clear()
		
		(xposEventSet, yposEventSet) = self.textBoard.GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet+7, -(yposEventSet+7))
		self.descriptionBox.SetIndex(self.descIndex)
		
		for i in xrange(self.LEN_STATPOINT):
			self.GaugeList[i].SetPercentage(self.curGauge[i], 1.0)
	
	# def Refresh(self):
	def GetCharacterSlotPID(self, slotIndex):
		return net.GetAccountCharacterSlotDataInteger(slotIndex, net.ACCOUNT_CHARACTER_SLOT_ID)
	
	def All_ButtonInfoHide(self) :
		for i in xrange(5):
			self.CharacterButtonList[i].Hide()
			self.FaceImage[i].Hide()
			
	def InitDataSet(self, slot, name, level, real_slot):	
		width = self.CharacterButtonList[slot].GetWidth()
		height = self.CharacterButtonList[slot].GetHeight()

		self.CharacterButtonList[slot].AppendTextLine(name				, localeInfo.UI_DEF_FONT, self.Name_FontColor_Def	, "right", width - 12, height/4 + 2)
		self.CharacterButtonList[slot].AppendTextLine("Lv." + str(level), localeInfo.UI_DEF_FONT, self.Level_FontColor		, "left", width - 42, height*3/4)

		self.CharacterButtonList[slot].Show()
		self.FaceImage[slot].LoadImage(self.RACE_FACE_PATH[self.mycharacters.GetRace(slot)] + "2.sub")
		self.FaceImage[slot].Show()
		self.RealSlot.append(real_slot) ## 진짜 슬롯 ID ##
				
	def InputPrivateCode(self) :
		if not self.mycharacters.GetMyCharacterCount() :
			return
			
		import uiCommon
		privateInputBoard = uiCommon.InputDialogWithDescription()
		privateInputBoard.SetTitle(localeInfo.INPUT_PRIVATE_CODE_DIALOG_TITLE)
		privateInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrivateCode))
		privateInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrivateCode))
		
		privateInputBoard.SetNumberMode()
			
		privateInputBoard.SetSecretMode()
		privateInputBoard.SetMaxLength(7)
			
		privateInputBoard.SetBoardWidth(250)
		privateInputBoard.SetDescription(localeInfo.INPUT_PRIVATE_CODE_DIALOG_DESCRIPTION)
		privateInputBoard.Open()
		self.privateInputBoard = privateInputBoard
		
		self.DisableWindow()
		
		if not self.Not_SelectMotion:
			self.Not_SelectMotion = True
			chr.PushOnceMotion(chr.MOTION_INTRO_NOT_SELECTED, 0.1)
		
	def AcceptInputPrivateCode(self) :
		privateCode = self.privateInputBoard.GetText()
		if not privateCode:
			return
		
		pid = net.GetAccountCharacterSlotDataInteger(self.RealSlot[self.SelectSlot], net.ACCOUNT_CHARACTER_SLOT_ID)
		
		if not pid :
			self.PopupMessage(localeInfo.SELECT_EMPTY_SLOT)
			return

		net.SendDestroyCharacterPacket(self.RealSlot[self.SelectSlot], privateCode)
		self.PopupMessage(localeInfo.SELECT_DELEING)

		self.CancelInputPrivateCode()
		return True
	
	def CancelInputPrivateCode(self) :
		self.privateInputBoard = None
		self.Not_SelectMotion = False
		chr.SetLoopMotion(chr.MOTION_INTRO_WAIT)
		self.EnableWindow()
		return True
		
	def OnDeleteSuccess(self, slot):
		self.PopupMessage(localeInfo.SELECT_DELETED)
		for i in xrange(len(self.RealSlot)):
			chr.DeleteInstance(i)
			
		self.RealSlot = []
		self.SelectSlot = M2_INIT_VALUE
		
		for button in self.CharacterButtonList :
			button.AppendTextLineAllClear()
					
		if not self.mycharacters.LoadCharacterData() :
			self.stream.popupWindow.Close()
			self.stream.SetCharacterSlot(self.mycharacters.GetEmptySlot())
			self.SelectEmpire = True
	
	def OnDeleteFailure(self):
		self.PopupMessage(localeInfo.SELECT_CAN_NOT_DELETE)
			
	def EmptyFunc(self):
		pass

	def PopupMessage(self, msg, func=0):
		if not func:
			func=self.EmptyFunc

		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)
	
	def RefreshStat(self):
		statSummary = 90.0 
		self.curGauge =	[
			float(self.statpoint[0])/statSummary,
			float(self.statpoint[1])/statSummary,
			float(self.statpoint[2])/statSummary,
			float(self.statpoint[3])/statSummary,
		]
							
		for i in xrange(self.LEN_STATPOINT) :
			self.statValue[i].SetText(str(self.statpoint[i]))

	def ResetStat(self):
		myStatPoint = self.mycharacters.GetStatPoint(self.SelectSlot)
		
		if not myStatPoint :
			return
		
		for i in xrange(self.LEN_STATPOINT) :
			self.statpoint[i] = myStatPoint[i]
		
		self.RefreshStat()
	
	##Job Description Prev & Next Button##
	def PrevDescriptionPage(self):
		if True == event.IsWait(self.descIndex) :
			if event.GetVisibleStartLine(self.descIndex) - event.BOX_VISIBLE_LINE_COUNT >= 0:
				event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) - event.BOX_VISIBLE_LINE_COUNT)
				event.Skip(self.descIndex)
		else :
			event.Skip(self.descIndex)
	
	def NextDescriptionPage(self):
		if True == event.IsWait(self.descIndex) :
			event.SetVisibleStartLine(self.descIndex, event.GetVisibleStartLine(self.descIndex) + event.BOX_VISIBLE_LINE_COUNT)
			event.Skip(self.descIndex)
		else :
			event.Skip(self.descIndex)
	
	##ToolTip : GuildName, PlayTime##
	def OverInToolTip(self, slot) :
		GuildName = localeInfo.GUILD_NAME
		myGuildName, myPlayTime = self.mycharacters.GetGuildNamePlayTime(slot)
		pos_x, pos_y = self.CharacterButtonList[slot].GetGlobalPosition()
		
		if not myGuildName :
			myGuildName = localeInfo.SELECT_NOT_JOIN_GUILD

		guild_name = GuildName + " : " + myGuildName
		play_time = uiScriptLocale.SELECT_PLAYTIME + " :"
		day = myPlayTime / (60 * 24)
		if day : 
			play_time = play_time + " " + str(day) + localeInfo.DAY
		hour = (myPlayTime - (day * 60 * 24))/60
		if hour :
			play_time = play_time + " " + str(hour) + localeInfo.HOUR
		min = myPlayTime - (hour * 60) - (day * 60 * 24)
	
		play_time = play_time + " " + str(min) + localeInfo.MINUTE
		
		textlen = max(len(guild_name), len(play_time))
		tooltip_width = 6 * textlen + 22

		self.toolTip.ClearToolTip()
		self.toolTip.SetThinBoardSize(tooltip_width)

		self.toolTip.SetToolTipPosition(pos_x + 173 + tooltip_width/2, pos_y + 34)
		self.toolTip.AppendTextLine(guild_name, 0xffe4cb1b, False) 	##YELLOW## 
		self.toolTip.AppendTextLine(play_time, 0xffffff00, False) 	##YELLOW## 

		self.toolTip.Show()
	
	def OverInToolTipETC(self, arg) :
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
	
	def SameLoginDisconnect(self):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(localeInfo.LOGIN_FAILURE_SAMELOGIN, self.ExitButton, localeInfo.UI_OK)
		
	def OnIMEReturn(self):
		self.StartGameButton()
		return True
	
	def OnPressEscapeKey(self):
		self.ExitButton()
		return True
	
	def OnKeyDown(self, key):
		if self.MotionTime != 0.0 :
			return
			
		if 2 == key: #1
			self.SelectButton(0)
		elif 3 == key:
			self.SelectButton(1)
		elif 4 == key:
			self.SelectButton(2)
		elif 5 == key:
			self.SelectButton(3)
		elif 6 == key:
			self.SelectButton(4)
		elif 200 == key or 208 == key :
			self.KeyInputUpDown(key)
		else:
			return True

		return True
		
	def KeyInputUpDown(self, key) :
		idx = self.SelectSlot
		maxValue = self.mycharacters.GetMyCharacterCount()
		if 200 == key : #UP
			idx = idx - 1
			if idx < 0 :
				idx = maxValue - 1
 
		elif 208 == key : #DOWN
			idx = idx + 1 
			if idx >= maxValue :
				idx = 0
		else: # 이런 경우 없겠지만, 혹시 몰라 예외 처리
			self.SelectButton(0)

		self.SelectButton(idx)

	def OnPressExitKey(self):
		self.ExitButton()
		return True
	
	def DisableWindow(self):
		self.btnStart.Disable()
		self.btnCreate.Disable()
		self.btnExit.Disable()
		self.btnDelete.Disable()
		self.btnPrev.Disable()
		self.btnNext.Disable()
		self.toolTip.Hide()
		self.ShowToolTip = False
		self.Disable = True
		for button in self.CharacterButtonList :
			button.Disable()

	def EnableWindow(self):
		self.btnStart.Enable()
		self.btnCreate.Enable()
		self.btnExit.Enable()
		self.btnDelete.Enable()
		self.btnPrev.Enable()
		self.btnNext.Enable()
		self.Disable = False
		for button in self.CharacterButtonList :
			button.Enable()

	def OpenChangeNameDialog(self):
		import uiCommon
		nameInputBoard = uiCommon.InputDialogWithDescription()
		nameInputBoard.SetTitle(localeInfo.SELECT_CHANGE_NAME_TITLE)
		nameInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputName))
		nameInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputName))
		nameInputBoard.SetMaxLength(chr.PLAYER_NAME_MAX_LEN)
		nameInputBoard.SetBoardWidth(200)
		nameInputBoard.SetDescription(localeInfo.SELECT_INPUT_CHANGING_NAME)
		nameInputBoard.Open()
		nameInputBoard.slot = self.RealSlot[self.SelectSlot]
		self.nameInputBoard = nameInputBoard
		
	def AcceptInputName(self):
		changeName = self.nameInputBoard.GetText()
		if not changeName:
			return

		net.SendChangeNamePacket(self.nameInputBoard.slot, changeName)
		return self.CancelInputName()

	def CancelInputName(self):
		self.nameInputBoard.Close()
		self.nameInputBoard = None
		self.EnableWindow()
		return True

	def OnCreateFailure(self, type):
		if 0 == type:
			self.PopupMessage(localeInfo.SELECT_CHANGE_FAILURE_STRANGE_NAME)
		elif 1 == type:
			self.PopupMessage(localeInfo.SELECT_CHANGE_FAILURE_ALREADY_EXIST_NAME)
		elif 100 == type:
			self.PopupMessage(localeInfo.SELECT_CHANGE_FAILURE_STRANGE_INDEX)
			
	def OnChangeName(self, slot, name):
		for i in xrange(len(self.RealSlot)) :
			if self.RealSlot[i] == slot :
				self.ChangeNameButton(i, name)
				self.SelectButton(i)
				self.PopupMessage(localeInfo.SELECT_CHANGED_NAME)
				break
				
	def ChangeNameButton(self, slot, name) :
		self.CharacterButtonList[slot].SetAppendTextChangeText(0, name)
		self.mycharacters.SetChangeNameSuccess(slot)

