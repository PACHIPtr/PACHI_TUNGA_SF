import app
import net
import player
import background
import chrmgr
import ui
import uiScriptLocale
import dbg
import wndMgr

import localeInfo
import colorinfo
import constInfo
import playersettingmodule
import stringCommander
import emotion
import introLogin
####################################
# 빠른 실행을 위한 모듈 로딩 분담
####################################
import uiRefine
import uiToolTip
import uiAttachMetin
import uiPickMoney
import uiChat
import uiMessenger
import uiHelp
import uiWhisper
import uiPointReset
import uiShop
import uiExchange
import uiSystem
import uiOption
import uiRestart
####################################

class LoadingWindow(ui.ScriptWindow):
	def __init__(self, stream, name='exsta'):
		print "NEW LOADING WINDOW -------------------------------------------------------------------------------"
		ui.Window.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_LOAD, self)

		self.stream=stream
		self.loadingImage=0
		self.loadingGage=0
		self.errMsg=0
		self.update=0
		self.playerX=0
		self.playerY=0
		self.loadStepList=[]

	def __del__(self):
		print "---------------------------------------------------------------------------- DELETE LOADING WINDOW"
		net.SetPhaseWindow(net.PHASE_WINDOW_LOAD, 0)
		ui.Window.__del__(self)

	def Open(self):
		print "OPEN LOADING WINDOW -------------------------------------------------------------------------------"

		constInfo.SYSTEMS_WINDOW_KAPAT=1
		#app.HideCursor()
		try:
			pyScrLoader = ui.PythonScriptLoader()	
			pyScrLoader.LoadScriptFile(self, "UIScript/LoadingWindow.py")
		except:
			import exception
			exception.Abort("LodingWindow.Open - LoadScriptFile Error")

		try:
			self.loadingImage=self.GetChild("BackGround")
			self.errMsg=self.GetChild("ErrorMessage")
			self.loadingGage=self.GetChild("FullGage")
			self.loadingTip=self.GetChild("LoadingTip")
			self.logo=self.GetChild("logo")
		except:
			import exception
			exception.Abort("LodingWindow.Open - LoadScriptFile Error")

		self.errMsg.Hide()

		imgFileNameDict = {
			0 : "d:/ymir work/ui/intro/loading/new_loading.sub",
			1 : "d:/ymir work/ui/intro/loading/new_loading1.sub",
			2 : "d:/ymir work/ui/intro/loading/new_loading2.sub",
		}
		try:
			imgFileName = imgFileNameDict[app.GetRandom(0, len(imgFileNameDict) - 1)]
			self.loadingImage.LoadImage(imgFileName)

		except:
			print "LoadingWindow.Open.LoadImage - %s File Load Error" % (imgFileName)
			self.loadingImage.Hide()
			
		tipTexts = pack_open(uiScriptLocale.tipList, "r").readlines()
		tipID = app.GetRandom(1,len(tipTexts))
		for i in xrange(len(tipTexts)):
			lines = str(tipTexts[i])
			if lines == "\n" or lines == "":
				continue
			tokens = lines.split("\t")
			tipID_inFile, tipText_inFile = int(tokens[0]), str(tokens[1])
			if tipID == tipID_inFile:
				self.loadingTip.SetText(tipText_inFile)
				
		width = float(wndMgr.GetScreenWidth()) / float(self.loadingImage.GetWidth())
		height = float(wndMgr.GetScreenHeight()) / float(self.loadingImage.GetHeight())

		self.loadingImage.SetScale(width, height)
		self.loadingGage.SetPercentage(2, 100)
		self.logo.Hide() #logo gizlensin
		self.Show()

		chrSlot=self.stream.GetCharacterSlot()
		net.SendSelectCharacterPacket(chrSlot)

		app.SetFrameSkip(0)

	def Close(self):
		print "---------------------------------------------------------------------------- CLOSE LOADING WINDOW"

		app.SetFrameSkip(1)

		self.loadStepList=[]
		self.loadingImage=0
		self.loadingGage=0
		self.errMsg=0
		self.ClearDictionary()
		self.Hide()

	def OnPressEscapeKey(self):
		app.SetFrameSkip(1)
		self.stream.SetLoginPhase()
		return True

	def __SetNext(self, next):
		if next:
			self.update=ui.__mem_func__(next)
		else:
			self.update=0

	def __SetProgress(self, p):
		if self.loadingGage:
			self.loadingGage.SetPercentage(p, 100)

	def DEBUG_LoadData(self, playerX, playerY):
		self.playerX=playerX
		self.playerY=playerY

		self.__RegisterSkill() ## 로딩 중간에 실행 하면 문제 발생
		self.__RegisterTitleName()
		self.__RegisterColor()
		self.__InitData()
		self.__LoadMap()
		self.__LoadSound()
		self.__LoadEffect()
		self.__LoadWarrior()
		self.__LoadAssassin()
		self.__LoadSura()
		self.__LoadShaman()
		self.__LoadWolfman()
		self.__LoadSkill()
		self.__LoadEnemy()
		self.__LoadNPC()
		self.__LoadRaceHeight()
		self.__StartGame()

	def LoadData(self, playerX, playerY):
		self.playerX=playerX
		self.playerY=playerY

		self.__RegisterDungeonMapName()
		self.__RegisterSkill() ## 로딩 중간에 실행 하면 문제 발생
		self.__RegisterTitleName()
		self.__RegisterColor()
		self.__RegisterEmotionIcon()

		self.loadStepList=[
			(0, ui.__mem_func__(self.__InitData)),
			(10, ui.__mem_func__(self.__LoadMap)),
			(30, ui.__mem_func__(self.__LoadSound)),
			(40, ui.__mem_func__(self.__LoadEffect)),
			(50, ui.__mem_func__(self.__LoadWarrior)),
			(60, ui.__mem_func__(self.__LoadAssassin)),
			(70, ui.__mem_func__(self.__LoadSura)),
			(80, ui.__mem_func__(self.__LoadShaman)),
			(85, ui.__mem_func__(self.__LoadWolfman)),
			(90, ui.__mem_func__(self.__LoadSkill)),
			(93, ui.__mem_func__(self.__LoadEnemy)),
			(97, ui.__mem_func__(self.__LoadNPC)),
			(98, ui.__mem_func__(self.__LoadRaceHeight)),

			# GUILD_BUILDING
			(99, ui.__mem_func__(self.__LoadGuildBuilding)),
			# END_OF_GUILD_BUILDING

			(100, ui.__mem_func__(self.__StartGame)),
		]

		tmpLoadStepList = tuple(zip(*self.loadStepList))[0]
		for progress in range(tmpLoadStepList[0], tmpLoadStepList[-1] + 1):
			if progress not in tmpLoadStepList:
				self.loadStepList.append((progress, lambda: None))
		self.loadStepList.sort()
		self.__SetProgress(0)
		#self.__SetNext(self.__LoadMap)

	def OnUpdate(self):
		if len(self.loadStepList)>0:
			(progress, runFunc)=self.loadStepList[0]

			try:
				runFunc()
			except:	
				self.errMsg.Show()
				self.loadStepList=[]

				## 이곳에서 syserr.txt 를 보낸다.

				import dbg
				dbg.TraceError(" !!! Failed to load game data : STEP [%d]" % (progress))

				#import shutil
				#import os
				#shutil.copyfile("syserr.txt", "errorlog.txt")
				#os.system("errorlog.exe")

				app.Exit()

				return

			self.loadStepList.pop(0)

			self.__SetProgress(progress)

	def __InitData(self):
		playersettingmodule.LoadGameData("INIT")

	def __RegisterDungeonMapName(self):
		background.RegisterDungeonMapName("metin2_map_spiderdungeon")
		background.RegisterDungeonMapName("metin2_map_monkeydungeon")
		background.RegisterDungeonMapName("metin2_map_monkeydungeon_02")
		background.RegisterDungeonMapName("metin2_map_monkeydungeon_03")
		background.RegisterDungeonMapName("metin2_map_deviltower1")

	def __RegisterSkill(self):

		race = net.GetMainActorRace()
		group = net.GetMainActorSkillGroup()
		empire = net.GetMainActorEmpire()

		playersettingmodule.RegisterSkill(race, group, empire)

	def __RegisterTitleName(self):
		for i in xrange(len(localeInfo.TITLE_NAME_LIST)):
			chrmgr.RegisterTitleName(i, localeInfo.TITLE_NAME_LIST[i])
			
	def __RegisterColor(self):

		## Name
		NAME_COLOR_DICT = {
			chrmgr.NAMECOLOR_PC : colorinfo.CHR_NAME_RGB_PC,
			chrmgr.NAMECOLOR_NPC : colorinfo.CHR_NAME_RGB_NPC,
			chrmgr.NAMECOLOR_MOB : colorinfo.CHR_NAME_RGB_MOB,
			chrmgr.NAMECOLOR_PVP : colorinfo.CHR_NAME_RGB_PVP,
			chrmgr.NAMECOLOR_PK : colorinfo.CHR_NAME_RGB_PK,
			chrmgr.NAMECOLOR_PARTY : colorinfo.CHR_NAME_RGB_PARTY,
			chrmgr.NAMECOLOR_WARP : colorinfo.CHR_NAME_RGB_WARP,
			chrmgr.NAMECOLOR_WAYPOINT : colorinfo.CHR_NAME_RGB_WAYPOINT,
			chrmgr.NAMECOLOR_SHOP : colorinfo.CHR_NAME_RGB_SHOP,
			chrmgr.NAMECOLOR_SUPPORT : colorinfo.CHR_NAMR_RGB_SUPPORT,

			chrmgr.NAMECOLOR_EMPIRE_MOB : colorinfo.CHR_NAME_RGB_EMPIRE_MOB,
			chrmgr.NAMECOLOR_EMPIRE_NPC : colorinfo.CHR_NAME_RGB_EMPIRE_NPC,
			chrmgr.NAMECOLOR_EMPIRE_PC+1 : colorinfo.CHR_NAME_RGB_EMPIRE_PC_A,
			chrmgr.NAMECOLOR_EMPIRE_PC+2 : colorinfo.CHR_NAME_RGB_EMPIRE_PC_B,
			chrmgr.NAMECOLOR_EMPIRE_PC+3 : colorinfo.CHR_NAME_RGB_EMPIRE_PC_C,
			
			chrmgr.NAMECOLOR_STONE : colorinfo.CHR_NAME_RGB_METIN,
		}
		for name, rgb in NAME_COLOR_DICT.items():
			chrmgr.RegisterNameColor(name, rgb[0], rgb[1], rgb[2])
			
		## Title
		TITLE_COLOR_DICT = (	colorinfo.TITLE_RGB_GOOD_7,
								colorinfo.TITLE_RGB_GOOD_6,
								colorinfo.TITLE_RGB_GOOD_5,
								colorinfo.TITLE_RGB_GOOD_4,
								colorinfo.TITLE_RGB_GOOD_3,
								colorinfo.TITLE_RGB_GOOD_2,
								colorinfo.TITLE_RGB_GOOD_1,
								colorinfo.TITLE_RGB_NORMAL,
								colorinfo.TITLE_RGB_EVIL_1,
								colorinfo.TITLE_RGB_EVIL_2,
								colorinfo.TITLE_RGB_EVIL_3,
								colorinfo.TITLE_RGB_EVIL_4,	)
		count = 0
		for rgb in TITLE_COLOR_DICT:
			chrmgr.RegisterTitleColor(count, rgb[0], rgb[1], rgb[2])
			count += 1
			
	def __RegisterEmotionIcon(self):
		emotion.RegisterEmotionIcons()

	def __LoadMap(self):
		net.Warp(self.playerX, self.playerY)

	def __LoadSound(self):
		constInfo.MuzikKontrol = False
		playersettingmodule.LoadGameData("SOUND")
		

	def __LoadEffect(self):
		playersettingmodule.LoadGameData("EFFECT")

	def __LoadWarrior(self):
		playersettingmodule.LoadGameData("WARRIOR")

	def __LoadAssassin(self):
		playersettingmodule.LoadGameData("ASSASSIN")

	def __LoadSura(self):
		playersettingmodule.LoadGameData("SURA")

	def __LoadShaman(self):
		playersettingmodule.LoadGameData("SHAMAN")
		
	def __LoadWolfman(self):
		playersettingmodule.LoadGameData("WOLFMAN")

	def __LoadSkill(self):
		playersettingmodule.LoadGameData("SKILL")

	def __LoadEnemy(self):
		playersettingmodule.LoadGameData("ENEMY")

	def __LoadNPC(self):
		playersettingmodule.LoadGameData("NPC")
		
	def __LoadRaceHeight(self):
		playersettingmodule.LoadGameData("RACE_HEIGHT")

	# GUILD_BUILDING
	def __LoadGuildBuilding(self):
		playersettingmodule.LoadGuildBuildingList(localeInfo.GUILD_BUILDING_LIST_TXT)
	# END_OF_GUILD_BUILDING

	def __StartGame(self):
		background.SetViewDistanceSet(background.DISTANCE0, 6400)
		"""
		background.SetViewDistanceSet(background.DISTANCE1, 19200)
		background.SetViewDistanceSet(background.DISTANCE2, 12800)
		background.SetViewDistanceSet(background.DISTANCE3, 9600)
		background.SetViewDistanceSet(background.DISTANCE4, 25600)
		"""
		background.SelectViewDistanceNum(background.DISTANCE0)
		if app.ENABLE_GRAPHIC_MASK:
			background.LoadGraphicMaskSettings()

		app.SetGlobalCenterPosition(self.playerX, self.playerY)

		net.StartGame()
