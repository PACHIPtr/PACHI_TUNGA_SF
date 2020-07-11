import app
import chr
import player
import net
import skill
import quest
import event
import ui
import uiScriptLocale
import dbg
import snd
import mouseModule
import wndMgr
import playersettingmodule
import localeInfo
import uiToolTip
import constInfo
import emotion
import grp
if app.ENABLE_DETAILS_UI:
	import uiCharacterDetails
import uiQuest
import interfaceModule
import math

SHOW_ONLY_ACTIVE_SKILL = False
SHOW_LIMIT_SUPPORT_SKILL_LIST = []
HIDE_SUPPORT_SKILL_POINT = True

FACE_IMAGE_DICT = {
	playersettingmodule.RACE_WARRIOR_M	: "icon/face/warrior_m.tga",
	playersettingmodule.RACE_WARRIOR_W	: "icon/face/warrior_w.tga",
	playersettingmodule.RACE_ASSASSIN_M	: "icon/face/assassin_m.tga",
	playersettingmodule.RACE_ASSASSIN_W	: "icon/face/assassin_w.tga",
	playersettingmodule.RACE_SURA_M		: "icon/face/sura_m.tga",
	playersettingmodule.RACE_SURA_W		: "icon/face/sura_w.tga",
	playersettingmodule.RACE_SHAMAN_M	: "icon/face/shaman_m.tga",
	playersettingmodule.RACE_SHAMAN_W	: "icon/face/shaman_w.tga",
	playersettingmodule.RACE_WOLFMAN_M	: "icon/face/wolfman_m.tga",
}
def unsigned32(n):
	return n & 0xFFFFFFFFL
	
quest_slot_listbar = {
	"name" : "Quest_Slot",
	"type" : "listbar",
    
	"x" : 0,
	"y" : 0,

	"width" : 210,
	"height" : 20,
	"text" : "Quest title",
	"align" : "left",
	"horizontal_align" : "left",
	"vertical_align" : "left",
	"text_horizontal_align" : "left",
	"all_align" : "left",
	"text_height": 20
}
	
class CharacterWindow(ui.ScriptWindow):

	ACTIVE_PAGE_SLOT_COUNT = 8
	SUPPORT_PAGE_SLOT_COUNT = 12

	PAGE_SLOT_COUNT = 12
	PAGE_HORSE = 2

	SKILL_GROUP_NAME_DICT = {
		playersettingmodule.JOB_WARRIOR	: { 1 : localeInfo.SKILL_GROUP_WARRIOR_1,	2 : localeInfo.SKILL_GROUP_WARRIOR_2, },
		playersettingmodule.JOB_ASSASSIN	: { 1 : localeInfo.SKILL_GROUP_ASSASSIN_1,	2 : localeInfo.SKILL_GROUP_ASSASSIN_2, },
		playersettingmodule.JOB_SURA		: { 1 : localeInfo.SKILL_GROUP_SURA_1,		2 : localeInfo.SKILL_GROUP_SURA_2, },
		playersettingmodule.JOB_SHAMAN		: { 1 : localeInfo.SKILL_GROUP_SHAMAN_1,	2 : localeInfo.SKILL_GROUP_SHAMAN_2, },
		playersettingmodule.JOB_WOLFMAN		: { 1 : "Ýç Güdü",	2 : "N.A", },		
	}

	STAT_DESCRIPTION =	{
		"HTH" : localeInfo.STAT_TOOLTIP_CON,
		"INT" : localeInfo.STAT_TOOLTIP_INT,
		"STR" : localeInfo.STAT_TOOLTIP_STR,
		"DEX" : localeInfo.STAT_TOOLTIP_DEX,
	}

	MAX_QUEST_HEIGHT = 275

	STAT_MINUS_DESCRIPTION = localeInfo.STAT_MINUS_DESCRIPTION

	def __init__(self):

		if app.ENABLE_DETAILS_UI:
			self.chDetailsWnd = None
			self.isOpenedDetailsWnd = False
	
		ui.ScriptWindow.__init__(self)
		self.state = "STATUS"
		self.isLoaded = 0
		self.finishFirstLoad = False
		
		self.toolTipSkill = 0
				
		self.__Initialize()
		self.__LoadWindow()

		self.statusPlusCommandDict={
			"HTH" : "/stat ht",
			"INT" : "/stat iq",
			"STR" : "/stat st",
			"DEX" : "/stat dx",
		}

		self.statusMinusCommandDict={
			"HTH-" : "/stat- ht",
			"INT-" : "/stat- iq",
			"STR-" : "/stat- st",
			"DEX-" : "/stat- dx",
		}

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __Initialize(self):
		self.refreshToolTip = 0
		self.curSelectedSkillGroup = 0
		self.canUseHorseSkill = -1

		self.toolTip = None
		self.toolTipJob = None
		self.toolTipAlignment = None
		self.toolTipSkill = None

		self.faceImage = None
		self.statusPlusLabel = None
		self.statusPlusValue = None
		self.activeSlot = None
		self.tabDict = None
		self.tabButtonDict = None
		self.pageDict = None
		self.titleBarDict = None
		self.statusPlusButtonDict = None
		self.statusMinusButtonDict = None

		self.skillPageDict = None
		self.skillGroupButton = ()

		self.activeSlot = None
		self.activeSkillPointValue = None
		self.supportSkillPointValue = None
		self.skillGroupButton1 = None
		self.skillGroupButton2 = None
		self.activeSkillGroupName = None

		self.guildNameSlot = None
		self.guildNameValue = None
		self.characterNameSlot = None
		self.characterNameValue = None

		self.emotionToolTip = None
		self.soloEmotionSlot = None
		self.dualEmotionSlot = None
		self.specialEmotionSlot = None
		
		self.skillPage = None

		self.questScrollBar = None
		self.lastScrollPosition = 0
		self.questBoard = None
		self.questPage = None
		self.questTitleBar = None
		self.questShowingStartIndex = 0
		self.wndQuestSlideWindow = {}
		self.wndQuestSlideWindowNewKey = 0
		self.category_flag = -1
		self.cnt = -1
		#self.lp_x = 0
		#self.lp_y = 0
		self.slotIndex = 0
		self.questCategoryList = None
		#self.IsOpenCategoryList = None
		self.IsSameFlag = False
		self.willOpenCategory = None

		# Colored quests/categories
		self.qColorList = {'green': 0xFF83C055, 'blue': 0xFF45678D, 'golden': 0xFFCAB62F, 'default_title': 0xFFFFE3AD}

		# New quest stuff
		self.maxConcurrentCategories = 1 # NOTE! The current implementation of the system assumes at some places that
										 # there can only be one category open at a given time (as the original did)
		self.openCategories = []
		self.clickedQuests = []
		self.questIndexMap = {}		
		self.displayY = 0
		self.extraSpace = 0
		self.counterList = []
		self.clockList = []
		self.catPositionRender = []
		self.baseCutY = 0
		self.StatusUp = None
		self.open_gui_btn = []
		
	if app.ENABLE_DETAILS_UI:
		def OnTop(self):
			if self.chDetailsWnd:
				self.chDetailsWnd.SetTop()

	if app.ENABLE_DETAILS_UI:
		def Hide(self):
			if self.chDetailsWnd:
				self.isOpenedDetailsWnd = self.chDetailsWnd.IsShow()
				self.chDetailsWnd.Close()
			wndMgr.Hide(self.hWnd)

	def Show(self):
		self.__LoadWindow()
		
		if app.ENABLE_DETAILS_UI:
			self.__InitCharacterDetailsUIButton()
			if self.chDetailsWnd and self.isOpenedDetailsWnd:
				self.chDetailsWnd.Show()

		ui.ScriptWindow.Show(self)
		
	def __LoadScript(self, fileName):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, fileName)	
		
	def __BindObject(self):
		self.toolTip = uiToolTip.ToolTip()
		self.toolTipJob = uiToolTip.ToolTip()
		self.toolTipAlignment = uiToolTip.ToolTip(130)		

		self.faceImage = self.GetChild("Face_Image")

		faceSlot=self.GetChild("Face_Slot")
		if 949 == app.GetDefaultCodePage():
			faceSlot.SAFE_SetStringEvent("MOUSE_OVER_IN", self.__ShowJobToolTip)
			faceSlot.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.__HideJobToolTip)

		self.statusPlusLabel = self.GetChild("Status_Plus_Label")
		self.statusPlusValue = self.GetChild("Status_Plus_Value")
		
		self.characterNameSlot = self.GetChild("Character_Name_Slot")			
		self.characterNameValue = self.GetChild("Character_Name")
		self.guildNameSlot = self.GetChild("Guild_Name_Slot")
		self.guildNameValue = self.GetChild("Guild_Name")
		self.characterNameSlot.SAFE_SetStringEvent("MOUSE_OVER_IN", self.__ShowAlignmentToolTip)
		self.characterNameSlot.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.__HideAlignmentToolTip)

		self.activeSlot = self.GetChild("Skill_Active_Slot")
		self.activeSkillPointValue = self.GetChild("Active_Skill_Point_Value")
		self.supportSkillPointValue = self.GetChild("Support_Skill_Point_Value")
		self.skillGroupButton1 = self.GetChild("Skill_Group_Button_1")
		self.skillGroupButton2 = self.GetChild("Skill_Group_Button_2")
		self.activeSkillGroupName = self.GetChild("Active_Skill_Group_Name")
		self.skillPage = self.GetChild("Skill_Page")
		
		self.questBoard = self.GetChild("board")
		self.questPage = self.GetChild("Quest_Page")
		self.questTitleBar = self.GetChild("Quest_TitleBar")
		self.questShowingStartIndex = 0	
		self.questSlotList = []
		self.questCategoryList=[]

		self.tabDict = {
			"STATUS"	: self.GetChild("Tab_01"),
			"SKILL"		: self.GetChild("Tab_02"),
			"EMOTICON"	: self.GetChild("Tab_03"),
			"QUEST"		: self.GetChild("Tab_04"),
		}

		self.tabButtonDict = {
			"STATUS"	: self.GetChild("Tab_Button_01"),
			"SKILL"		: self.GetChild("Tab_Button_02"),
			"EMOTICON"	: self.GetChild("Tab_Button_03"),
			"QUEST"		: self.GetChild("Tab_Button_04")
		}

		self.pageDict = {
			"STATUS"	: self.GetChild("Character_Page"),
			"SKILL"		: self.GetChild("Skill_Page"),
			"EMOTICON"	: self.GetChild("Emoticon_Page"),
			"QUEST"		: self.GetChild("Quest_Page")
		}

		self.titleBarDict = {
			"STATUS"	: self.GetChild("Character_TitleBar"),
			"SKILL"		: self.GetChild("Skill_TitleBar"),
			"EMOTICON"	: self.GetChild("Emoticon_TitleBar"),
			"QUEST"		: self.GetChild("Quest_TitleBar")
		}

		self.statusPlusButtonDict = {
			"HTH"		: self.GetChild("HTH_Plus"),
			"INT"		: self.GetChild("INT_Plus"),
			"STR"		: self.GetChild("STR_Plus"),
			"DEX"		: self.GetChild("DEX_Plus"),
		}

		self.statusMinusButtonDict = {
			"HTH-"		: self.GetChild("HTH_Minus"),
			"INT-"		: self.GetChild("INT_Minus"),
			"STR-"		: self.GetChild("STR_Minus"),
			"DEX-"		: self.GetChild("DEX_Minus"),
		}

		self.skillPageDict = {
			"ACTIVE" : self.GetChild("Skill_Active_Slot"),
			"SUPPORT" : self.GetChild("Skill_ETC_Slot"),
			"HORSE" : self.GetChild("Skill_Active_Slot"),
		}

		self.skillPageStatDict = {
			"SUPPORT"	: player.SKILL_SUPPORT,
			"ACTIVE"	: player.SKILL_ACTIVE,
			"HORSE"		: player.SKILL_HORSE,
		}

		self.skillGroupButton = (
			self.GetChild("Skill_Group_Button_1"),
			self.GetChild("Skill_Group_Button_2"),
		)

		
		global SHOW_ONLY_ACTIVE_SKILL
		global HIDE_SUPPORT_SKILL_POINT
		if SHOW_ONLY_ACTIVE_SKILL or HIDE_SUPPORT_SKILL_POINT:	
			self.GetChild("Support_Skill_Point_Label").Hide()

		self.soloEmotionSlot = self.GetChild("SoloEmotionSlot")
		self.dualEmotionSlot = self.GetChild("DualEmotionSlot")
		self.specialEmotionSlot = self.GetChild("SpecialEmotionSlot")
		self.__SetEmotionSlot()

		for i in xrange(quest.QUEST_CATEGORY_MAX_NUM):
			self.questCategoryList.append(self.GetChild("Quest_Category_0" + str(i)))
			self.catPositionRender.append(0)

		self.RearrangeCategories(xrange(quest.QUEST_CATEGORY_MAX_NUM))
			
		if app.ENABLE_DETAILS_UI:
			self.MainBoard = self.GetChild("board")
			self.ExpandBtn = ui.MakeButton(self.MainBoard, 240, 120, "", "d:/ymir work/ui/game/belt_inventory/", "btn_minimize_normal.tga", "btn_minimize_over.tga", "btn_minimize_down.tga")
			self.ExpandBtn.SetEvent(ui.__mem_func__(self.__ClickExpandButton))
			self.MinimizeBtn = ui.MakeButton(self.MainBoard, 240, 120, "", "d:/ymir work/ui/game/belt_inventory/", "btn_expand_normal.tga", "btn_expand_over.tga", "btn_expand_down.tga")
			self.MinimizeBtn.SetEvent(ui.__mem_func__(self.__ClickMinimizeButton))
			
	if app.ENABLE_DETAILS_UI:
		def __InitCharacterDetailsUIButton(self):
			self.ExpandBtn.Show()
			self.MinimizeBtn.Hide()
			
		def __ClickExpandButton(self):
			if not self.chDetailsWnd:
				self.chDetailsWnd = uiCharacterDetails.CharacterDetailsUI(self)
				self.chDetailsWnd.Show()
			else:
				self.chDetailsWnd.Show()
				
			self.ExpandBtn.Hide()
			self.MinimizeBtn.Show()
				
		def __ClickMinimizeButton(self):
			self.chDetailsWnd.Hide()
			self.MinimizeBtn.Hide()
			self.ExpandBtn.Show()
			
		def OnMoveWindow(self, x, y):
			if self.chDetailsWnd:
				self.chDetailsWnd.AdjustPosition(x, y)
			if self.wndQuestSlideWindowNewKey-1 >= 0:
				if self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1] is not None:
					self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1].AdjustPositionAndSize()

	def __SetSkillSlotEvent(self):
		for skillPageValue in self.skillPageDict.itervalues():
			skillPageValue.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
			skillPageValue.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectSkill))
			skillPageValue.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
			skillPageValue.SetUnselectItemSlotEvent(ui.__mem_func__(self.ClickSkillSlot))
			skillPageValue.SetUseSlotEvent(ui.__mem_func__(self.ClickSkillSlot))
			skillPageValue.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			skillPageValue.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			skillPageValue.SetPressedSlotButtonEvent(ui.__mem_func__(self.OnPressedSlotButton))
			skillPageValue.AppendSlotButton("d:/ymir work/ui/game/windows/btn_plus_up.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_over.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_down.sub")

	def __SetEmotionSlot(self):

		self.emotionToolTip = uiToolTip.ToolTip()

		for slot in (self.soloEmotionSlot, self.dualEmotionSlot, self.specialEmotionSlot):
			slot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
			slot.SetSelectItemSlotEvent(ui.__mem_func__(self.__SelectEmotion))
			slot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__ClickEmotionSlot))
			slot.SetUseSlotEvent(ui.__mem_func__(self.__ClickEmotionSlot))
			slot.SetOverInItemEvent(ui.__mem_func__(self.__OverInEmotion))
			slot.SetOverOutItemEvent(ui.__mem_func__(self.__OverOutEmotion))
			slot.AppendSlotButton("d:/ymir work/ui/game/windows/btn_plus_up.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_over.sub",\
											"d:/ymir work/ui/game/windows/btn_plus_down.sub")

		for slotIdx, datadict in emotion.EMOTION_DICT.items():
			emotionIdx = slotIdx

			slot = self.soloEmotionSlot
			if slotIdx > 50 and slotIdx < 101:
				slot = self.dualEmotionSlot
			elif slotIdx >= 101:
				slot = self.specialEmotionSlot

			slot.SetEmotionSlot(slotIdx, emotionIdx)
			slot.SetCoverButton(slotIdx)

	def __SelectEmotion(self, slotIndex):
		if not slotIndex in emotion.EMOTION_DICT:
			return

		if app.IsPressed(app.DIK_LCONTROL):
			player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_EMOTION, slotIndex)
			return

		mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_EMOTION, slotIndex, slotIndex)

	def __ClickEmotionSlot(self, slotIndex):
		print "click emotion"
		if not slotIndex in emotion.EMOTION_DICT:
			return

		print "check acting"
		if player.IsActingEmotion():
			return

		command = emotion.EMOTION_DICT[slotIndex]["command"]
		print "command", command

		if slotIndex > 50 and slotIndex < 101:
			vid = player.GetTargetVID()

			if 0 == vid or vid == player.GetMainCharacterIndex() or chr.IsNPC(vid) or chr.IsEnemy(vid):
				import chat
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EMOTION_CHOOSE_ONE)
				return

			command += " " + chr.GetNameByVID(vid)

		print "send_command", command
		net.SendChatPacket(command)

	def ActEmotion(self, emotionIndex):
		self.__ClickEmotionSlot(emotionIndex)

	def __OverInEmotion(self, slotIndex):
		if self.emotionToolTip:

			if not slotIndex in emotion.EMOTION_DICT:
				return

			self.emotionToolTip.ClearToolTip()
			self.emotionToolTip.SetTitle(emotion.EMOTION_DICT[slotIndex]["name"])
			self.emotionToolTip.AlignHorizonalCenter()
			self.emotionToolTip.ShowToolTip()

	def __OverOutEmotion(self):
		if self.emotionToolTip:
			self.emotionToolTip.HideToolTip()

	def __BindEvent(self):
		for i in xrange(len(self.skillGroupButton)):
			self.skillGroupButton[i].SetEvent(lambda arg=i: self.__SelectSkillGroup(arg))

		self.RefreshQuest()
		self.__HideJobToolTip()

		for (tabKey, tabButton) in self.tabButtonDict.items():
			tabButton.SetEvent(ui.__mem_func__(self.__OnClickTabButton), tabKey)

		for (statusPlusKey, statusPlusButton) in self.statusPlusButtonDict.items():
			statusPlusButton.SAFE_SetEvent(self.__OnClickStatusPlusButton, statusPlusKey)
			statusPlusButton.ShowToolTip = lambda arg=statusPlusKey: self.__OverInStatButton(arg)
			statusPlusButton.HideToolTip = lambda arg=statusPlusKey: self.__OverOutStatButton()

		for (statusMinusKey, statusMinusButton) in self.statusMinusButtonDict.items():
			statusMinusButton.SAFE_SetEvent(self.__OnClickStatusMinusButton, statusMinusKey)
			statusMinusButton.ShowToolTip = lambda arg=statusMinusKey: self.__OverInStatMinusButton(arg)
			statusMinusButton.HideToolTip = lambda arg=statusMinusKey: self.__OverOutStatMinusButton()

		for titleBarValue in self.titleBarDict.itervalues():
			if app.ENABLE_DETAILS_UI:
				titleBarValue.SetCloseEvent(ui.__mem_func__(self.Close))
			else:
				titleBarValue.SetCloseEvent(ui.__mem_func__(self.Hide))

		self.questTitleBar.SetCloseEvent(ui.__mem_func__(self.Close))
       	
		for i in xrange(quest.QUEST_CATEGORY_MAX_NUM):
			self.questCategoryList[i].SetEvent(ui.__mem_func__(self.__OnClickQuestCategoryButton),i)
			
	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			self.__LoadScript("UIScript/CharacterWindow.py")
			self.__BindObject()
			self.__BindEvent()
			self.questScrollBar = self.GetChild("QuestScrollBar")
			self.questScrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScrollQuest))	
		except:
			import exception
			exception.Abort("CharacterWindow.__LoadWindow")

		#self.tabButtonDict["EMOTICON"].Disable()
		self.SetState("STATUS")

	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()

	def Close(self):
		if None != self.toolTipSkill:
			self.toolTipSkill.Hide()
			
		if app.ENABLE_DETAILS_UI:
			if self.chDetailsWnd and self.chDetailsWnd.IsShow():
				self.chDetailsWnd.Hide()
				
		if self.wndQuestSlideWindowNewKey>0:
			if self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1] is not None:
				self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1].CloseSelf()
			
		self.Hide()

	def SetSkillToolTip(self, toolTipSkill):
		self.toolTipSkill = toolTipSkill
					
	def __OnClickStatusPlusButton(self, statusKey):
		try:
			statusPlusCommand=self.statusPlusCommandDict[statusKey]
			net.SendChatPacket(statusPlusCommand)
		except KeyError, msg:
			dbg.TraceError("CharacterWindow.__OnClickStatusPlusButton KeyError: %s", msg)

	def __OnClickStatusMinusButton(self, statusKey):
		try:
			statusMinusCommand=self.statusMinusCommandDict[statusKey]
			net.SendChatPacket(statusMinusCommand)
		except KeyError, msg:
			dbg.TraceError("CharacterWindow.__OnClickStatusMinusButton KeyError: %s", msg)


	def __OnClickTabButton(self, stateKey):
		self.SetState(stateKey)

	def SetState(self, stateKey):
		if self.tabButtonDict == None:
			return
		
		self.state = stateKey
		
		if stateKey != "QUEST":
			self.questPage.Hide()
			if self.wndQuestSlideWindowNewKey > 0:
				if self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1] is not None:
					self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1].CloseSelf()
		else:
			self.__LoadQuestCat()
		
		for (tabKey, tabButton) in self.tabButtonDict.items():
			if stateKey!=tabKey:
				tabButton.SetUp()

		for tabValue in self.tabDict.itervalues():
			tabValue.Hide()

		for pageValue in self.pageDict.itervalues():
			pageValue.Hide()

		for titleBarValue in self.titleBarDict.itervalues():
			titleBarValue.Hide()

		self.titleBarDict[stateKey].Show()
		self.tabDict[stateKey].Show()
		self.pageDict[stateKey].Show()
		

	def GetState(self):
		return self.state

	def __GetTotalAtkText(self):
		minAtk=player.GetStatus(player.ATT_MIN)
		maxAtk=player.GetStatus(player.ATT_MAX)
		atkBonus=player.GetStatus(player.ATT_BONUS)
		attackerBonus=player.GetStatus(player.ATTACKER_BONUS)

		if minAtk==maxAtk:
			return "%d" % (minAtk+atkBonus+attackerBonus)
		else:
			return "%d-%d" % (minAtk+atkBonus+attackerBonus, maxAtk+atkBonus+attackerBonus)

	def __GetTotalMagAtkText(self):
		minMagAtk=player.GetStatus(player.MAG_ATT)+player.GetStatus(player.MIN_MAGIC_WEP)
		maxMagAtk=player.GetStatus(player.MAG_ATT)+player.GetStatus(player.MAX_MAGIC_WEP)

		if minMagAtk==maxMagAtk:
			return "%d" % (minMagAtk)
		else:
			return "%d-%d" % (minMagAtk, maxMagAtk)

	def __GetTotalDefText(self):
		defValue=player.GetStatus(player.DEF_GRADE)
		if constInfo.ADD_DEF_BONUS_ENABLE:
			defValue+=player.GetStatus(player.DEF_BONUS)
		return "%d" % (defValue)
	
	def RefreshStatus(self):
		if self.isLoaded==0:
			return

		try:
			self.GetChild("Level_Value").SetText(str(player.GetStatus(player.LEVEL)))
			self.GetChild("Exp_Value").SetText(str(unsigned32(player.GetEXP())))
			self.GetChild("RestExp_Value").SetText(str(unsigned32(player.GetStatus(player.NEXT_EXP)) - unsigned32(player.GetStatus(player.EXP))))
			self.GetChild("HP_Value").SetText(str(player.GetStatus(player.HP)) + '/' + str(player.GetStatus(player.MAX_HP)))
			self.GetChild("SP_Value").SetText(str(player.GetStatus(player.SP)) + '/' + str(player.GetStatus(player.MAX_SP)))

			self.GetChild("STR_Value").SetText(str(player.GetStatus(player.ST)))
			self.GetChild("DEX_Value").SetText(str(player.GetStatus(player.DX)))
			self.GetChild("HTH_Value").SetText(str(player.GetStatus(player.HT)))
			self.GetChild("INT_Value").SetText(str(player.GetStatus(player.IQ)))

			self.GetChild("ATT_Value").SetText(self.__GetTotalAtkText())
			self.GetChild("DEF_Value").SetText(self.__GetTotalDefText())

			self.GetChild("MATT_Value").SetText(self.__GetTotalMagAtkText())
			#self.GetChild("MATT_Value").SetText(str(player.GetStatus(player.MAG_ATT)))

			self.GetChild("MDEF_Value").SetText(str(player.GetStatus(player.MAG_DEF)))
			self.GetChild("ASPD_Value").SetText(str(player.GetStatus(player.ATT_SPEED)))
			self.GetChild("MSPD_Value").SetText(str(player.GetStatus(player.MOVING_SPEED)))
			self.GetChild("CSPD_Value").SetText(str(player.GetStatus(player.CASTING_SPEED)))
			self.GetChild("ER_Value").SetText(str(player.GetStatus(player.EVADE_RATE)))

		except:
			#import exception
			#exception.Abort("CharacterWindow.RefreshStatus.BindObject")
			## °ÔÀÓÀÌ Æ¨°Ü ¹ö¸²
			pass

		self.__RefreshStatusPlusButtonList()
		self.__RefreshStatusMinusButtonList()
		self.RefreshAlignment()

		if self.refreshToolTip:
			self.refreshToolTip()
			
		if app.ENABLE_DETAILS_UI:
			if self.chDetailsWnd and self.chDetailsWnd.IsShow():
				self.chDetailsWnd.RefreshLabel()

	def __RefreshStatusPlusButtonList(self):
		if self.isLoaded==0:
			return

		statusPlusPoint=player.GetStatus(player.STAT)

		if statusPlusPoint>0:
			self.statusPlusValue.SetText(str(statusPlusPoint))
			self.statusPlusLabel.Show()
			self.ShowStatusPlusButtonList()
		else:
			self.statusPlusValue.SetText(str(0))
			self.statusPlusLabel.Hide()
			self.HideStatusPlusButtonList()

	def __RefreshStatusMinusButtonList(self):
		if self.isLoaded==0:
			return

		statusMinusPoint=self.__GetStatMinusPoint()

		if statusMinusPoint>0:
			self.__ShowStatusMinusButtonList()
		else:
			self.__HideStatusMinusButtonList()

	def RefreshAlignment(self):
		point, grade = player.GetAlignmentData()

		import colorInfo
		COLOR_DICT = {	0 : colorInfo.TITLE_RGB_GOOD_7,
						1 : colorInfo.TITLE_RGB_GOOD_6,
						2 : colorInfo.TITLE_RGB_GOOD_5,
						3 : colorInfo.TITLE_RGB_GOOD_4,
						4 : colorInfo.TITLE_RGB_GOOD_3,
						5 : colorInfo.TITLE_RGB_GOOD_2,
						6 : colorInfo.TITLE_RGB_GOOD_1,
						7 : colorInfo.TITLE_RGB_NORMAL,
						8 : colorInfo.TITLE_RGB_EVIL_1,
						9 : colorInfo.TITLE_RGB_EVIL_2,
						10 : colorInfo.TITLE_RGB_EVIL_3,
						11 : colorInfo.TITLE_RGB_EVIL_4, }
		colorList = COLOR_DICT.get(grade, colorInfo.TITLE_RGB_NORMAL)
		gradeColor = ui.GenerateColor(colorList[0], colorList[1], colorList[2])

		self.toolTipAlignment.ClearToolTip()
		self.toolTipAlignment.AutoAppendTextLine(localeInfo.TITLE_NAME_LIST[grade], gradeColor)
		self.toolTipAlignment.AutoAppendTextLine(localeInfo.NumberToAlignmentString(point))
		self.toolTipAlignment.AppendSpace(2)
		
		if point >= 30000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_8_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_8_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_8_C, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(3)
		elif point >= 25000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_7_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_7_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		elif point >= 20000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_6_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_6_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		elif point >= 18000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_5_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_5_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		elif point >= 12000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_4_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_4_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		elif point >= 8000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_3_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_3_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		elif point >= 4000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_2_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_2_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		elif point >= 1000:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_1_A, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.ALIGN_BONUS_GOOD_1_B, self.toolTipAlignment.POSITIVE_COLOR)
			self.toolTipAlignment.AppendSpace(2)
		if app.ENABLE_WORSHIP_SYSTEM:
			self.toolTipAlignment.AutoAppendTextLine(localeInfo.NumberToWorshipPointString(player.GetWorshipPoint()))
			self.toolTipAlignment.AppendSpace(2)
		self.toolTipAlignment.AlignHorizonalCenter()

	def __ShowStatusMinusButtonList(self):
		for (stateMinusKey, statusMinusButton) in self.statusMinusButtonDict.items():
			statusMinusButton.Show()

	def __HideStatusMinusButtonList(self):
		for (stateMinusKey, statusMinusButton) in self.statusMinusButtonDict.items():
			statusMinusButton.Hide()

	def ShowStatusPlusButtonList(self):
		for (statePlusKey, statusPlusButton) in self.statusPlusButtonDict.items():
			statusPlusButton.Show()

	def HideStatusPlusButtonList(self):
		for (statePlusKey, statusPlusButton) in self.statusPlusButtonDict.items():
			statusPlusButton.Hide()

	def SelectSkill(self, skillSlotIndex):

		mouseController = mouseModule.mouseController

		if False == mouseController.isAttached():

			srcSlotIndex = self.__RealSkillSlotToSourceSlot(skillSlotIndex)
			selectedSkillIndex = player.GetSkillIndex(srcSlotIndex)

			if skill.CanUseSkill(selectedSkillIndex):

				if app.IsPressed(app.DIK_LCONTROL):

					player.RequestAddToEmptyLocalQuickSlot(player.SLOT_TYPE_SKILL, srcSlotIndex)
					return

				mouseController.AttachObject(self, player.SLOT_TYPE_SKILL, srcSlotIndex, selectedSkillIndex)

		else:

			mouseController.DeattachObject()

	def SelectEmptySlot(self, SlotIndex):
		mouseModule.mouseController.DeattachObject()

	## ToolTip
	def OverInItem(self, slotNumber):

		if mouseModule.mouseController.isAttached():
			return

		if 0 == self.toolTipSkill:
			return
			
		if None == self.toolTipSkill:
			return
			
		srcSlotIndex = self.__RealSkillSlotToSourceSlot(slotNumber)
		skillIndex = player.GetSkillIndex(srcSlotIndex)
		skillLevel = player.GetSkillLevel(srcSlotIndex)
		skillGrade = player.GetSkillGrade(srcSlotIndex)
		skillType = skill.GetSkillType(skillIndex)

		## ACTIVE
		if skill.SKILL_TYPE_ACTIVE == skillType:
			overInSkillGrade = self.__GetSkillGradeFromSlot(slotNumber)

			if overInSkillGrade == skill.SKILL_GRADE_COUNT-1 and skillGrade == skill.SKILL_GRADE_COUNT:
				self.toolTipSkill.SetSkillNew(srcSlotIndex, skillIndex, skillGrade, skillLevel)
			elif overInSkillGrade == skillGrade:
				self.toolTipSkill.SetSkillNew(srcSlotIndex, skillIndex, overInSkillGrade, skillLevel)
			else:
				self.toolTipSkill.SetSkillOnlyName(srcSlotIndex, skillIndex, overInSkillGrade)

		else:
			self.toolTipSkill.SetSkillNew(srcSlotIndex, skillIndex, skillGrade, skillLevel)

	def OverOutItem(self):
		if None == self.toolTipSkill:
			return
			
		if 0 != self.toolTipSkill:
			self.toolTipSkill.HideToolTip()

	def __GetStatMinusPoint(self):
		POINT_STAT_RESET_COUNT = 112
		return player.GetStatus(POINT_STAT_RESET_COUNT)

	def __OverInStatMinusButton(self, stat):
		try:
			self.__ShowStatToolTip(self.STAT_MINUS_DESCRIPTION[stat] % self.__GetStatMinusPoint())
		except KeyError:
			pass

		self.refreshToolTip = lambda arg=stat: self.__OverInStatMinusButton(arg) 

	def __OverOutStatMinusButton(self):
		self.__HideStatToolTip()
		self.refreshToolTip = 0

	def __OverInStatButton(self, stat):	
		try:
			self.__ShowStatToolTip(self.STAT_DESCRIPTION[stat])
		except KeyError:
			pass
			
	def __OverOutStatButton(self):
		self.__HideStatToolTip()

	def __ShowStatToolTip(self, statDesc):
		if self.toolTip:
			self.toolTip.ClearToolTip()
			self.toolTip.AppendTextLine(statDesc)
			self.toolTip.Show()

	def __HideStatToolTip(self):
		if self.toolTip:
			self.toolTip.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	## Skill Process
	def __RefreshSkillPage(self, name, slotCount):
		global SHOW_LIMIT_SUPPORT_SKILL_LIST

		skillPage = self.skillPageDict[name]

		startSlotIndex = skillPage.GetStartIndex()
		if "ACTIVE" == name:
			if self.PAGE_HORSE == self.curSelectedSkillGroup:
				startSlotIndex += slotCount

		getSkillType=skill.GetSkillType
		getSkillIndex=player.GetSkillIndex
		getSkillGrade=player.GetSkillGrade
		getSkillLevel=player.GetSkillLevel
		getSkillLevelUpPoint=skill.GetSkillLevelUpPoint
		getSkillMaxLevel=skill.GetSkillMaxLevel
		for i in xrange(slotCount+1):

			slotIndex = i + startSlotIndex
			skillIndex = getSkillIndex(slotIndex)

			for j in xrange(skill.SKILL_GRADE_COUNT):
				skillPage.ClearSlot(self.__GetRealSkillSlot(j, i))

			if 0 == skillIndex:
				continue

			skillGrade = getSkillGrade(slotIndex)
			skillLevel = getSkillLevel(slotIndex)
			skillType = getSkillType(skillIndex)

			## ½Â¸¶ ½ºÅ³ ¿¹¿Ü Ã³¸®
			if player.SKILL_INDEX_RIDING == skillIndex:
				if 1 == skillGrade:
					skillLevel += 19
				elif 2 == skillGrade:
					skillLevel += 29
				elif 3 == skillGrade:
					skillLevel = 40

				skillPage.SetSkillSlotNew(slotIndex, skillIndex, max(skillLevel-1, 0), skillLevel)
				skillPage.SetSlotCount(slotIndex, skillLevel)

			## ACTIVE
			elif skill.SKILL_TYPE_ACTIVE == skillType:
				for j in xrange(skill.SKILL_GRADE_COUNT):
					realSlotIndex = self.__GetRealSkillSlot(j, slotIndex)
					skillPage.SetSkillSlotNew(realSlotIndex, skillIndex, j, skillLevel)
					skillPage.SetCoverButton(realSlotIndex)

					if (skillGrade == skill.SKILL_GRADE_COUNT) and j == (skill.SKILL_GRADE_COUNT-1):
						skillPage.SetSlotCountNew(realSlotIndex, skillGrade, skillLevel)
					elif (not self.__CanUseSkillNow()) or (skillGrade != j):
						skillPage.SetSlotCount(realSlotIndex, 0)
						skillPage.DisableCoverButton(realSlotIndex)
						skillPage.DeactivateSlot(realSlotIndex) # fix
					else:
						skillPage.SetSlotCountNew(realSlotIndex, skillGrade, skillLevel)
						
					if player.IsSkillActive(slotIndex) and (skillGrade == j): # fix
						skillPage.ActivateSlot(realSlotIndex)

			## ±×¿Ü
			else:
				if not SHOW_LIMIT_SUPPORT_SKILL_LIST or skillIndex in SHOW_LIMIT_SUPPORT_SKILL_LIST:
					realSlotIndex = self.__GetETCSkillRealSlotIndex(slotIndex)
					skillPage.SetSkillSlot(realSlotIndex, skillIndex, skillLevel)
					skillPage.SetSlotCountNew(realSlotIndex, skillGrade, skillLevel)

					if skill.CanUseSkill(skillIndex):
						skillPage.SetCoverButton(realSlotIndex)
					skillPage.DeactivateSlot(realSlotIndex)

			skillPage.RefreshSlot()


	def RefreshSkill(self):

		if self.isLoaded==0:
			return

		if self.__IsChangedHorseRidingSkillLevel():
			self.RefreshCharacter()
			return


		global SHOW_ONLY_ACTIVE_SKILL
		if SHOW_ONLY_ACTIVE_SKILL:
			self.__RefreshSkillPage("ACTIVE", self.ACTIVE_PAGE_SLOT_COUNT)
		else:
			self.__RefreshSkillPage("ACTIVE", self.ACTIVE_PAGE_SLOT_COUNT)
			self.__RefreshSkillPage("SUPPORT", self.SUPPORT_PAGE_SLOT_COUNT)

		self.RefreshSkillPlusButtonList()

	def CanShowPlusButton(self, skillIndex, skillLevel, curStatPoint):

		## ½ºÅ³ÀÌ ÀÖÀ¸¸é
		if 0 == skillIndex:
			return False

		## ·¹º§¾÷ Á¶°ÇÀ» ¸¸Á·ÇÑ´Ù¸é
		if not skill.CanLevelUpSkill(skillIndex, skillLevel):
			return False

		return True

	def __RefreshSkillPlusButton(self, name):
		global HIDE_SUPPORT_SKILL_POINT
		if HIDE_SUPPORT_SKILL_POINT and "SUPPORT" == name:
			return

		slotWindow = self.skillPageDict[name]
		slotWindow.HideAllSlotButton()

		slotStatType = self.skillPageStatDict[name]
		if 0 == slotStatType:
			return

		statPoint = player.GetStatus(slotStatType)
		startSlotIndex = slotWindow.GetStartIndex()
		if "HORSE" == name:
			startSlotIndex += self.ACTIVE_PAGE_SLOT_COUNT

		if statPoint > 0:
			for i in xrange(self.PAGE_SLOT_COUNT):
				slotIndex = i + startSlotIndex
				skillIndex = player.GetSkillIndex(slotIndex)
				skillGrade = player.GetSkillGrade(slotIndex)
				skillLevel = player.GetSkillLevel(slotIndex)

				if skillIndex == 0:
					continue
				if skillGrade != 0:
					continue

				if name == "HORSE":
					if player.GetStatus(player.LEVEL) >= skill.GetSkillLevelLimit(skillIndex):
						if skillLevel < 20:
							slotWindow.ShowSlotButton(self.__GetETCSkillRealSlotIndex(slotIndex))

				else:
					if "SUPPORT" == name:						
						if not SHOW_LIMIT_SUPPORT_SKILL_LIST or skillIndex in SHOW_LIMIT_SUPPORT_SKILL_LIST:
							if self.CanShowPlusButton(skillIndex, skillLevel, statPoint):
								slotWindow.ShowSlotButton(slotIndex)
					else:
						if self.CanShowPlusButton(skillIndex, skillLevel, statPoint):
							slotWindow.ShowSlotButton(slotIndex)
					

	def RefreshSkillPlusButtonList(self):

		if self.isLoaded==0:
			return

		self.RefreshSkillPlusPointLabel()

		if not self.__CanUseSkillNow():
			return

		try:
			if self.PAGE_HORSE == self.curSelectedSkillGroup:
				self.__RefreshSkillPlusButton("HORSE")
			else:
				self.__RefreshSkillPlusButton("ACTIVE")

			self.__RefreshSkillPlusButton("SUPPORT")

		except:
			import exception
			exception.Abort("CharacterWindow.RefreshSkillPlusButtonList.BindObject")

	def RefreshSkillPlusPointLabel(self):
		if self.isLoaded==0:
			return

		if self.PAGE_HORSE == self.curSelectedSkillGroup:
			activeStatPoint = player.GetStatus(player.SKILL_HORSE)
			self.activeSkillPointValue.SetText(str(activeStatPoint))

		else:
			activeStatPoint = player.GetStatus(player.SKILL_ACTIVE)
			self.activeSkillPointValue.SetText(str(activeStatPoint))

		supportStatPoint = max(0, player.GetStatus(player.SKILL_SUPPORT))
		self.supportSkillPointValue.SetText(str(supportStatPoint))

	## Skill Level Up Button
	def OnPressedSlotButton(self, slotNumber):
		srcSlotIndex = self.__RealSkillSlotToSourceSlot(slotNumber)

		skillIndex = player.GetSkillIndex(srcSlotIndex)
		curLevel = player.GetSkillLevel(srcSlotIndex)
		maxLevel = skill.GetSkillMaxLevel(skillIndex)

		net.SendChatPacket("/skillup " + str(skillIndex))

	## Use Skill
	def ClickSkillSlot(self, slotIndex):

		srcSlotIndex = self.__RealSkillSlotToSourceSlot(slotIndex)
		skillIndex = player.GetSkillIndex(srcSlotIndex)
		skillType = skill.GetSkillType(skillIndex)

		if not self.__CanUseSkillNow():
			if skill.SKILL_TYPE_ACTIVE == skillType:
				return

		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				if skill.CanUseSkill(skillIndex):
					player.ClickSkillSlot(srcSlotIndex)
					return

		mouseModule.mouseController.DeattachObject()

	## FIXME : ½ºÅ³À» »ç¿ëÇßÀ»¶§ ½½·Ô ¹øÈ£¸¦ °¡Áö°í ÇØ´ç ½½·ÔÀ» Ã£¾Æ¼­ ¾÷µ¥ÀÌÆ® ÇÑ´Ù.
	##         ¸Å¿ì ºÒÇÕ¸®. ±¸Á¶ ÀÚÃ¼¸¦ °³¼±ÇØ¾ß ÇÒµí.
	def OnUseSkill(self, slotIndex, coolTime):

		skillIndex = player.GetSkillIndex(slotIndex)
		skillType = skill.GetSkillType(skillIndex)

		## ACTIVE
		if skill.SKILL_TYPE_ACTIVE == skillType:
			skillGrade = player.GetSkillGrade(slotIndex)
			slotIndex = self.__GetRealSkillSlot(skillGrade, slotIndex)
		## ETC
		else:
			slotIndex = self.__GetETCSkillRealSlotIndex(slotIndex)

		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				slotWindow.SetSlotCoolTime(slotIndex, coolTime)
				return

	def OnActivateSkill(self, slotIndex):
		skillGrade = player.GetSkillGrade(slotIndex)
		slotIndex = self.__GetRealSkillSlot(skillGrade, slotIndex)
		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				slotWindow.ActivateSlot(slotIndex)
				
				return

	def OnDeactivateSkill(self, slotIndex):
		skillGrade = player.GetSkillGrade(slotIndex)
		slotIndex = self.__GetRealSkillSlot(skillGrade, slotIndex)
		for slotWindow in self.skillPageDict.values():
			if slotWindow.HasSlot(slotIndex):
				slotWindow.DeactivateSlot(slotIndex)
				
				return

	def __ShowJobToolTip(self):
		self.toolTipJob.ShowToolTip()

	def __HideJobToolTip(self):
		self.toolTipJob.HideToolTip()

	def __SetJobText(self, mainJob, subJob):
		if player.GetStatus(player.LEVEL)<5:
			subJob=0

		if 949 == app.GetDefaultCodePage():
			self.toolTipJob.ClearToolTip()

			try:
				jobInfoTitle=localeInfo.JOBINFO_TITLE[mainJob][subJob]
				jobInfoData=localeInfo.JOBINFO_DATA_LIST[mainJob][subJob]
			except IndexError:
				print "uiCharacter.CharacterWindow.__SetJobText(mainJob=%d, subJob=%d)" % (mainJob, subJob)
				return

			self.toolTipJob.AutoAppendTextLine(jobInfoTitle)
			self.toolTipJob.AppendSpace(5)

			for jobInfoDataLine in jobInfoData:
				self.toolTipJob.AutoAppendTextLine(jobInfoDataLine)

			self.toolTipJob.AlignHorizonalCenter()

	def __ShowAlignmentToolTip(self):
		self.toolTipAlignment.ShowToolTip()

	def __HideAlignmentToolTip(self):
		self.toolTipAlignment.HideToolTip()

	def RefreshCharacter(self):

		if self.isLoaded==0:
			return

		## Name
		try:
			characterName = player.GetName()
			guildName = player.GetGuildName()
			self.characterNameValue.SetText(characterName)
			self.guildNameValue.SetText(guildName)
			if not guildName:
				self.characterNameSlot.SetPosition(109, 34)

				self.guildNameSlot.Hide()
			else:
				self.characterNameSlot.SetPosition(153, 34)
				self.guildNameSlot.Show()
		except:
			import exception
			exception.Abort("CharacterWindow.RefreshCharacter.BindObject")

		race = net.GetMainActorRace()
		group = net.GetMainActorSkillGroup()
		empire = net.GetMainActorEmpire()

		## Job Text
		job = chr.RaceToJob(race)
		self.__SetJobText(job, group)

		## FaceImage
		try:
			faceImageName = FACE_IMAGE_DICT[race]

			try:
				self.faceImage.LoadImage(faceImageName)
			except:
				print "CharacterWindow.RefreshCharacter(race=%d, faceImageName=%s)" % (race, faceImageName)
				self.faceImage.Hide()

		except KeyError:
			self.faceImage.Hide()

		## GroupName
		self.__SetSkillGroupName(race, group)

		## Skill
		if 0 == group:
			self.__SelectSkillGroup(0)

		else:
			self.__SetSkillSlotData(race, group, empire)

			if self.__CanUseHorseSkill():
				self.__SelectSkillGroup(0)

	def __SetSkillGroupName(self, race, group):

		job = chr.RaceToJob(race)

		if not self.SKILL_GROUP_NAME_DICT.has_key(job):
			return

		nameList = self.SKILL_GROUP_NAME_DICT[job]

		if 0 == group:
			self.skillGroupButton1.SetText(nameList[1])
			self.skillGroupButton2.SetText(nameList[2])
			self.skillGroupButton1.Show()
			self.skillGroupButton2.Show()
			self.activeSkillGroupName.Hide()

		else:

			if self.__CanUseHorseSkill():
				self.activeSkillGroupName.Hide()
				self.skillGroupButton1.SetText(nameList.get(group, "Noname"))
				self.skillGroupButton2.SetText(localeInfo.SKILL_GROUP_HORSE)
				self.skillGroupButton1.Show()
				self.skillGroupButton2.Show()

			else:
				self.activeSkillGroupName.SetText(nameList.get(group, "Noname"))
				self.activeSkillGroupName.Show()
				self.skillGroupButton1.Hide()
				self.skillGroupButton2.Hide()

	def __SetSkillSlotData(self, race, group, empire=0):

		## SkillIndex
		playersettingmodule.RegisterSkill(race, group, empire)

		## Event
		self.__SetSkillSlotEvent()

		## Refresh
		self.RefreshSkill()

	def __SelectSkillGroup(self, index):
		for btn in self.skillGroupButton:
			btn.SetUp()
		self.skillGroupButton[index].Down()

		if self.__CanUseHorseSkill():
			if 0 == index:
				index = net.GetMainActorSkillGroup()-1
			elif 1 == index:
				index = self.PAGE_HORSE
				
		self.curSelectedSkillGroup = index
		self.__SetSkillSlotData(net.GetMainActorRace(), index+1, net.GetMainActorEmpire())

	def __CanUseSkillNow(self):
		if 0 == net.GetMainActorSkillGroup():
			return False

		return True

	def __CanUseHorseSkill(self):

		slotIndex = player.GetSkillSlotIndex(player.SKILL_INDEX_RIDING)

		if not slotIndex:
			return False

		grade = player.GetSkillGrade(slotIndex)
		level = player.GetSkillLevel(slotIndex)
		if level < 0:
			level *= -1
		if grade >= 1 and level >= 1:
			return True

		return False

	def __IsChangedHorseRidingSkillLevel(self):
		ret = False

		if -1 == self.canUseHorseSkill:
			self.canUseHorseSkill = self.__CanUseHorseSkill()

		if self.canUseHorseSkill != self.__CanUseHorseSkill():
			ret = True

		self.canUseHorseSkill = self.__CanUseHorseSkill()
		return ret

	def __GetRealSkillSlot(self, skillGrade, skillSlot):
		return skillSlot + min(skill.SKILL_GRADE_COUNT-1, skillGrade)*skill.SKILL_GRADE_STEP_COUNT

	def __GetETCSkillRealSlotIndex(self, skillSlot):
		if skillSlot > 100:
			return skillSlot
		return skillSlot % self.ACTIVE_PAGE_SLOT_COUNT

	def __RealSkillSlotToSourceSlot(self, realSkillSlot):
		if realSkillSlot > 100:
			return realSkillSlot
		if self.PAGE_HORSE == self.curSelectedSkillGroup:
			return realSkillSlot + self.ACTIVE_PAGE_SLOT_COUNT
		return realSkillSlot % skill.SKILL_GRADE_STEP_COUNT

	def __GetSkillGradeFromSlot(self, skillSlot):
		return int(skillSlot / skill.SKILL_GRADE_STEP_COUNT)

	def SelectSkillGroup(self, index):
		self.__SelectSkillGroup(index)

	###################################
	############# QUESTS ##############
	###################################
	
	def __OnScrollQuest(self):
		if self.state != "QUEST":
			return

		curPos = self.questScrollBar.GetPos()
		if math.fabs(curPos - self.lastScrollPosition) >= 0.001:
			self.RerenderArea()
			self.lastScrollPosition = curPos

	def ResetQuestScroll(self):
		self.questScrollBar.Hide()

		if self.questScrollBar.GetPos() != 0:
			self.questScrollBar.SetPos(0)

	def RerenderArea(self):
		overflowingY = self.displayY - self.MAX_QUEST_HEIGHT
		if overflowingY < 0:
			overflowingY = 0
		
		self.baseCutY = math.ceil(overflowingY * self.questScrollBar.GetPos()/20) * 20
		print "OverflowingY: ",overflowingY, "baseCutY: ", self.baseCutY
		self.displayY = 0
		self.RearrangeCategories(xrange(quest.QUEST_CATEGORY_MAX_NUM))
		self.RefreshCategory()

		if overflowingY > 0:
			self.questScrollBar.Show()
		else:
			self.ResetQuestScroll()

	def __LoadQuestCat(self):
		self.questPage.Show()

		if self.isLoaded==0:
			return
		
		for i in xrange(quest.QUEST_CATEGORY_MAX_NUM):
			cat = self.questCategoryList[i]

			catName = cat.GetProperty("name")
			if not catName:
				cat.SetProperty("name", cat.GetText())
				catName = cat.GetText()
			
			count = self.GetCountQuestInCategory(i)
			print "Loading quests: "+catName+", "+str(count)
			self.questCategoryList[i].SetTextAlignLeft(catName + " (" + str(count) + ")")
			self.questCategoryList[i].SetTextColor(self.GetCategoryColor(i))
			self.questCategoryList[i].Show()
		
		self.RefreshCategory()
		if self.finishFirstLoad == False:
			self.questScrollBar.Hide()
		else:
			self.RerenderArea()
		self.finishFirstLoad = True

	def GetCategoryColor(self, cat):
		return self.qColorList['default_title']

	def GetQuestProperties(self, questName):
		# If it finds one of the tokens in the list it colors the whole quest that way
		find = {"&": "green", 
				"*": "blue", 
				"~": "golden",
				"+": "purple"}

		if questName[0] in find:
			return (questName[1:], find[questName[0]])

		return (questName, None)

	def IsCategoryOpen(self, cat):
		return (cat in self.openCategories)

	def ToggleCategory(self, cat):
		if self.IsCategoryOpen(cat):
			print "Toggle:close"
			self.CloseCategory(cat)
		else:
			print "Toggle:Open"
			self.OpenCategory(cat)

	def RearrangeCategories(self, catRange):
		i = 0
		for i in catRange:
			if self.displayY - self.baseCutY >= 0 and self.displayY - self.baseCutY < self.MAX_QUEST_HEIGHT:
				self.questCategoryList[i].SetPosition(7, self.displayY - self.baseCutY)
				self.questCategoryList[i].Show()
			else:
				self.questCategoryList[i].Hide()

			self.displayY += 20
			self.catPositionRender[i] = self.displayY

	def CloseCategory(self, cat):
		if cat in self.openCategories:
			self.openCategories.remove(cat)

		for curSlot in self.questSlotList:
			if curSlot.GetProperty("category") == cat:
				curSlot.Hide()
				self.displayY -= curSlot.GetHeight()

		self.RerenderArea()
			
	def OpenCategory(self, cat):
		print "Opening category "+str(cat)

		while len(self.openCategories) >= self.maxConcurrentCategories: #close opened ones if we go over the limit
			ccat = self.openCategories.pop()
			self.CloseCategory(ccat)
			print "Closing category "+str(ccat)

		self.openCategories.append(cat)
		self.RefreshCategory(cat)
		self.RerenderArea()
		
	def __SelectQuest(self, slotIndex):
		questIndex = self.questIndexMap[slotIndex]

		if not questIndex in self.clickedQuests:
			self.clickedQuests.append(questIndex)

		import event
		event.QuestButtonClick(-2147483648 + questIndex)

	def RefreshCategory(self, cat = -1):
		if self.isLoaded == 0 or self.state != "QUEST":
			return

		cats = []
		if cat == -1:
			cats = self.openCategories
		elif not cat in self.openCategories:
			self.OpenCategory(cat)
			return
		else:
			cats.append(cat)

		for curCat in cats:
			self.displayY = self.catPositionRender[curCat]
			print "Rendering category "+str(curCat)+", position: "+str(self.displayY)
			# Load
			self.LoadCategory(curCat)
			# Rearrange
			self.RearrangeCategories(xrange(curCat+1, quest.QUEST_CATEGORY_MAX_NUM))

	def ReceiveNewQuest(self, idx):
		if not self.finishFirstLoad:
			return

		for cat in xrange(quest.QUEST_CATEGORY_MAX_NUM):
			for q in self.GetQuestsInCategory(cat):
				(questID, questIndex, questName, questCategory, questIcon, questCounterName, questCounterValue) = q
				if questIndex == idx:
					self.RefreshCategory(cat)
					self.RefreshCategoriesCount()
					self.RerenderArea()

	def RefreshCategoriesCount(self):
		for cat in xrange(quest.QUEST_CATEGORY_MAX_NUM):
			# Refresh category count
			catName = self.questCategoryList[cat].GetProperty("name")
			count = self.GetCountQuestInCategory(cat)
			print "Refreshing cat count:",cat,",",count
			self.questCategoryList[cat].SetTextAlignLeft(catName + " (" + str(count) + ")")

	def RefreshQuest(self):
		if self.isLoaded == 0 or self.state != "QUEST":
			return

		print "Refreshing ALL"
		for cat in self.openCategories:
			print "Refreshing cat:",cat
			self.RefreshCategory(cat)

		self.RefreshCategoriesCount()

	def CreateQuestSlot(self, name):
		for cSlot in self.questSlotList:
			if cSlot.GetWindowName() == name:
				return cSlot

		pyScrLoader = ui.PythonScriptLoader()
		slot = ui.ListBar()
		pyScrLoader.LoadElementListBar(slot, quest_slot_listbar, self.questPage)
		
		slot.SetParent(self.questPage)
		slot.SetWindowName(name)
		slot.Hide()

		# Store it
		self.questSlotList.append(slot)

		return slot

	def SetQuest(self, slot, questID, questName, questCounterName, questCounterValue):
		(name, color) = self.GetQuestProperties(questName)
		slot.SetTextAlignLeft(name)
		#if color:
			#slot.SetTextColor(self.qColorList[color])
		slot.SetEvent(ui.__mem_func__(self.__SelectQuest), questID)
		slot.SetWindowHorizontalAlignLeft()
		slot.Show()

	def LoadCategory(self, cat):
		# Reinitialize everything
		self.slotIndex = 0
		self.questIndexMap = {}
		self.extraSpace = 0
		self.counterList = []
		self.clockList = []

		# Hidea all the slots first
		for slot in self.questSlotList:
			slot.Hide()
		
		qPos = 0		
		for q in self.GetQuestsInCategory(cat):
			(questID, questIndex, questName, questCategory, _, questCounterName, questCounterValue) = q
			(questClockName, questClockTime) = quest.GetQuestLastTime(questID)

			# Generate slot
			slot = self.CreateQuestSlot("QuestSlotList_"+str(questCategory)+"_"+str(questID))

			# Position it
			slot.SetPosition(10, self.displayY - self.baseCutY)
			baseDisplayY = self.displayY
			
			# Set counters if any
			hasCounter = False
			if questCounterName != "":
				self.displayY += 15

				counter = ui.TextLine()
				counter.SetParent(slot)
				counter.SetPosition(35, 20)
				counter.SetText(questCounterName+": "+str(questCounterValue))
				counter.Show()

				self.counterList.append(counter)
				hasCounter = True

			# Show clocks if any
			if len(questClockName) > 1:
				if questClockTime <= 0:
					clockText = localeInfo.QUEST_TIMEOVER
				else:
					questLastMinute = questClockTime / 60
					questLastSecond = questClockTime % 60

					clockText = questClockName + ": "

					if questLastMinute > 0:
						clockText += str(questLastMinute) + localeInfo.QUEST_MIN
						if questLastSecond > 0:
							clockText += " "

					if questLastSecond > 0:
						clockText += str(questLastSecond) + localeInfo.QUEST_SEC

				clock = ui.TextLine()
				clock.SetParent(slot)
				clock.SetPosition(35, 20 + int(hasCounter)*14)
				clock.SetText(clockText)
				clock.SetProperty("idx", questID)
				clock.Show()

				self.clockList.append(clock)

				self.displayY += 20

			slot.SetProperty("category", questCategory)

			if questIndex in self.clickedQuests:
				slot.OnClickEvent() # mark it

			# Display and save it if necessary
			if baseDisplayY - self.baseCutY >= 0 and baseDisplayY - self.baseCutY < self.MAX_QUEST_HEIGHT:
				# print "Saved "+questName+" at "+str(self.slotIndex)+", id "+str(self.questIndexMap[questID])
				self.questIndexMap[questID] = questIndex
				self.SetQuest(slot, questID, questName, questCounterName, questCounterValue)

			self.displayY += 20

		# Clean non-displayed slots!
		oldList = self.questSlotList
		newList = []
		for slot in self.questSlotList:
			if slot.IsShow():
				newList.append(slot)

		self.questSlotList = newList

	def __UpdateQuestClock(self):
		if "QUEST" == self.state:
			for clock in self.clockList:
				if not clock.GetProperty("idx"):
					print "Invalid clock property"
					return

				(questClockName, questClockTime) = quest.GetQuestLastTime(clock.GetProperty("idx"))

				if questClockTime <= 0:
					clockText = localeInfo.QUEST_TIMEOVER
				else:
					questLastMinute = questClockTime / 60
					questLastSecond = questClockTime % 60

					clockText = questClockName + ": "

					if questLastMinute > 0:
						clockText += str(questLastMinute) + localeInfo.QUEST_MIN
						if questLastSecond > 0:
							clockText += " "

					if questLastSecond > 0:
						clockText += str(questLastSecond) + localeInfo.QUEST_SEC

				clock.SetText(clockText)
	
	def __OnClickQuestCategoryButton(self, category):
		self.ToggleCategory(category)

	def GetQuestsInCategory(self, category, retCount = False):
		qlist = []
		count = 0
		for i in xrange(quest.GetQuestCount()):
			(questIndex, questName, questCategory, questIcon, questCounterName, questCounterValue) = quest.GetQuestData(i)
			if questCategory == category:
				count += 1
				qlist.append((i, questIndex, questName, questCategory, questIcon, questCounterName, questCounterValue))
		
		if retCount:
			return count

		return qlist

	def GetCountQuestInCategory(self, category):
		return self.GetQuestsInCategory(category, True)
		
	def OpenQuestSlideWindow(self, skin, idx):
		#wnds = ()
		#
		#if self.IsShow() == False:
		#	return
		#q = uiQuest.QuestSlideDialog(self, skin, idx)
		#q.SetWindowName("QuestSlideWindow" + str(idx))
		#self.SetTop()
		#q.Show()
        #
		##?? ???? ????? ? ??? ??? ???? ??? ??? ??
		#if self.wndQuestSlideWindowNewKey > 0 and self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1] is not None:
		#	self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1].CloseSelf()
		#
		#self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey] = q
		#
		#self.wndQuestSlideWindowNewKey = self.wndQuestSlideWindowNewKey + 1
		#if self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1] is not None:
		#	self.wndQuestSlideWindow[self.wndQuestSlideWindowNewKey-1].AddOnCloseEvent(lambda key = self.wndQuestSlideWindowNewKey:ui.__mem_func__(self.RemoveQuestSlideDialog)(key-1))
		return
	
	def RemoveQuestSlideDialog(self,key):
		if self.wndQuestSlideWindow[key]:
			self.wndQuestSlideWindow[key] = None
		
