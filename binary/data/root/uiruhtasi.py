import net
import app
import player
import item
import chr
import chrmgr
import skill
import chat
import ui
import wndMgr
import localeInfo

SKILL_INDEX_DICT = []
JOB_WARRIOR		= 0
JOB_ASSASSIN	= 1
JOB_SURA		= 2
JOB_SHAMAN		= 3
JOB_WOLFMAN		= 4

RACE_WARRIOR_M	= 0
RACE_ASSASSIN_W	= 1
RACE_SURA_M		= 2
RACE_SHAMAN_W	= 3
RACE_WARRIOR_W	= 4
RACE_ASSASSIN_M	= 5
RACE_SURA_W		= 6
RACE_SHAMAN_M	= 7
RACE_WOLFMAN_M	= 8

class RuhTasi(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		self.skilleritanimla()
		self.yenile()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0
		self.ruhslot = None
		self.zenslot = None
		self.kotuslot = None
		self.munzevislot = None
		self.skillslot = None
		self.derece = None
		self.derece2 = None
		self.aktifslot = None

	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY GAME OPTION DIALOG"
	
	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception as exception
			exception.Abort("OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			
			self.tek = GetObject("tek")
			self.hepsi = GetObject("hepsi")
			ruhslott = self.GetChild("ruhslot")
			zenslott = self.GetChild("zenslot")
			kotuslott = self.GetChild("kotuslot")
			munzevislott = self.GetChild("munzevislot")
			skillslott = self.GetChild("skillslot")
			derece = self.GetChild("derece")
			derece2 = self.GetChild("derece2")
			
			self.ruhslot = ruhslott
			self.zenslot = zenslott
			self.kotuslot = kotuslott
			self.munzevislot = munzevislott
			self.skillslot = skillslott
			self.derece = derece
			self.derece2 = derece2

			self.tek.SAFE_SetEvent(self.butonlar,1)
			self.hepsi.SAFE_SetEvent(self.butonlar,2)
			
		except:
			import exception as exception
			exception.Abort("OptionDialog.__Load_BindObject")
	
	def __Load(self):
		self.__Load_LoadScript("uiscript/RuhTasi.py")
		self.__Load_BindObject()
		self.SetCenterPosition()
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

	def butonlar(self, func):
		if self.aktifslot == None:
			chat.AppendChat(1,"Önce geliştirmek istediğin yeteneği seç!")
			return
		skillIndexx = player.GetSkillIndex(int(self.aktifslot+1))
		if func == 1:
			net.SendChatPacket("/ruhoku 1 " + str(skillIndexx))
		elif func == 2:
			net.SendChatPacket("/ruhoku 0 " + str(skillIndexx))
		return
		
	def OnUpdate(self):
		self.yenile()
	
	def skilleritanimla(self):
		global SKILL_INDEX_DICT

		SKILL_INDEX_DICT = {
			JOB_WARRIOR : { 
				1 : (1, 2, 3, 4, 5, 6, 0, 0, 0, 137, 0, 138, 0, 139, 0,), 
				2 : (16, 17, 18, 19, 20, 21, 0, 0, 0, 137, 0, 138, 0, 139, 0,), 
				"SUPPORT" : (122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 141, 142,),
			},
			JOB_ASSASSIN : { 
				1 : (31, 32, 33, 34, 35, 36, 0, 0, 0, 137, 0, 138, 0, 139, 0, 140,), 
				2 : (46, 47, 48, 49, 50, 51, 0, 0, 0, 137, 0, 138, 0, 139, 0, 140,), 
				"SUPPORT" : (122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 141, 142,),
			},
			JOB_SURA : { 
				1 : (61, 62, 63, 64, 65, 66, 0, 0, 137, 0, 138, 0, 139, 0,),
				2 : (76, 77, 78, 79, 80, 81, 0, 0, 137, 0, 138, 0, 139, 0,),
				"SUPPORT" : (122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 141, 142,),
			},
			JOB_SHAMAN : { 
				1 : (91, 92, 93, 94, 95, 96, 0, 0, 137, 0, 138, 0, 139, 0,),
				2 : (106, 107, 108, 109, 110, 111, 0, 0, 137, 0, 138, 0, 139, 0,),
				"SUPPORT" : (122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 141, 142,),
			},
			JOB_WOLFMAN : {1 : (170, 171, 172, 173, 174, 175, 0, 0, 137, 0, 138, 0, 139, 0,), "SUPPORT" : (122, 123, 121, 124, 125, 129, 0, 0, 130, 131,),}
		}
		
	def derecegetir(self):
		point, grade = player.GetAlignmentData()

		import colorinfo as colorInfo
		COLOR_DICT = {	0 : colorInfo.TITLE_RGB_GOOD_4,
						1 : colorInfo.TITLE_RGB_GOOD_3,
						2 : colorInfo.TITLE_RGB_GOOD_2,
						3 : colorInfo.TITLE_RGB_GOOD_1,
						4 : colorInfo.TITLE_RGB_NORMAL,
						5 : colorInfo.TITLE_RGB_EVIL_1,
						6 : colorInfo.TITLE_RGB_EVIL_2,
						7 : colorInfo.TITLE_RGB_EVIL_3,
						8 : colorInfo.TITLE_RGB_EVIL_4, }
		colorList = COLOR_DICT.get(grade, colorInfo.TITLE_RGB_NORMAL)
		gradeColor = ui.GenerateColor(colorList[0], colorList[1], colorList[2])
		
		self.derece.SetText(localeInfo.TITLE_NAME_LIST[grade])
		self.derece.SetPackedFontColor(gradeColor)
		self.derece2.SetText("Mevcut Derecen : "+str(point)+" - ")
	
	def yenile(self):
		self.ruhslot.SetItemSlot(0, 50513, player.GetItemCountByVnum(50513))
		self.zenslot.SetItemSlot(1, 70102, player.GetItemCountByVnum(70102))
		self.kotuslot.SetItemSlot(2, 71001, player.GetItemCountByVnum(71001))
		self.munzevislot.SetItemSlot(3, 71094, player.GetItemCountByVnum(71094))
		
		race = net.GetMainActorRace()
		group = net.GetMainActorSkillGroup()
		job = chr.RaceToJob(race)
		if SKILL_INDEX_DICT.has_key(job):
			if SKILL_INDEX_DICT[job].has_key(group):
				activeSkillList = SKILL_INDEX_DICT[job][group]
				for i in xrange(len(activeSkillList)):
					skillIndexx = activeSkillList[i]
					if i != 6 and i != 7:
						if i >= 0 and i <= 5:
							skillLevel = player.GetSkillLevel(i+1)
							skillGrade = player.GetSkillGrade(i+1)
							self.skillslot.SetSkillSlotNew(i, skillIndexx, skillGrade, skillLevel)
							self.skillslot.SetSlotCountNew(i, skillGrade, skillLevel)
							self.skillslot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
							self.skillslot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectSkill))

		self.ruhslot.RefreshSlot()
		self.zenslot.RefreshSlot()
		self.kotuslot.RefreshSlot()
		self.munzevislot.RefreshSlot()
		self.skillslot.RefreshSlot()
		self.derecegetir()

	def SelectSkill(self, value):
		skillIndexx = player.GetSkillIndex(value+1)
		skillGrade = player.GetSkillGrade(value+1)
		if skillGrade == 2:
			self.aktifslot = value
			chat.AppendChat(1,"Artık okumaya başlayabilirsin!")
			for i in xrange(0,6):
				if i == value:
					self.skillslot.ActivateSlot(i)
				else:
					self.skillslot.DeactivateSlot(i)
		else:
			chat.AppendChat(1,"Sadece grandmaster skill'leri seçebilirsin.")
			return
	
	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
