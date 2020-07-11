import app
import player
import item
import ui
import localeInfo
import uiToolTip
import wndMgr

class CharacterDetailsUI(ui.ScriptWindow):
	def __init__(self, parent):
		self.uiCharacterStatus = parent		
		ui.ScriptWindow.__init__(self)
		self.toolTip = uiToolTip.ToolTip()
		
		self.__LoadScript()
	
	def __del__(self):
		self.uiCharacterStatus = None
		self.toolTip = None
		ui.ScriptWindow.__del__(self)
		
	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/CharacterDetailsWindow.py")
		except:
			import exception
			exception.Abort("CharacterDetailsUI.__LoadScript")

		self.Width = 253 - 3

		self.GetChild("TitleBar").CloseButtonHide()
		self.ScrollBar = self.GetChild("ScrollBar")
		self.ScrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		
		## 출력되는 UI 최대 숫자
		self.UI_MAX_COUNT = 13
		self.UI_MAX_VIEW_COUNT = 6
		
		## UI KEY & VALUE
		self.INFO_TEXT	= 0
		self.INFO_TOOLTIP = 1
		self.INFO_VALUE	= 2
		self.CATEGORY_STARTLINE	= -1
		self.CATEGORY_ENDLINE	= -2
		
		## Child 셋팅
		self.labelList		= []
		self.labelValueList	= []
		self.labelTextList	= []
		self.horizonBarList	= []
		self.horizonBarNameList = []
		
		for i in xrange(self.UI_MAX_COUNT):
			self.labelList.append( self.GetChild("label%s"%i) )
			self.labelValueList.append( self.GetChild("labelvalue%s"%i) )
			self.labelTextList.append( self.GetChild("labelname%s"%i) )
			self.horizonBarList.append( self.GetChild("horizontalbar%s"%i) )
			self.horizonBarNameList.append( self.GetChild("horizontalbarName%s"%i) )
			
		for i in xrange(self.UI_MAX_COUNT):
			self.labelTextList[i].SetShowToolTipEvent(ui.__mem_func__(self.__ButtonOverIn), i )
			self.labelTextList[i].SetHideToolTipEvent(ui.__mem_func__(self.__ButtonOverOut), i )
			
		self.__Initialize()
		
	def __Initialize(self):
		self.InfoList = []

		self.InfoList.append( [ localeInfo.DETAILS_CATE_1, "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_1, localeInfo.DETAILS_TOOLTIP_1, item.GetApplyPoint( item.APPLY_ATTBONUS_HUMAN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_3, localeInfo.DETAILS_TOOLTIP_3, item.GetApplyPoint( item.APPLY_ATTBONUS_ORC ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_4, localeInfo.DETAILS_TOOLTIP_4, item.GetApplyPoint( item.APPLY_ATTBONUS_UNDEAD ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_5, localeInfo.DETAILS_TOOLTIP_5, item.GetApplyPoint( item.APPLY_ATTBONUS_MONSTER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_7, localeInfo.DETAILS_TOOLTIP_7, item.GetApplyPoint( item.APPLY_ATTBONUS_ANIMAL ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_8, localeInfo.DETAILS_TOOLTIP_8, item.GetApplyPoint( item.APPLY_ATTBONUS_MILGYO ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_9, localeInfo.DETAILS_TOOLTIP_9, item.GetApplyPoint( item.APPLY_ATTBONUS_DEVIL ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_METIN, localeInfo.DETAILS_TOOLTIP_ATTBONUS_METIN, item.GetApplyPoint( item.APPLY_ATTBONUS_STONE ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_12, localeInfo.DETAILS_TOOLTIP_12, item.GetApplyPoint( item.APPLY_ATT_GRADE_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_13, localeInfo.DETAILS_TOOLTIP_13, item.GetApplyPoint( item.APPLY_DEF_GRADE_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_14, localeInfo.DETAILS_TOOLTIP_14, item.GetApplyPoint( item.APPLY_NORMAL_HIT_DAMAGE_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_15, localeInfo.DETAILS_TOOLTIP_15, item.GetApplyPoint( item.APPLY_NORMAL_HIT_DEFEND_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_16, localeInfo.DETAILS_TOOLTIP_16, item.GetApplyPoint( item.APPLY_SKILL_DAMAGE_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_17, localeInfo.DETAILS_TOOLTIP_17, item.GetApplyPoint( item.APPLY_SKILL_DEFEND_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_18, localeInfo.DETAILS_TOOLTIP_18, item.GetApplyPoint( item.APPLY_MELEE_MAGIC_ATTBONUS_PER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_19, localeInfo.DETAILS_TOOLTIP_19, item.GetApplyPoint( item.APPLY_MAGIC_ATTBONUS_PER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_20, localeInfo.DETAILS_TOOLTIP_20, item.GetApplyPoint( item.APPLY_CRITICAL_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_21, localeInfo.DETAILS_TOOLTIP_21, item.GetApplyPoint( item.APPLY_PENETRATE_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_22, localeInfo.DETAILS_TOOLTIP_22, item.GetApplyPoint( item.APPLY_ANTI_CRITICAL_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_23, localeInfo.DETAILS_TOOLTIP_23, item.GetApplyPoint( item.APPLY_ANTI_PENETRATE_PCT ) ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )

		self.InfoList.append( [ localeInfo.DETAILS_CATE_2, "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_76, localeInfo.DETAILS_TOOLTIP_76, item.GetApplyPoint( item.APPLY_RESIST_MAGIC ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_24, localeInfo.DETAILS_TOOLTIP_24, item.GetApplyPoint( item.APPLY_RESIST_ELEC ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_25, localeInfo.DETAILS_TOOLTIP_25, item.GetApplyPoint( item.APPLY_RESIST_ICE ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_26, localeInfo.DETAILS_TOOLTIP_26, item.GetApplyPoint( item.APPLY_RESIST_DARK ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_27, localeInfo.DETAILS_TOOLTIP_27, item.GetApplyPoint( item.APPLY_RESIST_FIRE ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_28, localeInfo.DETAILS_TOOLTIP_28, item.GetApplyPoint( item.APPLY_RESIST_WIND ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_29, localeInfo.DETAILS_TOOLTIP_29, item.GetApplyPoint( item.APPLY_RESIST_EARTH ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_77, localeInfo.DETAILS_TOOLTIP_77, item.GetApplyPoint( item.APPLY_RESIST_MAGIC_REDUCTION ) ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )

		self.InfoList.append( [ localeInfo.DETAILS_CATE_3, "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_36, localeInfo.DETAILS_TOOLTIP_36, item.GetApplyPoint( item.APPLY_ATTBONUS_WARRIOR ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_37, localeInfo.DETAILS_TOOLTIP_37, item.GetApplyPoint( item.APPLY_ATTBONUS_ASSASSIN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_38, localeInfo.DETAILS_TOOLTIP_38, item.GetApplyPoint( item.APPLY_ATTBONUS_SURA ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_39, localeInfo.DETAILS_TOOLTIP_39, item.GetApplyPoint( item.APPLY_ATTBONUS_SHAMAN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_40, localeInfo.DETAILS_TOOLTIP_40, item.GetApplyPoint( item.APPLY_ATTBONUS_WOLFMAN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_41, localeInfo.DETAILS_TOOLTIP_41, item.GetApplyPoint( item.APPLY_RESIST_WARRIOR ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_42, localeInfo.DETAILS_TOOLTIP_42, item.GetApplyPoint( item.APPLY_RESIST_ASSASSIN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_43, localeInfo.DETAILS_TOOLTIP_43, item.GetApplyPoint( item.APPLY_RESIST_SURA ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_44, localeInfo.DETAILS_TOOLTIP_44, item.GetApplyPoint( item.APPLY_RESIST_SHAMAN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_45, localeInfo.DETAILS_TOOLTIP_45, item.GetApplyPoint( item.APPLY_RESIST_WOLFMAN ) ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )

		self.InfoList.append( [ localeInfo.DETAILS_CATE_4, "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_46, localeInfo.DETAILS_TOOLTIP_46, item.GetApplyPoint( item.APPLY_RESIST_SWORD ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_47, localeInfo.DETAILS_TOOLTIP_47, item.GetApplyPoint( item.APPLY_RESIST_TWOHAND ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_48, localeInfo.DETAILS_TOOLTIP_48, item.GetApplyPoint( item.APPLY_RESIST_DAGGER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_50, localeInfo.DETAILS_TOOLTIP_50, item.GetApplyPoint( item.APPLY_RESIST_BELL ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_51, localeInfo.DETAILS_TOOLTIP_51, item.GetApplyPoint( item.APPLY_RESIST_FAN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_52, localeInfo.DETAILS_TOOLTIP_52, item.GetApplyPoint( item.APPLY_RESIST_BOW ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_SWORD,		localeInfo.DETAILS_TOOLTIP_ATTBONUS_SWORD	, item.GetApplyPoint( item.APPLY_ATTBONUS_SWORD ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_TWOHAND,	localeInfo.DETAILS_TOOLTIP_ATTBONUS_TWOHAND, item.GetApplyPoint( item.APPLY_ATTBONUS_TWOHANDED ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_DAGGER,		localeInfo.DETAILS_TOOLTIP_ATTBONUS_DAGGER	, item.GetApplyPoint( item.APPLY_ATTBONUS_DAGGER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_BELL,			localeInfo.DETAILS_TOOLTIP_ATTBONUS_BELL	, item.GetApplyPoint( item.APPLY_ATTBONUS_BELL ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_FAN	,			localeInfo.DETAILS_TOOLTIP_ATTBONUS_FAN	, item.GetApplyPoint( item.APPLY_ATTBONUS_FAN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_ATTBONUS_BOW,			localeInfo.DETAILS_TOOLTIP_ATTBONUS_BOW	, item.GetApplyPoint( item.APPLY_ATTBONUS_BOW ) ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )

		self.InfoList.append( [ localeInfo.DETAILS_CATE_5, "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_53, localeInfo.DETAILS_TOOLTIP_53, item.GetApplyPoint( item.APPLY_STUN_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_54, localeInfo.DETAILS_TOOLTIP_54, item.GetApplyPoint( item.APPLY_SLOW_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_55, localeInfo.DETAILS_TOOLTIP_55, item.GetApplyPoint( item.APPLY_POISON_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_56, localeInfo.DETAILS_TOOLTIP_56, item.GetApplyPoint( item.APPLY_POISON_REDUCE) ] )
		self.InfoList.append( [ localeInfo.DETAILS_57, localeInfo.DETAILS_TOOLTIP_57, item.GetApplyPoint( item.APPLY_BLEEDING_PCT ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_58, localeInfo.DETAILS_TOOLTIP_58, item.GetApplyPoint( item.APPLY_BLEEDING_REDUCE ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_59, localeInfo.DETAILS_TOOLTIP_59, item.GetApplyPoint( item.APPLY_STEAL_HP ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_60, localeInfo.DETAILS_TOOLTIP_60, item.GetApplyPoint( item.APPLY_STEAL_SP ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_61, localeInfo.DETAILS_TOOLTIP_61, item.GetApplyPoint( item.APPLY_HP_REGEN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_62, localeInfo.DETAILS_TOOLTIP_62, item.GetApplyPoint( item.APPLY_SP_REGEN ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_63, localeInfo.DETAILS_TOOLTIP_63, item.GetApplyPoint( item.APPLY_BLOCK ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_64, localeInfo.DETAILS_TOOLTIP_64, item.GetApplyPoint( item.APPLY_DODGE ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_65, localeInfo.DETAILS_TOOLTIP_65, item.GetApplyPoint( item.APPLY_REFLECT_MELEE ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_66, localeInfo.DETAILS_TOOLTIP_66, item.GetApplyPoint( item.APPLY_KILL_HP_RECOVER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_67, localeInfo.DETAILS_TOOLTIP_67, item.GetApplyPoint( item.APPLY_KILL_SP_RECOVER ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_68, localeInfo.DETAILS_TOOLTIP_68, item.GetApplyPoint( item.APPLY_EXP_DOUBLE_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_69, localeInfo.DETAILS_TOOLTIP_69, item.GetApplyPoint( item.APPLY_GOLD_DOUBLE_BONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_70, localeInfo.DETAILS_TOOLTIP_70, item.GetApplyPoint( item.APPLY_ITEM_DROP_BONUS ) ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )

		self.InfoList.append( [ localeInfo.DETAILS_CATE_6, "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_71, localeInfo.DETAILS_TOOLTIP_71, item.GetApplyPoint( item.APPLY_MALL_ATTBONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_72, localeInfo.DETAILS_TOOLTIP_72, item.GetApplyPoint( item.APPLY_MALL_DEFBONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_73, localeInfo.DETAILS_TOOLTIP_73, item.GetApplyPoint( item.APPLY_MALL_EXPBONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_74, localeInfo.DETAILS_TOOLTIP_74, item.GetApplyPoint( item.APPLY_MALL_ITEMBONUS ) ] )
		self.InfoList.append( [ localeInfo.DETAILS_75, localeInfo.DETAILS_TOOLTIP_75, item.GetApplyPoint( item.APPLY_MALL_GOLDBONUS ) ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )
		
		self.Diff = len(self.InfoList) - self.UI_MAX_COUNT
		stepSize = 1.0 / self.Diff
		self.ScrollBar.SetScrollStep( stepSize )
		self.ScollPos = 0
		self.RefreshLabel()
		
	def Show(self):
		ui.ScriptWindow.Show(self)
		self.SetTop()
		
	def Close(self):
		self.Hide()
	
	def AdjustPosition(self, x, y):
		self.SetPosition(x + self.Width, y)
	
	def OnScroll(self):
		self.RefreshLabel()
			
	def RefreshLabel(self):
		self.ScollPos = int(self.ScrollBar.GetPos() * self.Diff)
		
		for i in xrange(self.UI_MAX_COUNT) :
			idx = i + self.ScollPos
			
			text = self.InfoList[idx][self.INFO_TEXT]
			type = self.InfoList[idx][self.INFO_VALUE]
			
			if type == self.CATEGORY_STARTLINE:
				self.__LabelTitleLine(i, text)
			elif type == self.CATEGORY_ENDLINE:
				self.__EmptyLine(i)
			else:
				value = player.GetStatus(type)
				
				self.__LabelLine(i, text, value)

				
	def __LabelTitleLine(self, idx, text):
		self.labelList[idx].Hide()
		self.labelTextList[idx].Hide()
		self.horizonBarList[idx].Show()
		self.horizonBarNameList[idx].SetText( text )
				
	def __EmptyLine(self, idx):
		self.labelList[idx].Hide()
		self.labelTextList[idx].Hide()
		self.horizonBarList[idx].Hide()
		
	def __LabelLine(self, idx, text, value):
		self.labelList[idx].Show()
		self.labelTextList[idx].Show()
		self.horizonBarList[idx].Hide()
		
		self.labelTextList[idx].SetText( text )
		self.labelValueList[idx].SetText( str(value) )
		
	def __ButtonOverIn(self, i):
		idx = i + self.ScollPos
		tooltip = self.InfoList[idx][self.INFO_TOOLTIP]
		
		arglen = len(str(tooltip))
		pos_x, pos_y = wndMgr.GetMousePosition()
		
		self.toolTip.ClearToolTip()
		self.toolTip.SetThinBoardSize(11 * arglen)
		self.toolTip.SetToolTipPosition(pos_x + 50, pos_y + 50)
		self.toolTip.AppendTextLine(tooltip, 0xffffff00)
		self.toolTip.Show()		
		
	def __ButtonOverOut(self, idx):
		self.toolTip.Hide()
		
	def OnRunMouseWheel(self, nLen):
		if nLen > 0:
			self.ScrollBar.OnUp()
		else:
			self.ScrollBar.OnDown()
		
	def OnTop(self):
		if self.uiCharacterStatus:
			self.uiCharacterStatus.SetTop()