import app
import chr
import player
import item
import skill
import net
import ui
import localeInfo
import uiToolTip
import math
import dbg

# WEDDING
class LovePointImage(ui.ExpandedImageBox):

	FILE_PATH = "d:/ymir work/ui/pattern/LovePoint/"
	FILE_DICT = {
		0 : FILE_PATH + "01.dds",
		1 : FILE_PATH + "02.dds",
		2 : FILE_PATH + "02.dds",
		3 : FILE_PATH + "03.dds",
		4 : FILE_PATH + "04.dds",
		5 : FILE_PATH + "05.dds",
	}

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.loverName = ""
		self.lovePoint = 0

		self.toolTip = uiToolTip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __del__(self):
		ui.ExpandedImageBox.__del__(self)

	def SetLoverInfo(self, name, lovePoint):
		self.loverName = name
		self.lovePoint = lovePoint
		self.__Refresh()

	def OnUpdateLovePoint(self, lovePoint):
		self.lovePoint = lovePoint
		self.__Refresh()

	def __Refresh(self):
		self.lovePoint = max(0, self.lovePoint)
		self.lovePoint = min(100, self.lovePoint)

		if 0 == self.lovePoint:
			loveGrade = 0
		else:
			loveGrade = self.lovePoint / 25 + 1
		fileName = self.FILE_DICT.get(loveGrade, self.FILE_PATH+"00.dds")

		try:
			self.LoadImage(fileName)
		except:
			import dbg
			dbg.TraceError("LovePointImage.SetLoverInfo(lovePoint=%d) - LoadError %s" % (self.lovePoint, fileName))

		self.SetScale(0.7, 0.7)

		self.toolTip.ClearToolTip()
		self.toolTip.SetTitle(self.loverName)
		self.toolTip.AppendTextLine(localeInfo.AFF_LOVE_POINT % (self.lovePoint))
		self.toolTip.ResizeToolTip()

	def OnMouseOverIn(self):
		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		self.toolTip.HideToolTip()
# END_OF_WEDDING


class HorseImage(ui.ExpandedImageBox):

	FILE_PATH = "d:/ymir work/ui/pattern/HorseState/"

	FILE_DICT = {
		00 : FILE_PATH+"00.dds",
		01 : FILE_PATH+"00.dds",
		02 : FILE_PATH+"00.dds",
		03 : FILE_PATH+"00.dds",
		10 : FILE_PATH+"10.dds",
		11 : FILE_PATH+"11.dds",
		12 : FILE_PATH+"12.dds",
		13 : FILE_PATH+"13.dds",
		20 : FILE_PATH+"20.dds",
		21 : FILE_PATH+"21.dds",
		22 : FILE_PATH+"22.dds",
		23 : FILE_PATH+"23.dds",
		30 : FILE_PATH+"30.dds",
		31 : FILE_PATH+"31.dds",
		32 : FILE_PATH+"32.dds",
		33 : FILE_PATH+"33.dds",
	}

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		#self.textLineList = []
		self.toolTip = uiToolTip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __GetHorseGrade(self, level):
		if 0 == level:
			return 0

		return (level-1)/10 + 1

	def SetState(self, level, health, battery):
		#self.textLineList=[]
		self.toolTip.ClearToolTip()

		if level>0:

			try:
				grade = self.__GetHorseGrade(level)
				self.__AppendText(localeInfo.LEVEL_LIST[grade])
			except IndexError:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - Unknown Index" % (level, health, battery)
				return

			try:
				healthName=localeInfo.HEALTH_LIST[health]
				if len(healthName)>0:
					self.__AppendText(healthName)
			except IndexError:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - Unknown Index" % (level, health, battery)
				return

			if health>0:
				if battery==0:
					self.__AppendText(localeInfo.NEEFD_REST)

			try:
				fileName=self.FILE_DICT[health*10+battery]
			except KeyError:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - KeyError" % (level, health, battery)

			try:
				self.LoadImage(fileName)
			except:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - LoadError %s" % (level, health, battery, fileName)

		self.SetScale(0.7, 0.7)

	def __AppendText(self, text):

		self.toolTip.AppendTextLine(text)
		self.toolTip.ResizeToolTip()

		#x=self.GetWidth()/2
		#textLine = ui.TextLine()
		#textLine.SetParent(self)
		#textLine.SetSize(0, 0)
		#textLine.SetOutline()
		#textLine.Hide()
		#textLine.SetPosition(x, 40+len(self.textLineList)*16)
		#textLine.SetText(text)
		#self.textLineList.append(textLine)

	def OnMouseOverIn(self):
		#for textLine in self.textLineList:
		#	textLine.Show()

		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		#for textLine in self.textLineList:
		#	textLine.Hide()

		self.toolTip.HideToolTip()


# AUTO_POTION
class AutoPotionImage(ui.ExpandedImageBox):

	FILE_PATH_HP = "d:/ymir work/ui/pattern/auto_hpgauge/"
	FILE_PATH_SP = "d:/ymir work/ui/pattern/auto_spgauge/"

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.loverName = ""
		self.lovePoint = 0
		self.potionType = player.AUTO_POTION_TYPE_HP
		self.filePath = ""

		self.toolTip = uiToolTip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __del__(self):
		ui.ExpandedImageBox.__del__(self)

	def SetPotionType(self, type):
		self.potionType = type
		
		if player.AUTO_POTION_TYPE_HP == type:
			self.filePath = self.FILE_PATH_HP
		elif player.AUTO_POTION_TYPE_SP == type:
			self.filePath = self.FILE_PATH_SP
			

	def OnUpdateAutoPotionImage(self):
		self.__Refresh()

	def __Refresh(self):
		print "__Refresh"
	
		isActivated, currentAmount, totalAmount, slotIndex = player.GetAutoPotionInfo(self.potionType)
				
		if 5000000 > totalAmount:
			grade = 0
			
		if 3000000 < totalAmount:
			grade = 4
			if 15000000 < totalAmount:
				grade = 5			

		fmt = self.filePath + "%.2d.dds"
		fileName = fmt % grade
		
		print self.potionType, fileName

		try:
			self.LoadImage(fileName)
		except:
			import dbg
			dbg.TraceError("AutoPotionImage.__Refresh(potionType=%d) - LoadError %s" % (self.potionType, fileName))

		self.SetScale(0.7, 0.7)

		self.toolTip.ClearToolTip()
		
		if player.AUTO_POTION_TYPE_HP == type:
			self.toolTip.SetTitle(localeInfo.TOOLTIP_AUTO_POTION_HP)
			self.toolTip.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT_STRING	% (localeInfo.NumberToHealthPointString(totalAmount)))
		else:
			self.toolTip.SetTitle(localeInfo.TOOLTIP_AUTO_POTION_SP)
			self.toolTip.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT_STRING	% (localeInfo.NumberToManaPointString(totalAmount)))

		self.toolTip.ResizeToolTip()

	def OnMouseOverIn(self):
		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		self.toolTip.HideToolTip()
# END_OF_AUTO_POTION


class AffectImage(ui.ExpandedImageBox):

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.toolTipText = None
		self.isSkillAffect = True
		self.description = None
		self.endTime = 0
		self.affect = None
		self.isClocked = True

	def SetAffect(self, affect):
		self.affect = affect

	def GetAffect(self):
		return self.affect

	def SetToolTipText(self, text, x = 0, y = -19):

		if not self.toolTipText:
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetSize(0, 0)
			textLine.SetOutline()
			textLine.Hide()
			self.toolTipText = textLine
			
		self.toolTipText.SetText(text)
		w, h = self.toolTipText.GetTextSize()
		self.toolTipText.SetPosition(max(0, x + self.GetWidth()/2 - w/2), y)

	def SetDescription(self, description):
		self.description = description

	def SetDuration(self, duration):
		self.endTime = 0
		if duration > 0:
			self.endTime = app.GetGlobalTimeStamp() + duration

	def UpdateAutoPotionDescription(self):		
		
		potionType = 0
		if self.affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
			potionType = player.AUTO_POTION_TYPE_HP
		else:
			potionType = player.AUTO_POTION_TYPE_SP	
		
		isActivated, currentAmount, totalAmount, slotIndex = player.GetAutoPotionInfo(potionType)

		if self.affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
			self.SetToolTipText(localeInfo.TOOLTIP_REST_USABLE_COUNT_STRING % (localeInfo.NumberToHealthPointString(totalAmount)), 0, 80)
		else:
			self.SetToolTipText(localeInfo.TOOLTIP_REST_USABLE_COUNT_STRING % (localeInfo.NumberToManaPointString(totalAmount)), 0, 80)
		
	def SetClock(self, isClocked):
		self.isClocked = isClocked
		
	def UpdateDescription(self):
		if not self.isClocked:
			self.__UpdateDescription2()
			return
	
		if not self.description:
			return
			
		toolTip = self.description
		if self.endTime > 0:
			leftTime = localeInfo.SecondToDHM(self.endTime - app.GetGlobalTimeStamp())
			toolTip += " (%s : %s)" % (localeInfo.LEFT_TIME, leftTime)
		self.SetToolTipText(toolTip, 0, 80)
		
	#독일버전에서 시간을 제거하기 위해서 사용 
	def __UpdateDescription2(self):
		if not self.description:
			return

		toolTip = self.description
		self.SetToolTipText(toolTip, 0, 80)

	def SetSkillAffectFlag(self, flag):
		self.isSkillAffect = flag

	def IsSkillAffect(self):
		return self.isSkillAffect

	def OnMouseOverIn(self):
		if self.toolTipText:
			self.toolTipText.Show()

	def OnMouseOverOut(self):
		if self.toolTipText:
			self.toolTipText.Hide()
			
class AffectShower(ui.Window):

	MALL_DESC_IDX_START = 1000
	IMAGE_STEP = 25
	AFFECT_MAX_NUM = 32

	INFINITE_AFFECT_DURATION = 0x1FFFFFFF 
	
	AFFECT_DATA_DICT =	{
			chr.AFFECT_POISON : (localeInfo.SKILL_TOXICDIE, "d:/ymir work/ui/skill/common/affect/poison.sub"),
			chr.AFFECT_SLOW : (localeInfo.SKILL_SLOW, "d:/ymir work/ui/skill/common/affect/slow.sub"),
			chr.AFFECT_STUN : (localeInfo.SKILL_STUN, "d:/ymir work/ui/skill/common/affect/stun.sub"),
			chr.AFFECT_BLEEDING : (localeInfo.SKILL_BLEEDING, "d:/ymir work/ui/skill/common/affect/poison.sub"),

			chr.AFFECT_ATT_SPEED_POTION : (localeInfo.SKILL_INC_ATKSPD, "icon/item/27102.tga"),
			chr.AFFECT_MOV_SPEED_POTION : (localeInfo.SKILL_INC_MOVSPD, "icon/item/27105.tga"),
			chr.AFFECT_FISH_MIND : (localeInfo.SKILL_FISHMIND, "icon/item/27610.tga"),

			chr.AFFECT_JEONGWI : (localeInfo.SKILL_JEONGWI, "d:/ymir work/ui/skill/warrior/jeongwi_03.sub",),
			chr.AFFECT_GEOMGYEONG : (localeInfo.SKILL_GEOMGYEONG, "d:/ymir work/ui/skill/warrior/geomgyeong_03.sub",),
			chr.AFFECT_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			chr.AFFECT_GYEONGGONG : (localeInfo.SKILL_GYEONGGONG, "d:/ymir work/ui/skill/assassin/gyeonggong_03.sub",),
			chr.AFFECT_EUNHYEONG : (localeInfo.SKILL_EUNHYEONG, "d:/ymir work/ui/skill/assassin/eunhyeong_03.sub",),
			chr.AFFECT_GWIGEOM : (localeInfo.SKILL_GWIGEOM, "d:/ymir work/ui/skill/sura/gwigeom_03.sub",),
			chr.AFFECT_GONGPO : (localeInfo.SKILL_GONGPO, "d:/ymir work/ui/skill/sura/gongpo_03.sub",),
			chr.AFFECT_JUMAGAP : (localeInfo.SKILL_JUMAGAP, "d:/ymir work/ui/skill/sura/jumagap_03.sub"),
			chr.AFFECT_HOSIN : (localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin_03.sub",),
			chr.AFFECT_BOHO : (localeInfo.SKILL_BOHO, "d:/ymir work/ui/skill/shaman/boho_03.sub",),
			chr.AFFECT_KWAESOK : (localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok_03.sub",),
			chr.AFFECT_HEUKSIN : (localeInfo.SKILL_HEUKSIN, "d:/ymir work/ui/skill/sura/heuksin_03.sub",),
			chr.AFFECT_MUYEONG : (localeInfo.SKILL_MUYEONG, "d:/ymir work/ui/skill/sura/muyeong_03.sub",),
			chr.AFFECT_GICHEON : (localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon_03.sub",),
			chr.AFFECT_JEUNGRYEOK : (localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok_03.sub",),
			chr.AFFECT_PABEOP : (localeInfo.SKILL_PABEOP, "d:/ymir work/ui/skill/sura/pabeop_03.sub",),
			chr.AFFECT_FALLEN_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			28 : (localeInfo.SKILL_FIRE, "d:/ymir work/ui/skill/sura/hwayeom_03.sub",),
			chr.AFFECT_CHINA_FIREWORK : (localeInfo.SKILL_POWERFUL_STRIKE, "icon/item/50107.tga",),
			43 : (localeInfo.SKILL_JEOKRANG, "d:/ymir work/ui/skill/wolfman/red_possession_03.sub"),
			44 : (localeInfo.SKILL_CHEONGRANG, "d:/ymir work/ui/skill/wolfman/blue_possession_03.sub"),			
			chr.AFFECT_WIND_SHOES : (localeInfo.TOOLTIP_WIND_SHOES, "icon/item/72701.tga"),

			#64 - END
			chr.NEW_AFFECT_EXP_BONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS_STATIC, "icon/item/70005.tga",),
			chr.NEW_AFFECT_ITEM_BONUS : (localeInfo.TOOLTIP_MALL_ITEMBONUS_STATIC, "icon/item/70043.tga",),
			chr.NEW_AFFECT_SAFEBOX : (localeInfo.TOOLTIP_MALL_SAFEBOX, "icon/item/71009.tga",),
			chr.NEW_AFFECT_AUTOLOOT : (localeInfo.TOOLTIP_MALL_AUTOLOOT, "icon/item/70002.tga",),
			chr.NEW_AFFECT_FISH_MIND : (localeInfo.TOOLTIP_MALL_FISH_MIND, "icon/item/27610.tga",),
			chr.NEW_AFFECT_MARRIAGE_FAST : (localeInfo.TOOLTIP_MALL_MARRIAGE_FAST, "icon/item/71068.tga",),
			chr.NEW_AFFECT_GOLD_BONUS : (localeInfo.TOOLTIP_MALL_GOLDBONUS_STATIC, "icon/item/71017.tga",),

			chr.NEW_AFFECT_NO_DEATH_PENALTY : (localeInfo.TOOLTIP_APPLY_NO_DEATH_PENALTY, "icon/item/71004.tga"),
			chr.NEW_AFFECT_SKILL_BOOK_BONUS : (localeInfo.TOOLTIP_APPLY_SKILL_BOOK_BONUS, "icon/item/71094.tga"),
			chr.NEW_AFFECT_SKILL_BOOK_NO_DELAY : (localeInfo.TOOLTIP_APPLY_SKILL_BOOK_NO_DELAY, "icon/item/71001.tga"),
			
			# 자동물약 hp, sp
			chr.NEW_AFFECT_AUTO_HP_RECOVERY : (localeInfo.TOOLTIP_REST_USABLE_COUNT_STRING, "d:/ymir work/ui/pattern/auto_hpgauge/05.dds"),			
			chr.NEW_AFFECT_AUTO_SP_RECOVERY : (localeInfo.TOOLTIP_REST_USABLE_COUNT_STRING, "d:/ymir work/ui/pattern/auto_spgauge/05.dds"),
			#chr.NEW_AFFECT_AUTO_HP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),			
			#chr.NEW_AFFECT_AUTO_SP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub"),			

			MALL_DESC_IDX_START+player.POINT_MALL_ATTBONUS : (localeInfo.TOOLTIP_MALL_ATTBONUS_STATIC, "icon/item/71028.tga",),
			MALL_DESC_IDX_START+player.POINT_MALL_DEFBONUS : (localeInfo.TOOLTIP_MALL_DEFBONUS_STATIC, "icon/item/71030.tga",),
			MALL_DESC_IDX_START+player.POINT_MALL_EXPBONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS, "icon/item/70005.tga",),
			MALL_DESC_IDX_START+player.POINT_MALL_ITEMBONUS : (localeInfo.TOOLTIP_MALL_ITEMBONUS, "icon/item/70043.tga",),
			MALL_DESC_IDX_START+player.POINT_MALL_GOLDBONUS : (localeInfo.TOOLTIP_MALL_GOLDBONUS, "icon/item/71017.tga",),
			MALL_DESC_IDX_START+player.POINT_CRITICAL_PCT : (localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,"icon/item/71044.tga"),
			MALL_DESC_IDX_START+player.POINT_PENETRATE_PCT : (localeInfo.TOOLTIP_APPLY_PENETRATE_PCT, "icon/item/71045.tga"),
			MALL_DESC_IDX_START+player.POINT_MAX_HP_PCT : (localeInfo.TOOLTIP_MAX_HP_PCT, "icon/item/71027.tga"),
			MALL_DESC_IDX_START+player.POINT_MAX_SP_PCT : (localeInfo.TOOLTIP_MAX_SP_PCT, "icon/item/71029.tga"),
	}
	if app.ENABLE_DRAGON_SOUL_SYSTEM:
		# 용혼석 천, 지 덱.
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DRAGON_SOUL_DECK1] = (localeInfo.TOOLTIP_DRAGON_SOUL_DECK1, "d:/ymir work/ui/dragonsoul/buff_ds_sky1.tga")
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DRAGON_SOUL_DECK2] = (localeInfo.TOOLTIP_DRAGON_SOUL_DECK2, "d:/ymir work/ui/dragonsoul/buff_ds_land1.tga")

	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		AFFECT_DATA_DICT[chr.NEW_AFFECT_COMBAT_ZONE_POTION] = (localeInfo.COMBAT_ZONE_TOOLTIP_BATTLE_POTION, "icon/item/27125.tga")	
		
	if app.ENABLE_ATTENDANCE_EVENT:
		AFFECT_DATA_DICT[557] = (localeInfo.TOOLTIP_EXP_BONUS_EVENT, "d:/ymir work/ui/skill/common/affect/exp_bonus.sub")
		AFFECT_DATA_DICT[558] = (localeInfo.TOOLTIP_ATT_SPEED_SLOW, "d:/ymir work/ui/skill/common/affect/att_slow.sub")
	
	if app.ENABLE_SHOP_DECORATION_SYSTEM:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_SHOP_DECO] = (localeInfo.TOOLTIP_AFFECT_SHOP_DECO, "icon/item/71221.tga")
		
	if app.ENABLE_AUTO_HUNTING_SYSTEM:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_AUTO_HUNT] = (localeInfo.TOOLTIP_AFFECT_AUTO_HUNT, "d:/ymir work/ui/skill/common/affect/auto_premium.sub")
	
	AFFECT_DATA_DICT[903] = (localeInfo.TOOLTIP_PREMIUM_1, "d:/ymir work/ui/premium_eq_lot.dds")
	AFFECT_DATA_DICT[904] = (localeInfo.TOOLTIP_PREMIUM_2, "d:/ymir work/ui/premium_monsterhp.dds")
	AFFECT_DATA_DICT[905] = (localeInfo.TOOLTIP_PREMIUM_3, "d:/ymir work/ui/premium_ore.dds")
		
	if app.ENABLE_AFFECT_POLYMORPH_REMOVE:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_POLYMORPH] = (localeInfo.POLYMORPH_AFFECT_TOOLTIP, "d:/ymir work/ui/polymorph_marble_icon.tga")

	AFFECT_PET_DATA_DICT ={
		5401 : (localeInfo.PET_AFFECT_RESIST_WARRIOR, "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5402 : (localeInfo.PET_AFFECT_RESIST_SURA, "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5403 : (localeInfo.PET_AFFECT_RESIST_NINJA, "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5404 : (localeInfo.PET_AFFECT_RESIST_SHAMAN, "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5405 : (localeInfo.PET_AFFECT_RESIST_LYCAN, "d:/ymir work/ui/skill/pet/jijoong.sub"),
		5406 : (localeInfo.PET_AFFECT_BERSERKER, "d:/ymir work/ui/skill/pet/pacheon.sub"),
		5407 : (localeInfo.PET_AFFECT_ANTI_MAGIC, "d:/ymir work/ui/skill/pet/cheonryeong.sub"),
		5408 : (localeInfo.PET_AFFECT_MAGIC_SPEED, "d:/ymir work/ui/skill/pet/banya.sub"),
		5409 : (localeInfo.PET_AFFECT_PENETRATE, "d:/ymir work/ui/skill/pet/choehoenbimu.sub"),
		5410 : (localeInfo.PET_AFFECT_HEAL, "d:/ymir work/ui/skill/pet/heal.sub"),
		5411 : (localeInfo.PET_AFFECT_VAMPIRISM, "d:/ymir work/ui/skill/pet/stealhp.sub"),
		5412 : (localeInfo.PET_AFFECT_SPRITISM, "d:/ymir work/ui/skill/pet/stealmp.sub"),
		5413 : (localeInfo.PET_AFFECT_BLOCK, "d:/ymir work/ui/skill/pet/block.sub"),
		5414 : (localeInfo.PET_AFFECT_REFLECT_MELEE, "d:/ymir work/ui/skill/pet/reflect_melee.sub"),
		5415 : (localeInfo.PET_AFFECT_DROP_GOLD, "d:/ymir work/ui/skill/pet/gold_drop.sub"),
		5416 : (localeInfo.PET_AFFECT_BOW_DISTANCE, "d:/ymir work/ui/skill/pet/bow_distance.sub"),
		5417 : (localeInfo.PET_AFFECT_IMMORTAL, "d:/ymir work/ui/skill/pet/invincibility.sub"),
		5418 : (localeInfo.PET_AFFECT_HEAL2, "d:/ymir work/ui/skill/pet/removal.sub"),
	}

	def __init__(self):
		ui.Window.__init__(self)

		self.serverPlayTime=0
		self.clientPlayTime=0
		
		self.lastUpdateTime=0
		self.affectImageDict={}
		self.horseImage=None
		self.lovePointImage=None
		if app.ENABLE_AFFECT_POLYMORPH_REMOVE:
			self.polymorphQuestionDialog = None
		if app.ENABLE_SKILL_AFFECT_REMOVE:
			self.skillAffectQuestionDialog = None
		self.autoPotionImageHP = AutoPotionImage()
		self.autoPotionImageSP = AutoPotionImage()
		self.SetPosition(10, 10)
		self.Show()
		
	if app.ENABLE_AFFECT_POLYMORPH_REMOVE:
		def OnPolymorphQuestionDialog(self):
			import uiCommon
			self.polymorphQuestionDialog = uiCommon.QuestionDialog()
			self.polymorphQuestionDialog.SetText(localeInfo.POLYMORPH_AFFECT_REMOVE_QUESTION)
			self.polymorphQuestionDialog.SetWidth(350)
			self.polymorphQuestionDialog.SetAcceptEvent(lambda arg = True: self.OnClosePolymorphQuestionDialog(arg))
			self.polymorphQuestionDialog.SetCancelEvent(lambda arg = False: self.OnClosePolymorphQuestionDialog(arg))
			self.polymorphQuestionDialog.Open()
			
		def OnClosePolymorphQuestionDialog(self, answer):
			if not self.polymorphQuestionDialog:
				return

			self.polymorphQuestionDialog.Close()
			self.polymorphQuestionDialog = None
					
			if not answer:
				return

			net.SendChatPacket("/remove_polymorph")
			return True
			
	if app.ENABLE_SKILL_AFFECT_REMOVE:
		def OnSkillAffectQuestionDialog(self, affect):
			import uiCommon
			self.skillAffectQuestionDialog = uiCommon.QuestionDialog()
			self.skillAffectQuestionDialog.SetText(localeInfo.SKILL_AFFECT_REMOVE_QUESTION)
			self.skillAffectQuestionDialog.SetWidth(350)
			self.skillAffectQuestionDialog.SetAcceptEvent(lambda arg = True: self.OnCloseSkillAffectQuestionDialog(arg))
			self.skillAffectQuestionDialog.SetCancelEvent(lambda arg = False: self.OnCloseSkillAffectQuestionDialog(arg))
			self.skillAffectQuestionDialog.affect = affect
			self.skillAffectQuestionDialog.Open()
			
		def OnCloseSkillAffectQuestionDialog(self, answer):
			if not self.skillAffectQuestionDialog:
				return

			if not answer:
				self.skillAffectQuestionDialog.Close()
				self.skillAffectQuestionDialog = None
				return
					
			net.SendChatPacket("/remove_skill_affect {0}".format(int(self.skillAffectQuestionDialog.affect+1)))
			self.skillAffectQuestionDialog.Close()
			self.skillAffectQuestionDialog = None
			return True

	def ClearAllAffects(self):
		self.horseImage=None
		self.lovePointImage=None
		self.affectImageDict={}
		self.__ArrangeImageList()

	def ClearAffects(self): ## 스킬 이펙트만 없앱니다.
		self.living_affectImageDict={}
		for key, image in self.affectImageDict.items():
			if not image.IsSkillAffect():
				self.living_affectImageDict[key] = image
		self.affectImageDict = self.living_affectImageDict
		self.__ArrangeImageList()

	def BINARY_NEW_AddAffect(self, type, pointIdx, value, duration):

		print "BINARY_NEW_AddAffect", type, pointIdx, value, duration

		if app.ENABLE_BLEND_ICON_SYSTEM:
			if type < 500 and not type == AFFECT_POTION["affect"][0] and not type == AFFECT_POTION["affect"][1] and not type == AFFECT_POTION["affect"][2] and not type == AFFECT_POTION["affect"][3] and not type == AFFECT_POTION["affect"][4] and not type == AFFECT_POTION["affect"][5] and not type == AFFECT_POTION["affect"][6] and not type == AFFECT_POTION["affect"][7] and not type == AFFECT_POTION["affect"][8] and not type == AFFECT_POTION["affect"][9] and not type == AFFECT_POTION["affect"][10] and not type == AFFECT_POTION["affect"][11] and not type == AFFECT_POTION["affect"][12] and not type == AFFECT_POTION["affect"][13] and not type == AFFECT_POTION["affect"][14] and not type == AFFECT_POTION["affect"][15] and not type == AFFECT_POTION["affect"][16] and not type == AFFECT_POTION["affect"][17] and not type == AFFECT_POTION["affect"][18] and not type == 613  and not type == 614 and not type == 615 and not type == 616 and not type == 617 and not type == 618 and not type == 200 and not type == 201 and type != chr.NEW_AFFECT_POLYMORPH:
				return
		else:
			if type < 500:
				return
		
		if type == chr.NEW_AFFECT_MALL:
			affect = self.MALL_DESC_IDX_START + pointIdx
		else:
			affect = type

		if self.affectImageDict.has_key(affect):
			return

		if not self.AFFECT_DATA_DICT.has_key(affect) and not self.AFFECT_PET_DATA_DICT.has_key(affect):
			return

		## 용신의 가호, 선인의 교훈은 Duration 을 0 으로 설정한다.
		if affect == chr.NEW_AFFECT_NO_DEATH_PENALTY or\
		   affect == chr.NEW_AFFECT_SKILL_BOOK_BONUS or\
		   affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or\
		   affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY or\
		   affect == chr.NEW_AFFECT_SKILL_BOOK_NO_DELAY:
			duration = 0

		if affect >= 5400 and affect <= 5419:
			affectData = self.AFFECT_PET_DATA_DICT[affect]
		else:
			affectData = self.AFFECT_DATA_DICT[affect]
		description = affectData[0]
		filename = affectData[1]

		if pointIdx == player.POINT_MALL_ITEMBONUS or\
		   pointIdx == player.POINT_MALL_GOLDBONUS:
			value = 1 + float(value) / 100.0

		trashValue = 123
		#if affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
		if trashValue == 1:
			try:
				#image = AutoPotionImage()
				#image.SetParent(self)
				image = None
				
				if affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
					image.SetPotionType(player.AUTO_POTION_TYPE_SP)
					image = self.autoPotionImageSP
					#self.autoPotionImageSP = image;
				else:
					image.SetPotionType(player.AUTO_POTION_TYPE_HP)
					image = self.autoPotionImageHP
					#self.autoPotionImageHP = image;
				
				image.SetParent(self)
				image.Show()
				image.OnUpdateAutoPotionImage()
				
				self.affectImageDict[affect] = image
				self.__ArrangeImageList()
				
			except Exception, e:
				print "except Aff auto potion affect ", e
				pass				
			
		else:
			if affect != chr.NEW_AFFECT_AUTO_SP_RECOVERY and affect != chr.NEW_AFFECT_AUTO_HP_RECOVERY and not self.AFFECT_PET_DATA_DICT.has_key(affect):
				description = description(int(value))

			try:
				print "Add affect %s" % affect
				image = AffectImage()
				image.SetParent(self)
				image.LoadImage(filename)
				image.SetDescription(description)
				image.SetDuration(duration)
				image.SetAffect(affect)
				if affect == chr.NEW_AFFECT_EXP_BONUS_EURO_FREE or\
					affect == chr.NEW_AFFECT_EXP_BONUS_EURO_FREE_UNDER_15 or\
					self.INFINITE_AFFECT_DURATION < duration:
					image.SetClock(False)
					image.UpdateDescription()
				elif app.ENABLE_ATTENDANCE_EVENT and affect == 558:
					image.SetClock(False)
					image.UpdateDescription()	
				elif affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
					image.UpdateAutoPotionDescription()
				else:
					image.UpdateDescription()
					
				if affect == chr.NEW_AFFECT_DRAGON_SOUL_DECK1 or affect == chr.NEW_AFFECT_DRAGON_SOUL_DECK2:
					image.SetScale(1, 1)
				else:
					image.SetScale(0.7, 0.7)
				image.SAFE_SetStringEvent("MOUSE_LEFT_UP",self.__OnSelect, affect)
				image.SetSkillAffectFlag(False)
				image.Show()
				self.affectImageDict[affect] = image
				self.__ArrangeImageList()
			except Exception, e:
				print "except Aff affect ", e
				pass
				
	def __OnSelect(self, affect):
		if app.ENABLE_AFFECT_POLYMORPH_REMOVE:	
			if affect == chr.NEW_AFFECT_POLYMORPH:
				self.OnPolymorphQuestionDialog()
			elif affect == chr.AFFECT_HOSIN or affect == chr.AFFECT_BOHO or affect == chr.AFFECT_GICHEON or affect == chr.AFFECT_KWAESOK or affect == chr.AFFECT_JEUNGRYEOK:
				self.OnSkillAffectQuestionDialog(affect)

	def BINARY_NEW_RemoveAffect(self, type, pointIdx):
		if type == chr.NEW_AFFECT_MALL:
			affect = self.MALL_DESC_IDX_START + pointIdx
		else:
			affect = type
	
		print "Remove Affect %s %s" % ( type , pointIdx )
		self.__RemoveAffect(affect)
		self.__ArrangeImageList()

	def SetAffect(self, affect):
		self.__AppendAffect(affect)
		self.__ArrangeImageList()

	def ResetAffect(self, affect):
		self.__RemoveAffect(affect)
		self.__ArrangeImageList()

	def SetLoverInfo(self, name, lovePoint):
		image = LovePointImage()
		image.SetParent(self)
		image.SetLoverInfo(name, lovePoint)
		self.lovePointImage = image
		self.__ArrangeImageList()

	def ShowLoverState(self):
		if self.lovePointImage:
			self.lovePointImage.Show()
			self.__ArrangeImageList()

	def HideLoverState(self):
		if self.lovePointImage:
			self.lovePointImage.Hide()
			self.__ArrangeImageList()

	def ClearLoverState(self):
		self.lovePointImage = None
		self.__ArrangeImageList()

	def OnUpdateLovePoint(self, lovePoint):
		if self.lovePointImage:
			self.lovePointImage.OnUpdateLovePoint(lovePoint)

	def SetHorseState(self, level, health, battery):
		if level==0:
			self.horseImage=None
		else:
			image = HorseImage()
			image.SetParent(self)
			image.SetState(level, health, battery)
			image.Show()

			self.horseImage=image
			self.__ArrangeImageList()

	def SetPlayTime(self, playTime):
		self.serverPlayTime = playTime
		self.clientPlayTime = app.GetTime()
		
	def __AppendAffect(self, affect):

		if self.affectImageDict.has_key(affect):
			return

		try:
			affectData = self.AFFECT_DATA_DICT[affect]
		except KeyError:
			return

		name = affectData[0]
		filename = affectData[1]

		skillIndex = player.AffectIndexToSkillIndex(affect)
		if 0 != skillIndex:
			name = skill.GetSkillName(skillIndex)

		image = AffectImage()
		image.SetParent(self)
		image.SetSkillAffectFlag(True)
		image.SAFE_SetStringEvent("MOUSE_LEFT_UP",self.__OnSelect, affect)

		try:
			image.LoadImage(filename)
		except:
			pass

		image.SetToolTipText(name, 0, 80)
		image.SetScale(0.7, 0.7)
		image.Show()
		self.affectImageDict[affect] = image

	def __RemoveAffect(self, affect):
		"""
		if affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
			self.autoPotionImageSP.Hide()

		if affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
			self.autoPotionImageHP.Hide()
		"""
			
		if not self.affectImageDict.has_key(affect):
			print "__RemoveAffect %s ( No Affect )" % affect
			return

		print "__RemoveAffect %s ( Affect )" % affect
		del self.affectImageDict[affect]
		
		self.__ArrangeImageList()

	def __ArrangeImageList(self):

		xPos = 0
		yPos = 0
		AffectCount = 0

		if self.lovePointImage:
			if self.lovePointImage.IsShow():
				self.lovePointImage.SetPosition(xPos, 0)
				xPos += self.IMAGE_STEP
				AffectCount += 1

		if self.horseImage:
			self.horseImage.SetPosition(xPos, 0)
			xPos += self.IMAGE_STEP
			AffectCount += 1

		for image in self.affectImageDict.values():
			image.SetPosition(xPos, yPos)
			xPos += self.IMAGE_STEP
			AffectCount += 1
			if AffectCount % 8 == 0:
				yPos += self.IMAGE_STEP
				xPos = 0

		self.SetSize(8 * self.IMAGE_STEP, yPos + self.IMAGE_STEP)

	def OnUpdate(self):
		if chr.RaceToJob(net.GetMainActorRace()) == 4:
			if self.affectImageDict.has_key(chr.AFFECT_CHEONGEUN):
				self.__RemoveAffect(chr.AFFECT_CHEONGEUN)
				self.__ArrangeImageList()
		try:
			if app.GetGlobalTime() - self.lastUpdateTime > 500:
			#if 0 < app.GetGlobalTime():
				self.lastUpdateTime = app.GetGlobalTime()

				for image in self.affectImageDict.values():
					if image.GetAffect() == chr.NEW_AFFECT_AUTO_HP_RECOVERY or image.GetAffect() == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
						image.UpdateAutoPotionDescription()
						continue
						
					if not image.IsSkillAffect():
						image.UpdateDescription()
		except Exception, e:
			print "AffectShower::OnUpdate error : ", e


