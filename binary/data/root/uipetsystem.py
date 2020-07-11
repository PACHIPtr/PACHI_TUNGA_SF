import player
import net
import app
import item
import chat
import ime
import skillpet
import os
import ui
import mouseModule
import snd
import grp
import uiScriptLocale
import localeInfo
import constInfo
import wndMgr
import uipetfeed
import uiToolTip
import uipetsystem
import interfaceModule
import dbg
import uiCommon

AFFECT_DICT = {
		item.APPLY_MAX_HP : localeInfo.TOOLTIP_MAX_HP,
		item.APPLY_MAX_SP : localeInfo.TOOLTIP_MAX_SP,
		item.APPLY_CON : localeInfo.TOOLTIP_CON,
		item.APPLY_INT : localeInfo.TOOLTIP_INT,
		item.APPLY_STR : localeInfo.TOOLTIP_STR,
		item.APPLY_DEX : localeInfo.TOOLTIP_DEX,
		item.APPLY_ATT_SPEED : localeInfo.TOOLTIP_ATT_SPEED,
		item.APPLY_MOV_SPEED : localeInfo.TOOLTIP_MOV_SPEED,
		item.APPLY_CAST_SPEED : localeInfo.TOOLTIP_CAST_SPEED,
		item.APPLY_HP_REGEN : localeInfo.TOOLTIP_HP_REGEN,
		item.APPLY_SP_REGEN : localeInfo.TOOLTIP_SP_REGEN,
		item.APPLY_POISON_PCT : localeInfo.TOOLTIP_APPLY_POISON_PCT,
		item.APPLY_STUN_PCT : localeInfo.TOOLTIP_APPLY_STUN_PCT,
		item.APPLY_SLOW_PCT : localeInfo.TOOLTIP_APPLY_SLOW_PCT,
		item.APPLY_CRITICAL_PCT : localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,
		item.APPLY_PENETRATE_PCT : localeInfo.TOOLTIP_APPLY_PENETRATE_PCT,

		item.APPLY_ATTBONUS_WARRIOR : localeInfo.TOOLTIP_APPLY_ATTBONUS_WARRIOR,
		item.APPLY_ATTBONUS_ASSASSIN : localeInfo.TOOLTIP_APPLY_ATTBONUS_ASSASSIN,
		item.APPLY_ATTBONUS_SURA : localeInfo.TOOLTIP_APPLY_ATTBONUS_SURA,
		item.APPLY_ATTBONUS_SHAMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_SHAMAN,
		item.APPLY_ATTBONUS_MONSTER : localeInfo.TOOLTIP_APPLY_ATTBONUS_MONSTER,

		item.APPLY_ATTBONUS_HUMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_HUMAN,
		item.APPLY_ATTBONUS_ANIMAL : localeInfo.TOOLTIP_APPLY_ATTBONUS_ANIMAL,
		item.APPLY_ATTBONUS_ORC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ORC,
		item.APPLY_ATTBONUS_MILGYO : localeInfo.TOOLTIP_APPLY_ATTBONUS_MILGYO,
		item.APPLY_ATTBONUS_UNDEAD : localeInfo.TOOLTIP_APPLY_ATTBONUS_UNDEAD,
		item.APPLY_ATTBONUS_DEVIL : localeInfo.TOOLTIP_APPLY_ATTBONUS_DEVIL,
		item.APPLY_STEAL_HP : localeInfo.TOOLTIP_APPLY_STEAL_HP,
		item.APPLY_STEAL_SP : localeInfo.TOOLTIP_APPLY_STEAL_SP,
		item.APPLY_MANA_BURN_PCT : localeInfo.TOOLTIP_APPLY_MANA_BURN_PCT,
		item.APPLY_DAMAGE_SP_RECOVER : localeInfo.TOOLTIP_APPLY_DAMAGE_SP_RECOVER,
		item.APPLY_BLOCK : localeInfo.TOOLTIP_APPLY_BLOCK,
		item.APPLY_DODGE : localeInfo.TOOLTIP_APPLY_DODGE,
		item.APPLY_RESIST_SWORD : localeInfo.TOOLTIP_APPLY_RESIST_SWORD,
		item.APPLY_RESIST_TWOHAND : localeInfo.TOOLTIP_APPLY_RESIST_TWOHAND,
		item.APPLY_RESIST_DAGGER : localeInfo.TOOLTIP_APPLY_RESIST_DAGGER,
		item.APPLY_RESIST_BELL : localeInfo.TOOLTIP_APPLY_RESIST_BELL,
		item.APPLY_RESIST_FAN : localeInfo.TOOLTIP_APPLY_RESIST_FAN,
		item.APPLY_RESIST_BOW : localeInfo.TOOLTIP_RESIST_BOW,
		item.APPLY_RESIST_FIRE : localeInfo.TOOLTIP_RESIST_FIRE,
		item.APPLY_RESIST_ELEC : localeInfo.TOOLTIP_RESIST_ELEC,
		item.APPLY_RESIST_MAGIC : localeInfo.TOOLTIP_RESIST_MAGIC,
		item.APPLY_RESIST_WIND : localeInfo.TOOLTIP_APPLY_RESIST_WIND,
		item.APPLY_REFLECT_MELEE : localeInfo.TOOLTIP_APPLY_REFLECT_MELEE,
		item.APPLY_REFLECT_CURSE : localeInfo.TOOLTIP_APPLY_REFLECT_CURSE,
		item.APPLY_POISON_REDUCE : localeInfo.TOOLTIP_APPLY_POISON_REDUCE,
		item.APPLY_KILL_SP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_SP_RECOVER,
		item.APPLY_EXP_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_EXP_DOUBLE_BONUS,
		item.APPLY_GOLD_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_GOLD_DOUBLE_BONUS,
		item.APPLY_ITEM_DROP_BONUS : localeInfo.TOOLTIP_APPLY_ITEM_DROP_BONUS,
		item.APPLY_POTION_BONUS : localeInfo.TOOLTIP_APPLY_POTION_BONUS,
		item.APPLY_KILL_HP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_HP_RECOVER,
		item.APPLY_IMMUNE_STUN : localeInfo.TOOLTIP_APPLY_IMMUNE_STUN,
		item.APPLY_IMMUNE_SLOW : localeInfo.TOOLTIP_APPLY_IMMUNE_SLOW,
		item.APPLY_IMMUNE_FALL : localeInfo.TOOLTIP_APPLY_IMMUNE_FALL,
		item.APPLY_BOW_DISTANCE : localeInfo.TOOLTIP_BOW_DISTANCE,
		item.APPLY_DEF_GRADE_BONUS : localeInfo.TOOLTIP_DEF_GRADE,
		item.APPLY_ATT_GRADE_BONUS : localeInfo.TOOLTIP_ATT_GRADE,
		item.APPLY_MAGIC_ATT_GRADE : localeInfo.TOOLTIP_MAGIC_ATT_GRADE,
		item.APPLY_MAGIC_DEF_GRADE : localeInfo.TOOLTIP_MAGIC_DEF_GRADE,
		item.APPLY_MAX_STAMINA : localeInfo.TOOLTIP_MAX_STAMINA,
		item.APPLY_MALL_ATTBONUS : localeInfo.TOOLTIP_MALL_ATTBONUS,
		item.APPLY_MALL_DEFBONUS : localeInfo.TOOLTIP_MALL_DEFBONUS,
		item.APPLY_MALL_EXPBONUS : localeInfo.TOOLTIP_MALL_EXPBONUS,
		item.APPLY_MALL_ITEMBONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS,
		item.APPLY_MALL_GOLDBONUS : localeInfo.TOOLTIP_MALL_GOLDBONUS,
		item.APPLY_SKILL_DAMAGE_BONUS : localeInfo.TOOLTIP_SKILL_DAMAGE_BONUS,
		item.APPLY_NORMAL_HIT_DAMAGE_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DAMAGE_BONUS,
		item.APPLY_SKILL_DEFEND_BONUS : localeInfo.TOOLTIP_SKILL_DEFEND_BONUS,
		item.APPLY_NORMAL_HIT_DEFEND_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DEFEND_BONUS,
		item.APPLY_RESIST_WARRIOR : localeInfo.TOOLTIP_APPLY_RESIST_WARRIOR,
		item.APPLY_RESIST_ASSASSIN : localeInfo.TOOLTIP_APPLY_RESIST_ASSASSIN,
		item.APPLY_RESIST_SURA : localeInfo.TOOLTIP_APPLY_RESIST_SURA,
		item.APPLY_RESIST_SHAMAN : localeInfo.TOOLTIP_APPLY_RESIST_SHAMAN,
		item.APPLY_RESIST_WOLFMAN : localeInfo.TOOLTIP_APPLY_RESIST_WOLFMAN,
		item.APPLY_MAX_HP_PCT : localeInfo.TOOLTIP_APPLY_MAX_HP_PCT,
		item.APPLY_MAX_SP_PCT : localeInfo.TOOLTIP_APPLY_MAX_SP_PCT,
		item.APPLY_ENERGY : localeInfo.TOOLTIP_ENERGY,
		item.APPLY_COSTUME_ATTR_BONUS : localeInfo.TOOLTIP_COSTUME_ATTR_BONUS,
		
		item.APPLY_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MAGIC_ATTBONUS_PER,
		item.APPLY_MELEE_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MELEE_MAGIC_ATTBONUS_PER,
		item.APPLY_RESIST_ICE : localeInfo.TOOLTIP_RESIST_ICE,
		item.APPLY_RESIST_EARTH : localeInfo.TOOLTIP_RESIST_EARTH,
		item.APPLY_RESIST_DARK : localeInfo.TOOLTIP_RESIST_DARK,
		item.APPLY_ANTI_CRITICAL_PCT : localeInfo.TOOLTIP_ANTI_CRITICAL_PCT,
		item.APPLY_ANTI_PENETRATE_PCT : localeInfo.TOOLTIP_ANTI_PENETRATE_PCT,
		item.APPLY_ATTBONUS_BOSS	:	localeInfo.TOOLTIP_APPLY_ATTBONUS_BOSS,
	}
	
	
def checkdiv(n):
	x = str(n/10.0)
	if len(x) > 3:
		return str(x)[0:3]
	return str(x)

def pointop(n):
	t = int(n)
	if t / 10 < 1:
		return "0."+n
	else:		
		return n[0:len(n)-1]+"."+n[len(n)-1:]
		
		
def GetAffectString(affectType, affectValue):
	if 0 == affectType:
		return None

	if 0 == affectValue:
		return None

	try:
		return AFFECT_DICT[affectType](affectValue)
	except TypeError:
		return "UNKNOWN_VALUE[%s] %s" % (affectType, affectValue)
	except KeyError:
		return "UNKNOWN_TYPE[%s] %s" % (affectType, affectValue)
		
class PetSystemMain(ui.ScriptWindow):
	class TextToolTip(ui.Window):
		def __init__(self, y):
			ui.Window.__init__(self, "TOP_MOST")

			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetHorizontalAlignLeft()
			textLine.SetOutline()
			textLine.Show()
			self.y = y
			self.textLine = textLine

		def __del__(self):
			ui.Window.__del__(self)

		def SetText(self, text):
			self.textLine.SetText(text)

		def OnRender(self):
			(mouseX, mouseY) = wndMgr.GetMousePosition()
			self.textLine.SetPosition(mouseX, mouseY - 60 + self.y)

	def __init__(self, vnum = 0):
		ui.ScriptWindow.__init__(self)
		self.vnum = vnum
		self.interface = None
		self.__LoadWindow()
		if (app.ENABLE_PET_ATTR_DETERMINE):
			self.slotPositionsDict = {}
		
	def BindInterface(self, interface):
		self.interface = interface

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		
		if (app.ENABLE_PET_ATTR_DETERMINE):
			self.wndPetChangeSlot.ClearSlot(0)
			self.wndPetChangeSlot.ClearSlot(1)
			self.wndPetChangeSlot.ClearSlot(2)
			self.petDetermineInfoText.SetText("")
			self.petDetermineInfoText2.SetText("")
			self.slotPositionsDict = {}
	
		self.Hide()
		self.feedwind.Close()
	
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PetInformationWindow.py")
		except:
			import exception as exception
			exception.Abort("PetInformationWindow.LoadWindow.LoadObject")
			
		try:
			self.feedwind = uipetfeed.PetFeedWindow()
			self.board = self.GetChild("board")
			if (not app.ENABLE_PET_ATTR_DETERMINE):
				self.boardclose = self.GetChild("CloseButton")
			
			self.slotimgpet = self.GetChild("UpBringing_Pet_Slot")
			self.evolname = self.GetChild("EvolName")
			self.petname = self.GetChild("PetName")
						
			self.expwind = self.GetChild("UpBringing_Pet_EXP_Gauge_Board")
			self.tooltipexp = []
			for i in range(0,4):
				self.tooltipexp.append(self.TextToolTip(15*i))
				self.tooltipexp[i].Hide()
			
			self.petlifeg = self.GetChild("LifeGauge")
			
			self.petlevel = self.GetChild("LevelValue")
			self.petexpa = self.GetChild("UpBringing_Pet_EXPGauge_01")
			self.petexpb = self.GetChild("UpBringing_Pet_EXPGauge_02")
			self.petexpc = self.GetChild("UpBringing_Pet_EXPGauge_03")
			self.petexpd = self.GetChild("UpBringing_Pet_EXPGauge_04")
			self.petexpe = self.GetChild("UpBringing_Pet_EXPGauge_05")
			self.petexppages = []			
			self.petexppages.append(self.petexpa)
			self.petexppages.append(self.petexpb)
			self.petexppages.append(self.petexpc)
			self.petexppages.append(self.petexpd)
			self.petexppages.append(self.petexpe)
			
			for exp in self.petexppages:
				exp.SetSize(0, 0)

			self.petages = self.GetChild("AgeValue")
			self.petdur = self.GetChild("LifeTextValue")
			
			# GaugeHP
			self.nutribtn = self.GetChild("FeedLifeTimeButton")
			self.sviluppobtn = self.GetChild("FeedEvolButton")
			self.itemexp = self.GetChild("FeedExpButton")
			
			self.pethp = self.GetChild("HpValue")
			self.petdef = self.GetChild("DefValue")
			self.petsp = self.GetChild("SpValue")
			
			self.petskill0 = self.GetChild("PetSkillSlot0")
			
			self.petskill0.SetSlot(0, 2, 32, 32, skillpet.GetEmptySkill())
			self.petskill0.SetSlot(1, 2, 32, 32, skillpet.GetEmptySkill())
			self.petskill0.SetSlot(2, 2, 32, 32, skillpet.GetEmptySkill())
			self.petskill0.SetSelectEmptySlotEvent(ui.__mem_func__(self.IncreaseSkill))
			self.petskill0.SetSelectItemSlotEvent(ui.__mem_func__(self.IncreaseSkill))
			self.petskill0.SAFE_SetButtonEvent("LEFT", "EMPTY", self.IncreaseSkill)
			self.petskill0.SAFE_SetButtonEvent("LEFT", "EXIST", self.IncreaseSkill)
			self.petskill0.SetUseSlotEvent(ui.__mem_func__(self.UseSkill))
			self.petskill0.SetOverInItemEvent(ui.__mem_func__(self.PetSkillTooltipShow))
			self.petskill0.SetOverOutItemEvent(ui.__mem_func__(self.PetSkillTooltipHide))	

			self.SetDefaultInfo()
			
			self.arrytooltip = [ [-1,-1], [-1,-1], [-1,-1]]
			PET_FILE_NAME = "%s/pet_skill.txt" % app.GetLocalePath()
			PET_FILE_SKILL = "%s/pet_skill_bonus.txt" % app.GetLocalePath()
			self.linespet = pack_open(PET_FILE_NAME,"r").readlines()
			self.linespetskill = pack_open(PET_FILE_SKILL,"r").readlines()
			self.SkillTooltip = uiToolTip.ToolTip(180)
			
			## Event
			if (not app.ENABLE_PET_ATTR_DETERMINE):
				self.boardclose.SetEvent(ui.__mem_func__(self.Close,))
			self.nutribtn.SetToggleDownEvent(lambda arg=0,arg1=1: self.OpenFeedBox(arg,arg1))
			self.nutribtn.SetToggleUpEvent(lambda arg=1,arg1=0: self.OpenFeedBox(arg,arg1))
			self.itemexp.SetToggleDownEvent(lambda arg=0,arg1=3: self.OpenFeedBox(arg,arg1))
			self.itemexp.SetToggleUpEvent(lambda arg=1,arg1=0: self.OpenFeedBox(arg,arg1))
			self.sviluppobtn.SetToggleDownEvent(lambda arg=0: self.evolution(arg))
			self.sviluppobtn.SetToggleUpEvent(lambda arg=1: self.evolution(arg))
			
			if (app.ENABLE_PET_ATTR_DETERMINE):
				self.tabImgs 				= [self.GetChild("Tab_01"), self.GetChild("Tab_02")]
				self.tabBtns				= [self.GetChild("Tab_Button_01"), self.GetChild("Tab_Button_02")]
				
				# Determine button configuration
				self.btnDetermine			= self.GetChild("DetermineButton")
				self.btnDetermine.SetEvent(ui.__mem_func__(self.__OnClickDetermineButton))
				
				# CLose Pet Info Window Button configuration
				self.btnClosePetInfoWnd		= self.GetChild("ClosePetInfoWndButton")
				self.btnClosePetInfoWnd.SetEvent(ui.__mem_func__(self.Close))
				
				# Close Pet Change Window Button configuration
				self.btnClosePetChangeWnd	= self.GetChild("ClosePetChangeWndButton")				
				self.btnClosePetChangeWnd.SetEvent(ui.__mem_func__(self.Close))				
				
				# Pet Change Button configuration
				self.btnPetChange			= self.GetChild("Pet_Change_Button")
				self.btnPetChange.SetEvent(ui.__mem_func__(self.__OnClickPetChangeButton))				
				
				# Pet OK Button configuration
				self.btnPetOk				= self.GetChild("Pet_OK_Button")
				self.btnPetOk.SetEvent(ui.__mem_func__(self.Close))
				
				# Tab Buttons configuration
				self.tabBtns[0].SAFE_SetEvent(self.__OnClickTabButton, 0)
				self.tabBtns[1].SAFE_SetEvent(self.__OnClickTabButton, 1)
				
				# Pet Change Slot configuration
				self.wndPetChangeSlot		= self.GetChild("Change_Pet_Slot")
				self.wndPetChangeSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectPetChangeSlotEmpty))
				self.wndPetChangeSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectPetChangeSlot))
				self.wndPetChangeSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInPetChangeSlotItem))
				self.wndPetChangeSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutPetChangeSlotItem))
				
				self.petAttrChangePage		= self.GetChild("PetAttrChange_Page")
				self.petDetermineInfoText	= self.GetChild("PetDetermineInfoText")
				self.petDetermineInfoText2	= self.GetChild("PetDetermineInfoText2")
				
				self.tabImgs[0].Show()
				self.tabImgs[1].Hide()
				
				self.tabBtns[0].Down()
				self.tabBtns[1].SetUp()
				
				self.petAttrChangePage.Hide()
		except:
			import exception as exception
			exception.Abort("PetInformationWindow.LoadWindow.BindObject")

	def PetSkillTooltipShow(self, slot):
		if self.arrytooltip[slot][0] > 0:
			tokens = self.linespet[self.arrytooltip[slot][0]-1][:-1].split("\t")
			tokens2 = self.linespetskill[self.arrytooltip[slot][0]-1][:-1].split("\t")
			self.SkillTooltip.ClearToolTip()
			self.SkillTooltip.AutoAppendTextLine(tokens[1], grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0))
			self.SkillTooltip.AppendDescription(tokens[4], 26)
			self.SkillTooltip.AppendSpace(5)

			if self.arrytooltip[slot][0] != 10 and self.arrytooltip[slot][0] != 17 and self.arrytooltip[slot][0] != 18:
				self.SkillTooltip.AutoAppendTextLine(GetAffectString(int(tokens2[1]), int(tokens2[self.arrytooltip[slot][1]+1])))
			elif self.arrytooltip[slot][0] == 10:
				self.SkillTooltip.AutoAppendTextLine(localeInfo.PET_SYSTEM_HP_RESTORED + str(tokens2[self.arrytooltip[slot][1]+1]))
			elif self.arrytooltip[slot][0] == 17:
				self.SkillTooltip.AutoAppendTextLine(localeInfo.PET_SYSTEM_IMMORTAL_TIME + checkdiv(int(tokens2[self.arrytooltip[slot][1]+1])) + "saniye")

			self.SkillTooltip.AutoAppendTextLine(localeInfo.PET_SYSTEM_SKILL_COOLDOWN + tokens[5] + "saniye", grp.GenerateColor(1.0, 0.7843, 0.0, 1.0))
			self.SkillTooltip.AlignHorizonalCenter()
			self.SkillTooltip.ShowToolTip()

	def PetSkillTooltipHide(self):
		self.SkillTooltip.HideToolTip()
		
	def evolution(self, mode):
		if mode == 0:
			net.SendChatPacket("/petvoincrease")
			self.sviluppobtn.Enable()

	def SetDefaultInfo(self):
		self.evolname.SetText("")
		self.petname.SetText("")

		self.petlevel.SetText("")
		self.petages.SetText("")
		self.petdur.SetText("")

		self.pethp.SetText("")
		self.petdef.SetText("")
		self.petsp.SetText("")

		self.SetDuration("0", "0")
		self.SetAgeDuration("0")

		self.slotimgpet.ClearSlot(0)
		self.petskill0.ClearSlot(0)
		self.petskill0.ClearSlot(1)
		self.petskill0.ClearSlot(2)
		self.petskill0.SetSlot(0, 2, 32, 32, skillpet.GetEmptySkill())
		self.petskill0.SetSlot(1, 2, 32, 32, skillpet.GetEmptySkill())
		self.petskill0.SetSlot(2, 2, 32, 32, skillpet.GetEmptySkill())

		self.SetExperience(0,0,0)

		self.arrytooltip = [ [-1,-1], [-1,-1], [-1,-1]]

		self.nutribtn.Disable()
		self.sviluppobtn.Disable()
		self.itemexp.Disable()

	def OpenFeedBox(self, mode, btn):
		if constInfo.FEEDWIND == btn or constInfo.FEEDWIND == 0:
			if mode == 0:
				self.interface.GetInventoryHandle().SetPetFeedWindow(self.feedwind)
				self.feedwind.BindInterface(self.interface)
				self.feedwind.Show()
				constInfo.FEEDWIND = btn
			else:
				self.feedwind.Close()
				constInfo.FEEDWIND = 0
		else:
			self.nutribtn.Enable()
			self.sviluppobtn.Enable()
			self.itemexp.Enable()
			self.feedwind.Close()
			constInfo.FEEDWIND = 0
	
	def SetImageSlot(self, vnum):
		self.slotimgpet.SetItemSlot(0, int(vnum), 0)
		self.slotimgpet.SetAlwaysRenderCoverButton(0, True)
	
	def SetEvolveName(self, name):
		self.evolname.SetText(name)
	
	def SetName(self, name):
		if name != "":
			self.nutribtn.Enable()
			self.sviluppobtn.Enable()
			self.itemexp.Enable()
		else:
			self.nutribtn.Disable()
			self.sviluppobtn.Disable()
			self.itemexp.Disable()
			
		self.petname.SetText(name)
	
	def SetLevel(self, level):
		if int(level) == 40 or int(level) == 60 or int(level) == 80:
			constInfo.EVOLUTION = int(level)
		else:
			constInfo.EVOLUTION = 0
		self.petlevel.SetText(level)
			
	def SetAges(self, ages):
		self.petages.SetText(ages)
	
	def SetDuration(self, dur, durt):
		if dur == "0" and durt == "0":
			self.petdur.SetText("0"+localeInfo.PET_SYSTEM_ORE)
			self.petlifeg.SetPercentage(0, 100)
			return
		else:
			durtmp = app.GetGlobalTimeStamp()-(int(dur))
			dur1 = (int(durtmp)/3600)
			durabs = abs(dur1)
			durt1 = (int(durt)/3600)

			if int(dur) > 0:
				if int(durabs) > int(durt1):
					self.petlifeg.SetPercentage(150+13, 100)
				else:
					self.petlifeg.SetPercentage(int(durabs)*1.6, int(durt1))
				self.petlifeg.Show()
			else:
				self.petlifeg.Hide()

			self.petdur.SetText(str(durabs)+"/"+str(durt1)+localeInfo.PET_SYSTEM_ORE)
		
	def SetAgeDuration(self, age):
		agedur = (app.GetGlobalTimeStamp()-int(age))/86400
		if age == "0":
			self.SetAges("0"+localeInfo.PET_SYSTEM_ZILE)
			return
			
		agedurtmp = abs(agedur)
		self.SetAges(str(agedurtmp)+localeInfo.PET_SYSTEM_ZILE)
		
	def SetHp(self, hp):
		self.pethp.SetText(pointop(hp)+"%")
		
	def SetDef(self, deff):
		self.petdef.SetText(pointop(deff)+"%")
		
	def SetSp(self, sp):
		self.petsp.SetText(pointop(sp)+"%")
	
	def SetSkill(self, slot, idx, lv):
		if int(idx) != -1:
			self.petskill0.ClearSlot(int(slot))
			self.petskill0.SetPetSkillSlot(int(slot), int(idx), int(lv))
			self.petskill0.SetCoverButton(int(slot))
			self.petskill0.SetAlwaysRenderCoverButton(int(slot), True)
			self.arrytooltip[int(slot)][0] = int(idx)
			self.arrytooltip[int(slot)][1] = int(lv)
			
	def SetExperience(self, expm, expi, exptot):
		expm = int(expm)
		expi = int(expi)
		exptot = int(exptot)
		
		if exptot > 0:	
			totalexp = exptot
			totexpm = int( float(totalexp) / 100 * 90 )
			totexpi = totalexp - totexpm
			expi = min(expi, totexpi)
			expmp =  float(expm) / totexpm * 100
			expip =  float(expi) / totexpi * 100
		else:
			totalexp = 0
			totexpm = 0
			totexpi = 0
			expmp =  0
			expip =  0
			
		curPoint = int(min(expm, totexpm))
		curPoint = int(max(expm, 0))
		maxPoint = int(max(totexpm, 0))
		
		curPointi = int(min(expi, totexpi))
		curPointi = int(max(expi, 0))
		maxPointi = int(max(totexpi, 0))

		quarterPoint = maxPoint / 4
		quarterPointi = maxPointi 
		FullCount = 0
		FullCounti = 0

		if 0 != quarterPoint:
			FullCount = min(4, curPoint / quarterPoint)
			
		if 0 != quarterPointi:
			FullCounti = min(1, curPointi / quarterPointi)

		for i in xrange(4):
			self.petexppages[i].Hide()
			
		self.petexppages[4].Hide()

		for i in xrange(FullCount):
			self.petexppages[i].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.petexppages[i].Show()
			
		for i in xrange(FullCounti):
			self.petexppages[4].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.petexppages[4].Show()

		if 0 != quarterPoint:
			if FullCount < 4:
				Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
				self.petexppages[FullCount].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.petexppages[FullCount].Show()
				
		if 0 != quarterPointi:
			if FullCounti < 1:
				Percentage = float(curPointi % quarterPointi) / quarterPointi - 1.0
				self.petexppages[4].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.petexppages[4].Show()
			
		self.tooltipexp[0].SetText(localeInfo.PET_SYSTEM_RELEASE_EXP % expmp)
		self.tooltipexp[1].SetText(localeInfo.PET_SYSTEM_REST_EXP % (totexpm - expm, totexpm))
		self.tooltipexp[2].SetText(localeInfo.PET_SYSTEM_RELEASE_IEXP % expip)
		self.tooltipexp[3].SetText(localeInfo.PET_SYSTEM_REST_IEXP % (totexpi - expi, totexpi))
	
	def UseSkill(self, slot):
		net.SendChatPacket("/petskills "+str(slot))
		
	def IncreaseSkill(self, slotIndex):
		try:
			if constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1:
				return

			if (mouseModule.mouseController.isAttached()):
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				mouseModule.mouseController.DeattachObject()

				itemVnum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
				if not item.IsNewPetBooks(itemVnum):
					return

				if (player.SLOT_TYPE_SKILL_BOOK_INVENTORY != attachedSlotType and player.SLOT_TYPE_INVENTORY != attachedSlotType):
					return

				item.SelectItem(itemVnum)
				questionDialog = uiCommon.QuestionDialog()
				questionDialog.SetText(localeInfo.PET_SKILL_OPEN % (item.GetItemName()))
				questionDialog.SetAcceptEvent(ui.__mem_func__(self.AcceptDialogSkill))
				questionDialog.SetCancelEvent(ui.__mem_func__(self.CancelDialogSkill))
				questionDialog.Open()
				self.questionDialog = questionDialog
				self.questionDialog.attachedSlotPos = attachedSlotPos
		except Exception, e:
			dbg.TraceError("Exception : IncreaseSkill, %s" %(e))
	
	def AcceptDialogSkill(self):
		net.SendChatPacket("/petincreaseskill "+str(self.questionDialog.attachedSlotPos))
		self.questionDialog.Close()
		return True

	def CancelDialogSkill(self):
		self.questionDialog.Close()
		return False

	def OnUpdate(self):
		if constInfo.FEEDWIND == 0:
			self.nutribtn.Enable()
			self.itemexp.Enable()
		if True == self.expwind.IsIn():
			for i in range(0,4):				
				self.tooltipexp[i].Show()
		else:
			for i in range(0,4):				
				self.tooltipexp[i].Hide()
				
		if (app.ENABLE_PET_ATTR_DETERMINE):
			if (self.petAttrChangePage.IsShow() == True):
				if (self.slotPositionsDict.has_key(0)):
					itemIndex = player.GetItemIndex(self.slotPositionsDict[0][0], self.slotPositionsDict[0][1])
					if (constInfo.IS_NEW_PET_ITEM(itemIndex) == False):
						self.wndPetChangeSlot.ClearSlot(0)
						del self.slotPositionsDict[0]
				
				if (self.slotPositionsDict.has_key(1)):			
					itemIndex = player.GetItemIndex(self.slotPositionsDict[1][0], self.slotPositionsDict[1][1])
					if (itemIndex != 55033):
						self.wndPetChangeSlot.ClearSlot(1)
						del self.slotPositionsDict[1]
	
	if (app.ENABLE_PET_ATTR_DETERMINE):
		def SetItemToolTip(self, tooltipItem):
			self.tooltipItem = tooltipItem
			
		def __CanAddPetIntoSlot(self, invenType, invenPos):
			itemIndex = player.GetItemIndex(invenType, invenPos)
			if (constInfo.IS_NEW_PET_ITEM(itemIndex) == False):
				return False
				
			isActivated = player.GetItemMetinSocket(invenType, invenPos, 0)
			if (isActivated):
				return False
				
			remainTime = app.GetGlobalTimeStamp() - player.GetItemMetinSocket(invenType, invenPos, 2)
			if (remainTime == app.GetGlobalTimeStamp()):
				return False
				
			return True
			
		def UseItemSlot(self, invenType, invenPos):
			if (self.slotPositionsDict.has_key(2)):
				self.wndPetChangeSlot.ClearSlot(2)
				self.petDetermineInfoText.SetText("")
				self.petDetermineInfoText2.SetText("")
				
			itemIndex	= player.GetItemIndex(invenType, invenPos)
			if (self.slotPositionsDict.has_key(0) == False and self.__CanAddPetIntoSlot(invenType, invenPos)):
				self.slotPositionsDict[0] = (invenType, invenPos)
				self.wndPetChangeSlot.SetItemSlot(0, itemIndex, 0)
				return
			
			if (self.slotPositionsDict.has_key(1) == False and itemIndex == 55033):
				self.slotPositionsDict[1] = (invenType, invenPos)
				self.wndPetChangeSlot.SetItemSlot(1, itemIndex, 0)
				return
				
		def __OnSelectPetChangeSlotEmpty(self, selectedSlotPos):
		
			if (self.slotPositionsDict.has_key(2)):
				self.wndPetChangeSlot.ClearSlot(2)
				self.petDetermineInfoText.SetText("")
				self.petDetermineInfoText2.SetText("")
			
			if (mouseModule.mouseController.isAttached() == False):
				return
				
			if (selectedSlotPos == 2):
				mouseModule.mouseController.DeattachObject()
				return
				
			attachedSlotType		= mouseModule.mouseController.GetAttachedType()
			attachedSlotPos			= mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()
			
			attachedInvenType		= player.SlotTypeToInvenType(attachedSlotType)
			itemIndex				= player.GetItemIndex(attachedInvenType, attachedSlotPos)
			
			if (selectedSlotPos == 0 and self.__CanAddPetIntoSlot(attachedInvenType, attachedSlotPos) == False):
				return
				
			if (selectedSlotPos == 1 and itemIndex != 55033):
				return
			
			self.slotPositionsDict[selectedSlotPos] = (attachedInvenType, attachedSlotPos)
			self.wndPetChangeSlot.SetItemSlot(selectedSlotPos, itemIndex, 0)
			
		def __OnSelectPetChangeSlot(self, selectedSlotPos):
			
			if (mouseModule.mouseController.isAttached() == True or selectedSlotPos == 2):
				return
			
			del self.slotPositionsDict[selectedSlotPos]
			self.wndPetChangeSlot.ClearSlot(selectedSlotPos)
			
		def __OnOverInPetChangeSlotItem(self, slotIndex):
			
			if (not slotIndex in self.slotPositionsDict):
				return
				
			(invenType, invenPos) = self.slotPositionsDict[slotIndex]
			self.tooltipItem.SetInventoryItem(invenPos, invenType)
			
		def __OnOverOutPetChangeSlotItem(self):
			self.tooltipItem.Hide()
		
		def __OnClickDetermineButton(self):
			if (len(self.petname.GetText()) == 0):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CALL_PET_FIRST)
				return
				
			if (player.GetItemCountByVnumNew(55032) == 0):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MISSING_PET_ITEM)
				return
				
			net.SendChatPacket("/determine_pet")
			
		def __GetPetDetermineText(self, pet_type):
			myList = [localeInfo.PET_ATTR_DETERMINE_TYPE1, localeInfo.PET_ATTR_DETERMINE_TYPE2, localeInfo.PET_ATTR_DETERMINE_TYPE3, localeInfo.PET_ATTR_DETERMINE_TYPE4, localeInfo.PET_ATTR_DETERMINE_TYPE5, localeInfo.PET_ATTR_DETERMINE_TYPE6, localeInfo.PET_ATTR_DETERMINE_TYPE7, localeInfo.PET_ATTR_DETERMINE_TYPE8]
			return myList[pet_type]
			
		def OnResultPetAttrDetermine(self, pet_type):
			petDescription		= self.__GetPetDetermineText(pet_type)
			petDescription		= petDescription.split("\\n")
			
			self.popupDlg		= uiCommon.PopupDialog2()
			self.popupDlg.SetText1(petDescription[0])
			self.popupDlg.SetText2(petDescription[1])
			self.popupDlg.Open()
			
		def OnResultPetAttrChange(self, pet_type):
			(invenType, invenPos) = self.slotPositionsDict[0]		
			self.slotPositionsDict[2] = (invenType, invenPos)
			
			del self.slotPositionsDict[0]
			del self.slotPositionsDict[1]
			
			self.wndPetChangeSlot.ClearSlot(0)
			self.wndPetChangeSlot.ClearSlot(1)
			self.wndPetChangeSlot.SetItemSlot(2, player.GetItemIndex(invenType, invenPos), 0)
			
			petDescription = self.__GetPetDetermineText(pet_type)
			petDescription = petDescription.split("\\n")
			
			self.petDetermineInfoText.SetText(petDescription[0])
			self.petDetermineInfoText2.SetText(petDescription[1])
			
		def __OnClickPetChangeButton(self):
			if (len(self.slotPositionsDict) == 0 or len(self.slotPositionsDict) < 2):
				return
		
			(firstInvenType, firstSlotPos) 		= self.slotPositionsDict[0]
			(secondInvenType, secondSlotPos)	= self.slotPositionsDict[1]			
			net.SendChatPacket("/change_pet %d %d %d %d" % (firstInvenType, firstSlotPos, secondInvenType, secondSlotPos))
			
		def __OnClickTabButton(self, window_type):
			self.slotPositionsDict = {}
			self.wndPetChangeSlot.ClearSlot(0)
			self.wndPetChangeSlot.ClearSlot(1)
			self.wndPetChangeSlot.ClearSlot(2)
			self.petDetermineInfoText.SetText("")
			self.petDetermineInfoText2.SetText("")
			
			if (window_type == 0):
				self.GetChild("PetInfo_Page").Show()
				self.petAttrChangePage.Hide()
				self.tabImgs[0].Show()
				self.tabImgs[1].Hide()
				
				self.tabBtns[0].Down()
				self.tabBtns[1].SetUp()
			else:
				self.GetChild("PetInfo_Page").Hide()
				self.petAttrChangePage.Show()
				self.tabImgs[1].Show()
				self.tabImgs[0].Hide()
				
				self.tabBtns[1].Down()
				self.tabBtns[0].SetUp()			

class PetSystemMini(ui.ScriptWindow):
	class TextToolTip(ui.Window):
		def __init__(self, y):
			ui.Window.__init__(self, "TOP_MOST")

			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetHorizontalAlignLeft()
			textLine.SetOutline()
			textLine.Show()
			self.y = y
			self.textLine = textLine

		def __del__(self):
			ui.Window.__del__(self)

		def SetText(self, text):
			self.textLine.SetText(text)

		def OnRender(self):
			(mouseX, mouseY) = wndMgr.GetMousePosition()
			self.textLine.SetPosition(mouseX, mouseY - 60 + self.y)

	def __init__(self, vnum = 0):
		ui.ScriptWindow.__init__(self)
		self.vnum = vnum
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
	
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PetMiniInformationWindow.py")
		except:
			import exception as exception
			exception.Abort("PetMiniInformationWindow.LoadWindow.LoadObject")
			
		try:
			self.expwind = self.GetChild("pet_mini_info_exp_gauge_board")
			self.mainbg = self.GetChild("main_bg")
			self.mainicon = self.GetChild("main_slot_img")
			self.main_slot_img = self.GetChild("pet_icon_slot")
			self.tooltipexp = []
			for i in range(0,4):
				self.tooltipexp.append(self.TextToolTip(15*i))
				self.tooltipexp[i].Hide()
			self.pet_icon_slot_ani_img = self.GetChild("pet_icon_slot_ani_img")
			self.pet_mini_exp_01 = self.GetChild("pet_mini_EXPGauge_01")
			self.pet_mini_exp_02 = self.GetChild("pet_mini_EXPGauge_02")
			self.pet_mini_exp_03 = self.GetChild("pet_mini_EXPGauge_03")
			self.pet_mini_exp_04 = self.GetChild("pet_mini_EXPGauge_04")
			self.pet_mini_exp_05 = self.GetChild("pet_mini_EXPGauge_05")
			self.petmini_exp = []
			self.petmini_exp.append(self.pet_mini_exp_01)
			self.petmini_exp.append(self.pet_mini_exp_02)
			self.petmini_exp.append(self.pet_mini_exp_03)
			self.petmini_exp.append(self.pet_mini_exp_04)
			self.petmini_exp.append(self.pet_mini_exp_05)
			self.petlifeg = self.GetChild("LifeGauge")
			
			self.skillslot = self.GetChild("mini_skill_slot0")
			self.skillslot.SetSelectItemSlotEvent(ui.__mem_func__(self.UseSkill))
			self.skillslot.SetUseSlotEvent(ui.__mem_func__(self.UseSkill))
			self.main_slot_img.SetUseSlotEvent(ui.__mem_func__(self.OpenPet))
			self.main_slot_img.SetSelectItemSlotEvent(ui.__mem_func__(self.OpenPet))

			self.SetDefaultInfo()
		except:
			import exception as exception
			exception.Abort("PetMiniInformationWindow.LoadWindow.BindObject")
			
	def SetDefaultInfo(self):		
		self.SetDuration("0", "0")
		self.main_slot_img.ClearSlot(0)
		self.skillslot.ClearSlot(0)
		self.skillslot.ClearSlot(1)
		self.skillslot.ClearSlot(2)
		self.skillslot.SetSlotScale(0, 2, 16, 16, skillpet.GetEmptySkill(), 0.5, 0.5)
		self.skillslot.SetSlotScale(1, 2, 16, 16, skillpet.GetEmptySkill(), 0.5, 0.5)
		self.skillslot.SetSlotScale(2, 2, 16, 16, skillpet.GetEmptySkill(), 0.5, 0.5)
		self.SetExperience(0,0,0)
		
	def OpenPet(self):
		net.SendChatPacket("/gift")

	def SetImageSlot(self, vnum):
		self.main_slot_img.SetItemSlot(0, int(vnum), 0)
		self.main_slot_img.SetAlwaysRenderCoverButton(0, True)
			
	def SetDuration(self, dur, durt):
		durtmp = app.GetGlobalTimeStamp()-(int(dur))
		# durttmp = app.GetGlobalTimeStamp()-(int(durt))
		dur1 = (int(durtmp)/3600)
		durabs = abs(dur1)
		durt1 = (int(durt)/3600)
		
		if int(dur) > 0:
			if int(dur) >= 100:
				self.petlifeg.SetPercentage(100, 100)
			else:
				self.petlifeg.SetPercentage(int(durabs), int(durt1))
			self.petlifeg.Show()
		else:
			self.petlifeg.Hide()
			
	def SetSkill(self, slot, idx, lv):
		if int(idx) != -1:
			self.skillslot.ClearSlot(int(slot))
			self.skillslot.SetPetSkillSlot(int(slot), int(idx), int(lv), 0.5, 0.5)
			self.skillslot.SetCoverButton(int(slot), "d:/ymir work/ui/pet/mini_window/pet_slot_corvermini.sub", "d:/ymir work/ui/pet/mini_window/pet_slot_corvermini.sub", "d:/ymir work/ui/pet/mini_window/pet_slot_corvermini.sub" , "d:/ymir work/ui/pet/mini_window/pet_slot_corvermini.sub")
			self.skillslot.SetAlwaysRenderCoverButton(int(slot), True)

	def SetExperience(self, expm, expi, exptot):
		expm = int(expm)
		expi = int(expi)
		exptot = int(exptot)
		
		if exptot > 0:	
			totalexp = exptot
			totexpm = int( float(totalexp) / 100 * 90 )
			totexpi = totalexp - totexpm
			expi = min(expi, totexpi)
			expmp =  float(expm) / totexpm * 100
			expip =  float(expi) / totexpi * 100
		else:
			totalexp = 0
			totexpm = 0
			totexpi = 0
			expmp =  0
			expip =  0
			
		curPoint = int(min(expm, totexpm))
		curPoint = int(max(expm, 0))
		maxPoint = int(max(totexpm, 0))
		
		curPointi = int(min(expi, totexpi))
		curPointi = int(max(expi, 0))
		maxPointi = int(max(totexpi, 0))

		quarterPoint = maxPoint / 4
		quarterPointi = maxPointi 
		FullCount = 0
		FullCounti = 0

		if 0 != quarterPoint:
			FullCount = min(4, curPoint / quarterPoint)
			
		if 0 != quarterPointi:
			FullCounti = min(1, curPointi / quarterPointi)

		for i in xrange(4):
			self.petmini_exp[i].Hide()
			
		self.petmini_exp[4].Hide()

		for i in xrange(FullCount):
			self.petmini_exp[i].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.petmini_exp[i].Show()
			
		for i in xrange(FullCounti):
			self.petmini_exp[4].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.petmini_exp[4].Show()

		if 0 != quarterPoint:
			if FullCount < 4:
				Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
				self.petmini_exp[FullCount].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.petmini_exp[FullCount].Show()
				
		if 0 != quarterPointi:
			if FullCounti < 1:
				Percentage = float(curPointi % quarterPointi) / quarterPointi - 1.0
				self.petmini_exp[4].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.petmini_exp[4].Show()

		self.tooltipexp[0].SetText(localeInfo.PET_SYSTEM_RELEASE_EXP % expmp)
		self.tooltipexp[1].SetText(localeInfo.PET_SYSTEM_REST_EXP % (totexpm - expm, totexpm))
		self.tooltipexp[2].SetText(localeInfo.PET_SYSTEM_RELEASE_IEXP % expip)
		self.tooltipexp[3].SetText(localeInfo.PET_SYSTEM_REST_IEXP % (totexpi - expi, totexpi))
	
	def UseSkill(self, slot):
		net.SendChatPacket("/petskills "+str(slot))

	def OnUpdate(self):
		if True == self.expwind.IsIn():
			for i in range(0,4):				
				self.tooltipexp[i].Show()
		else:
			for i in range(0,4):				
				self.tooltipexp[i].Hide()
